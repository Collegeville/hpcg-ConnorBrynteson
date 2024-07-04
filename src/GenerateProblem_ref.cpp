
//@HEADER
// ***************************************************
//
// HPCG: High Performance Conjugate Gradient Benchmark
//
// Contact:
// Michael A. Heroux ( maherou@sandia.gov)
// Jack Dongarra     (dongarra@eecs.utk.edu)
// Piotr Luszczek    (luszczek@eecs.utk.edu)
//
// ***************************************************
//@HEADER

/*!
 @file GenerateProblem_ref.cpp

 HPCG routine
 */

#ifndef HPCG_NO_MPI
#include <mpi.h>
#endif

#ifndef HPCG_NO_OPENMP
#include <omp.h>
#endif

#if defined(HPCG_DEBUG) || defined(HPCG_DETAILED_DEBUG)
using std::endl;
#include "hpcg.hpp"
#include <fstream>
#endif
#include <cassert>
#include <iostream>
#include "GenerateProblem_ref.hpp"

/*!
  Reference version of GenerateProblem to generate the sparse matrix, right hand side, initial guess, and exact solution.

  @param[in]  A      The known system matrix
  @param[inout] b      The newly allocated and generated right hand side vector (if b!=0 on entry)
  @param[inout] x      The newly allocated solution vector with entries set to 0.0 (if x!=0 on entry)
  @param[inout] xexact The newly allocated solution vector with entries set to the exact solution (if the xexact!=0 non-zero on entry)

  @see GenerateGeometry
*/


void GenerateProblem_ref(SparseMatrix &A, Vector *b, Vector *x, Vector *xexact) {
    // Check if A.geom is properly initialized
    if (!A.geom) {
        std::cerr << "Error: Geometry not initialized" << std::endl;
        return;
    }

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
    assert(localNumberOfRows > 0);

    local_int_t numberOfNonzerosPerRow = 27; // Assuming a 27-point stencil

    global_int_t totalNumberOfRows = gnx * gny * gnz; // Total number of grid points in the mesh
    assert(totalNumberOfRows > 0);

    // Allocate arrays that are of length localNumberOfRows
    char *nonzerosInRow = new char[localNumberOfRows];
    global_int_t **mtxIndG = new global_int_t *[localNumberOfRows];
    local_int_t **mtxIndL = new local_int_t *[localNumberOfRows];
    double **matrixValues = new double *[localNumberOfRows];
    double **matrixDiagonal = new double *[localNumberOfRows];

    assert(nonzerosInRow != nullptr);
    assert(mtxIndG != nullptr);
    assert(mtxIndL != nullptr);
    assert(matrixValues != nullptr);
    assert(matrixDiagonal != nullptr);


    if (b != nullptr) InitializeVector(*b, localNumberOfRows);
    if (x != nullptr) InitializeVector(*x, localNumberOfRows);
    if (xexact != nullptr) InitializeVector(*xexact, localNumberOfRows);

    double *bv = nullptr;
    double *xv = nullptr;
    double *xexactv = nullptr;
    if (b != nullptr) bv = b->values;
    if (x != nullptr) xv = x->values;
    if (xexact != nullptr) xexactv = xexact->values;

    assert(bv != nullptr || b == nullptr);
    assert(xv != nullptr || x == nullptr);
    assert(xexactv != nullptr || xexact == nullptr);

    A.localToGlobalMap.resize(localNumberOfRows);


#ifndef HPCG_NO_OPENMP
#pragma omp parallel for
#endif
    for (local_int_t i = 0; i < localNumberOfRows; ++i) {
        matrixValues[i] = nullptr;
        matrixDiagonal[i] = nullptr;
        mtxIndG[i] = nullptr;
        mtxIndL[i] = nullptr;
    }

#ifndef HPCG_CONTIGUOUS_ARRAYS
    for (local_int_t i = 0; i < localNumberOfRows; ++i) {
        mtxIndL[i] = new local_int_t[numberOfNonzerosPerRow];
        assert(mtxIndL[i] != nullptr);
        matrixValues[i] = new double[numberOfNonzerosPerRow];
        assert(matrixValues[i] != nullptr);
        mtxIndG[i] = new global_int_t[numberOfNonzerosPerRow];
        assert(mtxIndG[i] != nullptr);
    }
#else
    mtxIndL[0] = new local_int_t[localNumberOfRows * numberOfNonzerosPerRow];
    matrixValues[0] = new double[localNumberOfRows * numberOfNonzerosPerRow];
    mtxIndG[0] = new global_int_t[localNumberOfRows * numberOfNonzerosPerRow];
    assert(mtxIndL[0] != nullptr);
    assert(matrixValues[0] != nullptr);
    assert(mtxIndG[0] != nullptr);

    for (local_int_t i = 1; i < localNumberOfRows; ++i) {
        mtxIndL[i] = mtxIndL[0] + i * numberOfNonzerosPerRow;
        matrixValues[i] = matrixValues[0] + i * numberOfNonzerosPerRow;
        mtxIndG[i] = mtxIndG[0] + i * numberOfNonzerosPerRow;
    }
#endif


    local_int_t localNumberOfNonzeros = 0;

#ifndef HPCG_NO_OPENMP
#pragma omp parallel for
#endif
    for (local_int_t iz = 0; iz < nz; iz++) {
        global_int_t giz = giz0 + iz;
        for (local_int_t iy = 0; iy < ny; iy++) {
            global_int_t giy = giy0 + iy;
            for (local_int_t ix = 0; ix < nx; ix++) {
                global_int_t gix = gix0 + ix;
                local_int_t currentLocalRow = iz * nx * ny + iy * nx + ix;
                global_int_t currentGlobalRow = giz * gnx * gny + giy * gnx + gix;
                assert(currentLocalRow < localNumberOfRows);

#ifndef HPCG_NO_OPENMP
#pragma omp critical
#endif
                {
                    A.globalToLocalMap[currentGlobalRow] = currentLocalRow;
                    A.localToGlobalMap[currentLocalRow] = currentGlobalRow;
                }

                char numberOfNonzerosInRow = 0;
                double *currentValuePointer = matrixValues[currentLocalRow];
                global_int_t *currentIndexPointerG = mtxIndG[currentLocalRow];

                assert(currentValuePointer != nullptr);
                assert(currentIndexPointerG != nullptr);

                for (int sz = -1; sz <= 1; sz++) {
                    if (giz + sz > -1 && giz + sz < gnz) {
                        for (int sy = -1; sy <= 1; sy++) {
                            if (giy + sy > -1 && giy + sy < gny) {
                                for (int sx = -1; sx <= 1; sx++) {
                                    if (gix + sx > -1 && gix + sx < gnx) {
                                        global_int_t curcol = currentGlobalRow + sz * gnx * gny + sy * gnx + sx;
                                        if (curcol == currentGlobalRow) {
                                            matrixDiagonal[currentLocalRow] = currentValuePointer;
                                            *currentValuePointer++ = 26.0;
                                        } else {
                                            *currentValuePointer++ = -1.0;
                                        }
                                        *currentIndexPointerG++ = curcol;
                                        numberOfNonzerosInRow++;
                                        assert(numberOfNonzerosInRow <= numberOfNonzerosPerRow);
                                    }
                                }
                            }
                        }
                    }
                }

                nonzerosInRow[currentLocalRow] = numberOfNonzerosInRow;

#ifndef HPCG_NO_OPENMP
#pragma omp atomic
#endif
                localNumberOfNonzeros += numberOfNonzerosInRow;

                if (b != nullptr) bv[currentLocalRow] = 26.0 - ((double)(numberOfNonzerosInRow - 1));
                if (x != nullptr) xv[currentLocalRow] = 0.0;
                if (xexact != nullptr) xexactv[currentLocalRow] = 1.0;
            }
        }
    }


    global_int_t totalNumberOfNonzeros = 0;
#ifndef HPCG_NO_MPI
    // Use MPI's reduce function to sum all nonzeros
#ifdef HPCG_NO_LONG_LONG
    MPI_Allreduce(&localNumberOfNonzeros, &totalNumberOfNonzeros, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
#else
    long long lnnz = localNumberOfNonzeros;
    long long gnnz = 0; // convert to 64 bit for MPI call
    MPI_Allreduce(&lnnz, &gnnz, 1, MPI_LONG_LONG_INT, MPI_SUM, MPI_COMM_WORLD);
    totalNumberOfNonzeros = gnnz; // Copy back

#endif
#else
    totalNumberOfNonzeros = localNumberOfNonzeros;
#endif
    assert(totalNumberOfNonzeros > 0); // Ensure there are nonzeros
    A.title = 0;
    A.totalNumberOfRows = totalNumberOfRows;
    A.totalNumberOfNonzeros = totalNumberOfNonzeros;
    A.localNumberOfRows = localNumberOfRows;
    A.localNumberOfColumns = localNumberOfRows;
    A.localNumberOfNonzeros = localNumberOfNonzeros;
    A.nonzerosInRow = nonzerosInRow;
    A.mtxIndG = mtxIndG;
    A.mtxIndL = mtxIndL;
    A.matrixValues = matrixValues;
    A.matrixDiagonal = matrixDiagonal;
}
