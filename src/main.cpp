#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

#include "common/minesweeper.h"
#include "common/json.hpp"
#include "server/server.h"

using namespace std;
using json = nlohmann::json;

// Print Header of the level of the Game
void PrintHeader(int level) {
  switch (level){
    case 1:
      cout << endl << "Beginner Mode" << endl << endl;
      break;
    case 2:
      cout << endl << endl << "Intermediate Mode" << endl << endl;
      break;
    case 3:
      cout << endl << endl << "Expert Mode" << endl << endl;
      break;
    case 4:
      cout << endl << endl << "Custom Mode" << endl << endl;
  }
}

void GameLoop(MineSweeper* m) {
  while (!m->isGameEnd()) {
    m->PrintMineField();

    m->CheckWin();

    if (!m->isGameEnd()) {
      InGameTransfer(m);
    }
    else { 
      break;
    }
  }
}

int main() {
  string scripts_client;
  cout << "Level 1: Beginner (9x9, 10)" << endl;
  cout << "Level 2: Intermediate (16x16, 40)" << endl;
  cout << "Level 3: Expert (30x16, 10)" << endl;
  cout << "Level 4: Custom" << endl;

  MineSweeper minesweeper(StartGameTransfer());

  // loop until the game ends
  GameLoop(&minesweeper);

  minesweeper.EndGame(minesweeper.isWin());  
  
  int x;
  cin >> x;

  return 0;  
}


