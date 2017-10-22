#ifndef EXAMPLE
#define EXAMPLE

#include <unordered_map>
#include <string>
#include <vector>

class Example {
public:
  Example(std::vector<std::string> attributes, std::vector<std::string> exampleRow);
  ~Example();

  std::vector<std::string> getAttributes();
  std::string getValue(const std::string & attribute) const;
  std::unordered_map <std::string, std::string> getExample();
  const bool & getTargetValue() const;
private:
  // Map header => value, for that line of code
  std::unordered_map <std::string, std::string> mAttributes;
  // Target
  bool mTarget;
};

#endif