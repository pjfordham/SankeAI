#include "population.h"
#include <fmt/core.h>

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
      bestSnake.brain.show(0,0,360,790,bestSnake.vision, bestSnake.decision);  // show the brain of the best snake
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
      return snakes[maxIndex].cloneForReplay();
   } else {
      //samebest++;
      //if(samebest > 2) {  //if the best snake has remained the same for more than 3 generations, raise the mutation rate
      //mutationRate *= 2;
      //samebest = 0;
      return bestSnake.cloneForReplay();
   }
}

SnakeAI Population::selectParent() const {
   //selects a random number in range of the fitnesssum and if a snake falls in that range then select it
   std::uniform_real_distribution<float> randomLocationRange(0, fitnessSum);
   static std::random_device rd;
   static std::mt19937 randomNumbers(rd());

   float rand = randomLocationRange ( randomNumbers );
   float summation = 0;
   for(const auto &snake : snakes ) {
      summation += snake.fitness;
      if(summation > rand) {
         return snake;
      }
   }
   return bestSnake;
}

void Population::naturalSelection() {
   std::vector<SnakeAI>  newSnakes;
   calculateFitnessSum();

   bestSnake = getBestSnakeAI();
   bestSnake.replay = true;

   gen++;

   newSnakes.emplace_back( gen, bestSnake.brain );  // add the best snake of the prior generation into the new generation

   for(int i = 1; i < snakes.size(); i++) {
      SnakeAI child{ gen, selectParent().brain.crossover(selectParent().brain)};
      child.mutate();
      newSnakes.push_back(child);
   }
   snakes = std::move( newSnakes );
   evolution.push_back(bestSnakeScore);
}

void Population::calculateFitness() {
   // calculate the fitnesses for each snake
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
