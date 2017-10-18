#include "DecisionTree.h"
#include "StringUtils.h"
#include <iostream>
#include <fstream>

void DecisionTree::ParseInputFile(const char * inputFile) {

  std::ifstream dataFile(inputFile);
	if (!dataFile.is_open())
	{
    std::cerr << "Cannot open data file.\n";
    return;
	}

  std::string row;
  // Get header
  std::getline(dataFile, row);
  mHeader = StringUtils::SplitString(row, ",");
  // Get all data
	while (std::getline(dataFile, row))
	{
		mData.push_back(StringUtils::SplitString(row, ","));
	}
	dataFile.close();

  std::cout << "Data Loaded\n";
}

void DecisionTree::PrintDataTable() {
  // auto table_width = std::max_element(mHeader.begin(), mHeader.end(),
  //   [] (std::string const & lhs, std::string const & rhs) {
  //   return lhs.size() < rhs.size();
  // });
  // std::cout << *table_width << "\n";

  std::cout << mHeader.size() << ", " << mData.size() << "\n";
  for (const auto & h : mHeader) std::cout << h << " | ";
  std::cout << "\n";

  for (const auto & row : mData) {
    for (const auto & cell : row) {
      std::cout << cell << " | ";
    }    
    std::cout << "\n";
  }
  std::cout << "\n";
}