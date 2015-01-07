#ifndef __CHESS_OPENGL_H__
#define __CHESS_OPENGL_H__

#include "chess_vector.h"

class MD2Model;
class ChessGame;
class TextureMgr;

class ChessOGL {
private:
  int windowWidth;
  int windowHeight;

  TextureMgr texMgr;

  ChessGame *chessGame;

  unsigned int chessBoardDL;
  unsigned int boardTexture;
  unsigned int tableTexture;

  Vector whiteViewPos;
  Vector blackViewPos;
  char   currentView;

  bool isMenu;

  void generateChessBoardDL();
  void renderChessBoard();
  void renderSelections();
  void renderPieces();
  void renderTable();
  void renderMenu();
  void drawSelectionBox(float x, float y, float z);

public:
  ChessOGL();
  virtual ~ChessOGL();

  bool initialize();
  bool shutdown();

  void attachToGame(ChessGame *game);

  void setupProjection(int width, int height);

  void prepare(float dt);
  void render();

  void get3DIntersection(int winx, int winy, double &x, double &y, double &z);
};

#endif // __CHESS_OPENGL_H__