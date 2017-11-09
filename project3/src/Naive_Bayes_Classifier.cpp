#include "Naive_Bayes_Classifier.h"

#include "mnist_reader.hpp"
#include "mnist_utils.hpp"
#define MNIST_DATA_DIR "../mnist_data"

// Extra
#include <iostream>

std::vector<int> Naive_Bayes_Classifier::calculate_priori_probabilities(const std::vector<unsigned char> & labels) {
  std::vector<int> priori_probabilities(10, 0);
  
  {
    std::cout << "Vector len: " << priori_probabilities.size() << "\nBefore fill==\n";
    for (auto i : priori_probabilities) std::cout << i << ", ";
    std::cout << "\n";
  }
  
  for (const auto & label : labels) {
    int label_value = static_cast<int>(label);
    priori_probabilities[label_value] += 1;
  }

  {
    std::cout << "After fill==\n";
    for (auto i : priori_probabilities) std::cout << i << ", ";
    std::cout << "\n";
  }
  return priori_probabilities;
}

std::vector<std::vector<double>> Naive_Bayes_Classifier::calculate_conditional_probabilities(const std::vector<unsigned char> & labels, const std::vector<std::vector<unsigned char>> & images) {
  /* Outer class  */
  std::vector<std::vector<double>> probabilities(10);
  auto digit_list = get_digit_list(labels);
  
  // {
  //   /* print */
  //   std::cout << "**** IMAGES ****:\n";
  //   for (unsigned int i = 0; i < digit_list.size(); ++i) {
  //     std::cout << "DIGIT: " << i << " | Size: " << digit_list[i].size() << " =====\n";
  //     for (auto idx : digit_list[i]) std::cout << idx << ", ";
  //     std::cout << "\n";
  //   } 
  // } // end

  //There are 784 features (one per pixel in a 28x28 image)
  unsigned int num_features = 784;

  /* Calc all P(Fi | c) */
  for (unsigned int i = 0; i < digit_list.size(); ++i) {
    // std::cout << "*** P for " << i << "\n";
    int class_image_count = digit_list[i].size();
    for (unsigned int f = 0; f < num_features; f++) {    
      int num_white_pixels = 0;
      for (auto img_idx : digit_list[i]) {
        // get value of pixel f (0 or 1) from training image
        int pixel_value = static_cast<int>(images[img_idx][f]);
        if (pixel_value == 1) ++num_white_pixels;
      }
      double fj_given_c = double(num_white_pixels + 1) / class_image_count;
      std::cout << "P (F = " << f << " | c = " << i << ") = " << fj_given_c << "\n";
    }
  }
  return probabilities;
}

std::vector<std::vector<int>> Naive_Bayes_Classifier::get_digit_list(const std::vector<unsigned char> & labels) {
  std::vector<std::vector<int>> digit_list(10);

  for (unsigned int i = 0; i < labels.size(); ++i) {
    int label_value = static_cast<int>(labels[i]);
    digit_list[label_value].push_back(i);   
  }

  return digit_list;
}

void Naive_Bayes_Classifier::train() {
  //Read in the data set from the files
  mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset =
  mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>(MNIST_DATA_DIR);
  //Binarize the data set (so that pixels have values of either 0 or 1)
  mnist::binarize_dataset(dataset);

  /* Calculate Priori Probabilities */
  // get training labels
  std::vector<unsigned char> train_labels = dataset.training_labels;
  // get training images
  std::vector<std::vector<unsigned char>> train_images = dataset.training_images;

  auto t = calculate_priori_probabilities(train_labels);

  auto t2 = calculate_conditional_probabilities(train_labels, train_images);
}