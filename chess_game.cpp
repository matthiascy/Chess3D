#include "chess_piece.h"
#include "chess_board.h"
#include "chess_game.h"
#include <iostream>

ChessGame::ChessGame()
{

}

ChessGame::~ChessGame()
{

}

// private methods
void ChessGame::setupWhitePieces()
{
  whitePawn.load("hueteotl\\tris.md2", "hueteotl\\hueteotl_white.tga",
    "hueteotl\\weapon.md2", "hueteotl\\weapon.tga", 0.02f);

  whiteRook.load("rhino\\tris.md2", "rhino\\rhino_white.tga", NULL, NULL,
    0.015f);

  whiteKnight.load("pknight\\tris.md2", "pknight\\knight_white.tga",
    "pknight\\weapon.md2", "pknight\\weapon.tga", 0.025f);

  whiteBishop.load("phantom\\tris.md2", "phantom\\phantom_white.tga",
    "phantom\\weapon.md2", "phantom\\weapon.tga", 0.03f);

  whiteQueen.load("goblin\\tris.md2", "goblin\\goblin_white.tga",
    "goblin\\weapon.md2", "goblin\\weapon.tga", 0.03f);

  whiteKing.load("orc\\tris.md2", "orc\\burningblade.tga", "orc\\weapon.md2",
    "orc\\weapon.tga", 0.03f);

  // setup white pawns
  for (int idx = 0; idx < 8; idx++) {
    pieces[idx].type = PIECE_PAWN;
    pieces[idx].col = idx;
    pieces[idx].row = 1;
    pieces[idx].color = WHITE;
    pieces[idx].inPlay = true;
    pieces[idx].model = whitePawn.getInstance();
  }

  // white rooks
  pieces[8].type = pieces[9].type = PIECE_ROOK;
  pieces[8].color = pieces[9].color = WHITE;
  pieces[8].col = 0; pieces[9].col = 7;
  pieces[8].row = 0; pieces[9].row = 0;
  pieces[8].inPlay = pieces[9].inPlay = true;
  pieces[8].model = whiteRook.getInstance();
  pieces[9].model = whiteRook.getInstance();

  // white knights
  pieces[10].type = pieces[11].type = PIECE_KNIGHT;
  pieces[10].color = pieces[11].color = WHITE;
  pieces[10].col = 1; pieces[11].col = 6;
  pieces[10].row = 0; pieces[11].row = 0;
  pieces[10].inPlay = pieces[11].inPlay = true;
  pieces[10].model = whiteKnight.getInstance();
  pieces[11].model = whiteKnight.getInstance();

  // white bishops
  pieces[12].type = pieces[13].type = PIECE_BISHOP;
  pieces[12].color = pieces[13].color = WHITE;
  pieces[12].col = 2; pieces[13].col = 5;
  pieces[12].row = 0; pieces[13].row = 0;
  pieces[12].inPlay = pieces[13].inPlay = true;
  pieces[12].model = whiteBishop.getInstance();
  pieces[13].model = whiteBishop.getInstance();

  // white queen
  pieces[14].type = PIECE_QUEEN;
  pieces[14].color = WHITE;
  pieces[14].col = 3;
  pieces[14].row = 0;
  pieces[14].inPlay = true;
  pieces[14].model = whiteQueen.getInstance();

  // white king
  pieces[15].type = PIECE_KING;
  pieces[15].color = WHITE;
  pieces[15].col = 4;
  pieces[15].row = 0;
  pieces[15].inPlay = true;
  pieces[15].model = whiteKing.getInstance();
}

void ChessGame::setupBlackPieces()
{
  blackPawn.load("hueteotl\\tris.md2", "hueteotl\\hueteotl.tga", "hueteotl\\weapon.md2", "hueteotl\\weapon.tga", 0.02f);

  blackRook.load("rhino\\tris.md2", "rhino\\rhino.tga", NULL, NULL, 0.015f);

  blackKnight.load("pknight\\tris.md2", "pknight\\evil.tga", "pknight\\weapon.md2", "pknight\\weapon.tga", 0.025f);

  blackBishop.load("phantom\\tris.md2", "phantom\\phantom.tga", "phantom\\weapon.md2", "phantom\\weapon.tga", 0.03f);

  blackQueen.load("goblin\\tris.md2", "goblin\\goblin.tga", "goblin\\weapon.md2", "goblin\\weapon.tga", 0.03f);

  blackKing.load("orc\\tris.md2", "orc\\blackrock.tga", "orc\\weapon.md2", "orc\\weapon.tga", 0.03f);

  // setup black pawns
  for (int idx = 16; idx < 24; idx++)
  {
    pieces[idx].type = PIECE_PAWN;
    pieces[idx].col = idx - 16;
    pieces[idx].row = 6;
    pieces[idx].color = BLACK;
    pieces[idx].inPlay = true;
    pieces[idx].model = blackPawn.getInstance();
  }

  // black rooks
  pieces[24].type = pieces[25].type = PIECE_ROOK;
  pieces[24].color = pieces[25].color = BLACK;
  pieces[24].col = 0; pieces[25].col = 7;
  pieces[24].row = 7; pieces[25].row = 7;
  pieces[24].inPlay = pieces[25].inPlay = true;
  pieces[24].model = blackRook.getInstance();
  pieces[25].model = blackRook.getInstance();

  // black knights
  pieces[26].type = pieces[27].type = PIECE_KNIGHT;
  pieces[26].color = pieces[27].color = BLACK;
  pieces[26].col = 1; pieces[27].col = 6;
  pieces[26].row = 7; pieces[27].row = 7;
  pieces[26].inPlay = pieces[27].inPlay = true;
  pieces[26].model = blackKnight.getInstance();
  pieces[27].model = blackKnight.getInstance();

  // black bishops
  pieces[28].type = pieces[29].type = PIECE_BISHOP;
  pieces[28].color = pieces[29].color = BLACK;
  pieces[28].col = 2; pieces[29].col = 5;
  pieces[28].row = 7; pieces[29].row = 7;
  pieces[28].inPlay = pieces[29].inPlay = true;
  pieces[28].model = blackBishop.getInstance();
  pieces[29].model = blackBishop.getInstance();

  // black queen
  pieces[30].type = PIECE_QUEEN;
  pieces[30].color = BLACK;
  pieces[30].col = 3;
  pieces[30].row = 7;
  pieces[30].inPlay = true;
  pieces[30].model = blackQueen.getInstance();

  // black king
  pieces[31].type = PIECE_KING;
  pieces[31].color = BLACK;
  pieces[31].col = 4;
  pieces[31].row = 7;
  pieces[31].inPlay = true;
  pieces[31].model = blackKing.getInstance();
}

void ChessGame::movePiece(int pieceIdx, int destRow, int destCol,
                          bool captureMove /*= false*/)
{
  pieceMoveStartRow = pieces[pieceIdx].row + 0.5f;
  pieceMoveStartCol = pieces[pieceIdx].col + 0.5f;

  pieceMoveEndRow = destRow + 0.5f;
  pieceMoveEndCol = destCol + 0.5f;

  pieceMoveCurrentRow = pieceMoveStartRow;
  pieceMoveCurrentCol = pieceMoveStartCol;

  piecePos = Vector(pieceMoveStartCol, 0.0, pieceMoveStartRow);

  pieceVelocity = Vector(pieceMoveEndCol - pieceMoveStartCol, 0.0,
    pieceMoveEndRow - pieceMoveStartRow);

  pieceMoveDistance = pieceVelocity.length();
  pieceVelocity.normalize();

  if (pieceVelocity.z > 0)
    pieceRotateAngle = RAD2DEG(pieceVelocity.angle(Vector(1.0, 0.0, 0.0))) - 90.0f;
  else
    pieceRotateAngle = RAD2DEG(pieceVelocity.angle(Vector(-1.0, 0.0, 0.0))) + 90.0f;

  pieceMovingIdx = pieceIdx;
  pieceKilled = false;
  pieceCaptureIdx = -1;

  if ((gameState == CAPTURE_STATE) || (gameState == MOVING_STATE))
    pieces[pieceIdx].model->setAnimation(MD2Instance::RUN);
}

void ChessGame::movePieceImmediate(int pieceIdx, int destRow, int destCol)
{
  pieces[pieceIdx].row = destRow;
  pieces[pieceIdx].col = destCol;
}

void ChessGame::capturePiece(int pieceIdx, int destRow, int destCol)
{
  gameState = CAPTURE_STATE;
  pieceCaptureIdx = pieceIdx;

  pieces[pieceIdx].model->setAnimation(MD2Instance::WAVE);
}

bool ChessGame::isPieceInPlay(int r, int c)
{
  for (int idx = 0; idx < 32; idx++) {
    if (((pieces[idx].row == r) && (pieces[idx].col == c)) &&
      (pieces[idx].inPlay))
      return true;
  }

  return false;
}

bool ChessGame::isValidPawnMove(int pieceIdx, int newRow, int newCol)
{
  if ((newRow == pieces[pieceIdx].row) && (newCol == pieces[pieceIdx].col))
    return false;

  if (pieces[pieceIdx].color == WHITE) {

    // can only increase row, or rank, of pawns by one
    // unless it's current row/rank is 1 (starts at 0)
    if ((newRow == (pieces[pieceIdx].row + 1)) && 
      (newCol == pieces[pieceIdx].col)) {

        // make sure no pieces are already in this position
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return (!isPieceInPlay(newRow, newCol));

    } else if ((newRow == (pieces[pieceIdx].row + 2)) &&
      (pieces[pieceIdx].row == 1) && (newCol == pieces[pieceIdx].col)) {

        // make sure no pieces are already in this location
        // make sure no pieces are already in this location
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return (!isPieceInPlay(newRow, newCol));

    } else if (((newRow == (pieces[pieceIdx].row + 1)) && 
      (newCol == pieces[pieceIdx].col + 1)) ||
      ((newRow == (pieces[pieceIdx].row + 1)) && 
      (newCol == pieces[pieceIdx].col - 1))) {

        // capture move
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return false;
        else
          return isPieceCaptured(pieces[pieceIdx].color, newRow, newCol);

    } else
      return false;

  } else if (pieces[pieceIdx].color = BLACK) {

    // can only increase row, or rank, of pawns by one
    // unless it's current row/rank is 1 (starts at 0)
    if ((newRow == (pieces[pieceIdx].row - 1)) &&
      (newCol == pieces[pieceIdx].col)) {

        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return (!isPieceInPlay(newRow, newCol));

    } else if ((newRow == (pieces[pieceIdx].row - 2)) &&
      (pieces[pieceIdx].row = 6) &&
      (newCol == pieces[pieceIdx].col)) {

        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return (!isPieceInPlay(newRow, newCol));

    } else if (((newRow == (pieces[pieceIdx].row - 1)) &&
      (newCol == pieces[pieceIdx].col + 1)) ||
      ((newRow == (pieces[pieceIdx].row - 1)) &&
      (newCol == pieces[pieceIdx].col - 1))) {

        // capture move
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return false;
        else	// a piece is there
          return isPieceCaptured(pieces[pieceIdx].color, newRow, newCol);

    } else
      return false;
  }

  return false;
}

bool ChessGame::isValidRookMove(int pieceIdx, int newRow, int newCol)
{
  // same position
  if ((newRow == pieces[pieceIdx].row) && (newCol == pieces[pieceIdx].col))
    return false;

  // moving along same row
  if ((newRow == (pieces[pieceIdx].row)) && (newCol != pieces[pieceIdx].col)) {
    // iterate along spaces between current position and new position,
    // and see if there are any pieces
    if ((newCol - pieces[pieceIdx].col) > 0) {  // move in positive direction
      for (int c = pieces[pieceIdx].col+1; c <= newCol; c++) {
        if (chessBoard->getBoardSpace(pieces[pieceIdx].row, c) != PIECE_NONE) {
          if (c == newCol) {	
            // reached end of iteration, see if piece capture
            if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
              return true;
          }

          return (!isPieceInPlay(pieces[pieceIdx].row, c));
        }
      }

      // never found any pieces in the way
      return true;
    } else if ((newCol - pieces[pieceIdx].col) < 0) { // move in negative direction

      for (int c = pieces[pieceIdx].col-1; c >= newCol; c--) {
        if (chessBoard->getBoardSpace(pieces[pieceIdx].row, c) != PIECE_NONE) {
          // reached end of iteration, see if piece capture
          if (c == newCol) {
            if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
              return true;
          }

          return (!isPieceInPlay(pieces[pieceIdx].row, c));
        }
      }

      // never found any pieces obstructing the move
      return true;
    }

    // selected the same square, not a move
    return false;

  } else if ((newRow != (pieces[pieceIdx].row)) && // moving along same column
    (newCol == pieces[pieceIdx].col)) {
      // iterate along spaces between current position and new position,
      // and see if there are any pieces
      if ((newRow - pieces[pieceIdx].row) > 0) {  // move in positive direction

        for (int r = pieces[pieceIdx].row+1; r <= newRow; r++) {

          if ((chessBoard->getBoardSpace(r, pieces[pieceIdx].col) != PIECE_NONE)) {
            if (r == newRow) {// reached end of iteration, see if piece capture

              if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
                return true;
            }

            return (!isPieceInPlay(r, pieces[pieceIdx].col));
          }
        }

        // never found any pieces in the way
        return true;
      } else if ((newRow - pieces[pieceIdx].row) < 0) {// move in negative direction

        for (int r = pieces[pieceIdx].row-1; r >= newRow; r--) {
          if ((chessBoard->getBoardSpace(r, pieces[pieceIdx].col) != PIECE_NONE)) {
            if (r == newRow) { // reached end of iteration, see if piece capture
              if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
                return true;
            }

            return (!isPieceInPlay(r, pieces[pieceIdx].col));
          }
        }

        // never found any pieces obstructing the move
        return true;
      }

      // selected the same square, not a move
      return false;
  }

  // any other direction is invalid
  return false;
}

bool ChessGame::isValidKnightMove(int pieceIdx, int newRow, int newCol)
{
  // same position
  if ((newRow == pieces[pieceIdx].row) && (newCol == pieces[pieceIdx].col))
    return false;

  // knights have 8 possible moves, provided all these moves are within the bounds of the board
  // the checks for being out of bounds have already been executed prior to this function,
  // so we can just check for the normal 8 knight moves

  /*
  Layout of knight moves - X is the current position
  .2.1.
  3...8
  ..X..
  4...7
  .5.6.
  */

  // move 1
  if ((pieces[pieceIdx].row == newRow - 2) && (pieces[pieceIdx].col == newCol + 1)) {
    return (!isPieceInPlay(newRow, newCol) || isPieceCaptured(pieces[pieceIdx].color, newRow, newCol));
  } else if ((pieces[pieceIdx].row == newRow - 2) && (pieces[pieceIdx].col == newCol - 1)) {  // move 2
    return (!isPieceInPlay(newRow, newCol) || isPieceCaptured(pieces[pieceIdx].color, newRow, newCol));
  } else if ((pieces[pieceIdx].row == newRow - 1) && (pieces[pieceIdx].col == newCol - 2)) {  // move 3
    return (!isPieceInPlay(newRow, newCol) || isPieceCaptured(pieces[pieceIdx].color, newRow, newCol));
  } else if ((pieces[pieceIdx].row == newRow + 1) && (pieces[pieceIdx].col == newCol - 2)) {  // move 4
    return (!isPieceInPlay(newRow, newCol) || isPieceCaptured(pieces[pieceIdx].color, newRow, newCol));
  } else if ((pieces[pieceIdx].row == newRow + 2) && (pieces[pieceIdx].col == newCol - 1)) {  // move 5
    return (!isPieceInPlay(newRow, newCol) || isPieceCaptured(pieces[pieceIdx].color, newRow, newCol));
  } else if ((pieces[pieceIdx].row == newRow + 2) && (pieces[pieceIdx].col == newCol + 1)) {  // move 6
    return (!isPieceInPlay(newRow, newCol) || isPieceCaptured(pieces[pieceIdx].color, newRow, newCol));
  } else if ((pieces[pieceIdx].row == newRow + 1) && (pieces[pieceIdx].col == newCol + 2)) {  // move 7
    return (!isPieceInPlay(newRow, newCol) || isPieceCaptured(pieces[pieceIdx].color, newRow, newCol));
  } else if ((pieces[pieceIdx].row == newRow - 1) && (pieces[pieceIdx].col == newCol + 2)) {  // move 8
    return (!isPieceInPlay(newRow, newCol) || isPieceCaptured(pieces[pieceIdx].color, newRow, newCol));
  }
  return false;
}

bool ChessGame::isValidBishopMove(int pieceIdx, int newRow, int newCol)
{
  int r;
  int c;

  // same position
  if ((newRow == pieces[pieceIdx].row) && (newCol == pieces[pieceIdx].col))
    return false;

  // first verify that the new position is on a diagonal
  // for \ diagonal
  // if the sum of the row and col in the new position is the same
  // as the row and col of the current position then the move is diagonal

  if ((pieces[pieceIdx].row <= newRow) && (pieces[pieceIdx].col <= newCol)) {
    // move up,left  \ diagonal
    if (!((pieces[pieceIdx].row + newCol) == (newRow + pieces[pieceIdx].col)))
      return false;

    r = pieces[pieceIdx].row + 1;
    c = pieces[pieceIdx].col + 1;

    // check for obstructing pieces
    while ((r <= newRow) && (c <= newCol)) {
      if ((r == newRow) && (c == newCol))
        if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
          return true;

      if (isPieceInPlay(r, c))
        return false;

      r++;
      c++;
    }
  } else if ((pieces[pieceIdx].row <= newRow) && (pieces[pieceIdx].col >= newCol)) {
    // move up, right \ diagonal
    if (!((pieces[pieceIdx].row + pieces[pieceIdx].col) == (newRow + newCol)))
      return false;

    r = pieces[pieceIdx].row + 1;
    c = pieces[pieceIdx].col - 1;

    // check for obstructing pieces
    while ((r <= newRow) && (c >= newCol)) {
      if ((r == newRow) && (c == newCol))
        if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
          return true;

      if (isPieceInPlay(r, c))
        return false;

      r++;
      c--;
    }
  } else if ((pieces[pieceIdx].row >= newRow) && (pieces[pieceIdx].col >= newCol)) {
    // swap columns for / diagonal
    // move down, right / diagonal
    if (!((pieces[pieceIdx].row + newCol) == (newRow + pieces[pieceIdx].col)))
      return false;

    r = pieces[pieceIdx].row - 1;
    c = pieces[pieceIdx].col - 1;

    // check for obstructing pieces
    while ((r >= newRow) && (c >= newCol)) {
      if ((r == newRow) && (c == newCol))
        if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
          return true;

      if (isPieceInPlay(r, c))
        return false;

      r--;
      c--;
    }
  } else if ((pieces[pieceIdx].row >= newRow) && (pieces[pieceIdx].col <= newCol)) {
    // swap columns for / diagonal
    // move down, left / diagonal
    if (!((pieces[pieceIdx].row + pieces[pieceIdx].col) == (newRow + newCol)))
      return false;

    r = pieces[pieceIdx].row - 1;
    c = pieces[pieceIdx].col + 1;

    // check for obstructing pieces
    while ((r >= newRow) && (c <= newCol)) {
      if ((r == newRow) && (c == newCol))
        if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
          return true;

      if (isPieceInPlay(r, c))
        return false;

      r--;
      c++;
    }
  }
  else
    return false;

  //if (abs(pieces[pieceIdx].row - newRow) == abs(pieces[pieceIdx].col - newCol))
  //{
  //		return true;
  //	}
  //	else
  //		return false;

  return true;
}

bool ChessGame::isValidQueenMove(int pieceIdx, int newRow, int newCol)
{
  if ((newRow == pieces[pieceIdx].row) && (newCol == pieces[pieceIdx].col))
    return false;

  if (isValidRookMove(pieceIdx, newRow, newCol) || isValidBishopMove(pieceIdx, newRow, newCol))
    return true;

  return false;
}

bool ChessGame::isValidKingMove(int pieceIdx, int newRow, int newCol)
{
  // same position
  if ((newRow == pieces[pieceIdx].row) && (newCol == pieces[pieceIdx].col))
    return false;

  if (isCastlingMove(pieceIdx, newRow, newCol))
    return true;

  // kings may move only one space at a time, in any direction
  if ((abs(newRow - pieces[pieceIdx].row) > 1) || (abs(newCol - pieces[pieceIdx].col) > 1))
    return false;

  if (isPieceCaptured(pieces[pieceIdx].color, newRow, newCol))
    return true;

  if (isPieceInPlay(newRow, newCol))
    return false;

  return true;
}

bool ChessGame::isCastlingMove(int pieceIdx, int newRow, int newCol)
{
  // may not castle if:
  // 1. Your king has been moved earlier in the game. 
  // 2. The rook that castles has been moved earlier in the game. 
  // 3. There are pieces standing between your king and rook. 
  // 4. The king is in check. 
  // 5. The king moves through a square that is attacked by a piece of the opponent. 
  // 6. The king would be in check after castling. 

  // check for condition #1
  if ((pieces[pieceIdx].color == WHITE) &&
    (pieces[pieceIdx].row == 0) &&
    (pieces[pieceIdx].col == 4)) {
      // check for condition #3 and #5
      // row 0
      if (newCol == 2) { // queenside
        if (isPieceInPlay(0, 3) || isPieceInPlay(0, 2) || isPieceInPlay(0, 1))
          return false;
      } else if (newCol == 6)	{ // kingside
        if (isPieceInPlay(0, 5) || isPieceInPlay(0, 6))
          return false;
      }

      return true;

  } else if (pieces[pieceIdx].color == BLACK) {
    // check for condition #3 and #5
    // row 7
    if (newCol == 2) { // queenside
      if (isPieceInPlay(7, 3) || isPieceInPlay(7, 2) || isPieceInPlay(7, 1))
        return false;
    } else if (newCol == 6)	{ // kingside
      if (isPieceInPlay(7, 5) || isPieceInPlay(7, 6))
        return false;
    }
  }

  return true;
}

bool ChessGame::isPieceCaptured(char color, int r, int c)
{
  for (int idx = 0; idx < 32; idx++) {
    if (((pieces[idx].row == r) && (pieces[idx].col == c)) &&
      (pieces[idx].inPlay) && (pieces[idx].color != color))
      return true;
  }

  return false;
}

void ChessGame::updatePieceMovement(float dt)
{
  if (gameState == MOVING_STATE) {
    if (pieces[pieceMovingIdx].model->getAnimation() == MD2Instance::_STATIC) {
      pieces[pieceMovingIdx].model->rotation(pieceRotateAngle);
      pieces[pieceMovingIdx].model->setAnimation(MD2Instance::RUN);
    } else if (pieceMoveDistance > 0.75) {
      piecePos += pieceVelocity * dt;
      pieceMoveDistance -= pieceVelocity.length() * dt;
      pieceMoveCurrentRow = piecePos.x;
      pieceMoveCurrentCol = piecePos.z;

      pieces[pieceMovingIdx].model->rotation(pieceRotateAngle);
      pieces[pieceMovingIdx].model->move(piecePos.z, 0.0, piecePos.x);
    } else if ((pieceMoveDistance <= 0.75) && (!pieceKilled) &&
      (pieceCaptureIdx != -1)) {
        gameState = KILL_STATE;
    } else if ((pieceMoveDistance <= 0.75) && (pieceMoveDistance > 0.0)) {
      piecePos += pieceVelocity * dt;
      pieceMoveDistance -= pieceVelocity.length() * dt;
      pieceMoveCurrentRow = piecePos.x;
      pieceMoveCurrentCol = piecePos.z;

      pieces[pieceMovingIdx].model->rotation(pieceRotateAngle);
      pieces[pieceMovingIdx].model->move(piecePos.z, 0.0, piecePos.x);
    } else {
      pieces[pieceMovingIdx].col = (int)piecePos.x;
      pieces[pieceMovingIdx].row = (int)piecePos.z;
      pieces[pieceMovingIdx].model->setAnimation(MD2Instance::IDLE);

      // done moving
      gameState = WAIT_STATE;
      pieceMovingIdx = -1;
    }
  } else if (gameState == CAPTURE_STATE) {
    pieces[pieceMovingIdx].model->rotation(pieceRotateAngle);
    if (!pieceKilled)
      gameState = MOVING_STATE;
  } else if ((gameState == KILL_STATE) && 
    (pieces[pieceMovingIdx].model->getAnimation() == MD2Instance::_STATIC)) {
      // done with attack animation
      // move forward
      if (pieceCaptureDeadTime > 1.5f) {
        gameState = MOVING_STATE;
        pieceKilled = true;
      }
  } else if (gameState == KILL_STATE) {
    pieces[pieceMovingIdx].model->setAnimation(MD2Instance::ATTACK,
      MD2Instance::_STATIC);
  }
}

void ChessGame::updatePieceCapture(float dt)
{
  if (gameState == KILL_STATE) {
    if (pieces[pieceCaptureIdx].model->getAnimation() == MD2Instance::WAVE) {
      if (pieceMoveDistance <= 0.75f) {
        pieces[pieceCaptureIdx].model->setAnimation(MD2Instance::WAVE,
          MD2Instance::_STATIC);
        pieceDying = false;
      }
    }

    if (pieces[pieceCaptureIdx].model->getAnimation() == MD2Instance::_STATIC) {
      if ((pieces[pieceMovingIdx].model->getAnimation() ==
        MD2Instance::_STATIC) && (!pieceDying)) {
          pieces[pieceCaptureIdx].model->setAnimation(MD2Instance::DEATH1,
            MD2Instance::_STATIC);
          pieceDying = true;
          pieceCaptureDeadTime = 0.0f;
      }

      pieceCaptureDeadTime += dt;

      if (pieceCaptureDeadTime >= 1.5f) {
        pieceCaptureDeadTime = 0.0f;
        pieces[pieceCaptureIdx].inPlay = false;
        gameState = MOVING_STATE;
        pieceCaptureIdx = -1;
      }
    }
  } else if (gameState == KILL_STATE) {

  }
}


// public methods
void ChessGame::initialize()
{
  chessBoard = new ChessBoard;

  chessBoard->initialize();

  setupBlackPieces();
  setupWhitePieces();

  for (int idx = 0; idx < 32; idx++)
    chessBoard->setBoardSpace(pieces[idx].row, pieces[idx].col,
    pieces[idx].type, pieces[idx].color);

  selectedPieceIdx = -1;
  currentCol = -1;
  currentRow = -1;

  currentMoveColor = WHITE;
  checkColor = NO_COLOR;
  gameState = WAIT_STATE;
}

void ChessGame::update(float dt)
{
  chessBoard->clearBoardPieces();

  for (int idx = 0; idx < 32; idx++) {
    if (pieces[idx].inPlay) {
      if (pieces[idx].model)
        pieces[idx].model->animate(dt);

      chessBoard->setBoardSpace(pieces[idx].row, pieces[idx].col,
        pieces[idx].type, pieces[idx].color);
    }
  }

  updatePieceCapture(dt);
  updatePieceMovement(dt);
}

void ChessGame::render()
{
  for (int idx = 0; idx < 32; idx++) {
    if (pieces[idx].inPlay) {
      if (pieces[idx].model) {
        // rotation are done elsewhere during moving and capture states
        if ((gameState != MOVING_STATE) &&
          (gameState != CAPTURE_STATE) &&
          (gameState != KILL_STATE)) {
            if (pieces[idx].color == WHITE)
              pieces[idx].model->rotation(0.0f);
            else if (pieces[idx].color == BLACK)
              pieces[idx].model->rotation(180.0f);
        }

        if ((gameState != MOVING_STATE) && (gameState != KILL_STATE))
          pieces[idx].model->move((float)pieces[idx].row + 0.5f, 0.0f,
          (float)pieces[idx].col + 0.5f);

        pieces[idx].model->render();
      }
    }
  }
}

void ChessGame::release()
{
  chessBoard->release();

  for (int idx = 0; idx < 32; idx++) {
    if (pieces[idx].model) {
      pieces[idx].model->unload();
      delete pieces[idx].model;
    }
  }

  delete chessBoard;
}

void ChessGame::onSelection(float row, float col)
{
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      chessBoard->disableHightlight(x, y);

  if (gameState == WAIT_STATE) {
    if (((col <= 8.0) && (col >= 0.0)) && ((row >= 0.0) && (row <= 8.0))) {
      if (selectedPieceIdx != -1) {
        bool pieceCapture = false;

        // see if a piece of the opposite color is at the new position. if so,
        // then capture
        for (int idx = 0; idx < 32; idx++) {
          if ((pieces[idx].row == (int)row) &&
            (pieces[idx].col == (int)col) &&
            pieces[idx].inPlay &&
            (pieces[selectedPieceIdx].color != pieces[idx].color)) {
              // do we have a capture?
              if ((isPieceCaptured(currentMoveColor, (int)row, (int)col)) &&
                (isValidMove(selectedPieceIdx, (int)row, (int)col))) {

                  gameState = CAPTURE_STATE;

                  movePiece(selectedPieceIdx, (int)row, (int)col, true);
                  capturePiece(idx, (int)row, (int)col);
                  pieceCapture = true;

                  // we've moved, so now it's the other color's turn to move
                  if (currentMoveColor == WHITE)
                    currentMoveColor = BLACK;
                  else if (currentMoveColor == BLACK)
                    currentMoveColor = WHITE;
                  break;
              }
          }
        }

        // we already have a piece selected, so move it if possible
        if (!pieceCapture && isValidMove(selectedPieceIdx, (int)row, (int)col)) {
          gameState = MOVING_STATE;

          // check if castling
          if (isCastlingMove(selectedPieceIdx, (int)row, (int)col)) {
            // move rooks
            if (((int)row == 0) && ((int)col == 6))
              movePieceImmediate(9, 0, 5);
            else if (((int)row == 0) && ((int)col == 2))
              movePieceImmediate(8, 0, 3);
            else if (((int)row == 7) && ((int)col == 6))
              movePieceImmediate(25, 7, 5);
            else if (((int)row == 7) && ((int)col == 2))
              movePieceImmediate(24, 7, 3);
          }

          movePieceImmediate(selectedPieceIdx, (int)row, (int)col);

          // we've moved, so now it's the other color's turn to move
          if (currentMoveColor == WHITE)
            currentMoveColor = BLACK;
          else if (currentMoveColor == BLACK)
            currentMoveColor = WHITE;
        }

        selectedPieceIdx = -1;
        currentRow = -1;
        currentCol = -1;
      } else {
        // no pieces currently selected, so we select a square
        chessBoard->enableHighlight((int)row, (int)col);

        // check through pieces for this location, if a piece is there, select the piece
        for (int idx = 0; idx < 32; idx++) {
          if ((pieces[idx].row == (int)row) && (pieces[idx].col == (int)col) && (pieces[idx].inPlay)) {
            selectedPieceIdx = idx;
            break;
          }
        }

        currentRow = (int)row;
        currentCol = (int)col;
      }
    } else {
      selectedPieceIdx = -1;
      currentRow = -1;
      currentCol = -1;
    }
  }
}

bool ChessGame::isValidMove(int pieceIdx, int newRow, int newCol)
{
  if (pieces[pieceIdx].color == currentMoveColor) {
    switch (pieces[pieceIdx].type) {
      case PIECE_PAWN:
        return isValidPawnMove(pieceIdx, newRow, newCol);

      case PIECE_ROOK:
        return isValidRookMove(pieceIdx, newRow, newCol);

      case PIECE_KNIGHT:
        return isValidKnightMove(pieceIdx, newRow, newCol);

      case PIECE_BISHOP:
        return isValidBishopMove(pieceIdx, newRow, newCol);

      case PIECE_QUEEN:
        return isValidQueenMove(pieceIdx, newRow, newCol);

      case PIECE_KING:
        return isValidKingMove(pieceIdx, newRow, newCol);
    }
  }

  return false;
}
