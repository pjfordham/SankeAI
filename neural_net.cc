#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <fmt/core.h>

#include "globals.h"
#include "gfx.h"
#include "neural_net.h"

NeuralNet::NeuralNet(int input, int hidden, int output, int hiddenLayers) :
   iNodes{ input }, hNodes{ hidden },
   oNodes{ output }, hLayers{ hiddenLayers},
   weights{ static_cast<size_t>(hLayers+1) } {

   weights[0] = Matrix(hNodes, iNodes+1);
   for(int i=1; i<hLayers; i++) {
      weights[i] = Matrix(hNodes,hNodes+1);
   }
   auto size = weights.size();
   weights[size-1] = Matrix(oNodes,hNodes+1);

   for(auto&& w : weights) {
      w.randomize();
   }
}

void NeuralNet::mutate(float mr) {
   for(auto && w : weights) {
      w.mutate(mr);
   }
}

std::vector<float> NeuralNet::output(std::vector<float> inputsArr) {
   Matrix inputs = weights[0].singleColumnMatrixFromArray(inputsArr);

   Matrix curr_bias = inputs.addBias();

   for(int i=0; i<hLayers; i++) {
      Matrix hidden_ip = weights[i].dot(curr_bias);
      Matrix hidden_op = hidden_ip.activate();
      curr_bias = hidden_op.addBias();
   }

   Matrix output_ip = weights[weights.size()-1].dot(curr_bias);
   Matrix output = output_ip.activate();

   return output.toArray();
}

NeuralNet NeuralNet::crossover(NeuralNet partner) {
   NeuralNet child{iNodes,hNodes,oNodes,hLayers};
   for(int i=0; i<weights.size(); i++) {
      child.weights[i] = weights[i].crossover(partner.weights[i]);
   }
   return child;
}

void NeuralNet::show(float x, float y, float w, float h, std::vector<float> vision, int decision) const {
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

   const char *labels[] = { "U", "D", "L", "R" };

   for(int i = 0; i < oNodes; i++) {  //DRAW OUTPUTS
      sf::Color color = i == decision ? sf::Color::Green : sf::Color::White;
      draw_circle( *windowp,  x+(lc*nSpace)+(lc*nSize),y+oBuff+(i*(nSize+space)),nSize/2,color);
      draw_text_center(*windowp, labels[i],x+(lc*nSize)+(lc*nSpace)+nSize/2,y+oBuff+(nSize/2)+i*(space+nSize), nSize/2,sf::Color::Black);
   }

   lc = 1;

   //DRAW WEIGHTS
   for(int i = 0; i < weights[0].rows(); i++) {  //INPUT TO HIDDEN
      for(int j = 0; j < weights[0].cols()-1; j++) {
         sf::Color color =weights[0].m(i,j) < 0? sf::Color::Red : sf::Color::Blue;
         draw_line(*windowp,x+nSize,y+(nSize/2)+(j*(space+nSize)),x+nSize+nSpace,y+hBuff+(nSize/2)+(i*(space+nSize)),color);
      }
   }

   lc++;

   for(int a = 1; a < hLayers; a++) {
      for(int i = 0; i < weights[a].rows(); i++) {  //HIDDEN TO HIDDEN
         for(int j = 0; j < weights[a].cols()-1; j++) {
            sf::Color color = weights[a].m(i,j) < 0? sf::Color::Red : sf::Color::Blue;
            draw_line(*windowp,x+(lc*nSize)+((lc-1)*nSpace),y+hBuff+(nSize/2)+(j*(space+nSize)),x+(lc*nSize)+(lc*nSpace),y+hBuff+(nSize/2)+(i*(space+nSize)),color);
         }
      }
      lc++;
   }

   for(int i = 0; i < weights[weights.size()-1].rows(); i++) {  //HIDDEN TO OUTPUT
      for(int j = 0; j < weights[weights.size()-1].cols()-1; j++) {
         sf::Color color = weights[weights.size()-1].m(i,j) < 0? sf::Color::Red : sf::Color::Blue;
         draw_line(*windowp,x+(lc*nSize)+((lc-1)*nSpace),y+hBuff+(nSize/2)+(j*(space+nSize)),x+(lc*nSize)+(lc*nSpace),y+oBuff+(nSize/2)+(i*(space+nSize)),color);
      }
   }

}
