#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <cmath>
#include <vector>
#include "food.h"
#include "neural_net.h"
#include "globals.h"
#include <deque>

class SnakeBase {

  static unsigned int foodSeeds;

public:
   int score = 1;

   int xVel = 0;
   int yVel = 0;


   bool dead = false;

   Pos head;

   std::deque<Pos> body;  //snakes body

   FoodList foodList;  //list of food positions (used to replay the best snake)

   Pos food;


   int GAME_WIDTH=38;
   int GAME_HEIGHT=38;

    SnakeBase() :
      foodList( foodSeeds++ ) {
      head = Pos{GAME_WIDTH/2,GAME_HEIGHT/2};
      food = foodList.popFood();
   }

   SnakeBase(const FoodList &foods) :
      foodList( foods.getSeed() ) {
      head = Pos{GAME_WIDTH/2,GAME_HEIGHT/2};
      food = foodList.popFood();
   }

   bool bodyCollide(int x, int y) const;

   bool headCollide(int x, int y) const {
      // check if it collides with the head too.
      return x == head.x && y == head.y;
   }

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
