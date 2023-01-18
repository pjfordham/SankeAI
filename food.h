#ifndef _FOOD_H_
#define _FOOD_H_

#include "globals.h"
#include <cmath>
#include <random>

struct PVector {
   int x,y;
   constexpr PVector add(PVector z) const {
      return PVector{x+z.x, y+z.y};
   }
};

class Food {
public:
   PVector pos;
   
   Food() {
      static std::uniform_int_distribution<int> randomLocationRange(0, 38-1);
      static std::random_device rd;
      static std::mt19937 randomNumbers(rd());
      pos.x =  400 + SIZE + randomLocationRange( randomNumbers ) * SIZE;
      pos.y = SIZE + randomLocationRange( randomNumbers ) * SIZE;
   }

   void show() {
      sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
      shape.setFillColor(sf::Color(255,0,0));
      shape.setPosition(pos.x,pos.y);
      windowp->draw(shape);
   }

};

#endif
