#ifndef DECISION_TREE
#define DECISION_TREE

#include <string>
#include <unordered_map>

class DecisionTree {
public:
  DecisionTree();
  DecisionTree(std::string attributeName);
  ~DecisionTree();

private:
  std::string mAttributeName;
  std::unordered_map<std::string, DecisionTree> mNodes;
};

#endif