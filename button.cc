#include "globals.h"
#include "button.h"

void Button::show() const {
   draw_rectangle(*windowp, X,Y,W,H,sf::Color::White);
   draw_text_center(*windowp,text,X+W/2,Y+(H/2)-3,H*0.8,sf::Color::Black);
}
