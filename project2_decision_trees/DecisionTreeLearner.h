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
  void trainTree(const DataSet & dataset, const DataSet & parentExamples);
  DecisionTree buildTree(const DataSet & examples, const DataSet & parentExamples);
  
  std::string predict(const Example & e);
  DecisionTree getDecisionTree();

  // void DecisionTreeLearnerLearning();
  void pluralityValue();

  void PrintDataTable();
};

#endif