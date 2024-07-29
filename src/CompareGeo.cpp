#include "GenerateGeometry.hpp"
#include "GenerateProblem.hpp"
#include "GenerateProblem_ref.hpp"
#include "Geometry.hpp"
#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include "Vector.hpp"
#include "hpcg.hpp"
//Changing the col index to a vaid index but one that would not follow the same pattern as HPCG
TestResult TestNonUniformMatrix(int size, int rank, int numThreads, int pz,
                                local_int_t zl, local_int_t zu, int npx,
                                int npy, int npz) {
  TestResult results;
  results.testName = "Test Non-Uniform Matrix";
  results.passed = false;
  HPCG_Params param;
  Geometry *geom = new Geometry;
  GenerateGeometry(size, rank, numThreads, pz, zl, zu, 128, 256, 64, npx, npy,
                   npz, geom); 
  SparseMatrix A;
  SparseMatrix B;
  InitializeSparseMatrix(A, geom);
  Vector b, x, xexact;
  GenerateProblem_ref(A, &b, &x, &xexact);
  InitializeSparseMatrix(B, geom);
  GenerateProblem(B, &b, &x, &xexact);
  for (int i = 0; i <= A.localNumberOfRows; i++) {
    int nonZeroA = A.nonzerosInRow[i];
    for (int j = 0; j < nonZeroA; ++j) {
      double valueA = A.matrixValues[i][j];
      double valueB = B.matrixValues[i][j];
      if (valueA != valueB) {
        results.passed = false;
        return results;
      }
    }
  }
  results.passed = true;
  return results;
}
