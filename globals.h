#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <vector>
#include <SFML/Graphics.hpp>

extern int height;
extern int width;

extern int SIZE;
extern int hidden_nodes;
extern int hidden_layers;
extern int fps;  //15 is ideal for self play, increasing for AI does not directly increase speed, speed is dependant on processing power

extern int highscore;

extern float mutationRate;
extern float defaultmutation;

extern bool humanPlaying;  //false for AI, true to play yourself
extern bool replayBest;  //shows only the best of each generation
extern bool seeVision;  //see the snakes vision
extern bool modelLoaded;

extern std::vector<int> evolution;
extern sf::Font font;
extern sf::RenderWindow *windowp;

inline void draw_circle(sf::RenderWindow &window,  float x, float y, float radius,sf::Color color) {
   sf::CircleShape shape(radius);
   shape.setPosition(x,y);
   shape.setFillColor(color);
   window.draw(shape);
}


inline void draw_text_center(sf::RenderWindow &window, std::string txt, float x, float y, float size,sf::Color color) {
   sf::Text text;
   text.setFont(font);
   text.setString(txt);
   text.setCharacterSize(size);
   sf::FloatRect bounds = text.getLocalBounds();
   text.setPosition(x-bounds.width/2,y-bounds.height/2);
   text.setFillColor(color);
   window.draw(text);
}

inline void draw_text(sf::RenderWindow &window, std::string txt, float x, float y, float size,sf::Color color) {
   sf::Text text;
   text.setFont(font);
   text.setString(txt);
   text.setCharacterSize(size);
   text.setPosition(x,y);
   text.setFillColor(color);
   window.draw(text);
}

inline void draw_line(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color) {
   sf::Vertex line[] =
      {
         sf::Vertex(sf::Vector2f(x, y),color),
         sf::Vertex(sf::Vector2f(z, a),color)
      };
    window.draw(line, 2, sf::Lines);
}

inline void draw_rectangle(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color) {
   sf::RectangleShape shape(sf::Vector2f(z, a));
   shape.setPosition(x,y);
   shape.setFillColor(color);
   window.draw(shape);
}

inline void draw_rectangle_center(sf::RenderWindow &window, float x, float y, float z, float a,sf::Color color) {
   sf::RectangleShape shape(sf::Vector2f(z, a));
   shape.setPosition(x-z/2,y-a/2);
   shape.setFillColor(color);
   window.draw(shape);
}

#endif
