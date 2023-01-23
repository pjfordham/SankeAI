#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <cmath>
#include <vector>
#include "food.h"
#include "neural_net.h"
#include "globals.h"
#include "gfx.h"
#include <deque>

class Snake {

  static unsigned int foodSeeds;

public:
   static const int GAME_WIDTH=38;
   static const int GAME_HEIGHT=38;

   int score = 1;

   int xVel = 0;
   int yVel = 0;

   std::deque<Pos> body;  //snakes body

   bool dead = false;


   FoodList foodList;  //list of food positions (used to replay the best snake)

   Pos food;


   Snake() :
      foodList( foodSeeds++ ),
      body{Pos{GAME_WIDTH/2,GAME_HEIGHT/2} },
      food{ foodList.popFood() } {
   }

   Snake(const FoodList &foods) :
      foodList{ foods.getSeed() },
      body{Pos{GAME_WIDTH/2,GAME_HEIGHT/2}},
      food{ foodList.popFood() } {
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
