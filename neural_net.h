#ifndef _NEURAL_NET_H_
#define _NEURAL_NET_H_

#include <vector>
#include <Eigen/Dense>


class NeuralNet {

   struct Layer {
      Eigen::MatrixXf weights;
      Eigen::VectorXf bias;

      // set these during walk for training
      Eigen::VectorXf activation;
      Eigen::VectorXf activation_derivative;
      Eigen::VectorXf error_signal;

      Layer(int inputs, int outputs) :
         weights(outputs, inputs),
         bias(outputs),
         activation(outputs),
         activation_derivative(outputs),
         error_signal(outputs) {
      }
   };

   std::vector<Layer> layers;
   std::vector<int> get_sizes() const;

public:

   NeuralNet(const std::vector<int> &sizes);
   void mutate(float mr);

   Eigen::VectorXf output(const Eigen::VectorXf &inputsArr) const;
   Eigen::VectorXf doutput(const Eigen::VectorXf &inputsArr) ;

   NeuralNet crossover(const NeuralNet &partner) const;

   void show(float x, float y, float w, float h,
             const Eigen::VectorXf &vision, int decision) const;

   void train(const Eigen::VectorXf &inputs, const Eigen::VectorXf &targets, float eta);
};

bool test_training();

#endif
