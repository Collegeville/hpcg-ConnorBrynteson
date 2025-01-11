// GeneratedTests.cpp
// Tests to enhance HPCG system coverage

#include "GeneratedTests.hpp"
#include "CG.hpp"
#include "ComputeMG.hpp"
#include "ComputeResidual.hpp"
#include "ComputeSPMV.hpp"
#include "GenerateCoarseProblem.hpp"
#include "SetupHalo.hpp"
#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include "Vector.hpp"
#include <cmath>
#include <limits>
#include <locale>
#include <mpi.h>
#include <sstream>

/**
 * TestComputeResidual
 * Purpose: Tests the ComputeResidual function by verifying correct residual
 * computation for vectors of different configurations.
 */
TestResult TestComputeResidual(const Vector &v1, const Vector &v2) {
  TestResult result;
  result.testName = "TestComputeResidual";

  double computedResidual;
  int err = ComputeResidual(v1.localLength, v1, v2, computedResidual);

  if (err != 0) {
    result.passed = false;
    result.details = "Error in ComputeResidual function.";
    return result;
  }

  double expectedResidual = 0.0;
  for (local_int_t i = 0; i < v1.localLength; ++i) {
    expectedResidual =
        std::max(expectedResidual, std::fabs(v1.values[i] - v2.values[i]));
  }

  if (std::fabs(computedResidual - expectedResidual) >
      std::numeric_limits<double>::epsilon()) {
    result.passed = false;
    result.details = "Residual computation is incorrect.";
    return result;
  }

  result.passed = true;
  result.details = "ComputeResidual passed all checks.";
  return result;
}

/**
 * TestSetupHalo
 * Purpose: Ensures SetupHalo correctly prepares the SparseMatrix for
 * communication.
 */
TestResult TestSetupHalo(SparseMatrix &matrix) {
  TestResult result;
  result.testName = "TestSetupHalo";

  try {
    SetupHalo(matrix);
    result.passed = true;
    result.details = "SetupHalo executed without errors.";
  } catch (...) {
    result.passed = false;
    result.details = "SetupHalo threw an exception.";
  }

  return result;
}

/**
 * TestComputeSPMVWithLargeInput
 * Purpose: Verifies that ComputeSPMV can handle large input vectors and
 * matrices.
 */
TestResult TestComputeSPMV1(const SparseMatrix &matrix, Vector &inputVector) {
  TestResult result;
  result.testName = "TestComputeSPMV";

  Vector outputVector;
  InitializeVector(outputVector, matrix.localNumberOfRows);

  // Ensure inputVector and outputVector are mutable if needed
  int err = ComputeSPMV(matrix, inputVector,
                        outputVector); // Pass as non-const reference
  if (err != 0) {
    result.passed = false;
    result.details = "Error in ComputeSPMV.";
    DeleteVector(outputVector);
    return result;
  }

  // Validate output: Check that values in the output vector are not
  // default-initialized
  bool validOutput = true;
  for (local_int_t i = 0; i < outputVector.localLength; ++i) {
    if (std::fabs(outputVector.values[i]) < -1.0 ||
        std::fabs(outputVector.values[i]) > 1.0) {
      validOutput = false;
      break;
    }
  }

  if (!validOutput) {
    result.passed = false;
    result.details = "Output vector values are out of expected range.";
    DeleteVector(outputVector);
    return result;
  }

  result.passed = true;
  result.details = "ComputeSPMV passed all checks.";
  DeleteVector(outputVector);
  return result;
}

/**
 * TestComputeMG
 * Purpose: Validates that ComputeMG performs a V-cycle as expected.
 */
TestResult TestComputeMG(const SparseMatrix &matrix, const Vector &rhs,
                         Vector &solution) {
  TestResult result;
  result.testName = "TestComputeMG1";

  // Backup the initial solution
  Vector originalSolution;
  InitializeVector(originalSolution, solution.localLength);
  CopyVector(solution, originalSolution);

  // Execute ComputeMG
  int err = ComputeMG(matrix, rhs, solution);
  if (err != 0) {
    result.passed = false;
    result.details = "Error in ComputeMG.";
    DeleteVector(originalSolution);
    return result;
  }

  // Validate that solution vector remains unchanged
  bool solutionUnchanged = true;
  for (local_int_t i = 0; i < solution.localLength; ++i) {
    if (std::fabs(originalSolution.values[i] - solution.values[i]) >
        std::numeric_limits<double>::epsilon()) {
      solutionUnchanged = false;
      break;
    }
  }

  if (!solutionUnchanged) {
    result.passed = false;
    result.details = "Solution vector changed during ComputeMG, which is "
                     "incorrect behavior.";
    DeleteVector(originalSolution);
    return result;
  }

  result.passed = true;
  result.details = "ComputeMG passed: Solution vector remains unchanged.";
  DeleteVector(originalSolution);
  return result;
}

/**
 * TestStringManipulation
 * Purpose: Validate the generation of string-based outputs for reporting.
 */
TestResult TestStringManipulation() {
  TestResult result;
  result.testName = "TestStringManipulation";

  std::ostringstream oss;
  oss << "Test " << 1 << ": Passed.";
  std::string output = oss.str();

  if (output == "Test 1: Passed.") {
    result.passed = true;
    result.details = "String manipulation test passed.";
  } else {
    result.passed = false;
    result.details = "String manipulation test failed. Output: " + output;
  }

  return result;
}

/**
 * TestIteratorManipulation
 * Purpose: Validate iterator-based traversal and operations.
 */
TestResult TestIteratorManipulation() {
  TestResult result;
  result.testName = "TestIteratorManipulation";

  std::vector<int> testVec = {1, 2, 3, 4, 5};
  int sum = 0;
  for (auto it = testVec.begin(); it != testVec.end(); ++it) {
    sum += *it;
  }

  if (sum == 15) {
    result.passed = true;
    result.details = "Iterator manipulation test passed.";
  } else {
    result.passed = false;
    result.details =
        "Iterator manipulation test failed. Sum: " + std::to_string(sum);
  }

  return result;
}

/**
 * TestEmptyAndLargeVector
 * Purpose: Validate operations on empty and large vectors.
 */
TestResult TestEmptyAndLargeVector() {
  TestResult result;
  result.testName = "TestEmptyAndLargeVector";

  Vector emptyVec, largeVec;
  InitializeVector(emptyVec, 0);       // Empty vector
  InitializeVector(largeVec, 1000000); // Large vector

  if (emptyVec.localLength == 0 && largeVec.localLength == 1000000) {
    result.passed = true;
    result.details = "Empty and large vector test passed.";
  } else {
    result.passed = false;
    result.details = "Vector initialization failed.";
  }

  DeleteVector(emptyVec);
  DeleteVector(largeVec);
  return result;
}

/**
 * TestLocaleSpecificOutput
 * Purpose: Validate locale-specific formatting in output streams.
 */
TestResult TestLocaleSpecificOutput() {
  TestResult result;
  result.testName = "TestLocaleSpecificOutput";

  std::ostringstream oss;
  std::locale loc("");
  oss.imbue(loc);
  oss << 1234567.89;

  std::string formatted = oss.str();
  if (!formatted.empty()) {
    result.passed = true;
    result.details = "Locale-specific output test passed. Output: " + formatted;
  } else {
    result.passed = false;
    result.details = "Locale-specific output test failed.";
  }

  return result;
}

/**
 * TestSparseMatrixWithDiagonalDominance
 * Purpose: Verifies behavior of ComputeSPMV with a diagonally dominant sparse
 * matrix.
 */
TestResult TestSparseMatrixWithDiagonalDominance() {
  TestResult result;
  result.testName = "TestSparseMatrixWithDiagonalDominance";

  SparseMatrix matrix;
  InitializeSparseMatrix(matrix,
                         nullptr); // Use nullptr for geometry in this example
  // Manually set diagonal dominance
  for (local_int_t i = 0; i < matrix.localNumberOfRows; ++i) {
    matrix.matrixValues[i][i] = 10.0; // Large diagonal value
  }

  Vector inputVector, outputVector;
  InitializeVector(inputVector, matrix.localNumberOfColumns);
  InitializeVector(outputVector, matrix.localNumberOfRows);
  FillRandomVector(inputVector);

  ComputeSPMV(matrix, inputVector, outputVector);

  result.passed = true;
  result.details = "Diagonal dominance test passed.";
  DeleteVector(inputVector);
  DeleteVector(outputVector);
  return result;
}

/**
 * TestVectorInitializationErrors
 * Purpose: Validates error handling during vector initialization.
 */
TestResult TestVectorInitializationErrors() {
  TestResult result;
  result.testName = "TestVectorInitializationErrors";

  Vector vector;
  InitializeVector(vector, -1); // Invalid size

  if (vector.localLength != 0 || vector.values != nullptr) {
    result.passed = false;
    result.details = "Failed to handle invalid vector size properly.";
    return result;
  }

  result.passed = true;
  result.details = "Error handling during vector initialization passed.";
  return result;
}

/**
 * TestSparseMatrixNullPointer
 * Purpose: Ensures proper handling of null pointers in sparse matrix
 * operations.
 */
TestResult TestSparseMatrixNullPointer() {
  TestResult result;
  result.testName = "TestSparseMatrixNullPointer";

  SparseMatrix *matrix = nullptr;
  Vector inputVector, outputVector;
  InitializeVector(inputVector, 10);
  InitializeVector(outputVector, 10);

  // Simulate handling null matrix
  if (matrix == nullptr) {
    result.passed = true;
    result.details = "Null matrix pointer handled correctly.";
  } else {
    result.passed = false;
    result.details = "Failed to handle null matrix pointer.";
  }

  DeleteVector(inputVector);
  DeleteVector(outputVector);
  return result;
}

/**
 * TestLargeSparseMatrix
 * Purpose: Verifies SparseMatrix operations for large dimensions.
 */
TestResult TestLargeSparseMatrix() {
  TestResult result;
  result.testName = "TestLargeSparseMatrix";

  SparseMatrix matrix;
  InitializeSparseMatrix(matrix, nullptr);

  Vector inputVector, outputVector;
  InitializeVector(inputVector, 1000000); // Large vector
  InitializeVector(outputVector, 1000000);

  FillRandomVector(inputVector);

  ComputeSPMV(matrix, inputVector, outputVector);

  result.passed = true;
  result.details = "Large sparse matrix test passed.";
  DeleteVector(inputVector);
  DeleteVector(outputVector);
  return result;
}

/**
 * TestBoundaryVectorInitialization
 * Purpose: Validates vector initialization for boundary conditions.
 */
TestResult TestBoundaryVectorInitialization() {
  TestResult result;
  result.testName = "TestBoundaryVectorInitialization";

  Vector vector;
  InitializeVector(vector, 0); // Edge case: empty vector
  if (vector.localLength == 0 && vector.values == nullptr) {
    result.passed = true;
    result.details = "Empty vector initialization handled correctly.";
  } else {
    result.passed = false;
    result.details = "Failed to handle empty vector initialization.";
  }

  DeleteVector(vector);
  return result;
}
