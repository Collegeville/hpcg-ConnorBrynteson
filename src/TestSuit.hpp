#ifndef TESTSUIT_HPP
#define TESTSUIT_HPP
#include "ComputeSPMVTest.hpp"
#include "GeneratedTests.cpp"
#include "Geometry.hpp"
#include "TestResult.hpp"
#include <vector>

TestResult TestComputeSPMV1(const SparseMatrix &matrix, Vector &inputVector);
std::vector<TestResult> runAll(int size, int rank, int numThreads, int pz,
                               local_int_t zl, local_int_t zu);

#endif // !TESTSUIT_HPP
