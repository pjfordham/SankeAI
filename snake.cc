#include "snake.h"
#include <cmath>
#include <vector>
#include "food.h"
#include "neural_net.h"
#include "globals.h"
#include <deque>

unsigned int Snake::foodSeeds = 0;

bool Snake::bodyCollide(int x, int y) const {
   // check if a position collides with the snakes body
   for(const auto &i : body)  {
      if(x == i.x && y == i.y)  {
         return true;
      }
   }
   return false;
}

void Snake::food_show(int xoffset, int yoffset, int _SIZE, int x, int y) const {
   // show the food
   sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
   shape.setFillColor(sf::Color(255,0,0));
   shape.setPosition(xoffset + _SIZE * x, yoffset + _SIZE * y);
   windowp->draw(shape);
}

void Snake::show(int xoffset, int yoffset, int _SIZE) const {
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

void Snake::move() {
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
