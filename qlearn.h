#ifndef _QLEARN_H_
#define _QLEARN_H_

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdio.h>

class QTable {
public:
   const int STATES;
   const int ACTIONS;
   const double ALPHA;
   const double GAMMA;
   const double EPSILON;
   std::vector<std::vector<double>> Q;

   QTable(int states, int actions, double alpha, double gamma, double epsilon) :
      STATES( states ), ACTIONS( actions ), ALPHA( alpha ), GAMMA( gamma ), EPSILON( epsilon )
      {
         Q.resize(STATES);
         for (auto &&action : Q) {
            action.resize(ACTIONS);
         }
      }

   void print() {
      // Print learned Q-values
      printf("Learned Q-values:\n");
      for (int s = 0; s < STATES; ++s) {
         for (int a = 0; a < ACTIONS; ++a) {
            printf("Q[%d][%d] = %1.4f\n", s, a, Q[s][a]);
         }
      }
   }

   int greedy_action(int state) {
      // Greedy action
      int max_action = 0;
      for (int a = 1; a < ACTIONS; ++a) {
         if (Q[state][a] > Q[state][max_action]) {
            max_action = a;
         }
      }
      return max_action;
   }

   // Function to select an action based on epsilon-greedy policy
   int select_action(int state) {
      if ((double) rand() / (RAND_MAX + 1.0) < EPSILON) {
         // Random action
         return rand() % ACTIONS;
      } else {
         // Greedy action
         return greedy_action(state);
      }
   }

   // Function to update Q-values
   void update_Q(int state, int action, int next_state, double reward) {
      assert(state < STATES);
      assert(next_state < STATES);
      assert(action < ACTIONS);
      assert(state > -1);
      assert(next_state > -1);
      assert(action > -1);

      double max_Q = Q[next_state][0];
      for (int a = 0; a < ACTIONS; ++a) {
         if (Q[next_state][a] > max_Q) {
            max_Q = Q[next_state][a];
         }
      }
      Q[state][action] += ALPHA * (reward + GAMMA * max_Q - Q[state][action]);
   }

   void clear() {
      // Initialize Q-values to zero
      Q.clear();
      Q.resize(STATES);
      for (auto &&action : Q) {
         action.resize(ACTIONS);
      }
   }

};

void test_q_learning();

#endif
