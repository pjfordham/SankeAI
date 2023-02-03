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
   bool operator!=(const Pos&that) const {
      return !(x == that.x && y == that.y);
   }
   Pos operator+(const Pos&that) const {
      return {x+that.x, y+that.y};
   }
};

class Snake {

   int GAME_WIDTH;
   int GAME_HEIGHT;

   static unsigned int foodSeeds;

   std::uniform_int_distribution<int> randomLocationWidth;
   std::uniform_int_distribution<int> randomLocationHeight;
   std::mt19937 randomNumbers;

public:

   constexpr static const Pos    UP{ 0,-1};
   constexpr static const Pos  DOWN{ 0, 1};
   constexpr static const Pos  LEFT{-1, 0};
   constexpr static const Pos RIGHT{ 1, 0};


   int score = 1;

   Pos vel{0,0};

   std::deque<Pos> body;  //snakes body

   unsigned int foodSeed;  //list of food positions (used to replay the best snake)

   Pos food;

   bool dead = false;

   Snake(unsigned int _foodSeed, int width, int height) :
      GAME_WIDTH{ width },
      GAME_HEIGHT{ height },
      randomLocationWidth{ 0, GAME_WIDTH-1 },
      randomLocationHeight{ 0, GAME_HEIGHT-1 },
      randomNumbers( _foodSeed ),
      body{ Pos{GAME_WIDTH/2,GAME_HEIGHT/2}},
      foodSeed{ _foodSeed },
      food{ randomLocationWidth( randomNumbers ), randomLocationHeight( randomNumbers )  } {
   }

   Snake(int width, int height) :
      Snake{ foodSeeds ++ , width, height } {
   }

   bool bodyCollide(Pos pos) const;

   bool foodCollide(Pos pos) const {
      // check if a position collides with the food
      return pos == food;
   }

   bool wallCollide(Pos pos) const {
      // check if a position collides with the wall
      return pos.x >= GAME_WIDTH || pos.x < 0 || pos.y >= GAME_HEIGHT || pos.y < 0;
   }

   void food_show(int xoffset, int yoffset, int _SIZE, int x, int y) const;

   void show(int xoffset = 400+SIZE, int yoffset=SIZE, int _SIZE=SIZE) const;

   void move();

   void moveUp() {
      if(vel != DOWN) {
         vel = UP;
      }
   }
   void moveDown() {
      if(vel !=  UP) {
         vel = DOWN;
      }
   }
   void moveLeft() {
      if(vel != RIGHT) {
         vel = LEFT;
      }
   }
   void moveRight() {
      if(vel != LEFT) {
         vel = RIGHT;
      }
   }
};


#endif
