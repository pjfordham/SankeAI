#ifndef _NEURAL_NET_H_
#define _NEURAL_NET_H_

#include <vector>
#include <Eigen/Dense>


class NeuralNet {
   int iNodes, hNodes, oNodes, hLayers;
public:

   std::vector<Eigen::MatrixXf> weights;

   NeuralNet(int input, int hidden, int output, int hiddenLayers);
   void mutate(float mr);

   Eigen::VectorXf output(Eigen::VectorXf inputsArr) const;

   NeuralNet crossover(NeuralNet partner) const;

   void show(float x, float y, float w, float h, Eigen::VectorXf vision, int decision) const;
};

#endif
