#ifndef TESTSUIT_HPP
#define TESTSUIT_HPP
#include "Geometry.hpp"
#include "TestResult.hpp"
#include "GeneratedTests.cpp"
#include <vector>
std::vector<TestResult> runAll(int size, int rank, int numThreads, int pz,
                               local_int_t zl, local_int_t zu);

#endif // !TESTSUIT_HPP
