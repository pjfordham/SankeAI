#ifndef _GFX_H_
#define _GFX_H_

#include <string>
#include <SFML/Graphics.hpp>

extern int height;
extern int width;
extern int SIZE;
extern sf::Font font;
extern sf::RenderWindow *windowp;

void draw_circle(sf::RenderWindow &window, float x, float y, float radius,                 sf::Color color);
void draw_text_center(sf::RenderWindow &window, std::string txt, float x,                      float y, float size, sf::Color color);
void draw_text(sf::RenderWindow &window, std::string txt, float x, float y, float size,sf::Color color);
void draw_line(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color);
void draw_rectangle(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color);
void draw_rectangle_center(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color);

#endif
