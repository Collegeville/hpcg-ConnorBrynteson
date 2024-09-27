#include "ComputeSPMVTest.hpp"
#include "GenerateGeometry.hpp"
#include "GenerateProblem.hpp"
#include "GenerateProblem_ref.hpp"
#include "Geometry.hpp"
#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include "Vector.hpp"
#include "hpcg.hpp"
#include <chrono>
// Changing the col index to a vaid index but one that would not follow the same
// pattern as HPCG
TestResult TestNonUniformMatrix(int size, int rank, int numThreads, int pz,
                                local_int_t zl, local_int_t zu, int npx,
                                int npy, int npz) {
  TestResult results;
  results.testName = "Testing Geometry";
  results.passed = true;
  HPCG_Params param;
  Geometry *geom = new Geometry;
  GenerateGeometry(size, rank, numThreads, pz, zl, zu, 64, 64, 64, npx, npy,
                   npz, geom);
  SparseMatrix A;
  InitializeSparseMatrix(A, geom);
  Vector b, x, xexact;
  GenerateProblem(A, &b, &x, &xexact);
  int changedRows = 0;
  for (int i = 0; i <= A.localNumberOfRows; i++) {
    int nonZeroA = A.nonzerosInRow[i];
    for (int j = 0; j < nonZeroA; ++j) {
      if (changedRows < 5) {
        A.matrixValues[i][j] = -10;
        changedRows++;
      }
    }
  }
  local_int_t nrow = A.localNumberOfRows;
  local_int_t ncol = A.localNumberOfColumns;

  Vector x_overlap, b_computed;
  InitializeVector(x_overlap, ncol); // Overlapped copy of x vector
  InitializeVector(b_computed, nrow);
  FillRandomVector(x_overlap);

  if (ComputeSPMVTest(A, x_overlap, b_computed) == -1) {
    results.passed = false;
  }
  return results;
}
