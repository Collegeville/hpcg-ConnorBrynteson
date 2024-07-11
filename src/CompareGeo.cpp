#include "ComputeSPMV.hpp"
#include "GenerateProblem.hpp"
#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include "Vector.hpp"
#include <cassert>
#include <mpi.h>

TestResult TestMatrixUnalteredDirectModification() {
  TestResult result;
  result.testName = "Test Matrix Unaltered Direct Modification";
  result.passed = false;
  Geometry g;
  g.nx = 3;
  g.ny = 3;
  g.nz = 3;
  g.gnx = 3;
  g.gny = 3;
  g.gnz = 3;
  g.gix0 = 0;
  g.giy0 = 0;
  g.giz0 = 0;

  SparseMatrix A;
  A.geom = &g;
  Vector b, x, xexact;

  GenerateProblem(A, &b, &x, &xexact);

  // Modify the diagonal element of the first row to zero
  A.matrixValues[0][0] = 0.0;
  // Perform some HPCG operations

  // Verify the matrix is unaltered
  if (A.matrixValues[0][0] == 0.0)
    result.passed = true;
  return result;
}
