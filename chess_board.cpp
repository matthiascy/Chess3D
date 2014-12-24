/*
 * @file:        chess_board.cpp
 * @author:      matthiascy(matthias_cy@outlook.com)
 * @date:        2014-12-20
 * @description: 
 */
#include "chess_board.h"

void ChessBoard::initialize()
{
  clearBoardPieces();

  int col, row;

  // clear the board
  for (col = 0; col < 8; col++) {
    for (row = 0; row < 8; row++) {
      highlight[row][col] = false;
    }
  }
}

unsigned char ChessBoard::getBoardSpace(int row, int col)
{
  if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
    return board[row][col];
  else
    return PIECE_NONE;
}

void ChessBoard::setBoardSpace(int row, int col, unsigned char piece, char color)
{
  if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
    board[row][col] = piece;

}

void ChessBoard::enableHighlight(int row, int col)
{
  if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
    highlight[row][col] = true;
}

void ChessBoard::disableHighlight(int row, int col)
{
  if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
    highlight[row][col] = false;
}

bool ChessBoard::getHighlight(int row, int col)
{
  if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
    return highlight[row][col];
  else
    return false;
}

void ChessBoard::clearBoardPieces()
{
  int col, row;

  // clear the board
  for (col = 0; col < 8; col++)
    for (row = 0; row < 8; row++)
      board[row][col] = PIECE_NONE;
}

void ChessBoard::release()
{

}