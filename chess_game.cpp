#include "chess_piece.h"
#include "chess_board.h"
#include "chess_game.h"
#include "chess_network.h"
#include <iostream>

ChessGame::ChessGame()
{
}

ChessGame::~ChessGame()
{
}

void ChessGame::setupWhitePieces()
{
  whitePawn.load("res\\hueteotl\\tris.md2", "res\\hueteotl\\hueteotl_white.tga",
    "res\\hueteotl\\weapon.md2", "res\\hueteotl\\weapon.tga", 0.02f);
  whiteRook.load("res\\rhino\\tris.md2", "res\\rhino\\rhino_white.tga", NULL,
    NULL, 0.015f);
  whiteKnight.load("res\\pknight\\tris.md2", "res\\pknight\\knight_white.tga",
    "res\\pknight\\weapon.md2", "res\\pknight\\weapon.tga", 0.025f);
  whiteBishop.load("res\\phantom\\tris.md2", "res\\phantom\\phantom_white.tga",
    "res\\phantom\\weapon.md2", "res\\phantom\\weapon.tga", 0.03f);
  whiteQueen.load("res\\goblin\\tris.md2", "res\\goblin\\goblin_white.tga",
    "res\\goblin\\weapon.md2", "res\\goblin\\weapon.tga", 0.03f);
  whiteKing.load("res\\orc\\tris.md2", "res\\orc\\burningblade.tga",
    "res\\orc\\weapon.md2", "res\\orc\\weapon.tga", 0.03f);

  // setup white pawns
  for (int idx = 0; idx < 8; idx++) {
    pieces[idx].pieceType = PIECE_PAWN;
    pieces[idx].pieceCol = idx;
    pieces[idx].pieceRow = 1;
    pieces[idx].pieceColor = WHITE;
    pieces[idx].pieceInPlay = true;
    pieces[idx].pieceModel = whitePawn.getInstance();
  }

  // white rooks
  pieces[8].pieceType = pieces[9].pieceType = PIECE_ROOK;
  pieces[8].pieceColor = pieces[9].pieceColor = WHITE;
  pieces[8].pieceCol = 0; pieces[9].pieceCol = 7;
  pieces[8].pieceRow = 0;	pieces[9].pieceRow = 0;
  pieces[8].pieceInPlay = pieces[9].pieceInPlay = true;
  pieces[8].pieceModel = whiteRook.getInstance();
  pieces[9].pieceModel = whiteRook.getInstance();

  // white knights
  pieces[10].pieceType = pieces[11].pieceType = PIECE_KNIGHT;
  pieces[10].pieceColor = pieces[11].pieceColor = WHITE;
  pieces[10].pieceCol = 1; pieces[11].pieceCol = 6;
  pieces[10].pieceRow = 0; pieces[11].pieceRow = 0;
  pieces[10].pieceInPlay = pieces[11].pieceInPlay = true;
  pieces[10].pieceModel = whiteKnight.getInstance();
  pieces[11].pieceModel = whiteKnight.getInstance();

  // white bishops
  pieces[12].pieceType = pieces[13].pieceType = PIECE_BISHOP;
  pieces[12].pieceColor = pieces[13].pieceColor = WHITE;
  pieces[12].pieceCol = 2; pieces[13].pieceCol = 5;
  pieces[12].pieceRow = 0; pieces[13].pieceRow = 0;
  pieces[12].pieceInPlay = pieces[13].pieceInPlay = true;
  pieces[12].pieceModel = whiteBishop.getInstance();
  pieces[13].pieceModel = whiteBishop.getInstance();

  // white queen
  pieces[14].pieceType = PIECE_QUEEN;
  pieces[14].pieceColor = WHITE;
  pieces[14].pieceCol = 3;
  pieces[14].pieceRow = 0;
  pieces[14].pieceInPlay = true;
  pieces[14].pieceModel = whiteQueen.getInstance();

  // white king
  pieces[15].pieceType = PIECE_KING;
  pieces[15].pieceColor = WHITE;
  pieces[15].pieceCol = 4;
  pieces[15].pieceRow = 0;
  pieces[15].pieceInPlay = true;
  pieces[15].pieceModel = whiteKing.getInstance();
}

void ChessGame::setupBlackPieces()
{
  blackPawn.load("res\\hueteotl\\tris.md2", "res\\hueteotl\\hueteotl.tga",
    "res\\hueteotl\\weapon.md2", "res\\hueteotl\\weapon.tga", 0.02f);
  blackRook.load("res\\rhino\\tris.md2", "res\\rhino\\rhino.tga", NULL, NULL,
    0.015f);
  blackKnight.load("res\\pknight\\tris.md2", "res\\pknight\\evil.tga",
    "res\\pknight\\weapon.md2", "res\\pknight\\weapon.tga", 0.025f);
  blackBishop.load("res\\phantom\\tris.md2", "res\\phantom\\phantom.tga",
    "res\\phantom\\weapon.md2", "res\\phantom\\weapon.tga", 0.03f);
  blackQueen.load("res\\goblin\\tris.md2", "res\\goblin\\goblin.tga",
    "res\\goblin\\weapon.md2", "res\\goblin\\weapon.tga", 0.03f);
  blackKing.load("res\\orc\\tris.md2", "res\\orc\\blackrock.tga",
    "res\\orc\\weapon.md2", "res\\orc\\weapon.tga", 0.03f);

  // setup black pawns
  for (int idx = 16; idx < 24; idx++)
  {
    pieces[idx].pieceType = PIECE_PAWN;
    pieces[idx].pieceCol = idx - 16;
    pieces[idx].pieceRow = 6;
    pieces[idx].pieceColor = BLACK;
    pieces[idx].pieceInPlay = true;
    pieces[idx].pieceModel = blackPawn.getInstance();
  }

  // black rooks
  pieces[24].pieceType = pieces[25].pieceType = PIECE_ROOK;
  pieces[24].pieceColor = pieces[25].pieceColor = BLACK;
  pieces[24].pieceCol = 0; pieces[25].pieceCol = 7;
  pieces[24].pieceRow = 7; pieces[25].pieceRow = 7;
  pieces[24].pieceInPlay = pieces[25].pieceInPlay = true;
  pieces[24].pieceModel = blackRook.getInstance();
  pieces[25].pieceModel = blackRook.getInstance();

  // black knights
  pieces[26].pieceType = pieces[27].pieceType = PIECE_KNIGHT;
  pieces[26].pieceColor = pieces[27].pieceColor = BLACK;
  pieces[26].pieceCol = 1; pieces[27].pieceCol = 6;
  pieces[26].pieceRow = 7; pieces[27].pieceRow = 7;
  pieces[26].pieceInPlay = pieces[27].pieceInPlay = true;
  pieces[26].pieceModel = blackKnight.getInstance();
  pieces[27].pieceModel = blackKnight.getInstance();

  // black bishops
  pieces[28].pieceType = pieces[29].pieceType = PIECE_BISHOP;
  pieces[28].pieceColor = pieces[29].pieceColor = BLACK;
  pieces[28].pieceCol = 2; pieces[29].pieceCol = 5;
  pieces[28].pieceRow = 7; pieces[29].pieceRow = 7;
  pieces[28].pieceInPlay = pieces[29].pieceInPlay = true;
  pieces[28].pieceModel = blackBishop.getInstance();
  pieces[29].pieceModel = blackBishop.getInstance();

  // black queen
  pieces[30].pieceType = PIECE_QUEEN;
  pieces[30].pieceColor = BLACK;
  pieces[30].pieceCol = 3;
  pieces[30].pieceRow = 7;
  pieces[30].pieceInPlay = true;
  pieces[30].pieceModel = blackQueen.getInstance();

  // black king
  pieces[31].pieceType = PIECE_KING;
  pieces[31].pieceColor = BLACK;
  pieces[31].pieceCol = 4;
  pieces[31].pieceRow = 7;
  pieces[31].pieceInPlay = true;
  pieces[31].pieceModel = blackKing.getInstance();
}

void ChessGame::initialize()
{
  chessBoard = new ChessBoard;

  chessBoard->initialize();

  setupWhitePieces();
  setupBlackPieces();

  for (int idx = 0; idx < 32; idx++) {
    chessBoard->setBoardSpace(pieces[idx].pieceRow, pieces[idx].pieceCol,
      pieces[idx].pieceType, pieces[idx].pieceColor);
  }

  selectedPieceIdx = -1;
  currentCol = -1;
  currentRow = -1;

  currentMoveColor = WHITE;
  checkColor = NO_COLOR;
  gameState	= WAIT_STATE;
  oppsiteState = NT_MOVING;
}

void ChessGame::update(float dt)
{
  chessBoard->clearBoardPieces();

  for (int idx = 0; idx < 32; idx++) {
    if (pieces[idx].pieceInPlay) {
      if (pieces[idx].pieceModel)
        pieces[idx].pieceModel->animate(dt);

      chessBoard->setBoardSpace(pieces[idx].pieceRow, pieces[idx].pieceCol,
        pieces[idx].pieceType, pieces[idx].pieceColor);
    }
  }	

  updatePieceCapture(dt);
  updatePieceMovement(dt);
}

void ChessGame::render()
{
  for (int idx = 0; idx < 32; idx++) {
    if (pieces[idx].pieceInPlay) {
      if (pieces[idx].pieceModel) {
        // rotations are done elsewhere during moving and capture states
        if ((gameState != MOVING_STATE) &&
          (gameState != CAPTURE_STATE) &&
          (gameState != KILL_STATE)) {
            if (pieces[idx].pieceColor == WHITE)
              pieces[idx].pieceModel->rotate(0.0f);
            else if (pieces[idx].pieceColor == BLACK)
              pieces[idx].pieceModel->rotate(180.0f);
        }

        // movement is done elsewhere during the MOVING_STATE
        if ((gameState != MOVING_STATE) && (gameState != KILL_STATE))
          pieces[idx].pieceModel->move((float)pieces[idx].pieceRow+0.5f,
          0.0f, (float)pieces[idx].pieceCol+0.5f);

        pieces[idx].pieceModel->render();
      }
    }
  }
}

void ChessGame::release()
{
  chessBoard->release();

  for (int idx = 0; idx < 32; idx++) {
    if (pieces[idx].pieceModel) {
      pieces[idx].pieceModel->unload();
      delete pieces[idx].pieceModel;
    }
  }

  delete chessBoard;
}

bool ChessGame::isPieceInPlay(int r, int c)
{
  for (int idx = 0; idx < 32; idx++) {
    if (((pieces[idx].pieceRow == r) && (pieces[idx].pieceCol == c)) &&
      (pieces[idx].pieceInPlay))
      return true;
  }

  return false;
}

bool ChessGame::isPieceCaptured(char color, int r, int c)
{
  for (int idx = 0; idx < 32; idx++) {
    if (((pieces[idx].pieceRow == r) && (pieces[idx].pieceCol == c)) &&
      (pieces[idx].pieceInPlay) && (pieces[idx].pieceColor != color))
      return true;
  }

  return false;
}

void ChessGame::updatePieceMovement(float dt)
{
  if (gameState == MOVING_STATE) {
    if (pieces[pieceMovingIdx].pieceModel->getAnimation() == MD2Instance::_STATIC) {
      pieces[pieceMovingIdx].pieceModel->rotate(pieceRotateAngle);
      pieces[pieceMovingIdx].pieceModel->setAnimation(MD2Instance::RUN);
    } else if (pieceMoveDistance > 0.75) {
      // haven't reached the end yet
      piecePos += pieceVelocity * dt;
      pieceMoveDistance -= pieceVelocity.length() * dt;
      pieceMoveCurrentRow = piecePos.x;
      pieceMoveCurrentCol = piecePos.z;

      pieces[pieceMovingIdx].pieceModel->rotate(pieceRotateAngle);
      pieces[pieceMovingIdx].pieceModel->move(piecePos.z, 0.0, piecePos.x);
    } else if ((pieceMoveDistance <= 0.75) && (!pieceKilled) &&
      (pieceCaptureIdx != -1)) {
        gameState = KILL_STATE;
    } else if ((pieceMoveDistance <= 0.75) && (pieceMoveDistance > 0.0)) {
      // haven't reached the end yet
      piecePos += pieceVelocity * dt;
      pieceMoveDistance -= pieceVelocity.length() * dt;
      pieceMoveCurrentRow = piecePos.x;
      pieceMoveCurrentCol = piecePos.z;

      pieces[pieceMovingIdx].pieceModel->rotate(pieceRotateAngle);
      pieces[pieceMovingIdx].pieceModel->move(piecePos.z, 0.0, piecePos.x);
    } else {
      pieces[pieceMovingIdx].pieceCol = (int)piecePos.x;
      pieces[pieceMovingIdx].pieceRow = (int)piecePos.z;
      pieces[pieceMovingIdx].pieceModel->setAnimation(MD2Instance::IDLE);

      // done moving
      gameState = WAIT_STATE;
      pieceMovingIdx = -1;
    }
  } else if (gameState == CAPTURE_STATE) {
    pieces[pieceMovingIdx].pieceModel->rotate(pieceRotateAngle);
    if (!pieceKilled)
      gameState = MOVING_STATE;
  } else if ((gameState == KILL_STATE) && 
    (pieces[pieceMovingIdx].pieceModel->getAnimation() == MD2Instance::_STATIC)) {
      // done with attack animation
      // move forward
      if (pieceCaptureDeadTime > 1.5f) {
        gameState = MOVING_STATE;
        pieceKilled = true;
      } 
  } else if (gameState == KILL_STATE) {
    pieces[pieceMovingIdx].pieceModel->setAnimation(MD2Instance::ATTACK,
      MD2Instance::_STATIC);
  }
}

void ChessGame::updatePieceCapture(float dt)
{
  if (gameState == KILL_STATE) {
    if (pieces[pieceCaptureIdx].pieceModel->getAnimation() == MD2Instance::WAVE) {
      if (pieceMoveDistance <= 0.75f) {
        // flip off once more
        pieces[pieceCaptureIdx].pieceModel->setAnimation(MD2Instance::WAVE,
          MD2Instance::_STATIC);
        pieceDying = false;
      }
    }

    if (pieces[pieceCaptureIdx].pieceModel->getAnimation() == MD2Instance::_STATIC) {
      // stopped last flip off, time to die
      if ((pieces[pieceMovingIdx].pieceModel->getAnimation() == MD2Instance::_STATIC)
        && (!pieceDying)) {
          // set animation to death and have it stop on the last frame
          pieces[pieceCaptureIdx].pieceModel->setAnimation(MD2Instance::DEATH1,
            MD2Instance::_STATIC);
          pieceDying = true;
          pieceCaptureDeadTime = 0.0f;
      }

      pieceCaptureDeadTime += dt;

      if (pieceCaptureDeadTime >= 1.5f) {
        pieceCaptureDeadTime = 0.0f;
        pieces[pieceCaptureIdx].pieceInPlay = false;
        gameState = MOVING_STATE;
        pieceCaptureIdx = -1;
      }
    }
  } else if (gameState == KILL_STATE) {

  }
}

void ChessGame::movePiece(int pieceIdx, int destRow, int destCol, bool captureMove)
{
  pieceMoveStartRow = pieces[pieceIdx].pieceRow + 0.5f;
  pieceMoveStartCol = pieces[pieceIdx].pieceCol + 0.5f;
  pieceMoveEndRow   = destRow + 0.5f;
  pieceMoveEndCol   = destCol + 0.5f;

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
    pieces[pieceIdx].pieceModel->setAnimation(MD2Instance::RUN);
} 

void ChessGame::movePieceImmediate(int pieceIdx, int destRow, int destCol)
{
  pieces[pieceIdx].pieceRow = destRow;
  pieces[pieceIdx].pieceCol = destCol;
}

void ChessGame::capturePiece(int pieceIdx, int destRow, int destCol)
{
  gameState = CAPTURE_STATE;
  pieceCaptureIdx = pieceIdx;

  pieces[pieceIdx].pieceModel->setAnimation(MD2Instance::WAVE);
}

bool ChessGame::isValidPawnMove(int pieceIdx, int newRow, int newCol)
{
  // same position
  if ((newRow == pieces[pieceIdx].pieceRow) && (newCol == pieces[pieceIdx].pieceCol))
    return false;

  if (pieces[pieceIdx].pieceColor == WHITE) {
    // can only increase row, or rank, of pawns by one
    // unless it's current row/rank is 1 (starts at 0)
    if ((newRow == (pieces[pieceIdx].pieceRow + 1)) &&
      (newCol == pieces[pieceIdx].pieceCol)) {
        // make sure no pieces are already in this location
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return (!isPieceInPlay(newRow, newCol));
    } else if ((newRow == (pieces[pieceIdx].pieceRow + 2)) &&
      (pieces[pieceIdx].pieceRow == 1) && (newCol == pieces[pieceIdx].pieceCol)) {
        // make sure no pieces are already in this location
        // make sure no pieces are already in this location
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return (!isPieceInPlay(newRow, newCol));
    } else if ( ((newRow == (pieces[pieceIdx].pieceRow + 1)) &&
      (newCol == pieces[pieceIdx].pieceCol + 1)) || 
      ((newRow == (pieces[pieceIdx].pieceRow + 1)) && 
      (newCol == pieces[pieceIdx].pieceCol - 1)) ) {
        // capture move
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return false;
        else	// a piece is there
          return isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol);
    } else
      return false;
  } else if (pieces[pieceIdx].pieceColor == BLACK) {
    // can only increase row, or rank, of pawns by one
    // unless it's current row/rank is 1 (starts at 0)
    if ((newRow == (pieces[pieceIdx].pieceRow - 1)) &&
      (newCol == pieces[pieceIdx].pieceCol)) {
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return (!isPieceInPlay(newRow, newCol));
    } else if ((newRow == (pieces[pieceIdx].pieceRow - 2)) &&
      (pieces[pieceIdx].pieceRow == 6) && (newCol == pieces[pieceIdx].pieceCol)) {
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE) 
          return (!isPieceInPlay(newRow, newCol));
    } else if ( ((newRow == (pieces[pieceIdx].pieceRow - 1)) &&
      (newCol == pieces[pieceIdx].pieceCol + 1)) ||
      ((newRow == (pieces[pieceIdx].pieceRow - 1)) &&
      (newCol == pieces[pieceIdx].pieceCol - 1)) ) {
        // capture move
        if (chessBoard->getBoardSpace(newRow, newCol) == PIECE_NONE)
          return false;
        else	// a piece is there
          return isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol);
    } else
      return false;
  }

  return false;
}

bool ChessGame::isValidRookMove(int pieceIdx, int newRow, int newCol)
{
  // same position
  if ((newRow == pieces[pieceIdx].pieceRow) && (newCol == pieces[pieceIdx].pieceCol))
    return false;

  // moving along same row
  if ((newRow == (pieces[pieceIdx].pieceRow)) &&
    (newCol != pieces[pieceIdx].pieceCol)) {
      // iterate along spaces between current position and new position, and see if there are any pieces
      if ((newCol - pieces[pieceIdx].pieceCol) > 0) {	 // move in positive direction
        for (int c = pieces[pieceIdx].pieceCol+1; c <= newCol; c++) {
          if (chessBoard->getBoardSpace(pieces[pieceIdx].pieceRow, c) != PIECE_NONE) {
            if (c == newCol)	// reached end of iteration, see if piece capture
              if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
                return true;

            return (!isPieceInPlay(pieces[pieceIdx].pieceRow, c));
          }
        }

        // never found any pieces in the way
        return true;
      } else if ((newCol - pieces[pieceIdx].pieceCol) < 0) {  // move in negative direction
        for (int c = pieces[pieceIdx].pieceCol-1; c >= newCol; c--) {
          if (chessBoard->getBoardSpace(pieces[pieceIdx].pieceRow, c) != PIECE_NONE) {
            if (c == newCol) { // reached end of iteration, see if piece capture
              if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
                return true;
            }

            return (!isPieceInPlay(pieces[pieceIdx].pieceRow, c));
          }
        }

        // never found any pieces obstructing the move
        return true;
      }

      // selected the same square, not a move
      return false;
  } else if ((newRow != (pieces[pieceIdx].pieceRow)) &&
    (newCol == pieces[pieceIdx].pieceCol)) {  // moving along same column
      // iterate along spaces between current position and new position,
      // and see if there are any pieces
      if ((newRow - pieces[pieceIdx].pieceRow) > 0) {		// move in positive direction
        for (int r = pieces[pieceIdx].pieceRow+1; r <= newRow; r++) {
          if ((chessBoard->getBoardSpace(r, pieces[pieceIdx].pieceCol) != PIECE_NONE)) {
            if (r == newRow) {  // reached end of iteration, see if piece capture
              if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
                return true;
            }

            return (!isPieceInPlay(r, pieces[pieceIdx].pieceCol));
          }
        }

        // never found any pieces in the way
        return true;
      } else if ((newRow - pieces[pieceIdx].pieceRow) < 0) {  // move in negative direction
        for (int r = pieces[pieceIdx].pieceRow-1; r >= newRow; r--) {
          if ((chessBoard->getBoardSpace(r, pieces[pieceIdx].pieceCol) != PIECE_NONE)) {
            if (r == newRow) {  // reached end of iteration, see if piece capture
              if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
                return true;
            }

            return (!isPieceInPlay(r, pieces[pieceIdx].pieceCol));
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
  if ((newRow == pieces[pieceIdx].pieceRow) && (newCol == pieces[pieceIdx].pieceCol))
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
  if ((pieces[pieceIdx].pieceRow == newRow - 2) && (pieces[pieceIdx].pieceCol == newCol + 1)) {
    return (!isPieceInPlay(newRow, newCol) ||
      isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol));
  } else if ((pieces[pieceIdx].pieceRow == newRow - 2) &&    // move 2
    (pieces[pieceIdx].pieceCol == newCol - 1)) {
      return (!isPieceInPlay(newRow, newCol) ||
        isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol));
  } else if ((pieces[pieceIdx].pieceRow == newRow - 1) &&    // move 3
    (pieces[pieceIdx].pieceCol == newCol - 2)) {
      return (!isPieceInPlay(newRow, newCol) ||
        isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol));
  } else if ((pieces[pieceIdx].pieceRow == newRow + 1) &&    // move 4
    (pieces[pieceIdx].pieceCol == newCol - 2)) {
      return (!isPieceInPlay(newRow, newCol) ||
        isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol));
  } else if ((pieces[pieceIdx].pieceRow == newRow + 2) &&    // move 5
    (pieces[pieceIdx].pieceCol == newCol - 1)) {
      return (!isPieceInPlay(newRow, newCol) ||
        isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol));
  } else if ((pieces[pieceIdx].pieceRow == newRow + 2) &&    // move 6
    (pieces[pieceIdx].pieceCol == newCol + 1)) {
      return (!isPieceInPlay(newRow, newCol) ||
        isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol));
  } else if ((pieces[pieceIdx].pieceRow == newRow + 1) &&    // move 7
    (pieces[pieceIdx].pieceCol == newCol + 2)) {
      return (!isPieceInPlay(newRow, newCol) ||
        isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol));
  } else if ((pieces[pieceIdx].pieceRow == newRow - 1) &&    // move 8
    (pieces[pieceIdx].pieceCol == newCol + 2)) {
      return (!isPieceInPlay(newRow, newCol) ||
        isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol));
  }

  return false;
}

bool ChessGame::isValidBishopMove(int pieceIdx, int newRow, int newCol)
{
  int r;
  int c;

  // same position
  if ((newRow == pieces[pieceIdx].pieceRow) && (newCol == pieces[pieceIdx].pieceCol))
    return false;

  // first verify that the new position is on a diagonal
  // for \ diagonal
  // if the sum of the row and col in the new position is the same
  // as the row and col of the current position then the move is diagonal

  if ((pieces[pieceIdx].pieceRow <= newRow) && (pieces[pieceIdx].pieceCol <= newCol)) {
    // move up,left  \ diagonal
    if (!((pieces[pieceIdx].pieceRow + newCol) == (newRow + pieces[pieceIdx].pieceCol)))
      return false;

    r = pieces[pieceIdx].pieceRow + 1;
    c = pieces[pieceIdx].pieceCol + 1;

    // check for obstructing pieces
    while ((r <= newRow) && (c <= newCol)) {
      if ((r == newRow) && (c == newCol))
        if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
          return true;

      if (isPieceInPlay(r, c))
        return false;

      r++;
      c++;
    }
  } else if ((pieces[pieceIdx].pieceRow <= newRow) &&
    (pieces[pieceIdx].pieceCol >= newCol)) {
      // move up, right \ diagonal
      if (!((pieces[pieceIdx].pieceRow + pieces[pieceIdx].pieceCol) == (newRow + newCol)))
        return false;

      r = pieces[pieceIdx].pieceRow + 1;
      c = pieces[pieceIdx].pieceCol - 1;

      // check for obstructing pieces
      while ((r <= newRow) && (c >= newCol)) {
        if ((r == newRow) && (c == newCol))
          if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
            return true;

        if (isPieceInPlay(r, c))
          return false;

        r++;
        c--;
      }
  } else if ((pieces[pieceIdx].pieceRow >= newRow) &&
    (pieces[pieceIdx].pieceCol >= newCol)) {
      // swap columns for / diagonal
      // move down, right / diagonal
      if (!((pieces[pieceIdx].pieceRow + newCol) == 
        (newRow + pieces[pieceIdx].pieceCol)))
        return false;

      r = pieces[pieceIdx].pieceRow - 1;
      c = pieces[pieceIdx].pieceCol - 1;

      // check for obstructing pieces
      while ((r >= newRow) && (c >= newCol)) {
        if ((r == newRow) && (c == newCol))
          if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
            return true;

        if (isPieceInPlay(r, c))
          return false;

        r--;
        c--;
      }
  } else if ((pieces[pieceIdx].pieceRow >= newRow) &&
    (pieces[pieceIdx].pieceCol <= newCol)) {
      // swap columns for / diagonal
      // move down, left / diagonal
      if (!((pieces[pieceIdx].pieceRow + pieces[pieceIdx].pieceCol) ==
        (newRow + newCol)))
        return false;

      r = pieces[pieceIdx].pieceRow - 1;
      c = pieces[pieceIdx].pieceCol + 1;

      // check for obstructing pieces
      while ((r >= newRow) && (c <= newCol)) {
        if ((r == newRow) && (c == newCol))
          if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
            return true;

        if (isPieceInPlay(r, c))
          return false;

        r--;
        c++;
      }
  } else {
    return false;
  }

  return true;
}

bool ChessGame::isValidQueenMove(int pieceIdx, int newRow, int newCol)
{
  // same position
  if ((newRow == pieces[pieceIdx].pieceRow) && (newCol == pieces[pieceIdx].pieceCol))
    return false;

  if (isValidRookMove(pieceIdx, newRow, newCol) ||
    isValidBishopMove(pieceIdx, newRow, newCol))
    return true;

  return false;
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
  if ((pieces[pieceIdx].pieceColor == WHITE) && (pieces[pieceIdx].pieceRow == 0) &&
    (pieces[pieceIdx].pieceCol == 4))
  {
    // check for condition #3 and #5
    // row 0
    if (newCol == 2) { // queen side
      if (isPieceInPlay(0, 3) || isPieceInPlay(0, 2) || isPieceInPlay(0, 1))
        return false;
    } else if (newCol == 6)	{ // king side
      if (isPieceInPlay(0, 5) || isPieceInPlay(0, 6))
        return false;
    }

    return true;
  } else if (pieces[pieceIdx].pieceColor == BLACK) {
    // check for condition #3 and #5
    // row 7
    if (newCol == 2) { // queen side
      if (isPieceInPlay(7, 3) || isPieceInPlay(7, 2) || isPieceInPlay(7, 1))
        return false;
    } else if (newCol == 6)	{ // king side
      if (isPieceInPlay(7, 5) || isPieceInPlay(7, 6))
        return false;
    }
  }

  return true;
}

bool ChessGame::isValidKingMove(int pieceIdx, int newRow, int newCol)
{
  // same position
  if ((newRow == pieces[pieceIdx].pieceRow) && (newCol == pieces[pieceIdx].pieceCol))
    return false;

  if (isCastlingMove(pieceIdx, newRow, newCol))
    return true;

  // kings may move only one space at a time, in any direction
  if ((abs(newRow - pieces[pieceIdx].pieceRow) > 1) || (abs(newCol - pieces[pieceIdx].pieceCol) > 1))
    return false;

  if (isPieceCaptured(pieces[pieceIdx].pieceColor, newRow, newCol))
    return true;

  if (isPieceInPlay(newRow, newCol))
    return false;

  return true;
}

bool ChessGame::isValidMove(int pieceIdx, int newRow, int newCol)
{
  if (pieces[pieceIdx].pieceColor == currentMoveColor) {
    switch (pieces[pieceIdx].pieceType) {
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

void ChessGame::onSelection(float col, float row)
{
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      chessBoard->disableHighlight(x, y);

  if (gameState == WAIT_STATE) {
    if (((col <= 8.0) && (col >= 0.0)) && ((row >= 0.0) && (row <= 8.0))) {
      if (selectedPieceIdx != -1) {
        bool pieceCapture = false;

        // see if a piece of the opposite color is at the new position. if so, then capture
        for (int idx = 0; idx < 32; idx++) {
          if ((pieces[idx].pieceRow == (int)row) &&
            (pieces[idx].pieceCol == (int)col) &&
            pieces[idx].pieceInPlay &&
            (pieces[selectedPieceIdx].pieceColor != pieces[idx].pieceColor)) {
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

          movePiece(selectedPieceIdx, (int)row, (int)col);

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
          if ((pieces[idx].pieceRow == (int)row) && (pieces[idx].pieceCol == (int)col) && (pieces[idx].pieceInPlay)) {
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