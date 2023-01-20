#ifndef _POPULATION_H_
#define _POPULATION_H_

#include <vector>
#include "snake.h"
#include "globals.h"


class Population {
public:

   std::vector<Snake> snakes;
   Snake bestSnake;

   int bestSnakeScore = 0;
   int gen = 0;
   int samebest = 0;

   float bestFitness = 0;
   float fitnessSum = 0;

   Population(int size) :
      snakes( size ) {
      bestSnake.replay = true;
   }

   bool done() {  //check if all the snakes in the population are dead
      for(const auto &i : snakes) {
         if(!i.snake.dead)
            return false;
      }
      if(!bestSnake.snake.dead) {
         return false;
      }
      return true;
   }

   void update() {  //update all the snakes in the generation
      if(!bestSnake.snake.dead) {  //if the best snake is not dead update it, this snake is a replay of the best from the past generation
         bestSnake.look();
         bestSnake.think();
         bestSnake.move();
      }
      for(auto &snake : snakes ) {
         if(!snake.snake.dead) {
            snake.look();
            snake.think();
            snake.move();
         }
      }
   }

   void show() {  //show either the best snake or all the snakes
      if(replayBest) {
         bestSnake.snake.show();
         bestSnake.brain.show(0,0,360,790,bestSnake.vision, bestSnake.decision);  //show the brain of the best snake
      } else {
         for(const auto &snake : snakes ) {
            if(!snake.snake.dead) {
               snake.snake.show();
            }
         }
      }
   }

   void setBestSnake() {  //set the best snake of the generation
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
         bestSnake = snakes[maxIndex].cloneForReplay();
         bestSnakeScore = snakes[maxIndex].snake.score;
         //samebest = 0;
         //mutationRate = defaultMutation;
      } else {
         bestSnake = bestSnake.cloneForReplay();
         /*
           samebest++;
           if(samebest > 2) {  //if the best snake has remained the same for more than 3 generations, raise the mutation rate
           mutationRate *= 2;
           samebest = 0;
           }*/
      }
   }

   Snake selectParent() {  //selects a random number in range of the fitnesssum and if a snake falls in that range then select it
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

   void naturalSelection() {
      std::vector<Snake>  newSnakes;
      setBestSnake();
      calculateFitnessSum();

      newSnakes.push_back( bestSnake );  //add the best snake of the prior generation into the new generation
      for(int i = 1; i < snakes.size(); i++) {
         Snake child = selectParent().crossover(selectParent());
         child.mutate();
         newSnakes.push_back(child);
      }
      snakes = std::move( newSnakes );
      evolution.push_back(bestSnakeScore);
      gen+=1;
   }

   // void mutate() {
   //    for(int i = 1; i < snakes.size(); i++) {  //start from 1 as to not override the best snake placed in index 0
   //       snakes[i].mutate();
   //    }
   // }

   void calculateFitness() {  //calculate the fitnesses for each snake
      for(auto &snake : snakes) {
         snake.calculateFitness();
      }
   }

   void calculateFitnessSum() {  //calculate the sum of all the snakes fitnesses
      fitnessSum = 0;
      for(auto &snake : snakes)  {
         fitnessSum += snake.fitness;
      }
   }
};

#endif
