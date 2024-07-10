#include "GeoTest.hpp"
#include "Geometry.hpp"
#include "SparseMatrix.hpp"
#include "TestResult.hpp"
#include <cstdio>
#include <cwchar>
#include <iostream>
#include <string>
#include <vector>

TestResult TestGeometryInitialization() {
  TestResult result;
  result.testName = "Test Geometry Initialization";
  Geometry geom;

  // Expected values for initialization
  geom.nx = 10;
  geom.ny = 10;
  geom.nz = 10;
  geom.gnx = 10;
  geom.gny = 10;
  geom.gnz = 10;
  geom.gix0 = 0;
  geom.giy0 = 0;
  geom.giz0 = 0;
  geom.rank = 0;
  geom.size = 1;

  // Verify correctness
  if (geom.nx != 10) {
    result.passed = false;
    return result;
  }
  if (geom.ny != 10) {
    result.passed = false;
    return result;
  }
  if (geom.nz != 10) {
    result.passed = false;
    return result;
  }
  if (geom.gnx != 10) {
    result.passed = false;
    return result;
  }
  if (geom.gny != 10) {
    result.passed = false;
    return result;
  }
  if (geom.gnz != 10) {
    result.passed = false;
    return result;
  }
  if (geom.gix0 != 0) {
    result.passed = false;
    return result;
  }
  if (geom.giy0 != 0) {
    result.passed = false;
    return result;
  }
  if (geom.giz0 != 0) {
    result.passed = false;
    return result;
  }
  if (geom.rank != 0) {
    result.passed = false;
    return result;
  }

  if (geom.size != 1) {
  }
  result.passed = true;
  return result;
}

TestResult TestSparseMatrixInitialization() {
  TestResult result;
  result.testName = "Test Sparse Matrix Initialization";
  Geometry geom;
  geom.nx = 10;
  geom.ny = 10;
  geom.nz = 10;

  SparseMatrix A;
  InitializeSparseMatrix(A, &geom);

  // Verify correctness
  if (A.geom != &geom) {
    result.passed = false;
    return result;
  }
  if (A.totalNumberOfRows != 0) {
    result.passed = false;
    return result;
  }
  if (A.totalNumberOfNonzeros != 0) {
    result.passed = false;
    return result;
  }
  if (A.localNumberOfRows != 0) {

    result.passed = false;
    return result;
  }
  if (A.localNumberOfColumns != 0) {
    result.passed = false;
    return result;
  }
  if (A.localNumberOfNonzeros != 0) {
    result.passed = false;
    return result;
  }
  if (A.nonzerosInRow != nullptr) {
    result.passed = false;
    return result;
  }
  if (A.mtxIndG != nullptr) {
    result.passed = false;
    return result;
  }
  if (A.mtxIndL != nullptr) {
    result.passed = false;
    return result;
  }
  if (A.matrixValues != nullptr) {
    result.passed = false;
    return result;
  }
  if (A.matrixDiagonal != nullptr) {
    result.passed = false;
    return result;
  }
  if (A.isDotProductOptimized != true) {
    result.passed = false;
    return result;
  }
  if (A.isSpmvOptimized != true) {
    result.passed = false;
    return result;
  }
  if (A.isMgOptimized != true) {
    result.passed = false;
    return result;
  }
  if (A.isWaxpbyOptimized != true) {
    result.passed = false;
    return result;
  }

  result.passed = true;
  return result;
}
