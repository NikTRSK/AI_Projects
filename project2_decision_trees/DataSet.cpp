#include "DataSet.h"
#include "StringUtils.h"
#include "Example.h"
#include <iostream>
#include <fstream>
#include <random>
#include <math.h>
#include <unordered_set>
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

DataSet::DataSet() {}

DataSet::DataSet(const std::vector<std::string> & header, const std::vector<Example> & examples) {
  mHeader = header;
  mExamples = examples;
}

void DataSet::loadDataSet(const char * inputFile) {
    std::ifstream dataFile(inputFile);
    if (!dataFile.is_open())
    {
      std::cerr << "Cannot open data file.\n";
      return;
      // TODO: Throw exception here
    }
  
    std::string row;
    // Get header
    std::getline(dataFile, row);
    mHeader = StringUtils::SplitString(row, ",");
    // // Get all data
    while (std::getline(dataFile, row))
    {
      Example e(mHeader, StringUtils::SplitString(row, ","));
      mExamples.push_back(e);
    }
    // Remove the attribute for the label
	  mHeader.erase(mHeader.begin());
    dataFile.close();
    std::cout << "Data Loaded\n";
  }

  DataSet DataSet::filterAttribute(const std::string & attributeName, const std::string & value) const {
    DataSet newDataSet;
    for (auto & e: mExamples) {
      if (e.getValue(attributeName) == value) {
        newDataSet.addExample(e);
      }
    }
    return newDataSet;
  }

  std::vector<DataSet> DataSet::splitData() {
    std::vector<Example> mTrain;
    std::vector<Example> mTest;
    std::vector<Example> mValidation;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // Shuffle data
    shuffle (mExamples.begin(), mExamples.end(), std::default_random_engine(seed));
  
    // Get Training Samples (60%)
    int trainSize = floor(mExamples.size()*0.6);
    std::copy (mExamples.begin(), mExamples.begin() + trainSize + 1, back_inserter(mTrain));
    // Get Testing Samples (20%)
    int testSize = floor(mExamples.size()*0.2);
    std::copy (mExamples.begin() + trainSize + 1, mExamples.begin() + trainSize + testSize + 1, back_inserter(mTest));
    // Get Validation Samples (20%)
    int validationSize = floor(mExamples.size()*0.2);
    std::copy (mExamples.begin() + trainSize + testSize + 1, mExamples.begin() + trainSize + testSize + validationSize + 1, back_inserter(mValidation));

    DataSet train(mHeader, mTrain);
    DataSet validation(mHeader, mValidation);
    DataSet test(mHeader, mTest);
    std::vector<DataSet> datasets = { train, validation, test };
    return datasets;
  }

  void DataSet::setHeader(const std::vector<std::string> & header) {
    mHeader = header;
  }
  
  void DataSet::addExample(const Example & example) {
    mExamples.push_back(example);
  }

  std::vector<bool> DataSet::getTargets() const {
    std::vector<bool> targets;
    for (const Example & e : mExamples) {
      targets.push_back(e.getTargetValue());
    }
    return targets;
  }

  const std::vector<std::string> & DataSet::getHeader() const {
    return mHeader;
  }

  const std::vector<Example> & DataSet::getDataSet() const {
    return mExamples;
  }

  const std::unordered_set<std::string> DataSet::getAttributePossibities(const std::string & attributeName) const {
    std::unordered_set<std::string> values;
    for (auto value : getValuesForAttribute(attributeName)) {
      values.insert(value);
    }
    return values;
  }

  std::vector<std::string> DataSet::getValuesForAttribute(const std::string & attributeName) const {
    std::vector<std::string> col;
    for (const Example & e : mExamples) {
      col.push_back(e.getValue(attributeName));
    }
    return col;
  }

  const bool DataSet::allExamplesAreSame() const {
    auto targets = getTargets();
    bool head = targets[0];
    for (bool target : targets) {
      if (target != head)
        return false;
    }
    return true;
  }

  const unsigned int DataSet::size() const { return mExamples.size(); }

  std::string DataSet::maxGainAttribute(const std::vector<std::string> & attributes) const {
    // Iterate over all the headers and find the attribute that has the max gain
    double maxGain = -1;
    std::string maxAttrbiute;
    for (const auto & attribute : attributes) {
      double gain = calculateAttributeGain(attribute);
      if (gain > maxGain) {
        maxGain = gain;
        maxAttrbiute = attribute;
      }
    }
    
    return maxAttrbiute;
  }

  double DataSet::calculateAttributeGain(std::string attributeName) const {
    int exampleCount = mExamples.size();
    // Count how many positive and negative examples are there
    // pair: pos, neg
    std::unordered_map<std::string, std::pair<int, int>> exampleCounts;
    std::vector<bool> targets = getTargets();
    std::vector<std::string> attributes = getValuesForAttribute(attributeName);
    for (unsigned int i = 0; i < attributes.size(); ++i) {
      auto exampleCount = exampleCounts.find(attributes[i]);
      if (exampleCount == exampleCounts.end()) {
        std::pair<int, int> truthValues = (targets[i]) ? std::make_pair(1, 0) : std::make_pair(0, 1);
        exampleCounts.insert({attributes[i], truthValues});
      }
      else {
        if (targets[i])
          exampleCount->second.first++;
        else
          exampleCount->second.second++;
      }
    }
    // Calculate Gain
    double remainder = 0.0;
    int totalPositive = 0;
    for (auto attributeCount : exampleCounts) {
      totalPositive += attributeCount.second.first;
      int countPosNeg = attributeCount.second.first + attributeCount.second.second;
      remainder += (countPosNeg / (double)exampleCount)
                  * calculateEnthropy(attributeCount.second.first, attributeCount.second.second);
    }
    double b = -Log2((double)totalPositive / exampleCount);
    return b - remainder;
  }

  double DataSet::calculateEnthropy(int positiveExamples, int negativeExamples) const {
    double totalExamples = positiveExamples + negativeExamples;
    double h = -Log2((double)positiveExamples / totalExamples);
    return h;
  }

  double DataSet::Log2(double q) const {
    if (q == 0.0 || q == 1.0) return 0;
    return (q * log2(q) + (1 - q) * log2(1 - q));
  }

  DataSet DataSet::operator+(const DataSet& ds) {
	  std::vector<Example> data = { this->getDataSet() };
	  for (auto e : ds.getDataSet())
		  data.push_back(e);

    return DataSet(this->getHeader(), data);
  }