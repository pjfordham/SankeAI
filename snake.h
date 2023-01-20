#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <cmath>
#include <vector>
#include "food.h"
#include "neural_net.h"
#include "globals.h"

class SnakeBase {

  static unsigned int foodSeeds;

public:
   int score = 1;

   int xVel = 0;
   int yVel = 0;


   bool dead = false;

   PVector head;

   std::vector<PVector> body;  //snakes body

   FoodList foodList;  //list of food positions (used to replay the best snake)

   PVector food;


   int GAME_WIDTH=38;
   int GAME_HEIGHT=38;

   SnakeBase() :
      foodList( foodSeeds++ ) {
      head = PVector{GAME_WIDTH/2,GAME_HEIGHT/2};
      food = foodList.popFood();
   }

   SnakeBase(const FoodList &foods) :
      foodList( foods.getSeed() ) {
      head = PVector{GAME_WIDTH/2,GAME_HEIGHT/2};
      food = foodList.popFood();
   }

   bool bodyCollide(int x, int y) {  //check if a position collides with the snakes body
      for(auto &i : body) {
         if(x == i.x && y == i.y)  {
            return true;
         }
      }
      return false;
   }

   bool headCollide(int x, int y) {  // check if it collides with the head too.
      return x == head.x && y == head.y;
   }

   bool foodCollide(int x, int y) {  //check if a position collides with the food
      return x == food.x && y == food.y;
   }

   bool wallCollide(int x, int y) {  //check if a position collides with the wall
      return x >= GAME_WIDTH || x < 0 || y >= GAME_HEIGHT || y < 0;
   }

   void food_show(int xoffset, int yoffset, int _SIZE, int x, int y) const {
      sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
      shape.setFillColor(sf::Color(255,0,0));
      shape.setPosition(xoffset + _SIZE * x, yoffset + _SIZE * y);
      windowp->draw(shape);
   }

   void show(int xoffset = 400+SIZE, int yoffset=SIZE, int _SIZE=SIZE) const {  //show the snake
      food_show(xoffset,yoffset,_SIZE,food.x,food.y);
      sf::Color fill(255,255,255);
      for(const auto &i : body) {
         sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
         shape.setFillColor(fill);
         shape.setPosition(xoffset + _SIZE * i.x,
                           yoffset + _SIZE * i.y);
         windowp->draw(shape);
      }
      if(dead) {
         fill = sf::Color(150,150,150);
      } else {
         fill = sf::Color(255,255,255);
      }
      sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
      shape.setFillColor(fill);
      shape.setPosition(xoffset + _SIZE * head.x,
                        yoffset + _SIZE * head.y);
      windowp->draw(shape);
   }

   void move() {  //move the snake
      if(!dead){
         if(foodCollide(head.x,head.y)) {
            eat();
         }
         shiftBody();
         if(wallCollide(head.x,head.y)) {
            dead = true;
         } else if(bodyCollide(head.x,head.y)) {
            dead = true;
         }
      }
   }

   void eat() {  //eat food
      int len = body.size()-1;
      score++;
      if(len >= 0) {
         body.push_back(PVector{body[len].x,body[len].y});
      } else {
         body.push_back(PVector{head.x,head.y});
      }
      food = foodList.popFood();
      while(bodyCollide(food.x,food.y) || headCollide(food.x,food.y)) {
         food = foodList.popFood();
      }
   }

   void shiftBody() {  //shift the body to follow the head
      int tempx = head.x;
      int tempy = head.y;
      head.x += xVel;
      head.y += yVel;
      int temp2x;
      int temp2y;
      for(int i = 0; i < body.size(); i++) {
         temp2x = body[i].x;
         temp2y = body[i].y;
         body[i].x = tempx;
         body[i].y = tempy;
         tempx = temp2x;
         tempy = temp2y;
      }
   }


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

class Snake : public SnakeBase  {

public:
   int lifeLeft = 200;  //amount of moves the snake can make before it dies
   int lifetime = 0;  //amount of time the snake has been alive
   float fitness = 0;
   bool replay = false;  //if this snake is a replay of best snake

   std::vector<float> vision;  //snakes vision
   int decision;  //snakes decision

   NeuralNet brain;

   Snake() : Snake( hidden_layers ) {
   }

   Snake(int layers) :
      SnakeBase() {
      brain = NeuralNet(24,hidden_nodes,4,layers);
      body.push_back(PVector{GAME_WIDTH/2,1+GAME_HEIGHT/2});
      body.push_back(PVector{GAME_WIDTH/2,1+GAME_HEIGHT/2});
      score+=2;
   }

   Snake(const FoodList &foods) :
      SnakeBase( foods ) {
      brain = NeuralNet(24,hidden_nodes,4,hidden_layers);
      body.push_back(PVector{GAME_WIDTH/2,1+GAME_HEIGHT/2});
      body.push_back(PVector{GAME_WIDTH/2,2+GAME_HEIGHT/2});
      score+=2;
   }

   Snake(const NeuralNet &_brain) :
      SnakeBase() {
      brain = _brain;
      body.push_back(PVector{GAME_WIDTH/2,1+GAME_HEIGHT/2});
      body.push_back(PVector{GAME_WIDTH/2,2+GAME_HEIGHT/2});
      score+=2;
   }

   Snake(const FoodList &foods, const NeuralNet &_brain ) :
      SnakeBase( foods ),
      brain( _brain ) {
      body.push_back(PVector{GAME_WIDTH/2,1+GAME_HEIGHT/2});
      body.push_back(PVector{GAME_WIDTH/2,2+GAME_HEIGHT/2});
      score+=2;
      replay = true;
   }

   bool bodyCollide(int x, int y) {  //check if a position collides with the snakes body
      for(auto &i : body) {
         if(x == i.x && y == i.y)  {
            return true;
         }
      }
      return false;
   }


   void move() {  //move the snake
      int old_score = score;
      SnakeBase::move();
      // If the score changed when we moved then
      // assume we ate something and grant more
      // time.
      if (score != old_score) {
         if(!modelLoaded) {
            if(lifeLeft < 500) {
               lifeLeft = std::max( lifeLeft + 100 , 500 );
            }
         }
      }
      if(!dead){
         if(!modelLoaded) {
            lifetime++;
            lifeLeft--;
         }
         if(lifeLeft <= 0) {
            dead = true;
         }
      }
   }

   Snake cloneForReplay() {  //clone a version of the snake that will be used for a replay
      return { foodList, brain };
   }

   Snake crossover(Snake parent) {  //crossover the snake with another snake
      Snake child(hidden_layers);
      child.brain = brain.crossover(parent.brain);
      return { brain.crossover(parent.brain) };
   }

   void mutate() {  //mutate the snakes brain
      brain.mutate(mutationRate);
   }

   void calculateFitness() {  //calculate the fitness of the snake
      if(score < 10) {
         fitness = std::floor(lifetime * lifetime) * std::pow(2,score);
      } else {
         fitness = std::floor(lifetime * lifetime);
         fitness *= std::pow(2,10);
         fitness *= (score-9);
      }
   }

   void look() {  //look in all 8 directions and check for food, body and wall
      auto directions = {
         PVector{-1,0},
         PVector{-1,-1},
         PVector{0,-1},
         PVector{1,-1},
         PVector{1,0},
         PVector{1,1},
         PVector{0,1},
         PVector{-1,1} };

      vision.clear();
      vision.reserve(24);

      for( auto direction : directions ) {
         std::vector<float> temp = lookInDirection( direction );
         vision.push_back( temp[0] );
         vision.push_back( temp[1] );
         vision.push_back( temp[2] );
      }
   }

   std::vector<float> lookInDirection(PVector direction) {  //look in a direction and check for food, body and wall
      int xoffset = 400+SIZE;
      int yoffset = SIZE;

      std::vector<float> look;
      look.resize(3);
      PVector pos{head.x,  head.y};
      int distance = 0;
      bool foodFound = false;
      bool bodyFound = false;
      pos.add(direction);
      distance +=1;
      while (!wallCollide(pos.x,pos.y)) {
         if(!foodFound && foodCollide(pos.x,pos.y)) {
            foodFound = true;
            look[0] = 1;
         }
         if(!bodyFound && bodyCollide(pos.x,pos.y)) {
            bodyFound = true;
            look[1] = 1;
         }
         if(replay && seeVision) {
            draw_line(*windowp,
                      xoffset + (head.x * SIZE) + SIZE / 2,
                      yoffset + (head.y * SIZE) + SIZE / 2,
                      xoffset + (pos.x * SIZE) + SIZE / 2,
                      yoffset + (pos.y * SIZE) + SIZE / 2,
                      sf::Color::Green);
            if(foodFound) {
               draw_circle(*windowp, xoffset + (pos.x * SIZE), yoffset+ (pos.y*SIZE), SIZE/2.0,
                           sf::Color(255, 255, 51));
            }
            if(bodyFound) {
               draw_circle(*windowp, xoffset + (pos.x * SIZE),  yoffset+ (pos.y*SIZE), SIZE/2.0,
                           sf::Color(102, 0, 102));
            }
         }
         pos.add(direction);
         distance +=1;
      }
      if(replay && seeVision) {
         draw_circle(*windowp, xoffset + (pos.x*SIZE),  yoffset+ (pos.y*SIZE), SIZE/2.0,
                            sf::Color::Green);
      }
      look[2] = 1.0/distance;
      return look;
   }

   void think() {  //think about what direction to move
      std::vector<float> outputs = brain.output(vision);
      decision = 0;
      float max = 0;
      for(int i = 0; i < outputs.size(); i++) {
         if(outputs[i] > max) {
            max = outputs[i];
            decision = i;
         }
      }

      switch(decision) {
      case 0:
         moveUp();
         break;
      case 1:
         moveDown();
         break;
      case 2:
         moveLeft();
         break;
      case 3:
         moveRight();
         break;
      }
   }

};
#endif
