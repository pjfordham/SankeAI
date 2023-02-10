#include "snake.h"
#include <cmath>
#include <vector>
#include <random>
#include "neural_net.h"
#include "globals.h"
#include <deque>
#include "gfx.h"

unsigned int Snake::foodSeeds = 0;

bool Snake::bodyCollide(Pos pos) const {
   // check if a position collides with the snakes body
   for(const auto &i : body)  {
      if(pos == i) {
         return true;
      }
   }
   return false;
}

void Snake::show(int xoffset, int yoffset, int _SIZE) const {
   // show the food
   sf::RectangleShape shape(sf::Vector2f(SIZE-2, SIZE-2));
   shape.setFillColor(sf::Color(255,0,0));
   shape.setPosition(xoffset + 1 + _SIZE * food.x,
                     yoffset + 1 + _SIZE * food.y);
   windowp->draw(shape);

   // show the snake
   sf::Color fill(255,255,255);
   for(const auto &i : body) {
      sf::RectangleShape shape(sf::Vector2f(SIZE-2, SIZE-2));
      shape.setFillColor(fill);
      shape.setPosition(xoffset + 1 + _SIZE * i.x,
                        yoffset + 1 + _SIZE * i.y);
      windowp->draw(shape);
   }
   if(dead) {
      fill = sf::Color(150,150,150);
      sf::RectangleShape shape(sf::Vector2f(SIZE-2, SIZE-2));
      shape.setFillColor(fill);
      shape.setPosition(xoffset + 1 + _SIZE * body[0].x,
                        yoffset + 1 + _SIZE * body[0].y);
      windowp->draw(shape);
   }
}

void Snake::move() {
   if (dead || vel == Pos{0,0})
      return;

   Pos new_head = body[0];

   new_head = new_head + vel;

   if(wallCollide(new_head) || bodyCollide(new_head)) {
      dead = true;
      return;
   }

   body.push_front(new_head);

   if (foodCollide(new_head)) {
      score++;
      do {
         food =  Pos{
            randomLocationWidth( randomNumbers ) ,
            randomLocationHeight( randomNumbers ) };
      } while(bodyCollide(food));
   } else {
      body.pop_back();
   }
}
