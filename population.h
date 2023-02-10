#ifndef _POPULATION_H_
#define _POPULATION_H_

#include <vector>
#include "snake.h"
#include "snake_ai.h"
#include "globals.h"


class Population {
public:
   unsigned int gen;

   std::vector<SnakeAI> snakes;
   SnakeAI bestSnake;

   int bestSnakeScore = 0;
   // int samebest = 0;

   float bestFitness = 0;
   float fitnessSum = 0;

   Population(int size) :
      gen(0),
      bestSnake( gen ),
      snakes( size, SnakeAI( gen ) ) {
   }

   Population(int size, SnakeAI start) :
      gen(0),
      bestSnake( start ),
      snakes( size, start ) {
      for(auto &snake : snakes ) {
         snake.mutate();
      }
   }

   void runAllSnakes(int generations);
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
