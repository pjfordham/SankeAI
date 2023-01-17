#ifndef _FOOD_H_
#define _FOOD_H_

#include "globals.h"
#include <cmath>
#include <random>

inline int random(int) { return 0; }

struct PVector {
   int x,y;
   constexpr PVector add(PVector z) const {
      return PVector{x+z.x, y+z.y};
   }
};

class Food {
public:
   PVector pos;
   
   Food() : pos{
         400 + SIZE + (int)std::floor(random(38))*SIZE,
         SIZE + (int)std::floor(random(38))*SIZE} {
   }

   void show() {
      // stroke(0);
      // fill(255,0,0);
      // rect(pos.x,pos.y,SIZE,SIZE);
   }

};

#endif
