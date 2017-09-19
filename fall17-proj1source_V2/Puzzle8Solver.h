#ifndef PUZZLE_8_SOLVER_H
#define PUZZLE_8_SOLVER_H

#include <string>
#include <set>

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions);
double CalculateHeuristic(double w, int g, int h);
bool IsVisited(int stateID, const std::set<int> & set);
void TestGetNeighbors();
#endif
