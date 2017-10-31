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
  /* DATASET MANIPULATORS */
  void loadDataSet(const char * inputFile);
  DataSet filterAttribute(const std::string & attributeName, const std::string & value) const;  
  std::vector<DataSet> splitData(); // Used for cross validation
  void setHeader(const std::vector<std::string> & header);
  void addExample(const Example & example);

  /* GETTERS */
  std::vector<bool> getTargets() const;
  const std::vector<std::string> & getHeader() const;
  const std::vector<Example> & getDataSet() const;
  const std::unordered_set<std::string> getAttributePossibities(const std::string & attributeName) const;
  std::vector<std::string> getValuesForAttribute(const std::string & attributeName) const;
  const bool allExamplesAreSame() const;
  const unsigned int size() const;

  /* HEURISTICS */
  std::string maxGainAttribute(const std::vector<std::string> & attributes) const;
  double calculateAttributeGain(std::string attributeName) const;
  double calculateEnthropy(int positiveExamples, int negativeExamples) const;
  double Log2(double q) const;

  DataSet operator+(const DataSet& ds);
private:
  std::vector<std::string> mHeader;
  std::vector<Example> mExamples;
};

#endif