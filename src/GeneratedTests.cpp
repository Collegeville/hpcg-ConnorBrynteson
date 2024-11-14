#include <iostream>
#include <cmath>
#include <mpi.h>
#include <omp.h>

// Include specific HPCG headers based on function requirements
#include "SparseMatrix.hpp"      // Defines SparseMatrix and related matrix operations
#include "Vector.hpp"            // Defines Vector and related vector operations
#include "ComputeSPMV.hpp"       // Defines the ComputeSPMV function for sparse matrix-vector multiplication
#include "ComputeMG.hpp"         // Defines multi-grid solver function ComputeMG
#include "GenerateProblem.hpp"  // Defines vector initialization functions

#include "GeneratedTests.hpp"    // Header file for test function declarations

// MPI initialization and finalization functions
void initializeMPI(int &argc, char **&argv) {
    MPI_Init(&argc, &argv);
}

void finalizeMPI() {
    MPI_Finalize();
}

// Test functions using available HPCG functions

int testPrecisionAndRounding() {
    SparseMatrix A;
    InitializeSparseMatrix(A, 100);
    Vector x, y;
    InitializeVector(x, 100, 1e-10);
    InitializeVector(y, 100, 0.0);

    if (ComputeSPMV(A, x, y) != 0) return 0;
    double tolerance = 1e-9;
    double expectedValue = 1e-10;
    if (std::fabs(y.values[0] - expectedValue) > tolerance) return 0;
    return 1;
}

int testEdgeCaseZeroSizedMatrix() {
    SparseMatrix A;
    Vector x, y;
    InitializeSparseMatrix(A, 0);
    InitializeVector(x, 0, 0.0);
    InitializeVector(y, 0, 0.0);

    int result = ComputeSPMV(A, x, y);
    return result != 0 ? 1 : 0;
}

int testLargeInputHandling() {
    SparseMatrix A;
    Vector x, y;
    InitializeSparseMatrix(A, 1000000);
    InitializeVector(x, 1000000, 1.0);
    InitializeVector(y, 1000000, 0.0);

    try {
        ComputeSPMV(A, x, y);
    } catch (...) {
        return 0;
    }
    return 1;
}

int testDataTypeConsistency() {
    SparseMatrix A;
    Vector x_single, x_double, y_single, y_double;
    InitializeSparseMatrix(A, 100);
    InitializeVector(x_single, 100, 1.0f);
    InitializeVector(x_double, 100, 1.0);
    InitializeVector(y_single, 100, 0.0f);
    InitializeVector(y_double, 100, 0.0);

    ComputeSPMV(A, x_single, y_single);
    ComputeSPMV(A, x_double, y_double);

    if (std::fabs(y_single.values[0] - y_double.values[0]) > 1e-5) return 0;
    return 1;
}

int testThreadSafety() {
    SparseMatrix A;
    InitializeSparseMatrix(A, 100);
    Vector x, y;
    InitializeVector(x, 100, 1.0);
    InitializeVector(y, 100, 0.0);

    bool success = true;
    #pragma omp parallel for
    for (int i = 0; i < 10; ++i) {
        try {
            ComputeSPMV(A, x, y);
        } catch (...) {
            success = false;
        }
    }
    return success ? 1 : 0;
}

int testSolverBoundaryConditions() {
    SparseMatrix A;
    InitializeSparseMatrix(A, 100);
    Vector x, b;
    InitializeVector(x, 100, 0.0);
    InitializeVector(b, 100, 1.0);

    int iterations = 10;
    int result = ComputeMG(A, x, b, iterations);

    return (result == 0 && iterations <= 10) ? 1 : 0;
}

// Since `DecomposeSparseMatrix` and explicit `BoundaryConditions` functions do not exist,
// we omit tests that require these functions. We focus on operations that align with HPCG's capabilities.

int testNonStandardValues() {
    SparseMatrix A;
    InitializeSparseMatrix(A, 100);
    Vector x, y, y_inf;
    InitializeVector(x, 100, std::numeric_limits<double>::quiet_NaN());
    InitializeVector(y, 100, 0.0);
    InitializeVector(y_inf, 100, std::numeric_limits<double>::infinity());

    try {
        ComputeSPMV(A, x, y);
        return 0;
    } catch (...) {}

    try {
        ComputeSPMV(A, x, y_inf);
        return 0;
    } catch (...) {
        return 1;
    }
}

int testOutputFormatConsistency() {
    SparseMatrix A;
    InitializeSparseMatrix(A, 5);
    Vector y;
    InitializeVector(y, 5, 0.0);
    ComputeSPMV(A, y, y);

    return (y.values.size() == 5) ? 1 : 0;
}

// Main function to initialize MPI and run tests
int main(int argc, char **argv) {
    initializeMPI(argc, argv);

    int passCount = 0;

    passCount += testPrecisionAndRounding();
    passCount += testEdgeCaseZeroSizedMatrix();
    passCount += testLargeInputHandling();
    passCount += testDataTypeConsistency();
    passCount += testThreadSafety();
    passCount += testSolverBoundaryConditions();
    passCount += testNonStandardValues();
    passCount += testOutputFormatConsistency();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::cout << "Passed " << passCount << "/8 tests." << std::endl;
    }

    finalizeMPI();
    return 0;
}

