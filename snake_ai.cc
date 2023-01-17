#include "button.h"
#include "evolution_graph.h"
#include "food.h"
#include "matrix.h"
#include "neural_net.h"
#include "population.h"
#include "snake.h"
#include "globals.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Sleep.hpp>

int SIZE = 20;
int hidden_nodes = 16;
int hidden_layers = 2;
int fps = 100;  //15 is ideal for self play, increasing for AI does not directly increase speed, speed is dependant on processing power

int highscore = 0;

float mutationRate = 0.05;
float defaultmutation = mutationRate;

bool humanPlaying = false;  //false for AI, true to play yourself
bool replayBest = true;  //shows only the best of each generation
bool seeVision = false;  //see the snakes vision
bool modelLoaded = false;

int height = 800;
int width = 800;

//PFont font;

std::vector<int> evolution;

Button graphButton;
Button loadButton;
Button saveButton;
Button increaseMut;
Button decreaseMut;

EvolutionGraph graph;

Snake snake;
Snake model;

Population pop;

void settings() {
  // size(1200,800);
}

void setup() {
  // font = createFont("agencyfb-bold.ttf",32);
  evolution =  std::vector<int>();
  graphButton =  Button(349,15,100,30,"Graph");
  loadButton =  Button(249,15,100,30,"Load");
  saveButton =  Button(149,15,100,30,"Save");
  increaseMut =  Button(340,85,20,20,"+");
  decreaseMut =  Button(365,85,20,20,"-");
  // frameRate(fps);
  if(humanPlaying) {
    snake =  Snake();
  } else {
    pop =  Population(2000); //adjust size of population
  }
}

void draw() {
  // background(0);
  // noFill();
  // stroke(255);
  // line(400,0,400,height);
  // rectMode(CORNER);
  // rect(400 + SIZE,SIZE,width-400-40,height-40);
  // textFont(font);
  if(humanPlaying) {
    snake.move();
    snake.show();
    // fill(150);
    // textSize(20);
    // text("SCORE : "+snake.score,500,50);
    if(snake.dead) {
       snake =  Snake();
    }
  } else {
    if(!modelLoaded) {
      if(pop.done()) {
          highscore = pop.bestSnake.score;
          pop.calculateFitness();
          pop.naturalSelection();
      } else {
          pop.update();
          pop.show();
      }
      // fill(150);
      // textSize(25);
      // textAlign(LEFT);
      // text("GEN : "+pop.gen,120,60);
      //text("BEST FITNESS : "+pop.bestFitness,120,50);
      //text("MOVES LEFT : "+pop.bestSnake.lifeLeft,120,70);
      // text("MUTATION RATE : "+mutationRate*100+"%",120,90);
      // text("SCORE : "+pop.bestSnake.score,120,height-45);
      // text("HIGHSCORE : "+highscore,120,height-15);
      increaseMut.show();
      decreaseMut.show();
    } else {
      model.look();
      model.think();
      model.move();
      model.show();
      model.brain.show(0,0,360,790,model.vision, model.decision);
      if(model.dead) {
        Snake model =  Snake();
        model.brain = model.brain.clone();
        model = model;

     }
     // textSize(25);
     // fill(150);
     // textAlign(LEFT);
     // text("SCORE : "+model.score,120,height-45);
    }
    // textAlign(LEFT);
    // textSize(18);
    // fill(255,0,0);
    // text("RED < 0",120,height-75);
    // fill(0,0,255);
    // text("BLUE > 0",200,height-75);
    graphButton.show();
    loadButton.show();
    saveButton.show();
  }

}

// void fileSelectedIn(File selection) {
//   if (selection == null) {
//     println("Window was closed or the user hit cancel.");
//   } else {
//     String path = selection.getAbsolutePath();
//     Table modelTable = loadTable(path,"header");
//     Matrix[] weights =  Matrix[modelTable.getColumnCount()-1];
//     float[][] in =  float[hidden_nodes][25];
//     for(int i=0; i< hidden_nodes; i++) {
//       for(int j=0; j< 25; j++) {
//         in[i][j] = modelTable.getFloat(j+i*25,"L0");
//       }
//     }
//     weights[0] =  Matrix(in);

//     for(int h=1; h<weights.length-1; h++) {
//        float[][] hid =  float[hidden_nodes][hidden_nodes+1];
//        for(int i=0; i< hidden_nodes; i++) {
//           for(int j=0; j< hidden_nodes+1; j++) {
//             hid[i][j] = modelTable.getFloat(j+i*(hidden_nodes+1),"L"+h);
//           }
//        }
//        weights[h] =  Matrix(hid);
//     }

//     float[][] out =  float[4][hidden_nodes+1];
//     for(int i=0; i< 4; i++) {
//       for(int j=0; j< hidden_nodes+1; j++) {
//         out[i][j] = modelTable.getFloat(j+i*(hidden_nodes+1),"L"+(weights.length-1));
//       }
//     }
//     weights[weights.length-1] =  Matrix(out);

//     evolution =  std::vector<Integer>();
//     int g = 0;
//     int genscore = modelTable.getInt(g,"Graph");
//     while(genscore != 0) {
//        evolution.add(genscore);
//        g++;
//        genscore = modelTable.getInt(g,"Graph");
//     }
//     modelLoaded = true;
//     humanPlaying = false;
//     model =  Snake(weights.length-1);
//     model.brain.load(weights);
//   }
// }

// void fileSelectedOut(File selection) {
//   if (selection == null) {
//     println("Window was closed or the user hit cancel.");
//   } else {
//     String path = selection.getAbsolutePath();
//     Table modelTable =  Table();
//     Snake modelToSave = pop.bestSnake.clone();
//     Matrix[] modelWeights = modelToSave.brain.pull();
//     float[][] weights =  float[modelWeights.length][];
//     for(int i=0; i<weights.length; i++) {
//        weights[i] = modelWeights[i].toArray();
//     }
//     for(int i=0; i<weights.length; i++) {
//        modelTable.addColumn("L"+i);
//     }
//     modelTable.addColumn("Graph");
//     int maxLen = weights[0].length;
//     for(int i=1; i<weights.length; i++) {
//        if(weights[i].length > maxLen) {
//           maxLen = weights[i].length;
//        }
//     }
//     int g = 0;
//     for(int i=0; i<maxLen; i++) {
//        TableRow Row = modelTable.addRow();
//        for(int j=0; j<weights.length+1; j++) {
//            if(j == weights.length) {
//              if(g < evolution.size()) {
//                 Row.setInt("Graph",evolution.get(g));
//                 g++;
//              }
//            } else if(i < weights[j].length) {
//               Row.setFloat("L"+j,weights[j][i]);
//            }
//        }
//     }
//     saveTable(modelTable, path);

//   }
// }

// void mousePressed() {
//    if(graphButton.collide(mouseX,mouseY)) {
//        graph =  EvolutionGraph();
//    }
//    if(loadButton.collide(mouseX,mouseY)) {
//        selectInput("Load Snake Model", "fileSelectedIn");
//    }
//    if(saveButton.collide(mouseX,mouseY)) {
//        selectOutput("Save Snake Model", "fileSelectedOut");
//    }
//    if(increaseMut.collide(mouseX,mouseY)) {
//       mutationRate *= 2;
//       defaultmutation = mutationRate;
//    }
//    if(decreaseMut.collide(mouseX,mouseY)) {
//       mutationRate /= 2;
//       defaultmutation = mutationRate;
//    }
// }


// void keyPressed() {
//   if(humanPlaying) {
//     if(key == CODED) {
//        switch(keyCode) {
//           case UP:
//             snake.moveUp();
//             break;
//           case DOWN:
//             snake.moveDown();
//             break;
//           case LEFT:
//             snake.moveLeft();
//             break;
//           case RIGHT:
//             snake.moveRight();
//             break;
//        }
//     }
//   }
// }

const float TILE_SIZE = 20.0;

static void draw( sf::RenderWindow &window, Snake &snake ) {
   window.clear( sf::Color::Blue );

   int BOARD_SIZE = 40;//snake.get_board_size();
   sf::RectangleShape shape(sf::Vector2f(TILE_SIZE*BOARD_SIZE, TILE_SIZE*BOARD_SIZE));
   shape.setFillColor(sf::Color::Black);
   shape.setPosition(1*TILE_SIZE, 1*TILE_SIZE);
   window.draw(shape);

   // for (auto&& [ x, y, content ] : snake) {
   //    sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
   //    shape.setPosition((x+1)*TILE_SIZE, (y+1)*TILE_SIZE);
   //    switch (content) {
   //    default:
   //       shape.setFillColor(sf::Color::Black);
   //       break;
   //    case Snake::Food:
   //       shape.setFillColor(sf::Color::Red);
   //       break;
   //    case Snake::Body:
   //       shape.setFillColor(sf::Color::White);
   //       break;
   //    case Snake::Corpse:
   //       shape.setFillColor(sf::Color::Yellow);
   //       break;
   //    case Snake::Head:
   //       shape.setFillColor(sf::Color::Green);
   //       break;
   //    }
   //    window.draw(shape);
   // }
   window.display();
}

int main()
{
   const unsigned int BOARD_SIZE = 40;
   const unsigned int SCREEN_WIDTH  = (2 + BOARD_SIZE) * (int)TILE_SIZE;
   const unsigned int SCREEN_HEIGHT = (2 + BOARD_SIZE) * (int)TILE_SIZE;

   sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake");

   Snake snake(BOARD_SIZE);

   sf::Clock clock;

   while (window.isOpen()) {

      sf::sleep(sf::milliseconds(5));

      bool skip_pulse = true;

      if (clock.getElapsedTime().asMilliseconds() > 200) {
         skip_pulse = false;
      }

      for ( sf::Event event; window.pollEvent(event);) {
         if (event.type == sf::Event::Closed) {
            window.close();
         } else if ( event.type == sf::Event::KeyPressed ) {
            // Respond to key pressed events
            switch (event.key.code) {
            default:
               break;
            case sf::Keyboard::Escape:
               return 0;
               break;
            case sf::Keyboard::Space:
               // snake.reset();
               skip_pulse = false;
               break;
            case sf::Keyboard::Left:
               // snake.setDirection( Snake::Left );
               skip_pulse = false;
               break;
            case sf::Keyboard::Right:
               // snake.setDirection( Snake::Right );
               skip_pulse = false;
               break;
            case sf::Keyboard::Up:
               // snake.setDirection( Snake::Up );
               skip_pulse = false;
               break;
            case sf::Keyboard::Down:
               // snake.setDirection( Snake::Down );
               skip_pulse = false;
               break;
            }
            // Make the game more responsive. Accelerate pulse rate inline
            // with rate of keypresses.
            if (skip_pulse == false) {
               break;
            }
         }
      }

      if (!skip_pulse /*&& snake.pulse()*/) {
         clock.restart();
         draw( window, snake );
      }
   }

   return 0;
}
