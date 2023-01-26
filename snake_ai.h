#ifndef _SNAKE_AI_H_
#define _SNAKE_AI_H_

#include <cmath>
#include <vector>
#include "neural_net.h"
#include "snake.h"
#include <deque>


class SnakeAI   {

public:
   Snake snake;

   int lifeLeft = 200;  //amount of moves the snake can make before it dies
   int lifetime = 0;  //amount of time the snake has been alive
   float fitness = 0;
   bool replay = false;  //if this snake is a replay of best snake

   Eigen::VectorXf vision;  //snakes vision
   int decision;  //snakes decision

   NeuralNet brain;

   SnakeAI(unsigned int foodSeed);

   SnakeAI(unsigned int foodSeed, const NeuralNet &_brain );

   void move();

   SnakeAI cloneForReplay() const;

   void mutate();

   void calculateFitness();

   void look( bool seeVision );

   std::vector<float> lookInDirection(Pos direction, bool seeVision) const;

   void think();

};
#endif
