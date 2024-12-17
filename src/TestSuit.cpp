#include "GeneratedTests.hpp"
#include "SparseMatrix.hpp"
#include "Vector.hpp"
#include "GenerateGeometry.hpp"
#include "Geometry.hpp"  // Include the Geometry definition
#include <vector>

std::vector<TestResult> runAll(int size, int rank, int numThreads, int pz,
                               local_int_t zl, local_int_t zu, int npx, int npy,
                               int npz) {
    std::vector<TestResult> results;

    // Create and initialize geometry
    Geometry *geometry = new Geometry;
    local_int_t nx = size / npx;
    local_int_t ny = size / npy;
    local_int_t nz = size / npz;

    GenerateGeometry(size, rank, numThreads, pz, zl, zu, nx, ny, nz, npx, npy, npz, geometry);

    // Initialize matrix and vector
    SparseMatrix testMatrix;
    InitializeSparseMatrix(testMatrix, geometry);  // Pass Geometry pointer

    Vector testVector;
    InitializeVector(testVector, size);

    // Run tests
    results.push_back(TestZeroMatrixVectorMultiplication(testMatrix, testVector));
    results.push_back(TestSparseMatrixEdgeCases(testMatrix, testVector));

    // Example expected result vector
    Vector expectedResult;
    InitializeVector(expectedResult, size);
    for (local_int_t i = 0; i < expectedResult.localLength; ++i) {
        expectedResult.values[i] = 1.0;  // Dummy expected values
    }

    results.push_back(TestLargeMatrixVectorMultiplication(testMatrix, testVector, expectedResult));
    results.push_back(TestRandomMatrixVectorMultiplication(testMatrix, testVector, expectedResult));
    results.push_back(TestIdentityMatrixVectorMultiplication(testMatrix, testVector));

    return results;
}

