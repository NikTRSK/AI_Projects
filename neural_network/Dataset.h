#ifndef DATASET
#define DATASET

#include <vector>
#include <string>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

class Dataset {
public:
  Dataset(char const *filename)
  {
    load_training(filename);
  };
  
  void load_training(const char *filename)
  {
    mInput.resize(num_examples);
    mOutput.resize(num_examples);

    std::ifstream file(filename);    
    assert(file.is_open());

    std::string line;
    int n;

    for (unsigned int i = 0; i < num_examples; ++i) {
      // Images are 32x32 pixels
      for (unsigned int height = 0; height < 32; ++height) {
        getline(file, line);
        for (unsigned int width = 0; width < 32; ++width) {
          mInput[i].push_back(atoi(line.substr(width, 1).c_str()));
        }
      }
      getline(file, line);
      mOutput[i].resize(10); // 10 categories for 0 through 9
      n = atoi(line.substr(0, 1).c_str());
      mOutput[i][n] = 1;
    }

    file.close();
  };

  const std::vector<std::vector<double> > & examples() const {
    return mInput;
  };

  const std::vector<std::vector<double> > & labels() const {
    return mOutput;
  }

  const unsigned int num_examples = 946;
private:
  std::vector<std::vector<double> > mInput;
  std::vector<std::vector<double> > mOutput;
};

#endif