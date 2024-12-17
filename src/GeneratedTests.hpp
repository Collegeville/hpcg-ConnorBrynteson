#ifndef GENERATED_TESTS_HPP
#define GENERATED_TESTS_HPP

#include "SparseMatrix.hpp"
#include "Vector.hpp"
#include "TestResult.hpp"

TestResult TestZeroMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector);
TestResult TestSparseMatrixEdgeCases(const SparseMatrix &matrix, const Vector &inputVector);
TestResult TestLargeMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector, const Vector &expectedResult);
TestResult TestRandomMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector, const Vector &expectedResult);
TestResult TestIdentityMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector);

#endif // GENERATED_TESTS_HPP

