#include "GenerateGeometry.hpp"
#include "GeneratedTests.hpp"
#include "Geometry.hpp"
#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include "Vector.hpp"
#include <iostream>
#include <vector>

TestResult TestComputeSPMV1(const SparseMatrix &matrix,
                            const Vector &inputVector);

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
    std::exit(EXIT_FAILURE);
  }

  // Local grid dimensions
  local_int_t nx = size / npx;
  local_int_t ny = size / npy;
  local_int_t nz = size / npz;

  // Create and initialize geometry
  Geometry geometry;
  GenerateGeometry(size, rank, numThreads, pz, zl, zu, nx, ny, nz, npx, npy,
                   npz, &geometry);

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
    inputVector.values[i] = 1.0; // Example input values
  }

  // Run tests and collect results
  results.push_back(TestComputeResidual(expectedResult, inputVector));
  results.push_back(TestSetupHalo(testMatrix));
  results.push_back(TestComputeMG(testMatrix, inputVector, expectedResult));
  results.push_back(TestStringManipulation());
  results.push_back(TestIteratorManipulation());
  results.push_back(TestEmptyAndLargeVector());
  results.push_back(TestLocaleSpecificOutput());
  results.push_back(TestSparseMatrixWithDiagonalDominance());
  // results.push_back(TestVectorInitializationErrors());
  results.push_back(TestSparseMatrixNullPointer());
  // results.push_back(TestLargeSparseMatrix());
  results.push_back(TestBoundaryVectorInitialization());
  return results;
}
