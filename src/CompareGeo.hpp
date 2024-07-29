/*

Goal for morning is have it read two matrix into the files one at the start of
gen problem and one at then end then compare the two. This would allow us to
skip the hard stuff lol

*/

#ifndef COMPAREGEO_HPP
#define COMPAREGEO_HPP
#include "Geometry.hpp"
#include "SparseMatrix.hpp"
#include "TestResult.hpp"

TestResult TestNonUniformMatrix(int size, int rank, int numThreads, int pz,
                                local_int_t zl, local_int_t zu, int npx,
                                int npy, int npz);

#endif
