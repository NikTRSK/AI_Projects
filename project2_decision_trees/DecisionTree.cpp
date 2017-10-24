#include "DecisionTree.h"

// DecisionTree::DecisionTree() { isLeaf = false; }

DecisionTree::DecisionTree(std::string attributeName) {
  mAttributeName = attributeName;
  isLeaf = false;
}

DecisionTree::DecisionTree(std::string attributeName, bool leaf) {
  mAttributeName = attributeName;
  isLeaf = true;
}

void DecisionTree::addNode(std::string value, DecisionTree * subTree) {
  // std::unique_ptr<DecisionTree> st = std::make_unique<DecisionTree> (subTree);
  mNodes.insert({value, subTree});
}

const std::unordered_map<std::string, DecisionTree*> & DecisionTree::getNodes() {
  return mNodes;
}

const std::string & DecisionTree::getName() { return mAttributeName; }

DecisionTree *DecisionTree::getNode(const std::string &attributeName) {
  auto value = mNodes.find(attributeName);
  if (value == mNodes.end()) {
    return __DEFAULT__;
  }
  return value->second;
}

bool DecisionTree::isLeafNode() { return isLeaf; }

void DecisionTree::setDefault(const std::string & defaultValue) { __DEFAULT__ = new DecisionTree(defaultValue, true); }

DecisionTree::~DecisionTree() {
   for (auto kv: mNodes) {
     if (kv.second != nullptr)
       delete kv.second;
   }
}