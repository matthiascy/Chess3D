#ifndef __CHESS_PIECE_H__
#define __CHESS_PIECE_H__

#include "chess_md2.h"

#define PIECE_IDLE    0
#define PIECE_MOVING  1
#define PIECE_DYING   2

struct ChessPiece {
  char color;
  unsigned char type;
  bool inPlay;
  int row;
  int col;
  char state;    // IDLE, MOVING, DYING
  MD2Instance *model;
};

#endif // __CHESS_PIECE_H__