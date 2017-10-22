#include "DecisionTreeLearner.h"
// #include "DecisionTree.h"
#include <iostream>
#include <math.h>
#include <unordered_set>

void DecisionTreeLearner::trainTree(const DataSet & examples) {
  DataSet parent;
  mTree = buildTree(examples, examples.getHeader(), parent);
}

DecisionTree DecisionTreeLearner::buildTree(const DataSet & examples,
  const std::vector<std::string> & attributes, const DataSet & parentExamples) {
  
  // If examples are empty then create a leaf node with the most common value
  if (examples.size() == 0) {
    DecisionTree(majorityVote(parentExamples));
  }
  // If all examples have the same value then create a leaf node with that value
  if (examples.allExamplesAreSame()) {
    bool val = examples.getTargets()[0];
    std::string exampleValue = (val) ? "True" : "False";
    return DecisionTree(exampleValue);
  }
  // If attributes are empty then create a leaf node with the most common value
  if (attributes.size() == 0) {
    return DecisionTree(majorityVote(examples));
  }
  
  std::string maxAttribute = examples.maxGainAttribute();
  // addAttribute(maxAttribute);

  DecisionTree tree(maxAttribute);
  // create a leaf node
  std::unordered_set<std::string> values = examples.getAttributePossibities(maxAttribute);
  for (auto value: values) {
    DataSet filteredData = examples.filterAttribute(maxAttribute, value);

    // DecisionTree subTree = buildTree(filteredData,);
    // tree.addNode(value, subTree);
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