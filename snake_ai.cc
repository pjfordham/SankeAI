#include <cmath>
#include <vector>
#include <deque>
#include <fmt/core.h>

#include "snake_ai.h"
#include "neural_net.h"
#include "globals.h"
#include "gfx.h"

// Numbers of nodes in each layer starting with inputs, then hidden layers,
// then outputs.
const std::vector<int> topology{ 3 * 3 + 1 + 3, 10, 7, 3};

const int game_width = 6;
const int game_height = 6;

SnakeAI::SnakeAI( unsigned int foodSeed, const NeuralNet &_brain ) :
   snake( foodSeed, game_width, game_height ),
   brain( _brain ) {
   snake.moveUp();
}

SnakeAI::SnakeAI( unsigned int foodSeed ) :
   SnakeAI( foodSeed , { topology } ) {
}

void SnakeAI::move() {  //move the snake
   int old_score = snake.score;
   snake.move();
   // If the score changed when we moved then
   // assume we ate something and grant more
   // time.
   if (snake.score != old_score) {
      if(!modelLoaded) {
         lifeLeft = std::min( lifeLeft + 100 , 500 );
         lifetime = 0;
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
   return { snake.foodSeed, brain };
}

void SnakeAI::mutate() {  //mutate the snakes brain
   brain.mutate(mutationRate);
}

void SnakeAI::calculateFitness() {  //calculate the fitness of the snake
   fitness = snake.score * 1000 + lifetime;
}

static Pos rotate_right(Pos vec) {
   return { vec.y, -vec.x };
}

static Pos rotate_left(Pos vec) {
   return { -vec.y, vec.x };
}

static void output( Eigen::MatrixXf m )  {
   for(int i = 0; i < m.rows(); i++) {
      for(int j = 0; j < m.cols(); j++) {
         fmt::print("{:<+06.3f} ", m(i,j));
      }
      fmt::print("\n");
   }
   fmt::print("\n");
}

void SnakeAI::look( bool seeVision ) {  //look in all 8 directions and check for food, body and wall
   vision.resize( topology[0] ); // number of inputs
   int x = 0;

   for( const auto &direction : {  rotate_left( snake.vel ), snake.vel, rotate_right( snake.vel ) } ) {
      std::vector<float> temp = lookInDirection( direction, seeVision );
      vision(x++) = temp[0];
      vision(x++) = temp[1];
      vision(x++) = temp[2];
   }

   vision(x++) = 1.0 / lifeLeft;         // Make last input how hungry the snake is.
   vision(x++) = decision == 0 ? 1 : 0;  // Remeber what turn we made last.
   vision(x++) = decision == 1 ? 1 : 0;
   vision(x++) = decision == 2 ? 1 : 0;
}

std::vector<float> SnakeAI::lookInDirection(Pos direction, bool seeVision ) const {  //look in a direction and check for food, body and wall
   int xoffset = 400+SIZE;
   int yoffset = SIZE;

   std::vector<float> look(3);

   bool foodFound = false;
   bool bodyFound = false;

   Pos pos = snake.body[0] + direction;
   int distance = 1;

   while (!snake.wallCollide(pos)) {
      if(!foodFound && snake.foodCollide(pos)) {
         foodFound = true;
         look[0] = 1;
      }
      if(!bodyFound && snake.bodyCollide(pos)) {
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
      distance++;
   }
   if(replay && seeVision) {
      draw_circle(*windowp, xoffset + (pos.x*SIZE),  yoffset+ (pos.y*SIZE), SIZE/2.0,
                  sf::Color::Green);
   }
   look[2] = 1.0/distance;
   return look;
}

void SnakeAI::think() {  //think about what direction to move
   auto outputs = brain.output(vision);
   decision = 0;
   float max = 0;
   for(int i = 0; i < outputs.size(); i++) {
      if(outputs[i] > max) {
         max = outputs[i];
         decision = i;
      }
   }

   Pos svel = snake.vel;
   switch(decision) {
   case 0:
      // Turn left from the snake POV
      snake.vel = rotate_left( snake.vel );
      break;
   case 1:
      // Keep going forward
      break;
   case 2:
      // Turn right from the snake POV
      snake.vel = rotate_right( snake.vel );
      break;
   }
}
