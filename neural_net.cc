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

static float activate(float x) {
   return x > 0.0 ? 1.0 : 0.0;//std::max(0.0f,x);
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

   weights.reserve( static_cast<size_t>(hLayers+1) );

   weights.push_back( Eigen::MatrixXf(hNodes, iNodes).unaryExpr([](float x){return randomize();}));
   for(int i=1; i<hLayers; i++) {
      weights.push_back( Eigen::MatrixXf(hNodes,hNodes).unaryExpr([](float x){return randomize();}));
   }
   weights.push_back( Eigen::MatrixXf(oNodes,hNodes).unaryExpr([](float x){return randomize();}) );

}

void NeuralNet::mutate(float mr) {
   for(auto && w : weights) {
      w = w.unaryExpr([mr](float x){return ::mutate(x,mr);});
   }
}



Eigen::VectorXf NeuralNet::output(Eigen::VectorXf inputs) const {

   // ::output( inputs );

   for(const auto &layer : weights) {
      // Matrix hidden_ip = layer.dot(curr_bias);
      // Matrix hidden_op = hidden_ip.activate();
      // curr_bias = hidden_op.addBias();
      inputs =  ( layer * inputs ).unaryExpr([](float x){return activate(x);}) ;
      // ::output( inputs );
   }

   // Matrix output_ip = weights[weights.size()-1].dot(curr_bias);
   // Matrix output = output_ip.activate();

   return inputs;
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

NeuralNet NeuralNet::crossover(NeuralNet partner) const {
   NeuralNet child{iNodes,hNodes,oNodes,hLayers};
   for(int i=0; i<weights.size(); i++) {
      child.weights[i] = ::crossover( weights[i] ,partner.weights[i]);
   }
   return child;
}

void NeuralNet::show(float x, float y, float w, float h, Eigen::VectorXf vision, int decision) const {
   float space = 5;  // vertical space betwee nodes
   float nSize = (h - ( space * ( iNodes-1 ) ) ) / iNodes; // Height less all the space between the nodes, shared equally between nodes
   float nSpace = (w - ((weights.size()+1)*nSize)) / (weights.size()); // Width less number of nodes deep times size of node.
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
   for(int i = 0; i < weights[0].rows(); i++) {  //INPUT TO HIDDEN
      for(int j = 0; j < weights[0].cols()-1; j++) {
         sf::Color color =weights[0](i,j) < 0? sf::Color::Red : sf::Color::Blue;
         draw_line(*windowp,x+nSize,y+(nSize/2)+(j*(space+nSize)),x+nSize+nSpace,y+hBuff+(nSize/2)+(i*(space+nSize)),color);
      }
   }

   lc++;

   for(int a = 1; a < hLayers; a++) {
      for(int i = 0; i < weights[a].rows(); i++) {  //HIDDEN TO HIDDEN
         for(int j = 0; j < weights[a].cols()-1; j++) {
            sf::Color color = weights[a](i,j) < 0? sf::Color::Red : sf::Color::Blue;
            draw_line(*windowp,x+(lc*nSize)+((lc-1)*nSpace),y+hBuff+(nSize/2)+(j*(space+nSize)),x+(lc*nSize)+(lc*nSpace),y+hBuff+(nSize/2)+(i*(space+nSize)),color);
         }
      }
      lc++;
   }

   for(int i = 0; i < weights[weights.size()-1].rows(); i++) {  //HIDDEN TO OUTPUT
      for(int j = 0; j < weights[weights.size()-1].cols()-1; j++) {
         sf::Color color = weights[weights.size()-1](i,j) < 0? sf::Color::Red : sf::Color::Blue;
         draw_line(*windowp,x+(lc*nSize)+((lc-1)*nSpace),y+hBuff+(nSize/2)+(j*(space+nSize)),x+(lc*nSize)+(lc*nSpace),y+oBuff+(nSize/2)+(i*(space+nSize)),color);
      }
   }

}
