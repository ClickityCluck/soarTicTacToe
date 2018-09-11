#ifndef TICTACTOE_H
#define TICTACTOE_H

#if __cplusplus
extern "C" {
#endif

typedef struct _ttt_t ttt_t;

typedef struct _ttt_state_t {
  char board[9];
  char turn;
  char winner;
} ttt_state_t;

typedef struct _ttt_move_t {
  char player;
  int x;
  int y;
} ttt_move_t;

/// Initialize a new tic tac toe client.
///
/// When finished, deallocate the returned pointer by calling
/// `ttt_destroy()`.
ttt_t *ttt_init();

/// Destroy the client. Do not use the pointer after calling this.
void ttt_destroy(ttt_t *);

/// Get the current state of the game. This tells you the value of
/// each board cell, whose turn it is, and whether there is winner.
///
/// You don't need to free the memory returned by this function.
ttt_state_t *ttt_get_state(ttt_t *);

/// Send a move to the server. If the move is invalid, it will be
/// rejected. Example:
///
/// ```
/// ttt_move_t move;
/// move.player = 'X';
/// move.x = 0;
/// move.y = 2;
/// ttt_make_move(ttt, move);
/// ```
void ttt_make_move(ttt_t *, ttt_move_t move);

/// Clear the game board and reset the game.
void ttt_reset(ttt_t *);

#if __cplusplus
} // extern "C"
#endif

#endif
