#ifndef DECISION_TREE_LEARNER
#define DECISION_TREE_LEARNER

#include "DecisionTree.h"
#include <string>
#include <vector>
#include "DataSet.h"

class DecisionTreeLearner {
private:
  DecisionTree *mTree;

public:
  ~DecisionTreeLearner();
  void trainTree(DataSet & dataset);
  void trainTree(DataSet & dataset, bool p1);
  DecisionTree *buildTree(const DataSet & examples, const std::vector<std::string> & attributes, const DataSet & parentExamples,
						  unsigned int &depthBound, unsigned int currDepth);
  
  std::string predict(const Example & e);
  std::string predict(DecisionTree & tree, const Example & e);
  DecisionTree getDecisionTree();
  std::string majorityVote(const DataSet & examples);
  std::vector<std::string> removeAttribute(const std::vector<std::string> &attributes, const std::string &toRemove);
  int validateTree(DecisionTree & tree, const std::vector<Example> & examples);
};

#endif