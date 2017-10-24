#ifndef DATA_SET
#define DATA_SET

#include "Example.h"
#include <vector>
#include <string>
#include <unordered_set>

class DataSet {
public:
  DataSet();
  DataSet(const std::vector<std::string> & header, const std::vector<Example> & examples);
  void loadDataSet(const char * inputFile);
  const unsigned int size() const;
  const std::vector<std::string> & getHeader() const;
  void setHeader(const std::vector<std::string> & header);
  const std::vector<Example> & getDataSet() const;
  void addExample(const Example & example);
  std::vector<std::string> getValuesForAttribute(const std::string & attributeName) const;
  bool allExamplesAreSame() const;
  const std::unordered_set<std::string> getAttributePossibities(const std::string & attributeName) const;
  DataSet filterAttribute(const std::string & attributeName, const std::string & value) const;

  // std::vector<bool> getTargetsForAttribute(const std::string & attributeName);
  std::vector<bool> getTargets() const;
  std::vector<DataSet> splitData();

  /* HEURISTICS */
  std::string maxGainAttribute(const std::vector<std::string> & attributes) const;
  double calculateAttributeGain(std::string attributeName) const;
  double calculateEnthropy(int positiveExamples, int negativeExamples) const;
  double Log2(double q) const;
private:
  std::vector<std::string> mHeader;
  std::vector<Example> mExamples;
};

#endif