/*

Goal for morning is have it read two matrix into the files one at the start of gen problem and one at then end then compare the two.
This would allow us to skip the hard stuff lol

*/

#ifndef COMPAREGEO_HPP
#define COMPAREGEO_HPP

#include "../SparseMatrix.hpp"
#include "../GenerateProblem.hpp"

void GenerateReferenceMatrix(SparseMatrix &A, Vector *b, Vector *x, Vector *xexact);
bool CompareMatrices(const SparseMatrix &A1, const SparseMatrix &A2);
bool TestMatrixGeneration();
void PrintSparseMatrix(const SparseMatrix &A);
void TestMatrixUnalteredDirectModification();
void TestMatrixUnalteredComparison();
#endif
