#include <cassert>
#include <iostream>
#include <fstream>

#include "BuildingResult.hpp"
#include "TestTest.hpp"

int main()
{
    // This is where we are going to be calling the main testing functions of this program
    // In addition to building the result of said run
    std::ofstream file("results.md", std::ios::out | std::ios::app);
    Result_Gen("TestRun", TestingPrint(), TestingPrint(), file);
    return 1;
}