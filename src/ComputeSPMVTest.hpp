
//@HEADER
// ***************************************************
//
// HPCG: High Performance Conjugate Gradient Benchmark
//
// Contact:
// Michael A. Heroux ( maherou@sandia.gov)
// Jack Dongarra     (dongarra@eecs.utk.edu)
// Piotr Luszczek    (luszczek@eecs.utk.edu)
//
// ***************************************************
//@HEADER

#ifndef COMPUTESPMVTEST_HPP
#define COMPUTESPMVTEST_HPP
#include "SparseMatrix.hpp"
#include "Vector.hpp"

int ComputeSPMVTest(const SparseMatrix &A, Vector &x, Vector &y);

#endif // COMPUTESPMV_REF_HPP
