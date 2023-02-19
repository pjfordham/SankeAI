#include "qlearn.h"

#define STATES 100
#define ACTIONS 4
#define ALPHA 0.1
#define GAMMA 0.9
#define EPSILON 0.1
#define EPISODES 1000000

const int BOARD_SIZE = 10;
const int TILE_SIZE = 100.0;

int game_do_action(int state, int action) {
   int x = state % 10;
   int y = state / 10;
   switch (action) {
   case 0: // x--
      if (x > 0) {
         --x;
      }
      break;
   case 1: // x++
      if (x < 9 ) {
         ++x;
      }
      break;
   case 2:
      if (y > 0 ) {
         --y;
      }
      break;
   case 3:
      if (y < 9 ) {
         ++y;
      }
      break;
   default: break;
   }
   int next_state = y * 10 + x;

   return next_state;
}

void test_q_learning() {
   QTable Q{ STATES, ACTIONS, ALPHA, GAMMA, EPSILON};
   Q.clear();

   for (int i = 0; i < EPISODES; ++i) {
      int state = rand() % STATES;
      int action = Q.select_action(state);

      int next_state = game_do_action( state, action );

      double reward = 0;
      if (next_state == STATES - 1) {
         // Reached goal state
         reward = 1;
      }

      Q.update_Q(state, action, next_state, reward);
   }

   Q.print();
}
