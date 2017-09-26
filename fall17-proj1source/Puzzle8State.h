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

		/* Init heuristic values */
		f = std::numeric_limits<double>::max();
		g = -1; h = 0;
		visited = false;
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

	Tile GetEmptyTile() {
		std::string state = GetLinearizedForm();
		std::size_t emptyIndex = state.find('0');
		int row = emptyIndex / 3;
		int col = emptyIndex % 3;

		return Tile(row, col);
	}

	Tile FindTile(char tile) {		
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				if (tiles[r][c] == tile) {
					return Tile(r, c);
				}
			}
		}
		return Tile();
	}

	std::vector<std::string> GetNeighbors() {
		std::vector<std::string> neighbors;
		Tile tile = GetEmptyTile();
		char emptyTile = tiles[tile.row][tile.col];

		// Check N
		if (tile.row - 1 >= 0) {
			Tile neighbor = Tile(tile.row-1, tile.col);
			char neighborTile = tiles[neighbor.row][neighbor.col];
			neighbors.push_back(SwapAndGenerateState(neighborTile, emptyTile));
		}
		// Get S
		if (tile.row + 1 < 3) {
			Tile neighbor = Tile(tile.row+1, tile.col);
			char neighborTile = tiles[neighbor.row][neighbor.col];
			neighbors.push_back(SwapAndGenerateState(neighborTile, emptyTile));
		}
		// Get E
		if (tile.col - 1 >= 0) {
			Tile neighbor = Tile(tile.row, tile.col-1);
			char neighborTile = tiles[neighbor.row][neighbor.col];
			neighbors.push_back(SwapAndGenerateState(neighborTile, emptyTile));
		}
		// Get W
		if (tile.col + 1 < 3) {
			Tile neighbor = Tile(tile.row, tile.col+1);
			char neighborTile = tiles[neighbor.row][neighbor.col];
			neighbors.push_back(SwapAndGenerateState(neighborTile, emptyTile));
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

	int CaclucateDistance(const std::unordered_map<char, Tile> & goalState) {
		int rowCnt = 0;
		int colCnt = 0;

		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				char currStateItem = tiles[row][col];
				if (currStateItem != '0') {
					Tile goalStateTile = goalState.find(currStateItem)->second;
					rowCnt += std::abs(goalStateTile.row - row);
					colCnt += std::abs(goalStateTile.col - col);
				}
			}
		}
		int hVal = rowCnt + colCnt;
		h = hVal;
		return hVal;
	}

	int GetH() { return h; }
	void SetH(int hVal) { h = hVal; }

	int GetG() { return g; }
	void SetG(int gVal) { g = gVal; }

	int GetF() { return f; }
	void SetF(int fVal) { f = fVal; }

	void MarkVisited() { visited = true; }
	bool IsVisited() { return visited; }
private:

	// tiles[r][c] is the tile (or blank) at row r (0-2) and column c (0-2)
	// 0th row is the top row, and 0th column is the leftmost column.
	char tiles[3][3];

	/* Heuristic information */
	int h;
	int g;
	double f;
	bool visited;
};

#endif
