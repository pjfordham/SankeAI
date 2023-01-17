#ifndef _FOOD_H_
#define _FOOD_H_
struct PVector {
   int x,y;
   PVector() {}
   
   PVector(int a, int b) {
      x = a;
      y = b;
   }
   PVector add(PVector z) {
      return PVector(x+z.x, y+z.y);
   }
};

class Food {
public:
  PVector pos;
    
   Food() {
      int x = 0;//400 + SIZE + floor(random(38))*SIZE;
      int y = 0;//SIZE + floor(random(38))*SIZE;
      pos = PVector{x,y};
   }
    
   void show() {
      // stroke(0);
      // fill(255,0,0);
      // rect(pos.x,pos.y,SIZE,SIZE);
   }
    
   Food clone() {
      Food clone;
      clone.pos.x = pos.x;
      clone.pos.y = pos.y;
       
      return clone;
   }
};

#endif
