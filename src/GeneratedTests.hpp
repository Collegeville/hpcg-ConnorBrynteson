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

#endif // GENERATED_TESTS_HPP
