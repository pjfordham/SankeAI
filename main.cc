#include "qlearn.h"
#include "button.h"
#include "evolution_graph.h"
#include "neural_net.h"
#include "population.h"
#include "snake.h"
#include "snake_ai.h"
#include "snake_q_ai.h"
#include "globals.h"
#include "gfx.h"
#include <fmt/core.h>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Sleep.hpp>

int SIZE = 20;
int fps = 100;  //15 is ideal for self play, increasing for AI does not directly increase speed, speed is dependant on processing power

int highscore = 0;

float mutationRate = 0.05;
float defaultmutation = mutationRate;

bool humanPlaying = false;  //false for AI, true to play yourself
bool replayBest = true;  //shows only the best of each generation
bool seeVision = false;  //see the snakes vision
bool modelLoaded = false;

int height = 800;
int width = 1200;

sf::Font font;
sf::RenderWindow *windowp;

std::vector<int> evolution;

Button graphButton;
Button loadButton;
Button saveButton;
Button increaseMut;
Button decreaseMut;
Button visionButton;
Button replayButton;

EvolutionGraph graph;

// SnakeAI model;

void draw_board( sf::RenderWindow &window, int width, int height ) {
   window.clear( sf::Color::Black );

   auto top = 0 + SIZE / 2;
   auto bottom = 0 + SIZE / 2 + SIZE *( width + 1 );
   auto left = 400 + SIZE / 2;
   auto right = 400 + SIZE / 2 + SIZE * ( height + 1 );

   draw_line( window, left, top, right, top, sf::Color::White );
   draw_line( window, right, top, right, bottom, sf::Color::White );
   draw_line( window, right, bottom, left, bottom, sf::Color::White );
   draw_line( window, left, bottom, left, top, sf::Color::White );
}

void draw_score( sf::RenderWindow &window, Snake &snake, int high_score) {
   draw_text(window,fmt::format("SCORE : {}",snake.score),   120,height-75,25,sf::Color(150,150,150));
   draw_text(window,fmt::format("HIGHSCORE : {}",highscore), 120,height-50,25,sf::Color(150,150,150));
}

void draw_human_player( sf::RenderWindow &window , Snake &snake) {
   snake.move();
   snake.show();
   highscore = std::max( highscore, snake.score );
   draw_score(window, snake, highscore);
}

void draw_ai_player( sf::RenderWindow &window, Population &pop ) {
   // if(!modelLoaded) {
      if(pop.done()) {
         // Best score of a generation might not be best score ever.
         highscore = std::max( highscore, pop.bestSnakeScore );
         pop.calculateFitness();
         pop.naturalSelection();
      } else {
         pop.update( seeVision );
         pop.show( replayBest );
      }
      draw_text(window,fmt::format("BEST FITNESS : {}",pop.bestFitness),      120,50,15,sf::Color(150,150,150));
      draw_text(window,fmt::format("GEN : {}",pop.gen),                       120,65,15,sf::Color(150,150,150));
      draw_text(window,fmt::format("MOVES LEFT : {}",pop.bestSnake.lifeLeft), 120,80,15,sf::Color(150,150,150));
      draw_text(window,fmt::format("MUTATION RATE : {}%",mutationRate*100),   120,95,15,sf::Color(150,150,150));

      draw_score(window, pop.bestSnake.snake, highscore);

      increaseMut.show();
      decreaseMut.show();
   // } else {
   //    model.look( seeVision );
   //    model.think();
   //    model.move();
   //    model.snake.show();
   //    model.brain.show(0,0,360,790,model.vision, model.decision);
   //    if(model.snake.dead) {
   //       model = SnakeAI();
   //    }
   //    draw_text(window,fmt::format("SCORE : {}",model.snake.score),120,height-50,25,sf::Color(150,150,150));
   // }
   draw_text(window,"BLUE > 0", 200, height-100, 18, sf::Color::Blue);
   draw_text(window,"RED < 0" , 120, height-100, 18, sf::Color::Red);
   graphButton.show();
   loadButton.show();
   saveButton.show();
   visionButton.show();
   replayButton.show();
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
//     model =  SnakeAI(weights.length-1);
//     model.brain.load(weights);
//   }
// }

// void fileSelectedOut(File selection) {
//   if (selection == null) {
//     println("Window was closed or the user hit cancel.");
//   } else {
//     String path = selection.getAbsolutePath();
//     Table modelTable =  Table();
//     SnakeAI modelToSave = pop.bestSnake.clone();
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



int main_ai()
{

   const int board_width = 10;
   const int board_height = 10;


   fmt::print("Please wait, snakes are training.\n");
   std::vector<Population> populations;
   populations.emplace_back( 2000 );
   populations.emplace_back( 2000 );
   populations.emplace_back( 2000 );
   populations.emplace_back( 2000 );

   std::vector<std::thread> threads;
   for (auto& population : populations)
   {
      threads.emplace_back(std::thread(&Population::runAllSnakes, &population, 100));
   }

   for (auto& t : threads)
   {
      t.join();
   }

   std::sort( populations.begin(), populations.end(),
              [](Population &a, Population &b) { return a.bestFitness > b.bestFitness; });
   populations[0].gen = 0;

   Population pop( populations[0] );

   sf::RenderWindow window(sf::VideoMode(width,height), "SnakeAI");
   windowp = &window;

   if (!font.loadFromFile("../agencyfb-bold.ttf") ) {
      exit(-1);
   }

   saveButton   = Button(100,  10,90,30,"Save");
   loadButton   = Button(200,  10,90,30,"Load");
   graphButton  = Button(300,  10,90,30,"Graph");
   visionButton = Button(300,  50,90,30,"Vision");
   replayButton = Button(300,  90,90,30,"Replay");
   increaseMut  = Button(315, 130,20,30,"+");
   decreaseMut  = Button(355, 130,20,30,"-");

   while (window.isOpen()) {

      for ( sf::Event event; window.pollEvent(event);) {
         if (event.type == sf::Event::Closed) {
            window.close();
         } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
               auto mouseX = event.mouseButton.x;
               auto mouseY = event.mouseButton.y;

               if(graphButton.collide(mouseX,mouseY)) {
                  graph =  EvolutionGraph();
               }
               if(visionButton.collide(mouseX,mouseY)) {
                  seeVision = !seeVision;
               }
               if(replayButton.collide(mouseX,mouseY)) {
                  replayBest = !replayBest;
               }
               // if(loadButton.collide(mouseX,mouseY)) {
               //    selectInput("Load SnakeAI Model", "fileSelectedIn");
               // }
               // if(saveButton.collide(mouseX,mouseY)) {
               //    selectOutput("Save SnakeAI Model", "fileSelectedOut");
               // }
               if(increaseMut.collide(mouseX,mouseY)) {
                  mutationRate *= 2;
                  defaultmutation = mutationRate;
               }
               if(decreaseMut.collide(mouseX,mouseY)) {
                  mutationRate /= 2;
                  defaultmutation = mutationRate;
               }
            }
         } else if ( event.type == sf::Event::KeyPressed ) {
            // Respond to key pressed events
            switch (event.key.code) {
            default:
               break;
            case sf::Keyboard::Escape:
               return 0;
               break;
            }
            break;
         }
      }

      draw_board(window, board_width, board_height);
      draw_ai_player(window, pop);
      window.display();
   }

   return 0;
}

int main_q_ai()
{
   const int board_width = 10;
   const int board_height = 10;



   sf::RenderWindow window(sf::VideoMode(width,height), "Snake Q AI");
   windowp = &window;

   if (!font.loadFromFile("../agencyfb-bold.ttf") ) {
      exit(-1);
   }

   SnakeQAI snake(0, board_width, board_height );;

   sf::Clock clock;

   visionButton = Button(300,  50,90,30,"Vision");

   bool greedy = false;
   while (window.isOpen()) {

      if (greedy)
         sf::sleep(sf::milliseconds(5));

      bool skip_pulse = greedy;

      if (greedy && clock.getElapsedTime().asMilliseconds() > 200) {
         skip_pulse = false;
      }

      for ( sf::Event event; window.pollEvent(event);) {
         if (event.type == sf::Event::Closed) {
            window.close();
         } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
               auto mouseX = event.mouseButton.x;
               auto mouseY = event.mouseButton.y;

                if(visionButton.collide(mouseX,mouseY)) {
                  seeVision = !seeVision;
               }
              }
         }  else if ( event.type == sf::Event::KeyPressed ) {
            // Respond to key pressed events
            switch (event.key.code) {
            default:
               break;
            case sf::Keyboard::Escape:
               return 0;
               break;
            case sf::Keyboard::Space:
               snake.reset();
               skip_pulse = false;
               break;
            case sf::Keyboard::Left:
               greedy = !greedy;
               break;
            }
            // Make the game more responsive. Accelerate pulse rate inline
            // with rate of keypresses.
            if (skip_pulse == false) {
               break;
            }
         }
      }

      if (!snake.snake.dead && !skip_pulse) {
         clock.restart();

         draw_board(window, board_width, board_height);
         visionButton.show();

         snake.snake.move();
         snake.snake.show();

         highscore = std::max( highscore, snake.snake.score );
         draw_score(window, snake.snake, highscore);

         snake.learn();
         if (snake.snake.dead && !greedy) {
            snake.reset();
         }

         snake.think(greedy,seeVision);

         window.display();
      }
   }

   return 0;
}

int main_human()
{

   const int board_width = 38;
   const int board_height = 38;


   sf::RenderWindow window(sf::VideoMode(width,height), "Snake");
   windowp = &window;

   if (!font.loadFromFile("../agencyfb-bold.ttf") ) {
      exit(-1);
   }

   Snake snake( board_width, board_height );;

   // frameRate(fps);

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
         } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
            }
         } else if ( event.type == sf::Event::KeyPressed ) {
            // Respond to key pressed events
            switch (event.key.code) {
            default:
               break;
            case sf::Keyboard::Escape:
               return 0;
               break;
            case sf::Keyboard::Space:
               snake = Snake( board_width, board_height);
               skip_pulse = false;
               break;
            case sf::Keyboard::Left:
               snake.moveLeft();
               skip_pulse = false;
               break;
            case sf::Keyboard::Right:
               snake.moveRight();
               skip_pulse = false;
               break;
            case sf::Keyboard::Up:
               snake.moveUp();
               skip_pulse = false;
               break;
            case sf::Keyboard::Down:
               snake.moveDown();
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
         draw_board(window, board_width, board_height);
         draw_human_player(window, snake);
         window.display();
      }
   }

   return 0;
}

int main() {
   // test_training();
   // test_q_learning();
   return humanPlaying ? main_human() : main_q_ai();
}
