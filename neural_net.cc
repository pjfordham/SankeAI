#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <fmt/core.h>
#include <random>
#include <algorithm>

#include "gfx.h"
#include "neural_net.h"

static float randomize() {
   static std::uniform_real_distribution<float> randomLocationRange(-1.0, 1.0);
   static std::random_device rd;
   static std::mt19937 randomNumbers(rd());
   return randomLocationRange( randomNumbers );
}

static float mutate(float x, float mutationRate) {
   static std::uniform_real_distribution<float> randomLocationRange(0.0, 1.0);
   static std::normal_distribution<float> randomNormal(0, 0.2);
   static std::random_device rd;
   static std::mt19937 randomNumbers(rd());
   float rand = randomLocationRange( randomNumbers );

   if(rand<mutationRate) {
      x = std::clamp(x + randomNormal( randomNumbers ), -1.0f, 1.0f);
   }
   return x;
}

void output( Eigen::MatrixXf m )  {
   for(int i = 0; i < m.rows(); i++) {
      for(int j = 0; j < m.cols(); j++) {
         fmt::print("{:<+06.3f} ", m(i,j));
      }
      fmt::print("\n");
   }
   fmt::print("\n");
}

NeuralNet::NeuralNet(int input, int hidden, int output, int hiddenLayers) :
   iNodes{ input }, hNodes{ hidden },
   oNodes{ output }, hLayers{ hiddenLayers} {

   layers.reserve( static_cast<size_t>(hLayers+1) );

   layers.push_back( Layer(iNodes, hNodes) );
   for(int i=1; i<hLayers; i++) {
      layers.push_back( Layer(hNodes,hNodes) );
   }
   layers.push_back( Layer(hNodes,oNodes) );

   for(auto && layer : layers) {
      layer.weights = layer.weights.unaryExpr([](float x){return ::randomize();});
      layer.bias = layer.bias.unaryExpr([](float x){return ::randomize();});
   }
}

void NeuralNet::mutate(float mr) {
   for(auto && layer : layers) {
       layer.weights = layer.weights.unaryExpr([mr](float x){return ::mutate(x,mr);});
       layer.bias = layer.bias.unaryExpr([mr](float x){return ::mutate(x,mr);});
   }
}

// Acctivation function (ReLU)
static float activate(float x) { return x > 0 ? x : x * 0.01f; }

// Derivative of the activation function (ReLU)
static float relu_derivative(float x) { return x > 0 ? 1 : 0.01f; }

Eigen::VectorXf NeuralNet::output(const Eigen::VectorXf &inputs) const {

   Eigen::VectorXf outputs = inputs;

   for(auto &&layer : layers) {
      outputs = (layer.weights * outputs + layer.bias )
         .unaryExpr([](float x){return activate(x);});
   }

   return outputs;

}


Eigen::MatrixXf crossover(const Eigen::MatrixXf& m , const Eigen::MatrixXf& partner)  {
   std::size_t rows = m.rows();
   std::size_t cols = m.cols();

   std::uniform_int_distribution<int> randomCols(0, cols);
   std::uniform_int_distribution<int> randomRows(0, rows);
   static std::random_device rd;
   static std::mt19937 randomNumbers(rd());

   Eigen::MatrixXf child{rows, cols};

   int randC = randomCols(randomNumbers);
   int randR = randomRows(randomNumbers);

   for(int i = 0; i < rows; i++) {
      for(int j = 0;  j < cols; j++) {
         if((i  < randR) || (i == randR && j <= randC)) {
            child(i,j) = m(i,j);
         } else {
            child(i,j) = partner(i,j);
         }
      }
   }
   return child;
}

NeuralNet NeuralNet::crossover(const NeuralNet &partner) const {
   NeuralNet child{iNodes,hNodes,oNodes,hLayers};
   for(int i=0; i<layers.size(); i++) {
      child.layers[i].weights = ::crossover( layers[i].weights ,partner.layers[i].weights);
      child.layers[i].bias = ::crossover( layers[i].bias ,partner.layers[i].bias);
   }
   return child;
}

void NeuralNet::show(float x, float y, float w, float h, const Eigen::VectorXf &vision, int decision) const {
   float space = 5;  // vertical space betwee nodes
   float nSize = (h - ( space * ( iNodes-1 ) ) ) / iNodes; // Height less all the space between the nodes, shared equally between nodes
   float nSpace = (w - ((layers.size()+1)*nSize)) / (layers.size()); // Width less number of nodes deep times size of node.
   float hBuff = (h - (space*(hNodes-1)) - (nSize*hNodes))/2; // horizontal buffer to center hidden nodes vertically
   float oBuff = (h - (space*(oNodes-1)) - (nSize*oNodes))/2; // horizontal buffer to center output nodes vertically.

   int lc = 0;  //Layer Count

   //DRAW NODES
   for(int i = 0; i < iNodes; i++) {  //DRAW INPUTS
      sf::Color color = vision[i] != 0 ? sf::Color::Green : sf::Color::White;
      draw_circle( *windowp, x,y+(i*(nSize+space)),nSize/2,color);
      draw_text_center( *windowp, fmt::format("{}",i), x+(nSize/2),y+(nSize/2)+(i*(nSize+space)), nSize/2 , sf::Color::Black );
   }

   lc++;

   for(int a = 0; a < hLayers; a++) {
      for(int i = 0; i < hNodes; i++) {  //DRAW HIDDEN
         sf::Color color =  sf::Color::White;
         draw_circle( *windowp, x+(lc*nSize)+(lc*nSpace),y+hBuff+(i*(nSize+space)),nSize/2,color);
      }
      lc++;
   }

   const char *labels[] = { "L","F","R" };

   for(int i = 0; i < oNodes; i++) {  //DRAW OUTPUTS
      sf::Color color = i == decision ? sf::Color::Green : sf::Color::White;
      draw_circle( *windowp,  x+(lc*nSpace)+(lc*nSize),y+oBuff+(i*(nSize+space)),nSize/2,color);
      draw_text_center(*windowp, labels[i],x+(lc*nSize)+(lc*nSpace)+nSize/2,y+oBuff+(nSize/2)+i*(space+nSize), nSize/2,sf::Color::Black);
   }

   lc = 1;

   //DRAW WEIGHTS
   for(int i = 0; i < layers[0].weights.rows(); i++) {  //INPUT TO HIDDEN
      for(int j = 0; j < layers[0].weights.cols(); j++) {
         sf::Color color = layers[0].weights(i,j) < 0? sf::Color::Red : sf::Color::Blue;
         draw_line(*windowp,x+nSize,y+(nSize/2)+(j*(space+nSize)),x+nSize+nSpace,y+hBuff+(nSize/2)+(i*(space+nSize)),color);
      }
   }

   lc++;

   for(int a = 1; a < hLayers; a++) {
      for(int i = 0; i < layers[a].weights.rows(); i++) {  //HIDDEN TO HIDDEN
         for(int j = 0; j < layers[a].weights.cols(); j++) {
            sf::Color color = layers[a].weights(i,j) < 0? sf::Color::Red : sf::Color::Blue;
            draw_line(*windowp,x+(lc*nSize)+((lc-1)*nSpace),y+hBuff+(nSize/2)+(j*(space+nSize)),x+(lc*nSize)+(lc*nSpace),y+hBuff+(nSize/2)+(i*(space+nSize)),color);
         }
      }
      lc++;
   }

   for(int i = 0; i < layers[layers.size()-1].weights.rows(); i++) {  //HIDDEN TO OUTPUT
      for(int j = 0; j < layers[layers.size()-1].weights.cols(); j++) {
         sf::Color color = layers[layers.size()-1].weights(i,j) < 0? sf::Color::Red : sf::Color::Blue;
         draw_line(*windowp,x+(lc*nSize)+((lc-1)*nSpace),y+hBuff+(nSize/2)+(j*(space+nSize)),x+(lc*nSize)+(lc*nSpace),y+oBuff+(nSize/2)+(i*(space+nSize)),color);
      }
   }

}
