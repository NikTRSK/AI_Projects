#include "Matrix.h"
#include "Dataset.h"
#include "NeuralNetwork.h"

int main()
{
  Dataset ds("minst_training.txt");
  NeuralNetwork nn;

  nn.train(ds);
  return 0;
}