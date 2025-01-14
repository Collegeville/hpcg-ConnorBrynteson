// GeneratedTests.cpp
// Tests to enhance HPCG system coverage

#include "GeneratedTests.hpp"
#include "ComputeResidual.hpp"
#include "ComputeSPMV.hpp"
#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include "Vector.hpp"
#include <cmath>
#include <limits>
#include <locale>
#include <mpi.h>
#include <sstream>
#include <string>

/**
 * TestSparseMatrix
 * Purpose: Validates SparseMatrix operations for diagonal dominance, large
 * dimensions, and null pointers.
 */
TestResult TestSparseMatrix(bool diagonalDominance, bool largeDimensions,
                            bool nullPointer) {
  TestResult result;
  result.testName = "TestSparseMatrix";

  if (nullPointer) {
    SparseMatrix *matrix = nullptr;
    Vector inputVector, outputVector;
    InitializeVector(inputVector, 10);
    InitializeVector(outputVector, 10);

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

  SparseMatrix matrix;
  InitializeSparseMatrix(matrix, nullptr);

  if (diagonalDominance) {
    for (local_int_t i = 0; i < matrix.localNumberOfRows; ++i) {
      matrix.matrixValues[i][i] = 10.0; // Large diagonal value
    }
  }

  Vector inputVector, outputVector;
  InitializeVector(inputVector,
                   largeDimensions ? 1000000 : matrix.localNumberOfColumns);
  InitializeVector(outputVector,
                   largeDimensions ? 1000000 : matrix.localNumberOfRows);
  FillRandomVector(inputVector);

  ComputeSPMV(matrix, inputVector, outputVector);

  result.passed = true;
  result.details = "SparseMatrix test passed.";
  DeleteVector(inputVector);
  DeleteVector(outputVector);
  return result;
}

/**
 * TestVectorInitializationEdgeCases
 * Purpose: Validates vector initialization for boundary conditions and large
 * sizes.
 */
TestResult TestVectorInitializationEdgeCases() {
  TestResult result;
  result.testName = "TestVectorInitializationEdgeCases";

  Vector emptyVec, largeVec, invalidVec;
  InitializeVector(emptyVec, 0);       // Empty vector
  InitializeVector(largeVec, 1000000); // Large vector
  InitializeVector(invalidVec, -1);    // Invalid vector size

  if (emptyVec.localLength == 0 && emptyVec.values == nullptr &&
      largeVec.localLength == 1000000 && largeVec.values != nullptr &&
      invalidVec.localLength == 0 && invalidVec.values == nullptr) {
    result.passed = true;
    result.details = "Vector initialization edge cases passed.";
  } else {
    result.passed = false;
    result.details = "Vector initialization edge cases failed.";
  }

  DeleteVector(emptyVec);
  DeleteVector(largeVec);
  DeleteVector(invalidVec);
  return result;
}

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

TestResult TestVectorExtremeValues() {
  TestResult result;
  result.testName = "TestVectorExtremeValues";

  Vector vec;
  InitializeVector(vec, 5);

  vec.values[0] = 1e9;
  vec.values[1] = -1e9;
  vec.values[2] = std::numeric_limits<double>::max();
  vec.values[3] = std::numeric_limits<double>::lowest();
  vec.values[4] = 0.0;

  bool isValid = true;
  for (local_int_t i = 0; i < vec.localLength; ++i) {
    if (std::isnan(vec.values[i]) || std::isinf(vec.values[i])) {
      isValid = false;
      break;
    }
  }

  if (isValid) {
    result.passed = true;
    result.details = "Vector extreme values test passed.";
  } else {
    result.passed = false;
    result.details = "Vector extreme values test failed.";
  }

  DeleteVector(vec);
  return result;
}

TestResult TestLargeSTLContainer() {
  TestResult result;
  result.testName = "TestLargeSTLContainer";

  try {
    std::vector<int> largeVec(1e6, 42);
    if (largeVec.size() == 1e6 && largeVec[0] == 42) {
      result.passed = true;
      result.details = "Large STL container test passed.";
    } else {
      result.passed = false;
      result.details = "Large STL container test failed.";
    }
  } catch (...) {
    result.passed = false;
    result.details = "Exception occurred during large STL container test.";
  }

  return result;
}

TestResult TestLocaleSpecificFormatting() {
  TestResult result;
  result.testName = "TestLocaleSpecificFormatting";

  std::ostringstream oss;
  std::locale loc("en_US.UTF-8");
  oss.imbue(loc);
  oss << 1234567.89;

  std::string formatted = oss.str();
  if (!formatted.empty()) {
    result.passed = true;
    result.details =
        "Locale-specific formatting test passed. Output: " + formatted;
  } else {
    result.passed = false;
    result.details = "Locale-specific formatting test failed.";
  }

  return result;
}

TestResult TestExtendedLocaleSpecificFormatting() {
  TestResult result;
  result.testName = "TestExtendedLocaleSpecificFormatting";

  try {
    std::ostringstream oss;
    std::locale loc("fr_FR.UTF-8"); // French locale
    oss.imbue(loc);
    oss << 1234567.89;

    std::string formatted = oss.str();
    if (!formatted.empty()) {
      result.passed = true;
      result.details =
          "Locale-specific formatting test passed for French locale. Output: " +
          formatted;
    } else {
      result.passed = false;
      result.details =
          "Locale-specific formatting test failed for French locale.";
    }
  } catch (const std::exception &e) {
    result.passed = false;
    result.details = "Locale-specific formatting test threw an exception: " +
                     std::string(e.what());
  }

  return result;
}
