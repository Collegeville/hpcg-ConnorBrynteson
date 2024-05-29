
//@HEADER
// ************************************************************************
//
//               HPCG: Simple Conjugate Gradient Benchmark Code
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ************************************************************************
//@HEADER

/*!
 @file main.cpp

 */

#include <iostream>
#include "GenerateGeometry.hpp"
// #include "hpcg.hpp"
int main(int argc, char *argv[])
{
  /*
    HPCG_Params params;

    HPCG_Init(&argc, &argv, params);

    int size = params.comm_size, rank = params.comm_rank; // Number of MPI processes, My process ID


    local_int_t nx,ny,nz;
    nx = (local_int_t)params.nx;
    ny = (local_int_t)params.ny;
    nz = (local_int_t)params.nz;
    int ierr = 0;  // Used to check return codes on function calls
  */

  // Construct the geometry and linear system
  Geometry geom;
  int size = 2048;
  int rank = 2047;
  int numThreads = 1;
  int nx, ny, nz;
  nx = ny = nz = 200;
  /*
  Missing the ints should be 12 values
  Due to the fact that you can not mix default arguments with non default arguments in C++ like GenerateGeometry(size, rank, numThreads, defualt, default, default, nx, ny, nz, default, default, default, geomPtr)
  Setting up the default values for the function call here will the best
  GenerateGeometry(size, rank, params.numThreads, params.pz, params.zl, params.zu, nx, ny, nz, params.npx, params.npy, params.npz, geom);
  */
  GenerateGeometry(size, rank, numThreads, 0, 0, 0, nx, ny, nz, 0, 0, 0, &geom);

  global_int_t index = ((global_int_t)(nx * ny * nz)) * size - 1;
  std::cout << "Global Index = " << index << std::endl;
  int owningRank = ComputeRankOfMatrixRow(geom, index);
  std::cout << "Owning rank should be " << size - 1 << ". Computed to be " << owningRank << "." << std::endl;

  // HPCG_Finalize();

  return 0;
}
