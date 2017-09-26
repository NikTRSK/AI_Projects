#ifndef PUZZLE_8_SOLVER_H
#define PUZZLE_8_SOLVER_H

#include <string>
#include <unordered_set>

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions);
double CalculateHeuristic(double w, int g, int h);
bool IsVisited(int stateID, const std::unordered_set<int> & set);
void TestGetNeighbors();
#endif
