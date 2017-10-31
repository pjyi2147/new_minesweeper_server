#pragma once 

class Tile {
 private:
  // universal 
  bool mine_ = false;
  bool revealed_ = false;
  bool flagged_ = false;

  int 
  int col_;
  int row_;
 
 public:
  // universal 
  Tile();
  Tile(int col, int row);

  void printTile();

  bool isMine();
  bool isRevealed();
  bool isFlagged();

  
}