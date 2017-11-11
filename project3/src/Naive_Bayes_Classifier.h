#ifndef NAIVE_BAYES_CLASSIFIER
#define NAIVE_BAYES_CLASSIFIER

#include "mnist_reader.hpp"
#include "mnist_utils.hpp"
#define MNIST_DATA_DIR "../mnist_data"
#include <vector>

class Naive_Bayes_Classifier {
public:
  Naive_Bayes_Classifier();
  void train();
  void calculate_priori_probabilities(const std::vector<unsigned char> & labels);
  void calculate_conditional_probabilities(const std::vector<unsigned char> & labels, const std::vector<std::vector<unsigned char>> & images);
  std::vector<std::vector<int>> get_digit_list(const std::vector<unsigned char> & labels);

  void evaluate();
  void generate_output_images();
  double calculate_probability_product_for_image(const std::vector<unsigned char> & image, unsigned int c);
private:
  const unsigned int num_training_images = 60000;
  const unsigned int num_test_images = 10000;
  const unsigned int num_features = 784;
  const unsigned int num_classes = 10;

  /* Dataset */
  mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset;

  std::vector<double> priori_probabilities;
  std::vector<std::vector<double>> probabilities;
};

#endif