#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "globals.h"

#include <string>

class Button {
public:

   float X, Y, W, H;
   std::string text;
   Button() {}
   Button(float x, float y, float w, float h, std::string t) {
      X = x;
      Y = y;
      W = w;
      H = h;
      text = t;
   }

   bool collide(float x, float y) {
      if(x >= X && x <= X+W && y >= Y && y <= Y+H) {
         return true;
      }
      return false;
   }

   void show() {
      draw_rectangle(*windowp, X,Y,W,H,sf::Color::White);
      draw_text_center(*windowp,text,X+W/2,Y+(H/2)-3,H*0.8,sf::Color::Black);
   }
};


#endif
