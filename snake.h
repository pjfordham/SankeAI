#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <cmath>
#include <vector>
#include "food.h"
#include "neural_net.h"
#include "globals.h"

class Snake {

public:
   int score = 1;
   int lifeLeft = 200;  //amount of moves the snake can make before it dies
   int lifetime = 0;  //amount of time the snake has been alive
   int xVel, yVel;
   int foodItterate = 0;  //itterator to run through the foodlist (used for replay)

   float fitness = 0;

   bool dead = false;
   bool replay = false;  //if this snake is a replay of best snake

   std::vector<float> vision;  //snakes vision
   std::vector<float> decision;  //snakes decision

   PVector head;

   std::vector<PVector> body;  //snakes body
   std::vector<Food> foodList;  //list of food positions (used to replay the best snake)

   Food food;
   NeuralNet brain;

   Snake() : Snake( hidden_layers ) {
   }

   Snake(int layers) {
      head = PVector{800,height/2};
      if(!humanPlaying) {
         foodList.push_back(food);
         brain = NeuralNet(24,hidden_nodes,4,layers);
         body.push_back(PVector{800,(height/2)+SIZE});
         body.push_back(PVector{800,(height/2)+(2*SIZE)});
         score+=2;
      }
   }

   Snake(std::vector<Food> foods) {  //this constructor passes in a list of food positions so that a replay can replay the best snake
      replay = true;
      for(Food f: foods) {  //clone all the food positions in the foodlist
         foodList.push_back(f);
      }
      food = foodList[foodItterate];
      foodItterate++;
      head = PVector{800,height/2};
      body.push_back(PVector{800,(height/2)+SIZE});
      body.push_back(PVector{800,(height/2)+(2*SIZE)});
      score+=2;
   }

   bool bodyCollide(float x, float y) {  //check if a position collides with the snakes body
      for(int i = 0; i < body.size(); i++) {
         if(x == body[i].x && y == body[i].y)  {
            return true;
         }
      }
      return false;
   }

   bool foodCollide(float x, float y) {  //check if a position collides with the food
      if(x == food.pos.x && y == food.pos.y) {
         return true;
      }
      return false;
   }

   bool wallCollide(float x, float y) {  //check if a position collides with the wall
      if(x >= width-(SIZE) || x < 400 + SIZE || y >= height-(SIZE) || y < SIZE) {
         return true;
      }
      return false;
   }

   void show() {  //show the snake
      food.show();
      sf::Color fill(255,255,255);
      // stroke(0);
      for(int i = 0; i < body.size(); i++) {
         sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
         shape.setFillColor(fill);
         shape.setPosition(body[i].x,body[i].y);
         windowp->draw(shape);
      }
      if(dead) {
         fill = sf::Color(150,150,150);
      } else {
         fill = sf::Color(255,255,255);
      }
      sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
      shape.setFillColor(fill);
      shape.setPosition(head.x,head.y);
      windowp->draw(shape);
   }

   void move() {  //move the snake
      if(!dead){
         if(!humanPlaying && !modelLoaded) {
            lifetime++;
            lifeLeft--;
         }
         if(foodCollide(head.x,head.y)) {
            eat();
         }
         shiftBody();
         if(wallCollide(head.x,head.y)) {
            dead = true;
         } else if(bodyCollide(head.x,head.y)) {
            dead = true;
         } else if(lifeLeft <= 0 && !humanPlaying) {
            dead = true;
         }
      }
   }

   void eat() {  //eat food
      int len = body.size()-1;
      score++;
      if(!humanPlaying && !modelLoaded) {
         if(lifeLeft < 500) {
            if(lifeLeft > 400) {
               lifeLeft = 500;
            } else {
               lifeLeft+=100;
            }
         }
      }
      if(len >= 0) {
         body.push_back(PVector{body[len].x,body[len].y});
      } else {
         body.push_back(PVector{head.x,head.y});
      }
      if(!replay) {
         food = Food();
         while(bodyCollide(food.pos.x,food.pos.y)) {
            food = Food();
         }
         if(!humanPlaying) {
            foodList.push_back(food);
         }
      } else {  //if the snake is a replay, then we dont want to create new random foods, we want to see the positions the best snake had to collect
         food = foodList[foodItterate];
         foodItterate++;
      }
   }

   void shiftBody() {  //shift the body to follow the head
      float tempx = head.x;
      float tempy = head.y;
      head.x += xVel;
      head.y += yVel;
      float temp2x;
      float temp2y;
      for(int i = 0; i < body.size(); i++) {
         temp2x = body[i].x;
         temp2y = body[i].y;
         body[i].x = tempx;
         body[i].y = tempy;
         tempx = temp2x;
         tempy = temp2y;
      }
   }

   Snake cloneForReplay() {  //clone a version of the snake that will be used for a replay
      Snake clone =  Snake(foodList);
      clone.brain = brain.clone();
      return clone;
   }

   Snake crossover(Snake parent) {  //crossover the snake with another snake
      Snake child = Snake(hidden_layers);
      child.brain = brain.crossover(parent.brain);
      return child;
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
      std::vector<float> temp = lookInDirection( PVector{-SIZE,0});
      vision[0] = temp[0];
      vision[1] = temp[1];
      vision[2] = temp[2];
      temp = lookInDirection( PVector{-SIZE,-SIZE});
      vision[3] = temp[0];
      vision[4] = temp[1];
      vision[5] = temp[2];
      temp = lookInDirection( PVector{0,-SIZE});
      vision[6] = temp[0];
      vision[7] = temp[1];
      vision[8] = temp[2];
      temp = lookInDirection( PVector{SIZE,-SIZE});
      vision[9] = temp[0];
      vision[10] = temp[1];
      vision[11] = temp[2];
      temp = lookInDirection( PVector{SIZE,0});
      vision[12] = temp[0];
      vision[13] = temp[1];
      vision[14] = temp[2];
      temp = lookInDirection( PVector{SIZE,SIZE});
      vision[15] = temp[0];
      vision[16] = temp[1];
      vision[17] = temp[2];
      temp = lookInDirection( PVector{0,SIZE});
      vision[18] = temp[0];
      vision[19] = temp[1];
      vision[20] = temp[2];
      temp = lookInDirection( PVector{-SIZE,SIZE});
      vision[21] = temp[0];
      vision[22] = temp[1];
      vision[23] = temp[2];
   }

   std::vector<float> lookInDirection(PVector direction) {  //look in a direction and check for food, body and wall
      std::vector<float> look;
      PVector pos{head.x,  head.y};
      float distance = 0;
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
            // stroke(0,255,0);
            // point(pos.x,pos.y);
            if(foodFound) {
               // noStroke();
               // fill(255,255,51);
               // ellipseMode(CENTER);
               // ellipse(pos.x,pos.y,5,5);
            }
            if(bodyFound) {
               // noStroke();
               // fill(102,0,102);
               // ellipseMode(CENTER);
               // ellipse(pos.x,pos.y,5,5);
            }
         }
         pos.add(direction);
         distance +=1;
      }
      if(replay && seeVision) {
         // noStroke();
         //  fill(0,255,0);
         // ellipseMode(CENTER);
         // ellipse(pos.x,pos.y,5,5);
      }
      look[2] = 1/distance;
      return look;
   }

   void think() {  //think about what direction to move
      decision = brain.output(vision);
      int maxIndex = 0;
      float max = 0;
      for(int i = 0; i < decision.size(); i++) {
         if(decision[i] > max) {
            max = decision[i];
            maxIndex = i;
         }
      }

      switch(maxIndex) {
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

   void moveUp() {
      if(yVel!=SIZE) {
         xVel = 0; yVel = -SIZE;
      }
   }
   void moveDown() {
      if(yVel!=-SIZE) {
         xVel = 0; yVel = SIZE;
      }
   }
   void moveLeft() {
      if(xVel!=SIZE) {
         xVel = -SIZE; yVel = 0;
      }
   }
   void moveRight() {
      if(xVel!=-SIZE) {
         xVel = SIZE; yVel = 0;
      }
   }
};
#endif
