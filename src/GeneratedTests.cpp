#include <iostream>  // Standard input/output
#include <cmath>     // Math functions like fabs
#include <cassert>   // Assertions for validation
#include <limits>    // For NaN and infinity

#include <mpi.h>     // MPI initialization and communication
#include <omp.h>     // OpenMP for threading

#include "SparseMatrix.hpp"     // SparseMatrix class and related operations
#include "Vector.hpp"           // Vector class and related operations
#include "ComputeSPMV.hpp"      // Sparse matrix-vector multiplication
#include "Geometry.hpp"         // Geometry struct for matrix dimensions
#include "GenerateGeometry.hpp" // Declaration of the GenerateGeometry function
#include "GeneratedTests.hpp"   // Declarations of test functions
#include "TestResult.hpp"


void initializeMPI(int &argc, char **&argv) {
    MPI_Init(&argc, &argv);
}

void finalizeMPI() {
    MPI_Finalize();
}

// Helper function to create geometry
Geometry* createGeometry(int nx, int ny, int nz, int size, int rank) {
    assert(nx > 0 && ny > 0 && nz > 0); // Validate dimensions
    Geometry *geom = new Geometry;
    int numThreads = 1;
    int npx = 1, npy = 1, npz = 1;
    double zl = 0.0, zu = 1.0;
    int pz = 1;

    GenerateGeometry(size, rank, numThreads, pz, zl, zu, nx, ny, nz, npx, npy, npz, geom);

    return geom;
}

// Test precision and rounding behavior of SPMV operation
int testPrecisionAndRounding(int size, int rank) {
    /**
     * Method Under Test: ComputeSPMV(SparseMatrix &A, Vector &x, Vector &y)
     * Parameters:
     * - A: SparseMatrix initialized with geometry.
     * - x: Vector with size equal to A's rows, initialized with small values (1e-10).
     * - y: Vector of the same size as x, will store the result of A * x.
     */
    Geometry *geom = createGeometry(100, 100, 100, size, rank);

    SparseMatrix A;
    InitializeSparseMatrix(A, geom);

    // Use SparseMatrix for local rows
    local_int_t localRows = A.localNumberOfRows;

    Vector x, y;
    InitializeVector(x, localRows);
    InitializeVector(y, localRows);

    // Assign small values to x
    for (local_int_t i = 0; i < localRows; ++i) {
        x.values[i] = 1e-10;
    }

    if (ComputeSPMV(A, x, y) != 0) {
        delete geom;
        return 0;
    }

    double tolerance = 1e-9;
    double expectedValue = 1e-10;

    // Validate precision
    if (std::fabs(y.values[0] - expectedValue) > tolerance) {
        delete geom;
        return 0;
    }

    delete geom;
    return 1;
}

// Test handling of a zero-sized matrix and vector in SPMV
int testEdgeCaseZeroSizedMatrix(int size, int rank) {
    /**
     * Method Under Test: ComputeSPMV(SparseMatrix &A, Vector &x, Vector &y)
     * Parameters:
     * - A: SparseMatrix initialized with zero-sized geometry.
     * - x: Empty vector.
     * - y: Empty vector.
     */
    Geometry *geom = createGeometry(0, 0, 0, size, rank);

    SparseMatrix A;
    InitializeSparseMatrix(A, geom);

    // Use SparseMatrix for local rows
    local_int_t localRows = A.localNumberOfRows;

    Vector x, y;
    InitializeVector(x, localRows);
    InitializeVector(y, localRows);

    // Validate sizes
    assert(localRows == 0);
    assert(A.localNumberOfRows == 0);

    int result = ComputeSPMV(A, x, y);

    delete geom;
    return result != 0 ? 1 : 0;
}

// Test performance and memory handling on a large matrix
int testLargeInputHandling(int size, int rank) {
    /**
     * Method Under Test: ComputeSPMV(SparseMatrix &A, Vector &x, Vector &y)
     * Parameters:
     * - A: SparseMatrix initialized with large geometry.
     * - x: Vector sized to local rows in A, filled with random values.
     * - y: Vector of the same size as x, will store the result of A * x.
     */
    Geometry *geom = createGeometry(1000, 1000, 1000, size, rank);

    SparseMatrix A;
    InitializeSparseMatrix(A, geom);

    // Use SparseMatrix for local rows
    local_int_t localRows = A.localNumberOfRows;

    Vector x, y;
    InitializeVector(x, localRows);
    InitializeVector(y, localRows);

    // Validate sizes
    assert(localRows > 0);

    try {
        ComputeSPMV(A, x, y);
    } catch (...) {
        delete geom;
        return 0;
    }

    delete geom;
    return 1;
}

// Test handling of non-standard values
int testNonStandardValues(int size, int rank) {
    /**
     * Method Under Test: ComputeSPMV(SparseMatrix &A, Vector &x, Vector &y)
     * Parameters:
     * - A: SparseMatrix initialized with geometry.
     * - x: Vector with `NaN` or `infinity` values.
     * - y: Vector of the same size as x.
     */
    Geometry *geom = createGeometry(100, 100, 100, size, rank);

    SparseMatrix A;
    InitializeSparseMatrix(A, geom);

    // Use SparseMatrix for local rows
    local_int_t localRows = A.localNumberOfRows;

    Vector x, y;
    InitializeVector(x, localRows);
    InitializeVector(y, localRows);

    // Assign NaN and infinity values
    for (local_int_t i = 0; i < localRows; ++i) {
        x.values[i] = (i % 2 == 0) ? std::numeric_limits<double>::quiet_NaN() 
                                   : std::numeric_limits<double>::infinity();
    }

    try {
        ComputeSPMV(A, x, y);
    } catch (...) {
        delete geom;
        return 1; // Expecting to handle gracefully
    }

    delete geom;
    return 0;
}

// Main function to run tests
int main(int argc, char **argv) {
    initializeMPI(argc, argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int passCount = 0;

    passCount += testPrecisionAndRounding(size, rank);
    passCount += testEdgeCaseZeroSizedMatrix(size, rank);
    passCount += testLargeInputHandling(size, rank);
    passCount += testNonStandardValues(size, rank);

    if (rank == 0) {
        std::cout << "Passed " << passCount << "/4 tests." << std::endl;
    }

    finalizeMPI();
    return 0;
}
