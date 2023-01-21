#ifndef _FOOD_H_
#define _FOOD_H_

#include "globals.h"
#include <cmath>
#include <random>
#include <fmt/core.h>

struct Pos {
   int x,y;
   bool operator==(const Pos&that) const {
      return x == that.x && y == that.y;
   }
   Pos operator+(const Pos&that) const {
      return {x+that.x, y+that.y};
   }
};

class FoodList {
   std::uniform_int_distribution<int> randomLocationRange;
   std::mt19937 randomNumbers;

   public:
   unsigned int seed;

   FoodList( const FoodList & that ) :
      randomLocationRange{ that.randomLocationRange },
      randomNumbers( that.seed ),
      seed{ that.seed } {
      // This copy constructor will restart the list from it's initial state
   }

   FoodList& operator= ( const FoodList &that ) {
      randomLocationRange = that.randomLocationRange;
      seed = that.seed;
      randomNumbers.seed( seed );
      return *this;
   }

   FoodList(unsigned int _seed = 0xDEADBEEF, int lowRange = 0, int highRange = 37) :
      randomLocationRange{ lowRange, highRange },
      randomNumbers{ _seed },
      seed( _seed ) {
   }

   unsigned int getSeed() const {
      return seed;
   }

   Pos popFood() {
      return {
         randomLocationRange( randomNumbers ) ,
         randomLocationRange( randomNumbers ) };
   }
};


#endif
