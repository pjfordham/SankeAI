#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <cmath>
#include <vector>
#include <random>
#include "neural_net.h"
#include "globals.h"
#include "gfx.h"
#include <deque>

struct Pos {
   int x,y;
   bool operator==(const Pos&that) const {
      return x == that.x && y == that.y;
   }
   Pos operator+(const Pos&that) const {
      return {x+that.x, y+that.y};
   }
};

class Snake {

   static unsigned int foodSeeds;

   std::uniform_int_distribution<int> randomLocationRange;
   std::mt19937 randomNumbers;

public:
   static const int GAME_WIDTH=38;
   static const int GAME_HEIGHT=38;

   int score = 1;

   int xVel = 0;
   int yVel = 0;

   std::deque<Pos> body;  //snakes body

   unsigned int foodSeed;  //list of food positions (used to replay the best snake)

   Pos food;

   bool dead = false;

   Snake() :
      randomLocationRange{ 0, 37 },
      randomNumbers{ foodSeeds },
      body{ Pos{GAME_WIDTH/2,GAME_HEIGHT/2} },
      foodSeed{ foodSeeds++ },
      food{ randomLocationRange( randomNumbers ), randomLocationRange( randomNumbers )  } {
   }

   Snake(unsigned int _foodSeed) :
      randomLocationRange{ 0, 37 },
      randomNumbers( _foodSeed ),
      body{ Pos{GAME_WIDTH/2,GAME_HEIGHT/2}},
      foodSeed{ _foodSeed },
      food{ randomLocationRange( randomNumbers ), randomLocationRange( randomNumbers )  } {
   }

   bool bodyCollide(int x, int y) const;

   bool foodCollide(int x, int y) const {
      // check if a position collides with the food
      return x == food.x && y == food.y;
   }

   bool wallCollide(int x, int y) const {
      // check if a position collides with the wall
      return x >= GAME_WIDTH || x < 0 || y >= GAME_HEIGHT || y < 0;
   }

   void food_show(int xoffset, int yoffset, int _SIZE, int x, int y) const;

   void show(int xoffset = 400+SIZE, int yoffset=SIZE, int _SIZE=SIZE) const;

   void move();

   void moveUp() {
      if(yVel!=1) {
         xVel = 0; yVel = -1;
      }
   }
   void moveDown() {
      if(yVel!=-1) {
         xVel = 0; yVel = 1;
      }
   }
   void moveLeft() {
      if(xVel!=1) {
         xVel = -1; yVel = 0;
      }
   }
   void moveRight() {
      if(xVel!=-1) {
         xVel = 1; yVel = 0;
      }
   }
};


#endif
