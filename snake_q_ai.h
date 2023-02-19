#ifndef _SNAKE_Q_AI_H_
#define _SNAKE_Q_AI_H_

#include <cmath>
#include <vector>
#include "qlearn.h"
#include "snake.h"
#include <deque>

#define STATES 1024
#define ACTIONS 3
#define ALPHA 0.1
#define GAMMA 0.9
#define EPSILON 0.01

class SnakeQAI   {

public:
   Snake snake;

   int width, height;
   int last_score = 1;
   int last_state = 1;

   int decision = 0;  //snakes decision, init since we need to value to look back at.

   QTable brain{ STATES, ACTIONS, ALPHA, GAMMA, EPSILON };

   SnakeQAI(unsigned int foodSeed, int width, int height);

   void reset();

   int get_state( bool seeVision) const ;
   bool lookInDirection(Pos direction, bool seeVision) const;
   bool lookForWall(Pos direction, bool seeVision=true) const;
   bool lookForBody(Pos direction, bool seeVision=true) const;

   void learn();

   void think(bool greedy, bool seeVision);

};
#endif
