// #pragma once
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

#include "DecisionTreeLearner.h"
#include "DataSet.h"
//#include <conio.h>

using namespace std;

void ParseInputFile(const char * inputFile);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Missing data file... Exiting.\n";
		return 1;
	}

	DataSet ds;
	ds.loadDataSet(argv[1]);
	{
		std::cout << "****************Part 1.1: 80/20 Split****************\n";
		DecisionTreeLearner dtl;
		dtl.trainTree(ds, true);
	}
	
	std::cout << "\n****************Part 1.2: 60/20/20 Split with depth test****************\n";
	DecisionTreeLearner dtl;
	dtl.trainTree(ds);

	return 0;
}