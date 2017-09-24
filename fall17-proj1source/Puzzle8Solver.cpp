#include "Puzzle8Solver.h"
#include <vector>
#include <unordered_map>
#include <set>
#include "Puzzle8State.h"
#include "Puzzle8StateManager.h"
#include "Puzzle8PQ.h"

using namespace std;

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions) {
	cost = 0;
	expansions = 0;

	Puzzle8State goalState ("012345678");
	std::unordered_map<char, Tile> goalStateLocations;
	for (const char & c: goalState.GetLinearizedForm()) {
		Tile gt = goalState.FindTile(c);
		goalStateLocations.insert({c, gt});
	}
	Puzzle8State startState(puzzle);
	Puzzle8StateManager stateManager;
	Puzzle8PQ pq;
	std::set<int> closedList;
	std::vector<Puzzle8State> generatedStates;
	
	/* Add initial state to the pq and the generatedStates */
	int stateID = stateManager.GenerateState(startState);

	int h = startState.CaclucateDistance(goalStateLocations);
	startState.SetG(0);
	double f = CalculateHeuristic(w, 0, h);
	startState.SetF(f);

	pq.push(PQElement(stateID, f));
	generatedStates.push_back(startState);

	while(!pq.empty()) {
		PQElement curr = pq.top();
		pq.pop();

		// Get current state
		Puzzle8State currState = generatedStates[curr.id];
		if (IsVisited(curr.id, closedList)) continue;

		// Add to closed set
		closedList.insert(curr.id);
		++expansions;

		if (currState.GetKey() == goalState.GetKey()) {
			cost = curr.f;
			return;
		}

		/* Explore all neighbors */
		std::vector<std::string> neighbors = currState.GetNeighbors();
		for (const auto & n : neighbors) {
			Puzzle8State neighborState(n);
			/* If key generated get state from generated States */
			if (stateManager.IsGenerated(neighborState)) {
				stateID = stateManager.GetStateId(neighborState);
				neighborState = generatedStates[stateID];
			}
			/* Otherwise generate key and add to set */
			else {
				stateID = stateManager.GenerateState(neighborState);
				generatedStates.push_back(neighborState);
			}

			/* Calculate heuristic */
			if (currState.GetG() + 1 < generatedStates[stateID].GetG() || generatedStates[stateID].GetG() != -1 || !IsVisited(stateID, closedList)) {
				int h = generatedStates[stateID].CaclucateDistance(goalStateLocations);
				int g = generatedStates[stateID].GetG() != -1 && generatedStates[stateID].GetG() < currState.GetG() + 1 ? generatedStates[stateID].GetG() : currState.GetG() + 1;
				generatedStates[stateID].SetG(g);
				double f = CalculateHeuristic(w, generatedStates[stateID].GetG(), h);
				generatedStates[stateID].SetF(f);
				pq.push(PQElement(stateID, f));
			}
		}
	}
}

double CalculateHeuristic(double w, int g, int h) {
	double f = g + w * h;
	return f;
}

bool IsVisited(int stateID, const std::set<int> & set) {
	return set.find(stateID) != set.end();
}