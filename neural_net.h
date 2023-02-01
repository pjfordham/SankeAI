#ifndef _NEURAL_NET_H_
#define _NEURAL_NET_H_

#include <vector>
#include <Eigen/Dense>


class NeuralNet {
   int iNodes, hNodes, oNodes, hLayers;

   struct Layer {
      Eigen::MatrixXf weights;
      Eigen::VectorXf bias;

      Layer(int inputs, int outputs) :
         weights(outputs, inputs),
         bias(outputs) {
      }
   };

   std::vector<Layer> layers;

public:

   NeuralNet(int input, int hidden, int output, int hiddenLayers);
   void mutate(float mr);

   Eigen::VectorXf output(const Eigen::VectorXf &inputsArr) const;

   NeuralNet crossover(const NeuralNet &partner) const;

   void show(float x, float y, float w, float h,
             const Eigen::VectorXf &vision, int decision) const;
};

#endif
