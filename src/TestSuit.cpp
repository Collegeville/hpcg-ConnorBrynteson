#include "GeneratedTests.hpp"
#include "SparseMatrix.hpp"
#include "Vector.hpp"
#include "Geometry.hpp"
#include "GenerateGeometry.hpp"
#include <vector>
#include <iostream>

/**
 * runAll
 * Purpose: Runs all tests and collects the results.
 */
std::vector<TestResult> runAll(int size, int rank, int numThreads, int pz,
                               local_int_t zl, local_int_t zu) {
    std::vector<TestResult> results;
    int npx = 2, npy = 2, npz = 2;
    // Validate grid dimensions
    if (npx <= 0 || npy <= 0 || npz <= 0) {
        std::cerr << "Error: Grid dimensions (npx, npy, npz) must be greater than zero." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Local grid dimensions
    local_int_t nx = size / npx;
    local_int_t ny = size / npy;
    local_int_t nz = size / npz;

    // Create and initialize geometry
    Geometry geometry;
    GenerateGeometry(size, rank, numThreads, pz, zl, zu, nx, ny, nz, npx, npy, npz, &geometry);

    // Initialize SparseMatrix
    SparseMatrix testMatrix;
    InitializeSparseMatrix(testMatrix, &geometry);

    // Ensure input vector size matches matrix columns
    Vector inputVector;
    InitializeVector(inputVector, testMatrix.localNumberOfColumns);

    // Ensure output vector size matches matrix rows
    Vector expectedResult;
    InitializeVector(expectedResult, testMatrix.localNumberOfRows);

    // Populate input vector with example values
    for (local_int_t i = 0; i < inputVector.localLength; ++i) {
        inputVector.values[i] = 1.0;  // Example input values
    }

    // Run tests and collect results
    results.push_back(TestZeroMatrixVectorMultiplication(testMatrix, inputVector));
    results.push_back(TestSparseMatrixEdgeCases(testMatrix, inputVector));
    results.push_back(TestLargeMatrixVectorMultiplication(testMatrix, inputVector, expectedResult));
    results.push_back(TestRandomMatrixVectorMultiplication(testMatrix, inputVector, expectedResult));
    results.push_back(TestIdentityMatrixVectorMultiplication(testMatrix, inputVector));

    return results;
}
