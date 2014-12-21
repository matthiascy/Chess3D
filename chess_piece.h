#ifndef __CHESS_PIECE_H__
#define __CHESS_PIECE_H__

#include "chess_md2.h"

#define PIECE_IDLE    0
#define PIECE_MOVING  1
#define PIECE_DYING   2

struct ChessPiece {
  char pieceColor;
  unsigned char	pieceType;
  bool pieceInPlay;
  int pieceRow;		// row
  int pieceCol;		// col
  char pieceState;	// IDLE, MOVING, DYING
  MD2Instance* pieceModel;
};

#endif // __CHESS_PIECE_H__