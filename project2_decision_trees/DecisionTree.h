#ifndef DECISION_TREE
#define DECISION_TREE

#include <string>
#include <unordered_map>
#include <memory>

class DecisionTree {
public:
  DecisionTree(const std::string & attributeName);
  DecisionTree(const std::string & attributeName, bool leaf);
  ~DecisionTree();

  void addNode(const std::string & value, DecisionTree * subTree);
  const std::unordered_map<std::string, DecisionTree*> & getNodes();
  const std::string & getName();
  DecisionTree *getNode(const std::string & attributeName);
  const bool isLeafNode();
  void setDefault(const std::string & defaultValue);

private:
  std::string mAttributeName;
  std::unordered_map<std::string, DecisionTree*> mNodes;
  bool isLeaf;

  DecisionTree* __DEFAULT__ = nullptr;
};

#endif