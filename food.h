#ifndef _FOOD_H_
#define _FOOD_H_

#include "globals.h"
#include <cmath>
#include <random>
#include <fmt/core.h>

struct PVector {
   int x,y;
   void add(PVector z) {
      x = x + z.x;
      y = y + z.y;
   }
};

class FoodList {
   std::uniform_int_distribution<int> randomLocationRange;
   std::seed_seq seq;
   std::mt19937 randomNumbers;

   unsigned int seed;
   public:

   FoodList( const FoodList & that ) :
      randomLocationRange{ that.randomLocationRange },
      seq{ that.seed },
      randomNumbers{ seq },
      seed{ that.seed } {
      // This copy constructor will restart the list from it's initial state
   }

   FoodList& operator= ( const FoodList &that ) {
      randomLocationRange = that.randomLocationRange;
      seed = that.seed;
      std::seed_seq _seq{ seed };
      randomNumbers.seed( _seq );
      return *this;
   }

   FoodList(unsigned int _seed = 0xDEADBEEF, int lowRange = 0, int highRange = 37) :
      randomLocationRange{ lowRange, highRange },
      seq{ _seed },
      randomNumbers{ seq },
      seed( _seed ) {
   }

   unsigned int getSeed() const {
      return seed;
   }

   PVector popFood(int xoffset, int _SIZE) {
      return {
         xoffset + _SIZE + randomLocationRange( randomNumbers ) * _SIZE,
         _SIZE + randomLocationRange( randomNumbers ) * _SIZE };
   }
};


#endif
