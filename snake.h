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

   static unsigned int foodSeeds;

   std::uniform_int_distribution<int> randomLocationRange;
   std::mt19937 randomNumbers;

public:
   static const int GAME_WIDTH=38;
   static const int GAME_HEIGHT=38;

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
