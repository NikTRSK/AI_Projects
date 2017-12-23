#include "NeuralNetwork.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>

NeuralNetwork::NeuralNetwork()
{
  srand (time(NULL));
}

void NeuralNetwork::init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate)
{
  auto random = [](double x) { return (double)(rand() % 10000 + 1)/10000-0.5; };

  learningRate = rate;
  // Weights
  W1 = Matrix(inputNeuron, hiddenNeuron);
  W2 = Matrix(hiddenNeuron, outputNeuron);
  // Biases
  B1 = Matrix(1, hiddenNeuron);
  B2 = Matrix(1, outputNeuron);

  /* we cannot set the parameters to zero because the derivative of the loss function with respect
     to the parameters will be all the same and the program will never start to learn. */
  W1 = W1.applyFunction(random);
  W2 = W2.applyFunction(random);
  B1 = B1.applyFunction(random);
  B2 = B2.applyFunction(random);
}

Matrix NeuralNetwork::computeOutput(std::vector<double> input)
{
  auto sigmoid = [](double x) { return 1 / (1 + exp(-x)); };

  X = Matrix({input}); // Raw Matrix
  H = X.dot(W1).add(B1).applyFunction(sigmoid);
  Y = H.dot(W2).add(B2).applyFunction(sigmoid);

  return Y;
}

void NeuralNetwork::learn(std::vector<double> expectedOutput)
{
  Y2 = Matrix({expectedOutput}); // row matrix

  /* Compute partial derivatives of J (error function) with respect to W1, W2, B1, B2 */

  auto sigmoidePrime = [](double x) { return exp(-x)/(pow(1+exp(-x), 2)); };

  // Gradients
  dJdB2 = Y.subtract(Y2).multiply(H.dot(W2).add(B2).applyFunction(sigmoidePrime));
  dJdB1 = dJdB2.dot(W2.transpose()).multiply(X.dot(W1).add(B1).applyFunction(sigmoidePrime));
  dJdW2 = H.transpose().dot(dJdB2);
  dJdW1 = X.transpose().dot(dJdB1);

  // Weight update
  W1 = W1.subtract(dJdW1.multiply(learningRate));
  W2 = W2.subtract(dJdW2.multiply(learningRate));
  B1 = B1.subtract(dJdB1.multiply(learningRate));
  B2 = B2.subtract(dJdB2.multiply(learningRate));
}

void NeuralNetwork::train(Dataset const &ds)
{
  auto examples = ds.examples();
  auto labels = ds.labels();

  // 32*32=1024 input neurons (images are 32*32 pixels)
  // 15 hidden neurons (experimental)
  // 10 output neurons (for each image output is a vector of size 10, full of zeros and a 1 at the index of the number represented)
  // 0.7 learning rate (experimental)
  init(1024, 15, 10, 0.7);

  // Train for 30 iterations
  for (unsigned int i = 0; i < 30; ++i) {
    // skip the last 10 examples to test the program at the end
    for (unsigned int j = 0; j < examples.size()-10; ++j) {
      computeOutput(examples[i]);
      learn(labels[i]);
    }
    std::cout << "EPOCH: " << i + 1 << "/30\n";
  }

  // Test
  auto stepFunction = [](double x)
  {
    if(x > 0.9) return 1.0;
    if(x < 0.1) return 0.0;
    return x;
  };

  std::cout << "Expected Output : Actual Output\n";
  for (unsigned int i = examples.size()-10; i < examples.size(); ++i) {
    for (unsigned int j = 0; j < 10; ++j) {
      std::cout << labels[i][j] << " ";
    }
    Matrix r = computeOutput(examples[i]).applyFunction(stepFunction);
    std::cout << r << std::endl;
    // std::cout << " : " << computeOutput(examples[i]).applyFunction(stepFunction) << std::endl;
    // std::cout << " : " << computeOutput(examples[i]) << std::endl;
  }
}