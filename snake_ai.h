#ifndef _SNAKE_AI_H_
#define _SNAKE_AI_H_

#include <cmath>
#include <vector>
#include "food.h"
#include "neural_net.h"
#include "snake.h"
#include <deque>


class Snake   {

public:
   SnakeBase snake;

   int lifeLeft = 200;  //amount of moves the snake can make before it dies
   int lifetime = 0;  //amount of time the snake has been alive
   float fitness = 0;
   bool replay = false;  //if this snake is a replay of best snake

   std::vector<float> vision;  //snakes vision
   int decision;  //snakes decision

   NeuralNet brain;

   Snake() : Snake( hidden_layers ) {
   }

   Snake(int layers);

   Snake(const FoodList &foods);

   Snake(const NeuralNet &_brain);

   Snake(const FoodList &foods, const NeuralNet &_brain );

   void move();

   Snake cloneForReplay() const;

   Snake crossover(Snake parent);

   void mutate();

   void calculateFitness();

   void look();

   std::vector<float> lookInDirection(Pos direction) const;

   void think();

};
#endif
