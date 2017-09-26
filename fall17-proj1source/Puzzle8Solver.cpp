#include "Puzzle8Solver.h"
#include <vector>
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
	std::vector<Puzzle8State> generatedStates;
	
	/* Add initial state to the pq and the generatedStates */
	int stateID = stateManager.GenerateState(startState);
	startState.SetG(0);
	int h = startState.CaclucateDistance(goalStateLocations);
	double f = CalculateHeuristic(w, 0, h);
	int g = 0;
	startState.SetF(f);

	pq.push(PQElement(stateID, f));
	generatedStates.push_back(startState);
	Puzzle8State neighborState;
	Puzzle8State currState;
	while(!pq.empty()) {
		PQElement curr = pq.top();
		pq.pop();

		// Get current state
		currState = generatedStates[curr.id];
		if (generatedStates[curr.id].IsVisited()) continue;

		// Add to closed set
		generatedStates[curr.id].MarkVisited();
		++expansions;

		if (currState.GetKey() == goalState.GetKey()) {
			cost = curr.f;
			return;
		}

		/* Explore all neighbors */
		std::vector<std::string> neighbors = currState.GetNeighbors();
		for (const auto & n : neighbors) {
			neighborState = Puzzle8State(n);
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
			if (currState.GetG() + 1 < generatedStates[stateID].GetG() || generatedStates[stateID].GetG() != -1 || !generatedStates[stateID].IsVisited()) {
				h = generatedStates[stateID].CaclucateDistance(goalStateLocations);
				g = generatedStates[stateID].GetG() != -1 && generatedStates[stateID].GetG() < currState.GetG() + 1 ? generatedStates[stateID].GetG() : currState.GetG() + 1;
				generatedStates[stateID].SetG(g);
				f = CalculateHeuristic(w, generatedStates[stateID].GetG(), h);
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