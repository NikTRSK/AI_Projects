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
		// std::cout << "Next state to expand is " << curr.id << " with f-value " << curr.f << ",Size: " << generatedStates.size() << std::endl;
		pq.pop();

		// Get current state
		Puzzle8State currState = generatedStates[curr.id];
		// currState.Print();
		if (IsVisited(curr.id, closedList)) continue;
	
		// Add to closed set
		closedList.insert(curr.id);
		++expansions;

		if (currState.GetKey() == goalState.GetKey()) {
			// std:: cout << "FOUND RESULT\n";
			// generatedStates[stateID].Print();
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
				int g = currState.GetG() + 1;
				if (generatedStates[stateID].GetG() != -1) {
					if (generatedStates[stateID].GetG() < g)
						g = generatedStates[stateID].GetG();
				}
				generatedStates[stateID].SetG(g);
				double f = CalculateHeuristic(w, generatedStates[stateID].GetG(), h);
				generatedStates[stateID].SetF(f);
				pq.push(PQElement(stateID, f));
			}

			// int h = generatedStates[stateID].CaclucateDistance(goalStateLocations);
			// generatedStates[stateID].SetG(currState.GetG() + 1);
			// double f = CalculateHeuristic(w, generatedStates[stateID].GetG(), h);

			// if (!IsVisited(stateID, closedList) || f < generatedStates[stateID].GetF()) {
			// 	generatedStates[stateID].SetF(f);
			// 	pq.push(PQElement(stateID, f));
			// }
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

/* Test functions */
void TestGetNeighbors() {

	Tile t(5, 5);
	std::vector<Tile> v;
	v.push_back(t);
	
	Tile &b = v[0];
	
	b.row = 10;

	std::cout << v[0].row << ", " << v[0].col << "\n";
	std::cout << b.row << ", " << b.col << "\n";
	std::cout << t.row << ", " << t.col << "\n";
// your code goes here
return;
	

	std::cout << "====TEST GET NEIGHBORS\n";
	Puzzle8State goalState("012345678");
	std::vector<std::string> neighbors = goalState.GetNeighbors();
	for (std::string n : neighbors) {
		Puzzle8State ns(n);
		ns.Print();
	}

	std::cout << "\nFOR 876543210\n";
	Puzzle8State startState("876543210");
	neighbors = startState.GetNeighbors();
	for (std::string n : neighbors) {
		Puzzle8State ns(n);
		ns.Print();
	}

	std::cout << "\nFOR 421305678\n";
	Puzzle8State midState("421305678");
	neighbors = midState.GetNeighbors();
	for (std::string n : neighbors) {
		Puzzle8State ns(n);
		ns.Print();
	}
}