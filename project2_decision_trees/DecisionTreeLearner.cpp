#include "DecisionTreeLearner.h"
// #include "DecisionTree.h"
#include <iostream>
#include <math.h>
#include <unordered_set>

DecisionTreeLearner::~DecisionTreeLearner() { delete mTree; }

void DecisionTreeLearner::trainTree(const DataSet & examples) {
  DataSet parent;
  mTree = buildTree(examples, examples.getHeader(), parent);
}

DecisionTree *DecisionTreeLearner::buildTree(const DataSet & examples,
  const std::vector<std::string> & attributes, const DataSet & parentExamples) {
  
  // If examples are empty then create a leaf node with the most common value
  if (examples.size() == 0) {
    return new DecisionTree(majorityVote(parentExamples), true);
  }
  // If all examples have the same value then create a leaf node with that value
  if (examples.allExamplesAreSame()) {
    bool val = examples.getTargets()[0];
    std::string exampleValue = (val) ? "True" : "False";
    return new DecisionTree(exampleValue, true);
  }
  // If attributes are empty then create a leaf node with the most common value
  if (attributes.size() == 0) {
    return new DecisionTree(majorityVote(examples), true);
  }
  
  std::string maxAttribute = examples.maxGainAttribute(attributes);
  // addAttribute(maxAttribute);
//  std::cout << "Max: " << maxAttribute << "\n";

  DecisionTree *tree = new DecisionTree(maxAttribute);
  // std::string m = majorityVote(examples);
  // create a leaf node
  std::unordered_set<std::string> values = examples.getAttributePossibities(maxAttribute);
  for (auto value: values) {
    DataSet filteredData = examples.filterAttribute(maxAttribute, value);
    auto newAttributes = removeAttribute(attributes, maxAttribute);
    DecisionTree *subTree = buildTree(filteredData, newAttributes, examples);
    tree->addNode(value, subTree);
  }

  return tree;
}

std::string DecisionTreeLearner::majorityVote(const DataSet & examples) {
  int posCount = 0;
  int negCount = 0;
  for (auto & e: examples.getDataSet()) {
    if (e.getTargetValue())
      ++posCount;
    else
      ++negCount;
  }
  if (posCount >= negCount)
    return "True";
  else
    return "False";
}

std::vector<std::string> DecisionTreeLearner::removeAttribute(const std::vector<std::string> &attributes, const std::string &toRemove) {
  std::vector<std::string> newAttributes;
  for (const auto & a : attributes) {
    if (a != toRemove) {
      newAttributes.push_back(a);
    }
  }
  return newAttributes;
}

std::string DecisionTreeLearner::predict(const Example & e) {
  return predict(*mTree, e);
}

std::string DecisionTreeLearner::predict(DecisionTree & tree, const Example & e) {
  if (tree.isLeafNode()) {
    return tree.getName();
  }

  std::string name = tree.getName();
  std::string exampleValue = e.getValue(name);
  DecisionTree *child = tree.getNode(exampleValue);
  return predict(*child, e);
}

void DecisionTreeLearner::printTree() { printTree(mTree); }

void DecisionTreeLearner::printTree(DecisionTree *tree) {
  std::cout << "Node: " << tree->getName() << "\n";
  const auto & children = tree->getNodes();
  if (children.size() != 0) {  
    std::cout << "Children: ";
    for (auto kv : children) std::cout << "\t" << kv.first << "\n";
    std::cout << "\n";

    for (auto kv : children) {
      if (!kv.second->isLeafNode())
        printTree(kv.second);
    }
  }
}