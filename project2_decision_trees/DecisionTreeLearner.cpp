#include "DecisionTreeLearner.h"
#include "StringUtils.h"
#include <iostream>
#include <math.h>
#include <unordered_set>
#include <iterator>
#include <iomanip>      // std::setfill, std::setw

DecisionTreeLearner::~DecisionTreeLearner() { delete mTree; }

void DecisionTreeLearner::trainTree(DataSet & examples) {
  DataSet parent;
  // Split data and find optimum depth
  // DecisionTree *currTree;
  unsigned int currMax = 0;
  unsigned int optimumDepth = 0;
  std::cout << StringUtils::centerString("depth", 12)
            << StringUtils::centerString("train%", 12)
            << StringUtils::centerString("valid%", 12) << "\n";
            // << StringUtils::centerString("test%", 12) << "\n";
  for (unsigned int depthBound = 1; depthBound <= 15; ++depthBound) {
    std::vector<DataSet> datasets = examples.splitData();
    // Train the tree
    mTree = buildTree(datasets[0], datasets[0].getHeader(), parent, depthBound, 0);
    // Check it against the validation set
    unsigned int correctCountTrain = validateTree(*mTree, datasets[0].getDataSet());
    unsigned int correctCountValidate = validateTree(*mTree, datasets[1].getDataSet());
    // unsigned int correctCountTest = validateTree(*mTree, datasets[2].getDataSet());
    std::cout << StringUtils::centerString(std::to_string(depthBound), 12)
              << StringUtils::centerString(std::to_string((double)correctCountTrain / datasets[0].size() * 100), 12)
              << StringUtils::centerString(std::to_string((double)correctCountValidate / datasets[1].size() * 100), 12) << "\n";
    if (correctCountValidate > currMax) {
      // mTree = currTree;
      currMax = correctCountValidate;
      optimumDepth = depthBound;
    }
    delete mTree;
  } // end
  // Build the tree with the ideal depth
  std::vector<DataSet> datasets = examples.splitData();
  DataSet training_validation = datasets[0] + datasets[1];
  mTree = buildTree(training_validation, training_validation.getHeader(), parent, optimumDepth, 0);
  unsigned int correctCountTest = validateTree(*mTree, datasets[2].getDataSet());
  std::cout << "Test accuracy: " << (double)correctCountTest / datasets[2].size() * 100 << "%, at depth: " << optimumDepth << "\n";
}

void DecisionTreeLearner::trainTree(DataSet & examples, bool p1) {
  DataSet parent;
  std::vector<DataSet> datasets = examples.splitData();
  DataSet training = datasets[0] + datasets[1];
  unsigned int depth = 0;
  mTree = buildTree(training, training.getHeader(), parent, depth, 0);
  // Build the tree with the ideal depth
  unsigned int correctCountTrain = validateTree(*mTree, training.getDataSet());
  unsigned int correctCountTest = validateTree(*mTree, datasets[2].getDataSet());
  std::cout << "Training accuracy: " << (double)correctCountTrain / training.size() * 100 << "%, Test accuracy: " << (double)correctCountTest / datasets[2].size() * 100 << "%\n";
}

DecisionTree *DecisionTreeLearner::buildTree(const DataSet & examples,
  const std::vector<std::string> & attributes, const DataSet & parentExamples,
								unsigned int &depthBound, unsigned int currDepth) {
  // If depth bound reached. 0 signifies unbounded
  if (depthBound != 0 && depthBound == currDepth) {
    return new DecisionTree(majorityVote(examples), true);
  }
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
  DecisionTree *tree = new DecisionTree(maxAttribute);
  // create a leaf node
  std::unordered_set<std::string> values = examples.getAttributePossibities(maxAttribute);
  for (auto value: values) {
    DataSet filteredData = examples.filterAttribute(maxAttribute, value);
    auto newAttributes = removeAttribute(attributes, maxAttribute);
    DecisionTree *subTree = buildTree(filteredData, newAttributes, examples, depthBound, currDepth + 1);
    tree->addNode(value, subTree);
  }
  // Handle missing values
  std::string __OTHER__ = majorityVote(parentExamples);
  tree->setDefault(__OTHER__);
  return tree;
}

std::string DecisionTreeLearner::majorityVote(const DataSet & examples) {
  if (examples.size() == 0)
    return "True";
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

std::vector<std::string> DecisionTreeLearner::removeAttribute(const std::vector<std::string> &attributes, const std::string &toRemove) {
  std::vector<std::string> newAttributes;
  for (const auto & a : attributes) {
    if (a != toRemove) {
      newAttributes.push_back(a);
    }
  }
  return newAttributes;
}

int DecisionTreeLearner::validateTree(DecisionTree & tree, const std::vector<Example> & examples) {
  int correctCount = 0;
  std::string prediction, expected;
  for (const Example & e: examples) {
    prediction = predict(tree, e);
    expected = e.getTargetValue() ? "True" : "False";
    if (prediction == expected)
      ++correctCount;
  }
  return correctCount;
}