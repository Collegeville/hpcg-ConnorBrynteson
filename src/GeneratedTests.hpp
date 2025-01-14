#ifndef GENERATED_TESTS_HPP
#define GENERATED_TESTS_HPP

#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include "Vector.hpp"

/**
 * TestSparseMatrix
 * Purpose: Validates SparseMatrix operations for diagonal dominance, large
 * dimensions, and null pointers.
 * @param diagonalDominance Whether to test with a diagonally dominant matrix.
 * @param largeDimensions Whether to test with large matrix dimensions.
 * @param nullPointer Whether to test null pointer handling.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestSparseMatrix(bool diagonalDominance, bool largeDimensions,
                            bool nullPointer);

/**
 * TestVectorInitializationEdgeCases
 * Purpose: Validates vector initialization for boundary conditions and large
 * sizes.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestVectorInitializationEdgeCases();

/**
 * TestComputeResidual
 * Purpose: Tests the ComputeResidual function by verifying correct residual
 * computation for vectors of different configurations.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestComputeResidual(const Vector &v1, const Vector &v2);

/**
 * TestStringManipulation
 * Purpose: Validate the generation of string-based outputs for reporting.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestStringManipulation();

/**
 * TestLocaleSpecificOutput
 * Purpose: Validate locale-specific formatting in output streams.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestLocaleSpecificOutput();

/**
 * TestVectorExtremeValues
 * Purpose: Validates vector operations involving extreme values.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestVectorExtremeValues();

/**
 * TestLargeSTLContainer
 * Purpose: Verifies that large STL containers function correctly.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestLargeSTLContainer();

/**
 * TestLocaleSpecificFormatting
 * Purpose: Validates locale-specific number formatting in output streams.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestLocaleSpecificFormatting();

/**
 * TestExtendedLocaleSpecificFormatting
 * Purpose: Validates locale-specific number formatting for different locales.
 * @return TestResult containing the outcome of the test.
 */
TestResult TestExtendedLocaleSpecificFormatting();

#endif // GENERATED_TESTS_HPP
