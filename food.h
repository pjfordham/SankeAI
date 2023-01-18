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
      sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
      shape.setFillColor(sf::Color(255,0,0));
      shape.setPosition(pos.x,pos.y);
      windowp->draw(shape);
   }

};

#endif
