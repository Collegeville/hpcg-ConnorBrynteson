#include "TestSuit.hpp"
#include "CompareGeo.hpp"
#include "GeoTest.hpp"
#include "Geometry.hpp"
#include "TestResult.hpp"
#include "hpcg.hpp"
#include <cassert>
#include <cstring>
#include <mpi.h>
#include <vector>

std::vector<TestResult> runAll(int size, int rank, int numThreads, int pz,
                               local_int_t zl, local_int_t zu, int npx, int npy,
                               int npz) {
  std::vector<TestResult> results;
  results.push_back(
      TestNonUniformMatrix(size, rank, numThreads, pz, zl, zu, npx, npy, npz));
  results.push_back(TestGeometryInitialization());
  results.push_back(TestSparseMatrixInitialization());
  return results;
}
