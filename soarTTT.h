#ifndef soarTTT_H
#define soarTTT_H

#include <sml_Client.h>
#include "tic_tac_toe.h"

using namespace sml;

class Player{
public:
  Kernel* kernel;
  Agent* agent;

  ttt_t* connection;
  Player(ttt_t* apiHandle);

  StringElement* turn;
  StringElement* winner;
  Identifier* board;
  Identifier* a0;
  Identifier* a1;
  Identifier* a2;
  Identifier* b0;
  Identifier* b1;
  Identifier* b2;
  Identifier* c0;
  Identifier* c1;
  Identifier* c2;
  StringElement* A0;
  StringElement* A1;
  StringElement* A2;
  StringElement* B0;
  StringElement* B1;
  StringElement* B2;
  StringElement* C0;
  StringElement* C1;
  StringElement* C2;
};

#endif
