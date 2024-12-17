#ifndef GENERATED_TESTS_HPP
#define GENERATED_TESTS_HPP

#include "SparseMatrix.hpp"
#include "Vector.hpp"
#include "TestResult.hpp"

extern TestResult TestZeroMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector);
extern TestResult TestSparseMatrixEdgeCases(const SparseMatrix &matrix, const Vector &inputVector);
extern TestResult TestLargeMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector, const Vector &expectedResult);
extern TestResult TestRandomMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector, const Vector &expectedResult);
extern TestResult TestIdentityMatrixVectorMultiplication(const SparseMatrix &matrix, const Vector &inputVector);

#endif // GENERATED_TESTS_HPP

