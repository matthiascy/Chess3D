#ifndef __CHESS_GAME_H__
#define __CHESS_GAME_H__

#include "chess_vector.h"
#include "chess_md2.h"
#include "chess_piece.h"
#include "chess_board.h"

// game states
const char WAIT_STATE	 = 0x01;	// waiting for input
const char MOVING_STATE  = 0x02;	// moving a piece
const char CAPTURE_STATE = 0x04;	// capturing a piece
const char KILL_STATE    = 0x08;	// killing a piece
const char SWITCH_STATE  = 0x10;	// switching player

class ChessGame {
private:
  ChessBoard* chessBoard;
  ChessPiece  pieces[32];

  MD2Data whitePawn;
  MD2Data blackPawn;
  MD2Data whiteRook;
  MD2Data blackRook;
  MD2Data whiteKnight;
  MD2Data blackKnight;
  MD2Data whiteBishop;
  MD2Data blackBishop;
  MD2Data whiteQueen;
  MD2Data blackQueen;
  MD2Data whiteKing;
  MD2Data blackKing;

  int selectedPieceIdx;

  int currentRow;
  int currentCol;

  int pieceMovingIdx;	    // current moving piece index
  int pieceCaptureIdx;	// current piece being captured index

  char	currentMoveColor;	// color with the current move
  char	checkColor;		    // color of king in check, if any (NO_COLOR is possible)

  char	gameState;

  float	pieceMoveStartRow;	// starting position of a piece move
  float	pieceMoveStartCol;

  float	pieceMoveEndRow;		// ending position of a piece move
  float	pieceMoveEndCol;

  float	pieceMoveCurrentRow;
  float	pieceMoveCurrentCol;

  float	pieceMoveDistance;	// distance the piece needs to move in world coordinates
  float	pieceRotateAngle;	

  Vector pieceVelocity;	// velocity vector for movement
  Vector piecePos;			// position vector for movement
  float	 pieceCaptureDeadTime;
  bool pieceKilled;
  bool pieceDying;

  void setupWhitePieces();
  void setupBlackPieces();

  void movePiece(int pieceIdx, int destRow, int destCol, bool captureMove = false);
  void movePieceImmediate(int pieceIdx, int destRow, int destCol);
  void capturePiece(int pieceIdx, int destRow, int destCol);

  bool isPieceInPlay(int r, int c);
  bool isPieceCaptured(char color, int r, int c);

  bool isValidPawnMove(int pieceIdx, int newRow, int newCol);
  bool isValidRookMove(int pieceIdx, int newRow, int newCol);

  bool isValidKnightMove(int pieceIdx, int newRow, int newCol);
  bool isValidBishopMove(int pieceIdx, int newRow, int newCol);

  bool isValidQueenMove(int pieceIdx, int newRow, int newCol);
  bool isValidKingMove(int pieceIdx, int newRow, int newCol);

  bool isCastlingMove(int pieceIdx, int newRow, int newCol);

  void updatePieceMovement(float dt);
  void updatePieceCapture(float dt);

public:
  ChessGame();
  virtual ~ChessGame();

  void initialize();
  void update(float dt);
  void render();
  void release();

  ChessBoard *getBoard() { return chessBoard; }
  void onSelection(float row, float col);

  bool getBoardHighlight(int row, int col) { 
    return chessBoard->getHighlight(row, col); 
  }

  bool isValidMove(int pieceIdx, int newRow, int newCol);

  void setCurrentState(char state) { gameState = state; }
  char getCurrentState() { return gameState; }
  char getCurrentMoveColor() { return currentMoveColor; }
};

#endif // __CHESS_GAME_H__