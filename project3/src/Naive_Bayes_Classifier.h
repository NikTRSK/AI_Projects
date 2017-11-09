#ifndef NAIVE_BAYES_CLASSIFIER
#define NAIVE_BAYES_CLASSIFIER

#include <vector>

class Naive_Bayes_Classifier {
public:
  void train();
  std::vector<int> calculate_priori_probabilities(const std::vector<unsigned char> & labels);
  std::vector<std::vector<double>> calculate_conditional_probabilities(const std::vector<unsigned char> & labels, const std::vector<std::vector<unsigned char>> & images);
  std::vector<std::vector<int>> get_digit_list(const std::vector<unsigned char> & labels);
private:
};

#endif