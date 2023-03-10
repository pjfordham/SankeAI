#include <string>
#include "gfx.h"

void draw_circle(sf::RenderWindow &window,  float x, float y, float radius,sf::Color color) {
   sf::CircleShape shape(radius);
   shape.setPosition(x,y);
   shape.setFillColor(color);
   window.draw(shape);
}


void draw_text_center(sf::RenderWindow &window, std::string txt, float x, float y, float size,sf::Color color) {
   sf::Text text;
   text.setFont(font);
   text.setString(txt);
   text.setCharacterSize(size);
   sf::FloatRect bounds = text.getLocalBounds();
   text.setPosition(x-bounds.width/2,y-size/2);
   text.setFillColor(color);
   window.draw(text);
}

void draw_text(sf::RenderWindow &window, std::string txt, float x, float y, float size,sf::Color color) {
   sf::Text text;
   text.setFont(font);
   text.setString(txt);
   text.setCharacterSize(size);
   text.setPosition(x,y);
   text.setFillColor(color);
   window.draw(text);
}

void draw_line(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color) {
   sf::Vertex line[] =
      {
         sf::Vertex(sf::Vector2f(x, y),color),
         sf::Vertex(sf::Vector2f(z, a),color)
      };
   window.draw(line, 2, sf::Lines);
}

void draw_rectangle(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color) {
   sf::RectangleShape shape(sf::Vector2f(z, a));
   shape.setPosition(x,y);
   shape.setFillColor(color);
   window.draw(shape);
}

void draw_rectangle_center(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color) {
   sf::RectangleShape shape(sf::Vector2f(z, a));
   shape.setPosition(x-z/2,y-a/2);
   shape.setFillColor(color);
   window.draw(shape);
}
