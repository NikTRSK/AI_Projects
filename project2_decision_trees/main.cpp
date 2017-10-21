// #pragma once
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

// #include "DecisionTree.h"
#include "DataSet.h"

using namespace std;

void ParseInputFile(const char * inputFile);

int main(int argc, char *argv[]) {

	if (argc < 2) {
		std::cerr << "Missing data file... Exiting.\n";
		return 1;
	}

	// DecisionTree dt;
	// dt.ParseInputFile(argv[1]);
	// dt.TestTrainValidationSplit();
	DataSet ds;
	ds.loadDataSet(argv[1]);
	// ds.splitData();
	std::string max = ds.maxGainAttribute();
	std::cout << "fin: " << max << "\n";
	return 0;
}