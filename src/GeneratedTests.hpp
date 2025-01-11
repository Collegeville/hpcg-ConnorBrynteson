#ifndef GENERATED_TESTS_HPP
#define GENERATED_TESTS_HPP

#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include "Vector.hpp"

extern TestResult TestComputeResidual(const Vector &v1, const Vector &v2);
extern TestResult TestSetupHalo(SparseMatrix &matrix);
TestResult TestComputeSPMV1(const SparseMatrix &matrix,
                            const Vector &inputVector);
extern TestResult TestComputeMG(const SparseMatrix &matrix, const Vector &rhs,
                                Vector &solution);

/**
 * TestStringManipulation
 * Purpose: Validate the generation of string-based outputs for reporting.
 */
TestResult TestStringManipulation();

/**
 * TestIteratorManipulation
 * Purpose: Validate iterator-based traversal and operations.
 */
TestResult TestIteratorManipulation();

/**
 * TestEmptyAndLargeVector
 * Purpose: Validate operations on empty and large vectors.
 */
TestResult TestEmptyAndLargeVector();

/**
 * TestLocaleSpecificOutput
 * Purpose: Validate locale-specific formatting in output streams.
 */
TestResult TestLocaleSpecificOutput();

TestResult TestSparseMatrixWithDiagonalDominance();
TestResult TestVectorInitializationErrors();
TestResult TestSparseMatrixNullPointer();
TestResult TestLargeSparseMatrix();
TestResult TestBoundaryVectorInitialization();

#endif // GENERATED_TESTS_HPP
