#ifndef _BUTTON_H_
#define _BUTTON_H_

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
      if(x >= X-W/2 && x <= X+W/2 && y >= Y-H/2 && y <= Y+H/2) {
         return true;
      }
      return false;
   }

   void show() {
      // fill(255);
      // stroke(0);
      // rectMode(CENTER);
      // rect(X, Y, W, H);
      // textSize(22);
      // textAlign(CENTER,CENTER);
      // fill(0);
      // noStroke();
      // text(text,X,Y-3);
   }
};


#endif
