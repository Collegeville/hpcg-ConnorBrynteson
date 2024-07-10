#include "TestSuit.hpp"
#include "CompareGeo.hpp"
#include "GeoTest.hpp"
#include "TestResult.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <mpi.h>
#include <vector>

std::vector<TestResult> runAll() {
  std::vector<TestResult> results;
  results.push_back(TestMatrixUnalteredDirectModification());
  results.push_back(TestGeometryInitialization());
  results.push_back(TestSparseMatrixInitialization());
  return results;
}
