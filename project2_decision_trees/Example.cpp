#include "Example.h"
#include <iostream>

Example::Example(std::vector<std::string> attributes, std::vector<std::string> exampleRow) {
 for (unsigned int i = 1; i < attributes.size(); ++i) {
  mAttributes.insert({attributes[i], exampleRow[i]});
 }
 
 mTarget = (exampleRow[0] == "0") ? false : true;
}

Example::Example(std::string attributes, std::string exampleRow, bool isPrediction) {
  auto attrbs = StringUtils::SplitString(attributes, ",");
  auto values = StringUtils::SplitString(exampleRow, ",");
  std::cout << "Sizes: " << attrbs.size() << ", " << values.size() << "\n";
  for (unsigned int i = 0; i < attrbs.size(); ++i) {
   std::cout << attrbs[i] << " | " << values[i] << "\n";
   mAttributes.insert({attrbs[i], values[i]});
  }
 }

Example::~Example() {}

std::vector<std::string> Example::getAttributes() {
  std::vector<std::string> attributes;
  for(auto kv : mAttributes) {
    attributes.push_back(kv.first);
  }
  return attributes;
}

std::string Example::getValue(const std::string & attribute) const { return mAttributes.find(attribute)->second; }

std::unordered_map <std::string, std::string> Example::getExample() { return mAttributes; }

const bool & Example::getTargetValue() const { return mTarget; }