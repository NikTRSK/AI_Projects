#ifndef DECISION_TREE_LEARNER
#define DECISION_TREE_LEARNER

#include "DecisionTree.h"
#include <string>
#include <vector>
#include "DataSet.h"

class DecisionTreeLearner {
private:
  DecisionTree mTree;

public:
  void trainTree(const DataSet & dataset);
  DecisionTree buildTree(const DataSet & examples, const std::vector<std::string> & attributes, const DataSet & parentExamples);
  
  std::string predict(const Example & e);
  DecisionTree getDecisionTree();
  std::string majorityVote(const DataSet & examples);
  // void DecisionTreeLearnerLearning();
  void pluralityValue();

  // void PrintDataTable();
};

#endif