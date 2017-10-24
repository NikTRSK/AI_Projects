#ifndef EXAMPLE
#define EXAMPLE

#include <unordered_map>
#include <string>
#include <vector>

class Example {
public:
  Example(std::vector<std::string> attributes, std::vector<std::string> exampleRow);
  Example(std::string attributes, std::string exampleRow, bool isPrediction);

  std::vector<std::string> getAttributes();
  const std::string & getValue(const std::string & attribute) const;
  const std::unordered_map <std::string, std::string> & getExample();
  const bool & getTargetValue() const;
private:
  // Map header => value, for that example line
  std::unordered_map<std::string, std::string> mAttributes;
  // Target
  bool mTarget;
};

#endif