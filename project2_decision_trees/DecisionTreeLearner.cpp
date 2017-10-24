#include "DecisionTreeLearner.h"
#include "StringUtils.hpp"
#include <iostream>
#include <math.h>
#include <unordered_set>
#include <iterator>
#include <iomanip>      // std::setfill, std::setw

DecisionTreeLearner::~DecisionTreeLearner() { delete mTree; }

void DecisionTreeLearner::trainTree(DataSet & examples) {
  DataSet parent;
//  std::cout << "!!!!!!!!!======== IN HEADER: \n";
//  std::vector<std::string> h = examples.getHeader();
//  for (std::string s : h) std::cout << s << " | ";
//  std::cout << "\n ----etaeteataetaeteat\n";

  // Split data and find optimum depth
  DecisionTree *currTree;
  unsigned int currMax = 0;
  std::cout << examples.size() << std::endl;
  std::cout << StringUtils::centerString("depth", 12)
            << StringUtils::centerString("train%", 12)
            << StringUtils::centerString("valid%", 12) 
            << StringUtils::centerString("test%", 12) << "\n";
  // std::cout << std::setw(12) << std::setfill(' ') << "depth" << "train%" << "valid%\n";
  for (unsigned int depthBound = 0; depthBound <= 15; ++depthBound) {
  // for (unsigned int depthBound = ceil(examples.size()*0.6); depthBound >= 1; --depthBound) {
    std::vector<DataSet> datasets = examples.splitData();
    // validataionSize = datasets[1].size();
    // Train the tree
    currTree = buildTree(datasets[0], datasets[0].getHeader(), parent, depthBound, 0);
    // Check it against the validation set
    // std::cout << "CurrDepth: " << depthBound << "\n";
    int correctCountTrain = validateTree(*currTree, datasets[0].getDataSet());
    int correctCountValidate = validateTree(*currTree, datasets[1].getDataSet());
    int correctCountTest = validateTree(*currTree, datasets[2].getDataSet());
    std::cout << StringUtils::centerString(std::to_string(depthBound), 12)
              << StringUtils::centerString(std::to_string((double)correctCountTrain / datasets[0].size() * 100), 12)
              << StringUtils::centerString(std::to_string((double)correctCountValidate / datasets[1].size() * 100), 12)
              << StringUtils::centerString(std::to_string((double)correctCountTest / datasets[1].size() * 100), 12) << "\n";
    if (correctCountValidate > currMax) {
      mTree = currTree;
      currMax = correctCountValidate;
      // std::cout << "MaxCnt: " << correctCountValidate << ", " << (double)currMax / validataionSize *100 << "%" << std::endl;
    }
  } // end

//  unsigned int depthBound = 10; int currMax = 0;
//   mTree = buildTree(examples, examples.getHeader(), parent, depthBound, 0);
//   currMax = validateTree(*mTree, examples.getDataSet());

  // std::cout << "Tree Depth: " << currMax << "\n";
  // std::cout << "Best: " << (double)currMax / validataionSize *100 << "%\n";
}

DecisionTree *DecisionTreeLearner::buildTree(const DataSet & examples,
  const std::vector<std::string> & attributes, const DataSet & parentExamples,
								unsigned int &depthBound, unsigned int currDepth) {
  
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
  // If depth bound reached
 if (depthBound == currDepth) {
	  return new DecisionTree(majorityVote(examples), true);
 }

  // std::cout << "!!!!!!!!!======== IN DECISION: \n";
  // for (const auto & s : attributes) std::cout << s << " | ";
  // std::cout << "\n";
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
    DecisionTree *subTree = buildTree(filteredData, newAttributes, examples, depthBound, currDepth + 1);
    tree->addNode(value, subTree);
  }
  // std::cout << "at d: " << currDepth << "\n";
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
//  std::cout << "Checking: " << tree.getName() << "\n";
  if (tree.isLeafNode()) {
//    std::cout << "AT LEAF\n";
    return tree.getName();
  }

  std::string name = tree.getName();
  std::string exampleValue = e.getValue(name);
  DecisionTree *child = tree.getNode(exampleValue);
  if (child == NULL)
	  std::cout << "NULL\n";
  return predict(*child, e);
}

/* 0: train, 1: test, 2: validation */
// std::vector<std::vector<std::string>> DecisionTreeLearner::splitData(std::vector<Example> & examples) const {
//   std::vector<std::string> train;
//   std::vector<std::string> test;
//   std::vector<std::string> validation;

//   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//   // Shuffle data
//   shuffle (examples.begin(), examples.end(), std::default_random_engine(seed));
//   /* SHOULD WE TAKE THE CEILING FOR TRAIN TEST */

//   // Get Training Samples (60%)
//   int sampleSize = ceil(examples.size()*0.6);
//   std::copy (examples.begin(), examples.begin() + sampleSize, back_inserter(train));
//   // Get Testing Samples (20%)
//   sampleSize = ceil(examples.size()*0.2);
//   std::copy (examples.begin(), examples.begin() + sampleSize, back_inserter(test));
//   // Get Validation Samples (20%)
//   sampleSize = ceil(examples.size()*0.2);
//   std::copy (examples.begin(), examples.begin() + sampleSize, back_inserter(validation));
//   std::cout << examples.size() << ", " << train.size() + test.size() + validation.size() << "\n";
//   std::vector<DataSet> out({train, test, validation});
//   return out;
// }

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