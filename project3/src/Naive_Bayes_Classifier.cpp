#include "Naive_Bayes_Classifier.h"

// Extra
#include <iostream>

Naive_Bayes_Classifier::Naive_Bayes_Classifier() {
  priori_probabilities = std::vector<double>(10, 0);
  probabilities.resize(10);
  
  //Read in the data set from the files
  dataset = mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>(MNIST_DATA_DIR);
  //Binarize the data set (so that pixels have values of either 0 or 1)
  mnist::binarize_dataset(dataset);
}

void Naive_Bayes_Classifier::calculate_priori_probabilities(const std::vector<unsigned char> & labels) {
  for (const auto & label : labels) {
    int label_value = static_cast<int>(label);
    priori_probabilities[label_value] += 1;
  }

  for (unsigned int digit = 0; digit < priori_probabilities.size(); ++digit) {
    priori_probabilities[digit] /= double(num_training_images);
  }
  
  double total_probability = 0.0;
  for (unsigned int digit = 0; digit < priori_probabilities.size(); ++digit) {
    std::cout << "DIGIT: " << digit << " = " <<  priori_probabilities[digit] << "\n";
    total_probability += priori_probabilities[digit];
  }
  std::cout << "Total Probability: " << total_probability << "\n";
}

void Naive_Bayes_Classifier::calculate_conditional_probabilities(const std::vector<unsigned char> & labels, const std::vector<std::vector<unsigned char>> & images) {
  /* Outer index is the class and the inner is the feature
     Ex: probabilites[5][512] = P(Fi = 512 | c = 5) */
  // std::vector<std::vector<double>> probabilities(10);
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
  // unsigned int num_features = 784;

  /* Calc all P(Fi | c) */
  for (unsigned int digit = 0; digit < digit_list.size(); ++digit) {
    // std::cout << "*** P for " << i << "\n";
    int class_image_count = digit_list[digit].size();
    for (unsigned int f = 0; f < num_features; f++) {    
      int num_white_pixels = 0;
      for (auto img_idx : digit_list[digit]) {
        // get value of pixel f (0 or 1) from training image
        int pixel_value = static_cast<int>(images[img_idx][f]);
        if (pixel_value == 1) ++num_white_pixels;
      }
      double fj_given_c = double(num_white_pixels + 1) / (class_image_count + 2); // also applies Laplace smoothing
      probabilities[digit].push_back(fj_given_c);
      std::cout << "P (F = " << f << " | c = " << digit << ") = " << fj_given_c << "\n";
    }
  }
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
  /* Calculate Priori Probabilities */
  // get training labels
  std::vector<unsigned char> train_labels = dataset.training_labels;
  // get training images
  std::vector<std::vector<unsigned char>> train_images = dataset.training_images;

  calculate_priori_probabilities(train_labels);

  calculate_conditional_probabilities(train_labels, train_images);
}

void Naive_Bayes_Classifier::evaluate() {
  // get test images
  std::vector<std::vector<unsigned char>> test_images = dataset.test_images;
  // get test labels
  std::vector<unsigned char> test_labels = dataset.test_labels;
}