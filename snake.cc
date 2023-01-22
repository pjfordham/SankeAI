#include "snake.h"
#include <cmath>
#include <vector>
#include "food.h"
#include "neural_net.h"
#include "globals.h"
#include <deque>

unsigned int SnakeBase::foodSeeds = 0;

bool SnakeBase::bodyCollide(int x, int y) const {
   // check if a position collides with the snakes body
   for(const auto &i : body)  {
      if(x == i.x && y == i.y)  {
         return true;
      }
   }
   return false;
}

void SnakeBase::food_show(int xoffset, int yoffset, int _SIZE, int x, int y) const {
   // show the food
   sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
   shape.setFillColor(sf::Color(255,0,0));
   shape.setPosition(xoffset + _SIZE * x, yoffset + _SIZE * y);
   windowp->draw(shape);
}

void SnakeBase::show(int xoffset, int yoffset, int _SIZE) const {
   // show the snake
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

void SnakeBase::move() {
   if (dead || (xVel == 0 && yVel == 0))
      return;

   int head_x = head.x;
   int head_y = head.y;

   head_x += xVel;
   head_y += yVel;

   if(wallCollide(head_x,head_y) || bodyCollide(head_x,head_y)) {
      dead = true;
      return;
   }

   body.push_front( head );
   head.x = head_x;
   head.y = head_y;

   if (foodCollide(head.x,head.y)) {
      score++;
      do {
         food = foodList.popFood();
      } while(bodyCollide(food.x,food.y) || headCollide(food.x,food.y));
   } else {
      body.pop_back();
   }
}

Snake::Snake(int layers) :
   snake() {
   brain = NeuralNet(24,hidden_nodes,4,layers);
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.score+=2;
}

Snake::Snake(const FoodList &foods) :
   snake( foods ) {
   brain = NeuralNet(24,hidden_nodes,4,hidden_layers);
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,2+snake.GAME_HEIGHT/2});
   snake.score+=2;
}

Snake::Snake(const NeuralNet &_brain) :
   snake() {
   brain = _brain;
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,2+snake.GAME_HEIGHT/2});
   snake.score+=2;
}

Snake::Snake(const FoodList &foods, const NeuralNet &_brain ) :
   snake( foods ),
   brain( _brain ) {
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,2+snake.GAME_HEIGHT/2});
   snake.score+=2;
   replay = true;
}

void Snake::move() {  //move the snake
   int old_score = snake.score;
   snake.move();
   // If the score changed when we moved then
   // assume we ate something and grant more
   // time.
   if (snake.score != old_score) {
      if(!modelLoaded) {
         if(lifeLeft < 500) {
            lifeLeft = std::max( lifeLeft + 100 , 500 );
         }
      }
   }
   if(!snake.dead){
      if(!modelLoaded) {
         lifetime++;
         lifeLeft--;
      }
      if(lifeLeft <= 0) {
         snake.dead = true;
      }
   }
}

Snake Snake::cloneForReplay() const {  //clone a version of the snake that will be used for a replay
   return { snake.foodList, brain };
}

Snake Snake::crossover(Snake parent) {  //crossover the snake with another snake
   Snake child(hidden_layers);
   child.brain = brain.crossover(parent.brain);
   return { brain.crossover(parent.brain) };
}

void Snake::mutate() {  //mutate the snakes brain
   brain.mutate(mutationRate);
}

void Snake::calculateFitness() {  //calculate the fitness of the snake
   if( snake.score < 10) {
      fitness = std::floor(lifetime * lifetime) * std::pow(2, snake.score);
   } else {
      fitness = std::floor(lifetime * lifetime);
      fitness *= std::pow(2,10);
      fitness *= (snake.score-9);
   }
}

void Snake::look() {  //look in all 8 directions and check for food, body and wall
   const auto directions = {
      Pos{-1,0},
      Pos{-1,-1},
      Pos{0,-1},
      Pos{1,-1},
      Pos{1,0},
      Pos{1,1},
      Pos{0,1},
      Pos{-1,1} };

   vision.clear();
   vision.reserve(24);

   for( const auto &direction : directions ) {
      std::vector<float> temp = lookInDirection( direction );
      vision.push_back( temp[0] );
      vision.push_back( temp[1] );
      vision.push_back( temp[2] );
   }
}

std::vector<float> Snake::lookInDirection(Pos direction) const {  //look in a direction and check for food, body and wall
   int xoffset = 400+SIZE;
   int yoffset = SIZE;

   std::vector<float> look;
   look.resize(3);
   Pos pos{snake.head.x, snake.head.y};
   int distance = 0;
   bool foodFound = false;
   bool bodyFound = false;
   pos = pos + direction;
   distance +=1;
   while (!snake.wallCollide(pos.x,pos.y)) {
      if(!foodFound && snake.foodCollide(pos.x,pos.y)) {
         foodFound = true;
         look[0] = 1;
      }
      if(!bodyFound && snake.bodyCollide(pos.x,pos.y)) {
         bodyFound = true;
         look[1] = 1;
      }
      if(replay && seeVision) {
         draw_line(*windowp,
                   xoffset + (snake.head.x * SIZE) + SIZE / 2,
                   yoffset + (snake.head.y * SIZE) + SIZE / 2,
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
      pos = pos + direction;
      distance +=1;
   }
   if(replay && seeVision) {
      draw_circle(*windowp, xoffset + (pos.x*SIZE),  yoffset+ (pos.y*SIZE), SIZE/2.0,
                  sf::Color::Green);
   }
   look[2] = 1.0/distance;
   return look;
}

void Snake::think() {  //think about what direction to move
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
      snake.moveUp();
      break;
   case 1:
      snake.moveDown();
      break;
   case 2:
      snake.moveLeft();
      break;
   case 3:
      snake.moveRight();
      break;
   }
}
