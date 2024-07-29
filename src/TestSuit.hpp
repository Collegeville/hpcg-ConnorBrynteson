#ifndef TESTSUIT_HPP
#define TESTSUIT_HPP
#include "Geometry.hpp"
#include "TestResult.hpp"
#include <vector>
std::vector<TestResult> runAll(int size, int rank, int numThreads, int pz,
                               local_int_t zl, local_int_t zu, int npx, int npy,
                               int npz);

#endif // !TESTSUIT_HPP
