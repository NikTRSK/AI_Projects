#include "DecisionTreeLearner.h"
#include <iostream>
#include <math.h>
#include <unordered_set>

void DecisionTreeLearner::trainTree(const DataSet & examples) {
  mTree = buildTree(examples, nullptr);
}

DecisionTree DecisionTreeLearner::buildTree(const DataSet & examples, const DataSet & parentExamples) {
  if (examples.size() == 0) {
    return pluralityValue(parentExamples); // fix this
  } else if (examples.allExamplesAreSame()) {
    return example.getTargets()[0];
  }
  
  std::string maxAttribute = examples.maxGainAttribute();
  addAttribute(maxAttribute);

  DecisionTree tree(maxAttribute);
  // create a leaf node


  return tree;
}

void DecisionTreeLearner::PrintDataTable() {
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