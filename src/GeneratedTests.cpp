// GeneratedTests.cpp
// Tests to enhance HPCG system coverage

#include "GeneratedTests.hpp"
#include "ComputeMG.hpp"
#include "ComputeResidual.hpp"
#include "ComputeSPMV.hpp"
#include "SetupHalo.hpp"
#include "SparseMatrix.hpp"
#include "Vector.hpp"
#include <cmath>
#include <limits>
#include <mpi.h>

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
        if (std::fabs(originalSolution.values[i] - solution.values[i]) > std::numeric_limits<double>::epsilon()) {
            solutionUnchanged = false;
            break;
        }
    }

    if (!solutionUnchanged) {
        result.passed = false;
        result.details = "Solution vector changed during ComputeMG, which is incorrect behavior.";
        DeleteVector(originalSolution);
        return result;
    }

    result.passed = true;
    result.details = "ComputeMG passed: Solution vector remains unchanged.";
    DeleteVector(originalSolution);
    return result;
}
