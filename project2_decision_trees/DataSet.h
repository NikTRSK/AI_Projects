#ifndef DATA_SET
#define DATA_SET

#include "Example.h"
#include <vector>
#include <string>

class DataSet {
public:
  void loadDataSet(const char * inputFile);
  const unsigned int & size();
  std::vector<std::string> getHeader();
  std::vector<Example> getDataSet();
  std::vector<std::string> getValuesForAttribute(std::string attributeName);
  const bool & allExamplesAreSame();
  std::vector<std::string> getAttributePossibities(std::string attributeName);

  // std::vector<bool> getTargetsForAttribute(const std::string & attributeName);
  std::vector<bool> getTargets();
  void splitData ();

  /* HEURISTICS */
  std::string maxGainAttribute();
  double calculateAttributeGain(std::string attributeName);
  double calculateEnthropy(int positiveExamples, int negativeExamples);
  double Log2(int a, int b);
  double Log2(double q);
private:
  std::vector<std::string> mHeader;
  std::vector<Example> mExamples;

  std::vector<Example> mTrain;
  std::vector<Example> mTest;
  std::vector<Example> mValidation;
};

#endif