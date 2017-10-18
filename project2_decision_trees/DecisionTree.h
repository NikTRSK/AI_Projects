#ifndef DECISION_TREE
#define DECISION_TREE

#include <string>
#include <vector>

class DecisionTree {
private:
  std::vector<std::vector<std::string>> mData;
  std::vector<std::string> mHeader;

public:
  // DecisionTree();
  // ~DecisionTree();

  void ParseInputFile(const char * inputFile);
  void PrintDataTable();
};

#endif