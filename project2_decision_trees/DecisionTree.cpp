#include "DecisionTree.h"

DecisionTree::DecisionTree(const std::string & attributeName) {
  mAttributeName = attributeName;
  isLeaf = false;
}

DecisionTree::DecisionTree(const std::string & attributeName, bool leaf) {
  mAttributeName = attributeName;
  isLeaf = true;
}

DecisionTree::~DecisionTree() {
  for (auto kv: mNodes) {
    if (kv.second != nullptr)
      delete kv.second;
  }
  if (__DEFAULT__ != nullptr)
  delete __DEFAULT__;
}

void DecisionTree::addNode(const std::string & value, DecisionTree * subTree) {
  mNodes.insert({value, subTree});
}

const std::unordered_map<std::string, DecisionTree*> & DecisionTree::getNodes() { return mNodes; }

const std::string & DecisionTree::getName() { return mAttributeName; }

DecisionTree *DecisionTree::getNode(const std::string & attributeName) {
  auto value = mNodes.find(attributeName);
  if (value == mNodes.end()) {
    return __DEFAULT__;
  }
  return value->second;
}

const bool DecisionTree::isLeafNode() { return isLeaf; }

void DecisionTree::setDefault(const std::string & defaultValue) { __DEFAULT__ = new DecisionTree(defaultValue, true); }