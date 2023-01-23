#include <cmath>
#include <vector>
#include <deque>

#include "snake_ai.h"
#include "food.h"
#include "neural_net.h"
#include "globals.h"

const auto directions = {
   Pos{-1, 0},
   Pos{-1,-1},
   Pos{ 0,-1},
   Pos{ 1,-1},
   Pos{ 1, 0},
   Pos{ 1, 1},
   Pos{ 0, 1},
   Pos{-1, 1},
};

const auto input_node_count = directions.size() * 3;

SnakeAI::SnakeAI(int layers) :
   snake() {
   brain = NeuralNet(input_node_count,hidden_nodes,4,layers);
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.score+=2;
}

SnakeAI::SnakeAI(const FoodList &foods) :
   snake( foods ) {
   brain = NeuralNet(input_node_count,hidden_nodes,4,hidden_layers);
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,2+snake.GAME_HEIGHT/2});
   snake.score+=2;
}

SnakeAI::SnakeAI(const NeuralNet &_brain) :
   snake() {
   brain = _brain;
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,2+snake.GAME_HEIGHT/2});
   snake.score+=2;
}

SnakeAI::SnakeAI(const FoodList &foods, const NeuralNet &_brain ) :
   snake( foods ),
   brain( _brain ) {
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,1+snake.GAME_HEIGHT/2});
   snake.body.push_back(Pos{snake.GAME_WIDTH/2,2+snake.GAME_HEIGHT/2});
   snake.score+=2;
   replay = true;
}

void SnakeAI::move() {  //move the snake
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

SnakeAI SnakeAI::cloneForReplay() const {  //clone a version of the snake that will be used for a replay
   return { snake.foodList, brain };
}

SnakeAI SnakeAI::crossover(SnakeAI parent) {  //crossover the snake with another snake
   SnakeAI child(hidden_layers);
   child.brain = brain.crossover(parent.brain);
   return { brain.crossover(parent.brain) };
}

void SnakeAI::mutate() {  //mutate the snakes brain
   brain.mutate(mutationRate);
}

void SnakeAI::calculateFitness() {  //calculate the fitness of the snake
   if( snake.score < 10) {
      fitness = std::floor(lifetime * lifetime) * std::pow(2, snake.score);
   } else {
      fitness = std::floor(lifetime * lifetime);
      fitness *= std::pow(2,10);
      fitness *= (snake.score-9);
   }
}

void SnakeAI::look() {  //look in all 8 directions and check for food, body and wall
   vision.clear();
   vision.reserve(input_node_count);

   for( const auto &direction : directions ) {
      std::vector<float> temp = lookInDirection( direction );
      vision.push_back( temp[0] );
      vision.push_back( temp[1] );
      vision.push_back( temp[2] );
   }
}

std::vector<float> SnakeAI::lookInDirection(Pos direction) const {  //look in a direction and check for food, body and wall
   int xoffset = 400+SIZE;
   int yoffset = SIZE;

   std::vector<float> look;
   look.resize(3);
   Pos pos{snake.body[0].x, snake.body[0].y};
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
                   xoffset + (snake.body[0].x * SIZE) + SIZE / 2,
                   yoffset + (snake.body[0].y * SIZE) + SIZE / 2,
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

void SnakeAI::think() {  //think about what direction to move
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
