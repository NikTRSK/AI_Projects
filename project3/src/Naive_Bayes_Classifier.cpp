#include "Naive_Bayes_Classifier.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include "bitmap.hpp"
#include <fstream>
#include <iomanip>
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
    // std::cout << "DIGIT: " << digit << " = " <<  priori_probabilities[digit] << "\n";
    total_probability += priori_probabilities[digit];
  }
  std::cout << "PRIORI PROBS:\n";
  for (double d : priori_probabilities) {
    std::cout << d << ", ";
  }
  std::cout << "\n";
  std::cout << "Total Probability: " << total_probability << "\n";
}

// void Naive_Bayes_Classifier::calculate_conditional_probabilities(const std::vector<unsigned char> & labels, const std::vector<std::vector<unsigned char>> & images) {
//   /* Outer index is the class and the inner is the feature
//      Ex: probabilites[5][512] = P(F512 = 1 | c = 5) */
//   auto digit_list = get_digit_list(labels);

//   /* Calc all P(Fi | c) */
//   for (unsigned int digit = 0; digit < digit_list.size(); ++digit) {
//     int class_image_count = digit_list[digit].size();
//     for (unsigned int f = 0; f < num_features; f++) {
//       int num_white_pixels = 0;
//       for (auto img_idx : digit_list[digit]) {
//         // get value of pixel f (0 or 1) from training image
//         int pixel_value = static_cast<int>(images[img_idx][f]);
//         if (pixel_value == 1) ++num_white_pixels;
//       }
//       double fj_given_c = double(num_white_pixels + 1) / (class_image_count + 2); // also applies Laplace smoothing
//       probabilities[digit].push_back(fj_given_c);
//       // std::cout << "P (F = " << f << " | c = " << digit << ") = " << fj_given_c << "\n";
//     }
//   }
// }

void Naive_Bayes_Classifier::calculate_conditional_probabilities(const std::vector<unsigned char> & labels, const std::vector<std::vector<unsigned char>> & images) {
  /* Outer index is the class and the inner is the feature
     Ex: probabilites[5][512] = P(F512 = 1 | c = 5) */
  auto digit_list = get_digit_list(labels);

  // std::cout << "===== TESTING: =====\n";
  // for (unsigned int c = 0; c < digit_list.size(); ++c) {
  //   std::cout << "Checking C = " << c << "\n";
  //   for (unsigned int i = 0; i < digit_list[c].size(); ++i) {
  //     int label_value = static_cast<int>(labels[digit_list[c][i]]);
  //     if (label_value != c) std::cout << "Expected: " << c << ", Got: " << label_value << "\n";
  //   }
  // }

  /* Calc all P(Fi | c) */
  for (unsigned int j = 0; j < num_features; ++j) {
    for (unsigned int c = 0; c < num_classes; ++c) {
      unsigned int num_white_pixels = 0;
      unsigned int num_images_for_class = digit_list[c].size();
      // std::cout << "Running for class: " << c << "\n";
      for (unsigned int i = 0; i < digit_list[c].size(); ++i) {
        unsigned int img_idx = digit_list[c][i];
        int pixel_value = static_cast<int>(images[img_idx][j]);
        if (pixel_value == 1) {
          ++num_white_pixels;
        }
      }
      double fj_given_c = double(num_white_pixels + 1) / (num_images_for_class + 2);
      probabilities[c].push_back(fj_given_c);
      // std::cout << "Running for class: " << c << "\n";
      // std::cout << "P (F = " << j << " | c = " << c << ") = " << fj_given_c << "\n";
    }
  }

}

/* Returns a list of lists of indexes where each digit occurs in the training set */
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

  /* P(c) = priori_probabilities[c] */
  int correct_count = 0;
  for (unsigned int i = 0; i < test_images.size(); ++i) {
    std::vector<double> probs(num_classes, 0);
    for (unsigned int c = 0; c < num_classes; ++c) {
      double fi_given_c = calculate_probability_product_for_image(test_images[i], c);
      probs[c] = fi_given_c;
    }
    auto max = std::max_element(probs.begin(), probs.end());
    auto max_class = std::distance(probs.begin(), max);
    int test_label = static_cast<int>(test_labels[i]);
    // std::cout << "Label: " << test_label << ", Prediction: " << max_class << "\n";
    if (max_class == test_label) {
      ++correct_count;
    }
  }
  std::cout << "Accuracy: " << (double)correct_count / num_test_images * 100 << "\n";
}

void Naive_Bayes_Classifier::generate_output_images() {
  for (unsigned int c = 0; c < num_classes; ++c) {
    std::vector<unsigned char> class_features(num_features);
    for (unsigned int f = 0; f < num_features; ++f) {
      double p = probabilities[c][f];
      uint8_t v = 255 * p;
      class_features[f] = (unsigned char)v;
    }
    std::stringstream ss;
    ss << "../output/digit" << c << ".bmp";
    Bitmap::writeBitmap(class_features, 28, 28, ss.str(), false);
  }
}

double Naive_Bayes_Classifier::calculate_probability_product_for_image(const std::vector<unsigned char> & image, unsigned int c) {
  /* Iterate over all the pixels for image and multiply probabilities that P(Fj = 1 | c) */
  double probability_sum = 0.0;
  for (unsigned int j = 0; j < image.size(); ++j) {
    int pixel_value = static_cast<int>(image[j]);
    if (pixel_value == 1) {
      probability_sum += log2(probabilities[c][j]);
    } else {
      probability_sum += log2(1- probabilities[c][j]);
    }
  }
  return probability_sum + log2(priori_probabilities[c]);
}

void Naive_Bayes_Classifier::output_network() {
  std::ofstream output;
  output << std::setprecision (6) << std::fixed;
  output.open("../output/network.txt");
  for (unsigned int c = 0; c < num_classes; ++c) {
    for (unsigned int f = 0; f < num_features; ++f) {
      output << probabilities[c][f];
      if (f < num_features - 1) output << ", ";
    }
    output << "\n";
  }

  for (unsigned int c = 0; c < num_classes; ++c) {
    output << priori_probabilities[c];
    if (c < num_classes - 1) output << "\n";
  }

  output.close();
}