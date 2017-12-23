#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include "Matrix.h"
#include "Dataset.h"

class NeuralNetwork {
public:
  NeuralNetwork();
  void init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate);

  Matrix computeOutput(std::vector<double> input);
  void learn(std::vector<double> expectedOutput);

  void train(Dataset const &ds);

private:
  Matrix X, W1, H, W2, Y, B1, B2, Y2, dJdB1, dJdB2, dJdW1, dJdW2;
  double learningRate;  
};

#endif