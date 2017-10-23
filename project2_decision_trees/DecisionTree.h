#ifndef DECISION_TREE
#define DECISION_TREE

#include <string>
#include <unordered_map>
#include <memory>

class DecisionTree {
public:
  // DecisionTree();
  DecisionTree(std::string attributeName);
  DecisionTree(std::string attributeName, bool leaf);
  ~DecisionTree();
  void addNode(std::string value, DecisionTree * subTree);
  const std::unordered_map<std::string, DecisionTree*> & getNodes();
  const std::string & getName();
  DecisionTree *getNode(const std::string &attributeName);
  bool isLeafNode();

private:
  std::string mAttributeName;
  std::unordered_map<std::string, DecisionTree*> mNodes;
  bool isLeaf;
};

#endif