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

#endif
