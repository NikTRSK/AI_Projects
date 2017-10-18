#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

#include "DecisionTree.h"

using namespace std;

int main(int argc, char *argv[]) {

	if (argc < 2) {
		std::cerr << "Missing data file... Exiting.\n";
		return 1;
	}

	DecisionTree dt;
	dt.ParseInputFile(argv[1]);
	// dt.PrintDataTable();
	return 0;
}
