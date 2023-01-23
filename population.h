#ifndef _POPULATION_H_
#define _POPULATION_H_

#include <vector>
#include "snake.h"
#include "snake_ai.h"
#include "globals.h"


class Population {
public:

   std::vector<SnakeAI> snakes;

   int bestSnakeScore = 0;
   int gen = 0;
   // int samebest = 0;

   float bestFitness = 0;
   float fitnessSum = 0;

   Population(int size) :
      snakes( size ) {
   }

   bool done() const ;

   void update( bool seeVision );

   void show( bool replayBest ) const ;

   SnakeAI getBestSnakeAI();

   SnakeAI selectParent() const;

   void naturalSelection();

   void calculateFitness();

   void calculateFitnessSum();
};

#endif
