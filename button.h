#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>

class Button {
   float X, Y, W, H;
   std::string text;

public:

   Button() {}

   Button(float x, float y, float w, float h, const std::string &t)
      : X{x}, Y{y}, W{w}, H{h}, text{t} {
   }

   bool collide(float x, float y) const {
      return x >= X && x <= X+W && y >= Y && y <= Y+H;
   }

   void show() const;
};


#endif
