#include <iostream>
#include <vector>
#include <cassert>
#include <mpi.h>
#include "../SparseMatrix.hpp"
#include "../Vector.hpp"
#include "../GenerateProblem.hpp"
#include "../ComputeSPMV.hpp"

void TestMatrixUnalteredDirectModification() {
    // Initialize MPI

    Geometry g;
    g.nx = 3; g.ny = 3; g.nz = 3;
    g.gnx = 3; g.gny = 3; g.gnz = 3;
    g.gix0 = 0; g.giy0 = 0; g.giz0 = 0;

    SparseMatrix A;
    A.geom = &g;
    Vector b, x, xexact;

    GenerateProblem(A, &b, &x, &xexact);

    // Modify the diagonal element of the first row to zero
    A.matrixValues[0][0] = 0.0;

    // Verify modification
    assert(A.matrixValues[0][0] == 0.0);

    // Perform some HPCG operations
    ComputeSPMV(A, x, b); // Example operation

    // Verify the matrix is unaltered
    assert(A.matrixValues[0][0] == 0.0);

    std::cout << "Test passed: Matrix is unaltered during HPCG operations." << std::endl;

    // Finalize MPI
}


bool TestMatrixGeneration() {
    TestMatrixUnalteredDirectModification();

    return true;
}
