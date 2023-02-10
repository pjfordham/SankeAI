#include "population.h"
#include <fmt/core.h>
#include <algorithm>
#include <execution>

void Population::runAllSnakes( int generations ) {

   for( int i = 0; i< generations ; ++i ) {
      while(!bestSnake.snake.dead) {
         bestSnake.look( false );
         bestSnake.think();
         bestSnake.move();
      }
     for(auto &snake : snakes ) {
         while(!snake.snake.dead) {
            snake.look( false );
            snake.think();
            snake.move();
         }
      }
      calculateFitness();
      naturalSelection();
   }

}

bool Population::done() const {
   // check if all the snakes in the population are dead
   if ( !bestSnake.snake.dead ) {
      return false;
   }
   for(const auto &i : snakes) {
      if(!i.snake.dead)
         return false;
   }
   return true;
}

void Population::update( bool seeVision ) {
   // update all the snakes in the generation
   if ( !bestSnake.snake.dead ) {
         bestSnake.look( seeVision );
         bestSnake.think();
         bestSnake.move();
   }
   for(auto &snake : snakes ) {
      if(!snake.snake.dead) {
         snake.look( false );
         snake.think();
         snake.move();
      }
   }
}

void Population::show( bool replayBest ) const {
   // show either the best snake or all the snakes
   if(replayBest) {
      bestSnake.snake.show();
      bestSnake.brain.show(0,200,400,500,bestSnake.vision, bestSnake.decision);  // show the brain of the best snake
   } else {
      for(auto snake = snakes.cbegin() + 1 ; snake != snakes.cend() ; snake++ ) {
         if(!snake->snake.dead ) {
            snake->snake.show();
         }
      }
   }
}

SnakeAI Population::getBestSnakeAI() {
   // set the best snake of the generation
   float max = 0;
   int maxIndex = 0;
   for(int i = 0; i < snakes.size(); i++) {
      if(snakes[i].fitness > max) {
         max = snakes[i].fitness;
         maxIndex = i;
      }
   }
   if(max > bestFitness) {
      bestFitness = max;
      bestSnakeScore = snakes[maxIndex].snake.score;
      //samebest = 0;
      //mutationRate = defaultMutation;
      fmt::print("NEW BEST: {:6} {:6} {:20} {}\n", snakes[maxIndex].snake.score, snakes[maxIndex].lifetime,  snakes[maxIndex].fitness,
                 snakes[maxIndex].snake.foodSeed );
      return snakes[maxIndex].cloneForReplay();
   } else {
      if (bestSnake.fitness != bestFitness) {
         fmt::print("WARNING: bestSnake is incosistent\n.");
      }
      //samebest++;
      //if(samebest > 2) {  //if the best snake has remained the same for more than 3 generations, raise the mutation rate
      //mutationRate *= 2;
      //samebest = 0;
      return bestSnake.cloneForReplay();
   }
}

void Population::naturalSelection() {
   std::vector<SnakeAI>  newSnakes;
   calculateFitnessSum();

   bestSnake = getBestSnakeAI();
   bestSnake.replay = true;

   gen++;

   std::sort( snakes.begin(), snakes.end(), []( const SnakeAI &a, const SnakeAI &b) { return a.fitness > b.fitness; });

   newSnakes.emplace_back( gen, bestSnake.brain );  // add the best snake overall into the new generation
   newSnakes.emplace_back( gen, snakes[0].brain );  // add the best snake of the prior generation into the new generation

   for(int i = 0; i < snakes.size()-2; i++) {
      SnakeAI child{ gen, snakes[i+1%500].brain.crossover(snakes[(i)%501].brain)};
      child.mutate();
      newSnakes.push_back(child);
   }
   snakes = std::move( newSnakes );
   evolution.push_back(bestSnakeScore);
}

void Population::calculateFitness() {
   // calculate the fitnesses for each snake
   bestSnake.calculateFitness();
   for(auto &snake : snakes) {
      snake.calculateFitness();
   }
}

void Population::calculateFitnessSum() {
   // calculate the sum of all the snakes fitnesses
   fitnessSum = 0;
   for(auto &snake : snakes)  {
      fitnessSum += snake.fitness;
   }
}
