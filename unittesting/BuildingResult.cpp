#include <iostream>
#include <fstream>
#include "BuildingResult.hpp"
using namespace std;

void Result_Gen(const std::string TestName, bool Result, std::ofstream &file)
{
    file << TestName << endl;
    file << " run " << endl;
    file << " " << (Result ? "ok" : "fail") << endl;
}