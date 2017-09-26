#ifndef PUZZLE_8_SOLVER_H
#define PUZZLE_8_SOLVER_H

#include <string>

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions);
double CalculateHeuristic(double w, int g, int h);
#endif
