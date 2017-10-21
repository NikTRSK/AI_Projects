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

const unsigned int & DataSet::size() { return mExamples.size() };

const bool & DataSet::allExamplesAreSame() {
  auto targets = getTargets();
  bool head = targets[0];
  for (bool target : targets) {
    if (target != head)
      return false;
  }
  return true;
}

std::vector<std::string> DataSet::getHeader() {
  return mHeader;
}

std::vector<Example> DataSet::getDataSet() {
  return mExamples;
}

std::vector<std::string> DataSet::getValuesForAttribute(std::string attributeName) {
  std::vector<std::string> col;
  for (Example & e : mExamples) {
    col.push_back(e.getValue(attributeName));
  }
  return col;
}

// std::vector<bool> DataSet::getTargetsForAttribute(const std::string & attributeName) {
//   std::vector<bool> targetList;
//   for (Example & e : mExamples) {
//     targetList.push_back(e.getTargetValue(attributeName));
//   }
//   return targetList;
// }

std::vector<bool> DataSet::getTargets() {
  std::vector<bool> targets;
  for (Example & e : mExamples) {
    targets.push_back(e.getTargetValue());
  }
  return targets;
}

void DataSet::loadDataSet(const char * inputFile) {
  
    std::ifstream dataFile(inputFile);
    if (!dataFile.is_open())
    {
      std::cerr << "Cannot open data file.\n";
      return;
    }
  
    std::string row;
    // Get header
    std::getline(dataFile, row);
    mHeader = StringUtils::SplitString(row, ",");
    // Get all data
    while (std::getline(dataFile, row))
    {
      Example e(mHeader, StringUtils::SplitString(row, ","));
      mExamples.push_back(e);
    }
    dataFile.close();
    mHeader.erase(mHeader.begin());
    
    std::cout << "Data Loaded\n";
  }

  void DataSet::splitData() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // Shuffle data
    shuffle (mExamples.begin(), mExamples.end(), std::default_random_engine(seed));
    /* SHOULD WE TAKE THE CEILING FOR TRAIN TEST */
  
    // Get Training Samples (60%)
    int sampleSize = ceil(mExamples.size()*0.6);
    std::copy (mExamples.begin(), mExamples.begin() + sampleSize, back_inserter(mTrain));
    // Get Testing Samples (20%)
    sampleSize = ceil(mExamples.size()*0.2);
    std::copy (mExamples.begin(), mExamples.begin() + sampleSize, back_inserter(mTest));
    // Get Validation Samples (20%)
    sampleSize = ceil(mExamples.size()*0.2);
    std::copy (mExamples.begin(), mExamples.begin() + sampleSize, back_inserter(mValidation));
    std::cout << mExamples.size() << ", " << mTrain.size() + mTest.size() + mValidation.size() << "\n";
  }

  std::string DataSet::maxGainAttribute() {
    // Iterate over all the headers
    double maxGain = -1;
    std::string maxAttrbiute;
    for (auto & attribute : getHeader()) {
      std::cout << "Checking " << attribute << "\n";
      double gain = calculateAttributeGain(attribute);
      std::cout << gain << "\n";
      if (gain > maxGain) {
        maxGain = gain;
        maxAttrbiute = attribute;
      }
    }

    std::cout << "Max: " << maxAttrbiute << " - " << maxGain << "\n";
    return maxAttrbiute;
  }

  double DataSet::calculateAttributeGain(std::string attributeName) {
    int exampleCount = mExamples.size();
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
        if (targets[i]) {
          exampleCount->second.first++;
        }
        else {
          exampleCount->second.second++;
        }
      }
    }
    double remainder = 0.0;
    double localGain = 0.0;
    int totalPositive = 0;
    for (auto attributeCount : exampleCounts) {
      std::cout << attributeCount.first << ": " << attributeCount.second.first << ", " << 
      attributeCount.second.second << "\n";
      totalPositive += attributeCount.second.first;
      int countPosNeg = attributeCount.second.first + attributeCount.second.second;
      remainder += (countPosNeg / (double)exampleCount)
                  * calculateEnthropy(attributeCount.second.first, attributeCount.second.second);
      std::cout << "Gain: " << localGain << "\n";
      // remainder += localGain;
    }
    double b = -Log2((double)totalPositive / exampleCount);
    std::cout << "TOTAL: " << exampleCount << ", Total Gain: " << b - remainder << "\n";
    return b - remainder;
  }

  // double DataSet::calculateEnthropy(int positiveExamples, int negativeExamples) {
  //   double totalExamples = positiveExamples + negativeExamples;
  //   std::cout << "totalEx: " << totalExamples << ", p: " << positiveExamples << ", n: " << negativeExamples << "\n";

  //   double h = -(positiveExamples / totalExamples) * Log2(positiveExamples, totalExamples)
  //              -(negativeExamples / totalExamples) * Log2(negativeExamples, totalExamples);
  //   std::cout << "H: " << h << "\n";
  //   return h;
  // }

  double DataSet::calculateEnthropy(int positiveExamples, int negativeExamples) {
    double totalExamples = positiveExamples + negativeExamples;
    std::cout << "totalEx: " << totalExamples << ", p: " << positiveExamples << ", n: " << negativeExamples << "\n";

    double h = -Log2((double)positiveExamples / totalExamples);
    // double h = -(positiveExamples / totalExamples) * Log2(positiveExamples, totalExamples);
              //  -(negativeExamples / totalExamples) * Log2(negativeExamples, totalExamples);
    std::cout << "H: " << h << "\n";
    return h;
  }

  double DataSet::Log2(double q) {
    if (q == 0.0 || q == 1.0) return 0;
    std::cout << "LOG: " << (q * log2(q) + (1 - q) * log2(1 - q)) << "\n";
    return (q * log2(q) + (1 - q) * log2(1 - q));
  }

  double DataSet::Log2(int a, int b) {
    if ((double)a / b == 0 || (double)a / b == 1) return 0;
    std::cout << "LOG: " << (log2(double(a) / b)) << "\n";
    return log2(double(a) / b);
  }