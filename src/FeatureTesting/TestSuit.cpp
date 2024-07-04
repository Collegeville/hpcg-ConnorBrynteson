#include <cassert>
#include <iostream>
#include <fstream>
#include <mpi.h>

#include "BuildingResult.hpp"
#include "CompareGeo.hpp"
int main(int argc, char *argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Run your test function
    bool result = TestMatrixGeneration();

    // Finalize MPI
    MPI_Finalize();

    return result ? 0 : 1;
}
