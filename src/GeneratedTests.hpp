#ifndef GENERATEDTESTS_HPP
#define GENERATEDTESTS_HPP

#include <cmath>
#include <stdexcept>
#include <vector>
#include "hpcg.hpp"  // Assumes this includes necessary HPCG structures and functions

// Initialize and finalize MPI for test execution
void initializeMPI(int &argc, char **&argv);
void finalizeMPI();

// Test functions returning 1 for pass and 0 for fail

/**
 * Tests precision and rounding handling in sparse matrix-vector multiplication.
 * @return 1 if the test passes, 0 if it fails.
 */
int testPrecisionAndRounding();

/**
 * Tests handling of zero-sized matrices.
 * @return 1 if the test passes, 0 if it fails.
 */
int testEdgeCaseZeroSizedMatrix();

/**
 * Tests handling of large matrices to check for memory management.
 * @return 1 if the test passes, 0 if it fails.
 */
int testLargeInputHandling();

/**
 * Tests consistency across different data types.
 * @return 1 if the test passes, 0 if it fails.
 */
int testDataTypeConsistency();

/**
 * Tests thread safety in sparse matrix-vector multiplication.
 * @return 1 if the test passes, 0 if it fails.
 */
int testThreadSafety();

/**
 * Tests solver functionality with boundary conditions.
 * @return 1 if the test passes, 0 if it fails.
 */
int testSolverBoundaryConditions();

/**
 * Tests matrix decomposition accuracy.
 * @return 1 if the test passes, 0 if it fails.
 */
int testMatrixDecomposition();

/**
 * Tests handling of non-standard floating-point values (NaN, Infinity).
 * @return 1 if the test passes, 0 if it fails.
 */
int testNonStandardValues();

/**
 * Tests output format consistency for vector sizes.
 * @return 1 if the test passes, 0 if it fails.
 */
int testOutputFormatConsistency();

/**
 * Tests enforcement of boundary conditions in the solver.
 * @return 1 if the test passes, 0 if it fails.
 */
int testBoundaryConditionsEnforcement();

#endif // HPCG_TESTS_HPP

