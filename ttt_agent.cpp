//! This is starter program that uses all the parts of the tic tac toe
//! client.

#include "tic_tac_toe.h"
#include "soarTTT.h"
#include <stdio.h>

using namespace std;

int main() {
  ttt_t *ttt = ttt_init();
  Player soar(ttt);

  // Get current board state, whose turn it is,
  // whether there is a winner.
  ttt_state_t *state = ttt_get_state(ttt);
  if (state->winner == '\0'){
      cout << "No winner yet" << endl;
  }
  if (!state) {
    return 1;
  }
  printf("It is %c's turn\n", state->turn);
  while(true){
  soar.agent->RunSelf(1);
  cin.get();
  }

  // If it's X's turn, then play a move. Otherwise reset the board.
    if (state->turn == 'X') {
    // Make move
    ttt_move_t move;
    move.player = 'X';
    move.x = 0;
    move.y = 2;
    ttt_make_move(ttt, move);
  } else {
    // Reset game
    ttt_reset(ttt);
  }

  // Deallocate memory
  ttt_destroy(ttt);
}
