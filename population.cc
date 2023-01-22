#include "population.h"


bool Population::done() const {
   // check if all the snakes in the population are dead
   for(const auto &i : snakes) {
      if(!i.snake.dead)
         return false;
   }
   return true;
}

void Population::update() {
   // update all the snakes in the generation
   for(auto &snake : snakes ) {
      if(!snake.snake.dead) {
         snake.look();
         snake.think();
         snake.move();
      }
   }
}

void Population::show() const {
   // show either the best snake or all the snakes
   if(replayBest) {
      snakes[0].snake.show();
      snakes[0].brain.show(0,0,360,790,snakes[0].vision, snakes[0].decision);  // show the brain of the best snake
   } else {
      for(const auto &snake : snakes ) {
         if(!snake.snake.dead) {
            snake.snake.show();
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
      return snakes[0].cloneForReplay();
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
   return snakes[0];
}

void Population::naturalSelection() {
   std::vector<SnakeAI>  newSnakes;
   calculateFitnessSum();

   newSnakes.push_back( getBestSnakeAI() );  // add the best snake of the prior generation into the new generation
   newSnakes[0].replay = true; // enable vision visualization
   for(int i = 1; i < snakes.size(); i++) {
      SnakeAI child = selectParent().crossover(selectParent());
      child.mutate();
      newSnakes.push_back(child);
   }
   snakes = std::move( newSnakes );
   evolution.push_back(bestSnakeScore);
   gen+=1;
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
