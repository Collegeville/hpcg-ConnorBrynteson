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

  // SparseMatrix Tests
  results.push_back(TestSparseMatrix(true, false, false)); // Diagonal
  // Dominance
  // results.push_back(TestSparseMatrix(false, true, false)); // Large
  // Dimensions
  results.push_back(TestSparseMatrix(false, false, true)); // Null
  // Pointer

  // Vector Tests
  // results.push_back(TestVectorInitializationEdgeCases());
  results.push_back(TestVectorExtremeValues());

  // Residual Tests
  Vector v1, v2;
  InitializeVector(v1, 10);
  InitializeVector(v2, 10);
  for (local_int_t i = 0; i < v1.localLength; ++i) {
    v1.values[i] = i;
    v2.values[i] = i + 1;
  }
  results.push_back(TestComputeResidual(v1, v2));
  DeleteVector(v1);
  DeleteVector(v2);

  // STL and Locale Tests
  results.push_back(TestStringManipulation());
  results.push_back(TestLocaleSpecificOutput());
  results.push_back(TestLargeSTLContainer());
  results.push_back(TestLocaleSpecificFormatting());
  results.push_back(TestExtendedLocaleSpecificFormatting());
  return results;
}
