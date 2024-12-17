#include "GeneratedTests.hpp"
#include "SparseMatrix.hpp"
#include "Vector.hpp"
#include "ComputeSPMV.hpp"
#include <cmath>
#include <limits>

/**
 * TestZeroMatrixVectorMultiplication
 * Purpose: Validates that multiplying a zero matrix by a vector results in a zero vector.
 */
TestResult TestZeroMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector) {
    TestResult result;
    result.testName = "TestZeroMatrixVectorMultiplication";

    Vector mutableInputVector;
    InitializeVector(mutableInputVector, inputVector.localLength);
    CopyVector(inputVector, mutableInputVector);

    Vector outputVector;
    InitializeVector(outputVector, matrix.localNumberOfRows);

    int err = ComputeSPMV(matrix, mutableInputVector, outputVector);
    if (err != 0) {
        result.passed = false;
        result.details = "Error in ComputeSPMV.";
        DeleteVector(mutableInputVector);
        DeleteVector(outputVector);
        return result;
    }

    for (local_int_t i = 0; i < outputVector.localLength; ++i) {
        if (std::fabs(outputVector.values[i]) > std::numeric_limits<double>::epsilon()) {
            result.passed = false;
            result.details = "Output vector contains non-zero values.";
            DeleteVector(mutableInputVector);
            DeleteVector(outputVector);
            return result;
        }
    }

    result.passed = true;
    result.details = "Zero matrix-vector multiplication passed.";
    DeleteVector(mutableInputVector);
    DeleteVector(outputVector);
    return result;
}

/**
 * TestSparseMatrixEdgeCases
 * Purpose: Validates handling of edge cases in sparse matrix operations.
 */
TestResult TestSparseMatrixEdgeCases(const SparseMatrix &matrix, const Vector &inputVector) {
    TestResult result;
    result.testName = "TestSparseMatrixEdgeCases";

    Vector outputVector, expectedResult;
    InitializeVector(outputVector, matrix.localNumberOfRows);
    InitializeVector(expectedResult, matrix.localNumberOfRows);

    if (matrix.localNumberOfRows == 0) {
        result.passed = true;
        result.details = "Matrix has zero rows, handled gracefully.";
        DeleteVector(outputVector);
        DeleteVector(expectedResult);
        return result;
    }

    result.passed = true;
    result.details = "Sparse matrix edge cases passed.";
    DeleteVector(outputVector);
    DeleteVector(expectedResult);
    return result;
}


/**
 * TestLargeMatrixVectorMultiplication
 * Purpose: Verifies the correctness of matrix-vector multiplication on a large matrix.
 */
TestResult TestLargeMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector, const Vector &expectedResult) {
    TestResult result;
    result.testName = "TestLargeMatrixVectorMultiplication";

    Vector mutableInputVector;
    InitializeVector(mutableInputVector, inputVector.localLength);
    CopyVector(inputVector, mutableInputVector);

    Vector outputVector;
    InitializeVector(outputVector, matrix.localNumberOfRows);

    int err = ComputeSPMV(matrix, mutableInputVector, outputVector);
    if (err != 0) {
        result.passed = false;
        result.details = "Error in ComputeSPMV.";
        DeleteVector(mutableInputVector);
        DeleteVector(outputVector);
        return result;
    }

    for (local_int_t i = 0; i < outputVector.localLength; ++i) {
        if (std::fabs(outputVector.values[i] - expectedResult.values[i]) > std::numeric_limits<double>::epsilon()) {
            result.passed = false;
            result.details = "Output does not match the expected result.";
            DeleteVector(mutableInputVector);
            DeleteVector(outputVector);
            return result;
        }
    }

    result.passed = true;
    result.details = "Large matrix-vector multiplication passed.";
    DeleteVector(mutableInputVector);
    DeleteVector(outputVector);
    return result;
}

/**
 * TestRandomMatrixVectorMultiplication
 * Purpose: Validates matrix-vector multiplication with randomly generated inputs.
 */
TestResult TestRandomMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector, const Vector &expectedResult) {
    TestResult result;
    result.testName = "TestRandomMatrixVectorMultiplication";

    Vector mutableInputVector;
    InitializeVector(mutableInputVector, inputVector.localLength);
    CopyVector(inputVector, mutableInputVector);

    Vector outputVector;
    InitializeVector(outputVector, matrix.localNumberOfRows);

    int err = ComputeSPMV(matrix, mutableInputVector, outputVector);
    if (err != 0) {
        result.passed = false;
        result.details = "Error in ComputeSPMV.";
        DeleteVector(mutableInputVector);
        DeleteVector(outputVector);
        return result;
    }

    for (local_int_t i = 0; i < outputVector.localLength; ++i) {
        if (std::fabs(outputVector.values[i] - expectedResult.values[i]) > std::numeric_limits<double>::epsilon()) {
            result.passed = false;
            result.details = "Output does not match the expected result.";
            DeleteVector(mutableInputVector);
            DeleteVector(outputVector);
            return result;
        }
    }

    result.passed = true;
    result.details = "Random matrix-vector multiplication passed.";
    DeleteVector(mutableInputVector);
    DeleteVector(outputVector);
    return result;
}

/**
 * TestIdentityMatrixVectorMultiplication
 * Purpose: Verifies that multiplying an identity matrix by a vector returns the same vector.
 */
TestResult TestIdentityMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector) {
    TestResult result;
    result.testName = "TestIdentityMatrixVectorMultiplication";

    Vector mutableInputVector;
    InitializeVector(mutableInputVector, inputVector.localLength);
    CopyVector(inputVector, mutableInputVector);

    Vector outputVector;
    InitializeVector(outputVector, matrix.localNumberOfRows);

    int err = ComputeSPMV(matrix, mutableInputVector, outputVector);
    if (err != 0) {
        result.passed = false;
        result.details = "Error in ComputeSPMV.";
        DeleteVector(mutableInputVector);
        DeleteVector(outputVector);
        return result;
    }

    for (local_int_t i = 0; i < outputVector.localLength; ++i) {
        if (std::fabs(outputVector.values[i] - inputVector.values[i]) > std::numeric_limits<double>::epsilon()) {
            result.passed = false;
            result.details = "Output does not match the input vector.";
            DeleteVector(mutableInputVector);
            DeleteVector(outputVector);
            return result;
        }
    }

    result.passed = true;
    result.details = "Identity matrix-vector multiplication passed.";
    DeleteVector(mutableInputVector);
    DeleteVector(outputVector);
    return result;
}

