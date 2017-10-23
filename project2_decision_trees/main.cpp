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

	// DecisionTree dt;
	// dt.ParseInputFile(argv[1]);
	// dt.TestTrainValidationSplit();
	DataSet ds;
	ds.loadDataSet(argv[1]);
	// ds.splitData();
	// std::string max = ds.maxGainAttribute();
	// std::cout << "fin: " << max << "\n";
	DecisionTreeLearner dtl;
	dtl.trainTree(ds);
//	dtl.printTree();

	std::string attributes = "alt,bar,fri,hun,pat,price,rain,res,type,est";
	std::string data = "n,y,n,n,some,1,n,n,burger,0";
	Example e(attributes, data, true);
	std::string prediction = dtl.predict(e);
	std::cout << "Prediction: " << prediction << "\n";

	attributes = "alt,bar,fri,hun,pat,price,rain,res,type,est";
	data = "y,n,n,y,full,1,n,n,thai,2";
	Example e2(attributes, data, true);
	prediction = dtl.predict(e2);
	std::cout << "Prediction: " << prediction << "\n";
//	system("pause");
	return 0;
}