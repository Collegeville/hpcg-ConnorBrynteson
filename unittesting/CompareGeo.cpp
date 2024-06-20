/*

Goal for morning is have it read two matrix into the files one at the start of gen problem and one at then end then compare the two.
This would allow us to skip the hard stuff lol

*/

#include "SparseMatrix.hpp"
#include "GenerateProblem_ref.hpp"
#include "CompareGeo.hpp"

void GenerateReferenceMatrix(SparseMatrix &A, Vector *b, Vector *x, Vector *xexact)
{
    global_int_t nx = A.geom->nx;
    global_int_t ny = A.geom->ny;
    global_int_t nz = A.geom->nz;
    global_int_t gnx = A.geom->gnx;
    global_int_t gny = A.geom->gny;
    global_int_t gnz = A.geom->gnz;
    global_int_t gix0 = A.geom->gix0;
    global_int_t giy0 = A.geom->giy0;
    global_int_t giz0 = A.geom->giz0;

    local_int_t localNumberOfRows = nx * ny * nz;
    local_int_t numberOfNonzerosPerRow = 27;

    global_int_t totalNumberOfRows = gnx * gny * gnz;

    char *nonzerosInRow = new char[localNumberOfRows];
    global_int_t **mtxIndG = new global_int_t *[localNumberOfRows];
    local_int_t **mtxIndL = new local_int_t *[localNumberOfRows];
    double **matrixValues = new double *[localNumberOfRows];
    double **matrixDiagonal = new double *[localNumberOfRows];

    if (b != nullptr)
        InitializeVector(*b, localNumberOfRows);
    if (x != nullptr)
        InitializeVector(*x, localNumberOfRows);
    if (xexact != nullptr)
        InitializeVector(*xexact, localNumberOfRows);

    double *bv = nullptr;
    double *xv = nullptr;
    double *xexactv = nullptr;
    if (b != nullptr)
        bv = b->values;
    if (x != nullptr)
        xv = x->values;
    if (xexact != nullptr)
        xexactv = xexact->values;

    A.localToGlobalMap.resize(localNumberOfRows);

    for (local_int_t i = 0; i < localNumberOfRows; ++i)
    {
        matrixValues[i] = nullptr;
        matrixDiagonal[i] = nullptr;
        mtxIndG[i] = nullptr;
        mtxIndL[i] = nullptr;
    }

#ifndef HPCG_CONTIGUOUS_ARRAYS
    for (local_int_t i = 0; i < localNumberOfRows; ++i)
        mtxIndL[i] = new local_int_t[numberOfNonzerosPerRow];
    for (local_int_t i = 0; i < localNumberOfRows; ++i)
        matrixValues[i] = new double[numberOfNonzerosPerRow];
    for (local_int_t i = 0; i < localNumberOfRows; ++i)
        mtxIndG[i] = new global_int_t[numberOfNonzerosPerRow];
#else
    mtxIndL[0] = new local_int_t[localNumberOfRows * numberOfNonzerosPerRow];
    matrixValues[0] = new double[localNumberOfRows * numberOfNonzerosPerRow];
    mtxIndG[0] = new global_int_t[localNumberOfRows * numberOfNonzerosPerRow];

    for (local_int_t i = 1; i < localNumberOfRows; ++i)
    {
        mtxIndL[i] = mtxIndL[0] + i * numberOfNonzerosPerRow;
        matrixValues[i] = matrixValues[0] + i * numberOfNonzerosPerRow;
        mtxIndG[i] = mtxIndG[0] + i * numberOfNonzerosPerRow;
    }
#endif

    local_int_t localNumberOfNonzeros = 0;

    for (local_int_t iz = 0; iz < nz; ++iz)
    {
        global_int_t giz = giz0 + iz;
        for (local_int_t iy = 0; iy < ny; ++iy)
        {
            global_int_t giy = giy0 + iy;
            for (local_int_t ix = 0; ix < nx; ++ix)
            {
                global_int_t gix = gix0 + ix;
                local_int_t currentLocalRow = iz * nx * ny + iy * nx + ix;
                global_int_t currentGlobalRow = giz * gnx * gny + giy * gnx + gix;

                A.globalToLocalMap[currentGlobalRow] = currentLocalRow;
                A.localToGlobalMap[currentLocalRow] = currentGlobalRow;

                char numberOfNonzerosInRow = 0;
                double *currentValuePointer = matrixValues[currentLocalRow];
                global_int_t *currentIndexPointerG = mtxIndG[currentLocalRow];

                for (int sz = -1; sz <= 1; ++sz)
                {
                    if (giz + sz > -1 && giz + sz < gnz)
                    {
                        for (int sy = -1; sy <= 1; ++sy)
                        {
                            if (giy + sy > -1 && giy + sy < gny)
                            {
                                for (int sx = -1; sx <= 1; ++sx)
                                {
                                    if (gix + sx > -1 && gix + sx < gnx)
                                    {
                                        global_int_t curcol = currentGlobalRow + sz * gnx * gny + sy * gnx + sx;
                                        if (curcol == currentGlobalRow)
                                        {
                                            matrixDiagonal[currentLocalRow] = currentValuePointer;
                                            *currentValuePointer++ = 26.0;
                                        }
                                        else
                                        {
                                            *currentValuePointer++ = -1.0;
                                        }
                                        *currentIndexPointerG++ = curcol;
                                        numberOfNonzerosInRow++;
                                    }
                                }
                            }
                        }
                    }
                }

                nonzerosInRow[currentLocalRow] = numberOfNonzerosInRow;
                localNumberOfNonzeros += numberOfNonzerosInRow;

                if (b != nullptr)
                    bv[currentLocalRow] = 26.0 - static_cast<double>(numberOfNonzerosInRow - 1);
                if (x != nullptr)
                    xv[currentLocalRow] = 0.0;
                if (xexact != nullptr)
                    xexactv[currentLocalRow] = 1.0;
            }
        }
    }

    A.title = nullptr;
    A.totalNumberOfRows = totalNumberOfRows;
    A.totalNumberOfNonzeros = localNumberOfNonzeros;
    A.localNumberOfRows = localNumberOfRows;
    A.localNumberOfColumns = localNumberOfRows;
    A.localNumberOfNonzeros = localNumberOfNonzeros;
    A.nonzerosInRow = nonzerosInRow;
    A.mtxIndG = mtxIndG;
    A.mtxIndL = mtxIndL;
    A.matrixValues = matrixValues;
    A.matrixDiagonal = matrixDiagonal;
}

bool CompareMatrices(const SparseMatrix &A1, const SparseMatrix &A2)
{
    if (A1.localNumberOfRows != A2.localNumberOfRows)
        return false;
    for (local_int_t i = 0; i < A1.localNumberOfRows; ++i)
    {
        if (A1.nonzerosInRow[i] != A2.nonzerosInRow[i])
            return false;
        for (char j = 0; j < A1.nonzerosInRow[i]; ++j)
        {
            if (A1.mtxIndG[i][j] != A2.mtxIndG[i][j] || A1.matrixValues[i][j] != A2.matrixValues[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool TestMatrixGeneration()
{
    SparseMatrix A1, A2;
    Vector b1, x1, xexact1;
    Vector b2, x2, xexact2;

    // Generate the sparse matrix using GenerateProblem_ref
    GenerateProblem_ref(A1, &b1, &x1, &xexact1);

    // Generate the reference matrix
    GenerateReferenceMatrix(A2, &b2, &x2, &xexact2);

    // Compare the matrices
    bool areEqual = CompareMatrices(A1, A2);
    if (areEqual)
    {
        return true;
    }
    else
    {
        return false;
    }
}
