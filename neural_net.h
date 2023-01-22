#ifndef _NEURAL_NET_H_
#define _NEURAL_NET_H_

#include <vector>
#include "matrix.h"

class NeuralNet {
public:

   int iNodes, hNodes, oNodes, hLayers;
   std::vector<Matrix> weights;
   NeuralNet() {};

   NeuralNet(int input, int hidden, int output, int hiddenLayers);
   void mutate(float mr);

   std::vector<float> output(std::vector<float> inputsArr);
   NeuralNet crossover(NeuralNet partner);

   void show(float x, float y, float w, float h, std::vector<float> vision, int decision) const;
};

#endif
