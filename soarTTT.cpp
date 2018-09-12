#include "soarTTT.h"
#include <cstring>
#include <iostream>

using namespace sml;
using namespace std;


const char* Converter(char character){
  if(character=='X'){
    return "X";
  }else if(character=='O'){
    return "O";
  }else{
    return "_";
  }
}

void inputEventHandler(smlRunEventId id, void* user_data, Agent* agent, smlPhase phase) {

  Player* player = (Player*)user_data;

  auto* state = ttt_get_state(player->connection);
  cout << state->turn << endl;
  cout << Converter(state->board[0]) << "|" << Converter(state->board[1]) << "|" << Converter(state->board[2]) << endl;
  cout << Converter(state->board[3]) << "|" << Converter(state->board[4]) << "|" << Converter(state->board[5]) << endl;
  cout << Converter(state->board[6]) << "|" << Converter(state->board[7]) << "|" << Converter(state->board[8]) << endl;
  player->turn->Update(Converter(state->turn));

  player->winner->Update(Converter(state->winner));


  player->A0->Update(Converter(state->board[0]));
  player->A1->Update(Converter(state->board[1]));
  player->A2->Update(Converter(state->board[2]));
  player->B0->Update(Converter(state->board[3]));
  player->B1->Update(Converter(state->board[4]));
  player->B2->Update(Converter(state->board[5]));
  player->C0->Update(Converter(state->board[6]));
  player->C1->Update(Converter(state->board[7]));
  player->C2->Update(Converter(state->board[8]));

  agent->Commit();

}

void outputEventHandler(smlRunEventId id, void* user_data, Agent* agent, smlPhase phase) {
  Player* player = (Player*)user_data;


  auto* ol = agent->GetOutputLink();

  int newOut = ol->GetNumberChildren();

  auto* Command = ol->FindByAttribute("command", 0)->ConvertToIdentifier();

  auto* name = Command->GetParameterValue("name");

  // cout << name << endl;

  if(newOut > 0){

    
    cout << "newOut#: " << newOut << endl;
    cout << agent->ExecuteCommandLine("p I3 -d 3") << endl;

    auto* Command = ol->FindByAttribute("command", 0)->ConvertToIdentifier();

    auto* whereTo = Command->GetParameterValue("whereTo");
    cout << whereTo << endl;

    ttt_move_t move;

    if(strcmp(whereTo, "A0") == 0){
      move.player = 'X';
      move.x = 0;
      move.y = 0;
    }else if(strcmp(whereTo, "A1") == 0){
      move.player = 'X';
      move.x = 1;
      move.y = 0;
    }else if(strcmp(whereTo, "A2") == 0){
      move.player = 'X';
      move.x = 2;
      move.y = 0;
    }else if(strcmp(whereTo, "B0") == 0){
      move.player = 'X';
      move.x = 0;
      move.y = 1;
    }else if(strcmp(whereTo, "B1") == 0){
      move.player = 'X';
      move.x = 1;
      move.y = 1;
    }else if(strcmp(whereTo, "B2") == 0){
      move.player = 'X';
      move.x = 2;
      move.y = 1;
    }else if(strcmp(whereTo, "C0") == 0){
      move.player = 'X';
      move.x = 0;
      move.y = 2;
    }else if(strcmp(whereTo, "C1") == 0){
      move.player = 'X';
      move.x = 1;
      move.y = 2;
    }else if(strcmp(whereTo, "C2") == 0){
      move.player = 'X';
      move.x = 2;
      move.y = 2;
    }

    ttt_make_move(player->connection, move);
    // cin.get();

    Command->AddStatusComplete();
    agent->Commit();
  }
  return;
}

void printEventHandler(
  smlPrintEventId id, void* user_data,
  Agent* agent, char const* message) {

    cout << message << endl;
}


Player::Player(ttt_t* apiHandle){

  connection = apiHandle;
  auto* initialState = ttt_get_state(connection);
  kernel = Kernel::CreateKernelInNewThread(20000);

  agent = kernel->CreateAgent("soarPlayer");

  agent->LoadProductions("/home/ztyree/Documents/soarTicTacToe/move.soar");
  auto* il = agent->GetInputLink();

  turn = agent->CreateStringWME(il, "turn", Converter(initialState->turn));

  winner = agent->CreateStringWME(il, "winner", Converter(initialState->winner));

  board = agent->CreateIdWME(il, "board");

  a0 = agent->CreateIdWME(board, "a0");
  a1 = agent->CreateIdWME(board, "a1");
  a2 = agent->CreateIdWME(board, "a2");
  b0 = agent->CreateIdWME(board, "b0");
  b1 = agent->CreateIdWME(board, "b1");
  b2 = agent->CreateIdWME(board, "b2");
  c0 = agent->CreateIdWME(board, "c0");
  c1 = agent->CreateIdWME(board, "c1");
  c2 = agent->CreateIdWME(board, "c2");

  A0 = agent->CreateStringWME(a0, "val", "_");
  A1 = agent->CreateStringWME(a1, "val", "_");
  A2 = agent->CreateStringWME(a2, "val", "_");
  B0 = agent->CreateStringWME(b0, "val", "_");
  B1 = agent->CreateStringWME(b1, "val", "_");
  B2 = agent->CreateStringWME(b2, "val", "_");
  C0 = agent->CreateStringWME(c0, "val", "_");
  C1 = agent->CreateStringWME(c1, "val", "_");
  C2 = agent->CreateStringWME(c2, "val", "_");

  auto tokenInput = agent->RegisterForRunEvent(smlEVENT_BEFORE_INPUT_PHASE,
                                               inputEventHandler, this);
  auto tokenOutput = agent->RegisterForRunEvent(smlEVENT_AFTER_OUTPUT_PHASE,
                                                outputEventHandler, this);
  auto tokenPrint = agent->RegisterForPrintEvent(smlEVENT_PRINT,
                                                 printEventHandler, this);
  // auto tokenEcho = agent->RegisterForPrintEvent(smlEVENT_ECHO,
  //                                               printEventHandler, this);

  agent->Commit();

}
