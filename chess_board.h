#ifndef __CHESS_BOARD_H__
#define __CHESS_BOARD_H__

const unsigned char PIECE_NONE   = 0x00;
const unsigned char PIECE_PAWN   = 0x01;
const unsigned char PIECE_ROOK	 = 0x02;
const unsigned char PIECE_KNIGHT = 0x04;
const unsigned char PIECE_BISHOP = 0x08;
const unsigned char PIECE_QUEEN	 = 0x10;
const unsigned char PIECE_KING	 = 0x20;

const char NO_COLOR	= 0x00;
const char WHITE	= 0x01;
const char BLACK	= 0x02;

class ChessBoard {
private:
  unsigned char board[8][8];
  bool highlight[8][8];

public:
  ChessBoard() {};
  virtual ~ChessBoard() {};

  void initialize();
  void setBoardSpace(int row, int col, unsigned char piece, char color);
  unsigned char getBoardSpace(int row, int col);

  void enableHighlight(int row, int col);
  void disableHighlight(int row, int col);
  bool getHighlight(int row, int col);

  void clearBoardPieces();

  void release();
};

#endif // __CHESS_BOARD_H__