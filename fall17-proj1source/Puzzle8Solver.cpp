#include "Puzzle8Solver.h"
#include "Tile.h"
#include "Puzzle8StateManager.h"
#include "Puzzle8PQ.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions) {
	cost = 0;
	expansions = 0;

	Puzzle8State goalState("012345678");
	// TODO


	Puzzle8StateManager stateManager;
	Puzzle8PQ pq; // Init pq
	std::vector<std::unique_ptr<Puzzle8State>> generatedStates;

	Puzzle8State start(puzzle);
	
	double fVal = 0;//CalculateHeuristic(w, );
	pq.push(PQElement(stateManager.GenerateState(start), fVal));
	generatedStates.push_back(std::make_unique<Puzzle8State>(start));
	int g = 0;
	while(!pq.empty()) {
		PQElement curr = pq.top();
		std::cout << "Next state to expand is " << curr.id << " with f-value " << curr.f << ",Size: " << generatedStates.size() << std::endl;
		pq.pop();

		// Get current state
		Puzzle8State currState = generatedStates[curr.id];
		currState.MarkVisited();
		if (currState.GetLinearizedForm() == goalState.GetLinearizedForm()) {
			std::cout << "FOUND RESULT\n";
			return;
		}

		// // Get goal tile from current state
		Tile emptyTile = currState.GetEmptyTile();
		
		std::cout << "KEY: " << currState.GetKey() << std::endl;
		currState.Print();

		// Get all neigbors of current state
		std::unordered_map<std::string, int> neighbors = currState.GetNeighbors(goalState);

		/* Check all neighbors */
		// Puzzle8State n;
		++g;
		for (const auto& neighbor : neighbors) {
			// Calculate heuristic for neigbour
			Puzzle8State neighborState(neighbor.first);
			int neighborStateID = stateManager.GetStateId(neighborState);
			if (stateManager.IsGenerated(neighborState)) {
				neighborState = generatedStates[neighborStateID];
			} else {
				neighborStateID = stateManager.GenerateState(neighborState);
			}
			
			std::cout << "Negihbor Key: " << neighborStateID << "\n";
			std::cout << neighbor.first << ", H: " << neighbor.second << std::endl;
			neighborState.Print();

			double f = CalculateHeuristic(w, g, neighbor.second);
			if (!neighborState.IsVisited()/* !stateManager.IsGenerated(neighborState)  */|| f < neighborState.GetFValue()) {
				neighborState.SetFValue(f);
				generatedStates.push_back(neighborState); // Ugh we don't need to add it again to the vector
				pq.push(PQElement(neighborStateID, f));
			}
		}
	}
}

double CalculateHeuristic(double w, int g, int h) {
	/* h(s) = dist(# rows curr => goal) + dist(# cols curr => goal) */
	// double h;
	double f = g + w * h;
	std::cout << "CALC H: " << h << ", G: " << g <<  ", F: " << f << "\n";
	return f;
}