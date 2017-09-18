#ifndef STATE_8_PUZZLE_H
#define STATE_8_PUZZLE_H

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <unordered_map>
#include "Tile.h"

// Represents an 8-puzzle state as a 3x3 array of chars. Each char can take values in range '0'-'9' (chars, not integers).
// '0' represents the blank tile.
// Provides GetKey to uniquely represent the puzzle as an integer (for hashing).
// You can extend this class to generate successors and get heuristic distance to '012345678', which is always the goal state.

class Puzzle8State {
public:
	Puzzle8State(std::string s = "012345678") {
		assert(s.length() == 9);
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				tiles[r][c] = s[r*3 + c];

		visited = false;
		fValue = std::numeric_limits<double>::max();
	}

	// Key generated as an integer for the hash function in Puzzle8StateManager.
	int GetKey() {
		int key = 0;
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				key = key*10 + int(tiles[r][c] - '0');

		return key;
	}

	// Return the linearized form as a string. (You don't need to use this.)
	std::string GetLinearizedForm () {
		std::string s = "";
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				s += tiles[r][c];
		return s;
	}

	// Print the puzzle in a 3x3 layout. (You don't need to use this.)
	void Print(std::ostream & out = std::cout) {
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				out<<tiles[r][c]<<" ";
			}
			out<<std::endl;
		}
		out<<GetKey()<<std::endl;
	}

	bool IsVisited() {
		return visited;
	}

	void MarkVisited() {
		visited = true;
	}

	void ResetVisited() {
		visited = false;
	}

	Tile GetEmptyTile() {
		std::string state = GetLinearizedForm();
		std::size_t emptyIndex = state.find('0');
		int row = emptyIndex / 3;
		int col = emptyIndex % 3;

		return Tile(row, col);
	}

	Tile FindTile(char tile) {
		// std::cout << "GOAL\n";
		// Print();
		// std::cout << "GETTING A TILE ============ " << tile << "\n";
		
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				// std::cout << "TILE: " << tile << " | "  << tiles[r][c] << " R: " << r << ", C: " << c << "\n";
				if (tiles[r][c] == tile) {
					// std::cout << "FOUND: " << tile << " | " << "R: " << r << ", C: " << c << "\n";
					return Tile(r, c);
				}
			}
		}
		return Tile();
	}

	std::unordered_map<std::string, int> GetNeighbors(Puzzle8State goalState) {
		std::unordered_map<std::string, int> neighbors;
		Tile tile = GetEmptyTile();
		char emptyTile = tiles[tile.row][tile.col];

		// std::cout << "GOAL STATE ===\n";
		// goalState.Print(); std::cout << std::endl;

		// Check N
		if (tile.row - 1 >= 0) {
			Tile neighbor = Tile(tile.row-1, tile.col);
			char neighborTile = tiles[neighbor.row][neighbor.col];
			Tile goalTile = goalState.FindTile(neighborTile);
			int h = CaclucateDistance(goalTile, tile);
			// std::cout << "HEIGHBOR: " << neighborTile << " R: " << neighbor.row << ", C: " << neighbor.col << "\n";
			neighbors.insert({SwapAndGenerateState(neighborTile, emptyTile), h});
			// std::cout << "RES: " << neighborTile << ", " << h << std::endl;
		}
		// Get S
		if (tile.row + 1 < 3) {
			Tile neighbor = Tile(tile.row+1, tile.col);
			char neighborTile = tiles[neighbor.row][neighbor.col];
			Tile goalTile = goalState.FindTile(neighborTile);
			int h = CaclucateDistance(goalTile, tile);
			// std::cout << "HEIGHBOR: " << neighborTile << " R: " << neighbor.row << ", C: " << neighbor.col << "\n";
			neighbors.insert({SwapAndGenerateState(neighborTile, emptyTile), h});
			// std::cout << "RES: " << neighborTile << ", " << h << std::endl;
		}
		// Get E
		if (tile.col - 1 >= 0) {
			Tile neighbor = Tile(tile.row, tile.col-1);
			char neighborTile = tiles[neighbor.row][neighbor.col];
			Tile goalTile = goalState.FindTile(neighborTile);
			int h = CaclucateDistance(goalTile, tile);
			// std::cout << "HEIGHBOR: " << neighborTile << " R: " << neighbor.row << ", C: " << neighbor.col << "\n";
			neighbors.insert({SwapAndGenerateState(neighborTile, emptyTile), h});
			// std::cout << "RES: " << neighborTile << ", " << h << std::endl;
		}
		// Get W
		if (tile.col + 1 < 3) {
			Tile neighbor = Tile(tile.row, tile.col+1);
			char neighborTile = tiles[neighbor.row][neighbor.col];
			Tile goalTile = goalState.FindTile(neighborTile);
			int h = CaclucateDistance(goalTile, tile);
			neighbors.insert({SwapAndGenerateState(neighborTile, emptyTile), h});
		}

		return neighbors;
	}

	std::string SwapAndGenerateState(char tile1, char tile2) {
		std::string state = GetLinearizedForm();
		std::size_t pos1 = state.find(tile1);
		std::size_t pos2 = state.find(tile2);

		char temp = state[pos1];
		state[pos1] = state[pos2];
		state[pos2] = temp;

		return state;
	}

	int CaclucateDistance(Tile goalStateTile, Tile currStateTile) {
		int rowCnt = std::abs(goalStateTile.row - currStateTile.row);
		int colCnt = std::abs(goalStateTile.col - currStateTile.col);

		// std::cout << tiles[goalStateTile.row][goalStateTile.col] << " | " <<
		// 	goalStateTile.row << ", " << goalStateTile.col << " | " << rowCnt << ", " << colCnt << std::endl;
		// std::cout << tiles[currStateTile.row][currStateTile.col] << " | " << 
		// 	currStateTile.row << ", " << currStateTile.col << " | " << rowCnt << ", " << colCnt << std::endl;

		return (rowCnt + colCnt);
	}

	double GetFValue() {
		return fValue;
	}

	void SetFValue(double f) {
		fValue = f;
	}

private:

	// tiles[r][c] is the tile (or blank) at row r (0-2) and column c (0-2)
	// 0th row is the top row, and 0th column is the leftmost column.
	char tiles[3][3];
	bool visited;
	double fValue;
	int h;
};

#endif
