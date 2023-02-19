#include <cmath>
#include <vector>
#include <deque>
#include <fmt/core.h>

#include "snake_q_ai.h"
#include "qlearn.h"
#include "globals.h"
#include "gfx.h"

SnakeQAI::SnakeQAI( unsigned int foodSeed, int _width, int _height ) :
   snake( foodSeed, _width, _height ),
   width(_width), height(_height)  {
   snake.moveUp();
}


static Pos rotate_left(Pos vec) {
   return { vec.y, -vec.x };
}

static Pos rotate_right_45(Pos vec) {
   return { vec.x + -vec.y, vec.x + vec.y };
}

static Pos rotate_right(Pos vec) {
   return { -vec.y, vec.x };
}

static Pos rotate_left_45(Pos vec) {
   return { vec.x + vec.y, -vec.x + vec.y };
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

int SnakeQAI::get_state(bool seeVision) const {  //look in all 8 directions and check for food, body and wall
   // check these are working?
   return
      (lookInDirection(rotate_left(snake.vel), seeVision) ? 1 : 0) |
      (lookInDirection(rotate_left_45(snake.vel), seeVision) ? 2 : 0) |
      (lookInDirection(snake.vel, seeVision) ? 4 : 0) |
      (lookInDirection(rotate_right_45(snake.vel), seeVision) ? 8 : 0) |
      (lookInDirection(rotate_right(snake.vel), seeVision) ? 16 : 0) |

      ( lookForWall( rotate_left( snake.vel ), seeVision ) ? 32 : 0 ) |
      ( lookForWall( rotate_left_45( snake.vel ) , seeVision) ? 64 : 0 ) |
      ( lookForWall( snake.vel , seeVision ) ? 128 : 0 ) |
      ( lookForWall( rotate_right_45( snake.vel ), seeVision ) ? 256 : 0 ) |
      ( lookForWall( rotate_right( snake.vel ), seeVision ) ? 512 : 0 ) |

      ( lookForBody( rotate_left( snake.vel ) , seeVision) ? 32 : 0 ) |
      ( lookForBody( rotate_left_45( snake.vel ), seeVision ) ? 64 : 0 ) |
      ( lookForBody( snake.vel , seeVision ) ? 128 : 0 ) |
      ( lookForBody( rotate_right_45( snake.vel ), seeVision ) ? 256 : 0 ) |
      ( lookForBody( rotate_right( snake.vel ), seeVision ) ? 512 : 0 );
}

bool SnakeQAI::lookInDirection(Pos direction, bool seeVision) const {  //look in a direction and check for food, body and wall
   int xoffset = 400+SIZE;
   int yoffset = SIZE;

   assert(!(direction.x == 0 && direction.y == 0));
   Pos pos = snake.body[0] + direction;
   int distance = 1;

   while (!snake.wallCollide(pos) && !snake.bodyCollide(pos)) {
      if(snake.foodCollide(pos)) {
         if (seeVision) {
            draw_line(*windowp,
                      xoffset + (snake.body[0].x * SIZE) + SIZE / 2,
                      yoffset + (snake.body[0].y * SIZE) + SIZE / 2,
                      xoffset + (pos.x * SIZE) + SIZE / 2,
                      yoffset + (pos.y * SIZE) + SIZE / 2,
                      sf::Color::Green);
         }
         return true;
      }
      pos = pos + direction;
   }
   return false;
}

bool SnakeQAI::lookForWall(Pos direction, bool seeVision) const {  //look in a direction and check for food, body and wall
   int xoffset = 400+SIZE;
   int yoffset = SIZE;
   assert(!(direction.x == 0 && direction.y == 0));
   Pos pos = snake.body[0] + direction;
   bool val =  snake.wallCollide(pos);
   if (val && seeVision) {
      sf::RectangleShape shape(sf::Vector2f(SIZE-2, SIZE-2));
      shape.setFillColor(sf::Color(255,255,0));
      shape.setPosition(xoffset + 1 + SIZE * pos.x,
                        yoffset + 1 + SIZE * pos.y);
      windowp->draw(shape);
   }
   return val;
}

bool SnakeQAI::lookForBody(Pos direction, bool seeVision) const {  //look in a direction and check for food, body and wall
   int xoffset = 400+SIZE;
   int yoffset = SIZE;
   assert(!(direction.x == 0 && direction.y == 0));
   Pos pos = snake.body[0] + direction;
   bool val =  snake.bodyCollide(pos);
   if (val && seeVision) {
      sf::RectangleShape shape(sf::Vector2f(SIZE-2, SIZE-2));
      shape.setFillColor(sf::Color(0,0,255));
      shape.setPosition(xoffset + 1 + SIZE * pos.x,
                        yoffset + 1 + SIZE * pos.y);
      windowp->draw(shape);
   }
   return val;
}

void SnakeQAI::think(bool greedy, bool seeVision) {  //think about what direction to move
   int state = get_state(seeVision);
   last_state = state;

   Pos svel = snake.vel;
   switch( decision = (greedy ? brain.greedy_action( state ) : brain.select_action( state )) ) {
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

void SnakeQAI::reset() {  //think about what direction to move
   snake = Snake(width,height);
   snake.moveUp();
   last_score = 1;
   last_state = 1;
}


void SnakeQAI::learn() {  //think about what direction to move
   int next_state = get_state(false);
   double reward = 100*(snake.score - last_score);
   last_score = snake.score;
   if ( reward == 0 ) reward = -1.0;
   if ( snake.dead ) {
      reward = -1000.0;
   }
   brain.update_Q(last_state, decision, next_state, reward);
   //if ( reward != -1.0 )
//      fprintf(stderr, "%2d %2d %2d %f\n", last_state, decision, next_state, reward);
}
