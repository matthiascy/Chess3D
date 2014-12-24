/*
 * @file:        chess_opengl.cpp
 * @author:      matthiascy(matthias_cy@outlook.com)
 * @date:        2014-12-20
 * @description: 
 */

#ifdef _WINDOWS
#include <Windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <cmath>
#include <cstdio>

#include "glext.h"
#include "chess_vector.h"
#include "chess_plane.h"

#include "chess_tga_image.h"
#include "chess_md2.h"
#include "chess_texture_manage.h"
#include "chess_opengl.h"
#include "chess_game.h"

#pragma warning(disable:4244)
#pragma warning(disable:4305)

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// public
ChessOGL::ChessOGL()
{

}

ChessOGL::~ChessOGL()
{

}
bool ChessOGL::initialize()
{	
  if (!texMgr.loadTextures("Textures.dat"))
    return false;

  glEnable(GL_TEXTURE_2D);

  tableTexture = texMgr.getTextureID("wood");
  boardTexture = texMgr.getTextureID("chessboard");
  texMgr.bindTex(boardTexture);

  generateChessBoardDL();

  whiteViewPos = Vector(-1.0, 8.0, 4.0);
  blackViewPos = Vector(9.0, 8.0, 4.0);
  currentView = WHITE;

  return true;
}

bool ChessOGL::shutdown()
{
  texMgr.releaseTextures();

  glDeleteLists(chessBoardDL, 1);

  return true;
}

void ChessOGL::attachToGame(ChessGame *game)
{
  chessGame = game;
}

void ChessOGL::setupProjection(int width, int height)
{
  if (height == 0)					// don't want a divide by zero
    height = 1;

  glViewport(0, 0, width, height);		// reset the viewport to new dimensions
  glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
  glLoadIdentity();						// reset projection matrix

  // calculate perspective
  gluPerspective(54.0f,(GLfloat)width/(GLfloat)height,1.0f,1000.0f);

  glMatrixMode(GL_MODELVIEW);				// set modelview matrix
  glLoadIdentity();						// reset modelview matrix

  windowWidth = width;
  windowHeight = height;
}

void ChessOGL::prepare(float dt)
{
  chessGame->update(dt);

  if (chessGame->getCurrentState() == WAIT_STATE)
    currentView = chessGame->getCurrentMoveColor();
}

void ChessOGL::render()
{
  glClearColor(0.0f, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glLoadIdentity();

  if (currentView == WHITE)
    gluLookAt(whiteViewPos.x, whiteViewPos.y, whiteViewPos.z, 4.0, 0.0, 4.0,
      0.0, 1.0, 0.0);
  else
    gluLookAt(blackViewPos.x, blackViewPos.y, blackViewPos.z, 4.0, 0.0, 4.0,
      0.0, 1.0, 0.0);

  // render the wood table
  glDisable(GL_DEPTH_TEST);
  renderTable();
  glEnable(GL_DEPTH_TEST);

  // prepare to write to the stencil buffer by turning off
  // writes to the color and depth buffer
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  // setup the stencil func and op to place a 1 in the stencil buffer
  // everywhere we're about to draw
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

  // render the chess board surface. Since the depth and color buffers are disabled,
  // only the stencil buffer will be modified
  renderChessBoard();

  // turn color and depth buffers back on
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);

  // from this point on, only draw where stencil buffer is set to 1
  glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);

  // don't modify the contents of the stencil buffer
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  // draw reflected chess pieces first
  glPushMatrix();
  glScalef(1.0, -1.0, 1.0);
  renderPieces();
  glPopMatrix();

  // draw chessboard and selection square with blending
  glEnable(GL_BLEND);
  renderSelections();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
  glColor4f(0.0, 0.0, 0.0, 0.1f);
  renderChessBoard();
  glDisable(GL_BLEND);

  // turn off stencil testing
  glDisable(GL_STENCIL_TEST);

  // draw pieces normally
  glPushMatrix();
  glColor4f(1.0, 1.0, 1.0, 1.0);
  renderPieces();
  glPopMatrix();
}

void ChessOGL::renderSelections()
{
  for (int x = 0; x < 8; x++) {
    for (int z = 0; z < 8; z++) {
      if (chessGame->getBoardHighlight(x, z)) {
        glColor4f(1.0, 1.0, 0.2, 1.0f);	// yellow color
        drawSelectionBox((float)x, 0.001f, (float)z);
      }
    }
  }
}

void ChessOGL::drawSelectionBox(float x, float y, float z)
{
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(x, y, z);
  glLineWidth(5.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_POLYGON);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.01, 0.0, 0.01);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.99, 0.0, 0.01);
  glTexCoord2f(0.0, 1.0); glVertex3f(0.99, 0.0, 0.99);
  glTexCoord2f(1.0, 1.0); glVertex3f(0.01, 0.0, 0.99);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glLineWidth(1.0f);
  glPopMatrix();
  glEnable(GL_TEXTURE_2D);
}

void ChessOGL::renderTable()
{
  texMgr.bindTex(tableTexture);
  glBegin(GL_TRIANGLE_STRIP);
  glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
  glTexCoord2f(10.0, 0.0);
  glVertex3f(20.0f, -0.1f, 20.0f);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-20.0f, -0.1f, 20.0f);
  glTexCoord2f(10.0, 10.0);
  glVertex3f(20.0f, -0.1f, -20.0f);
  glTexCoord2f(0.0, 10.0);
  glVertex3f(-20.0f, -0.1f, -20.0f);
  glEnd();
}

void ChessOGL::generateChessBoardDL()
{
  chessBoardDL = glGenLists(1);

  glNewList(chessBoardDL, GL_COMPILE);

  for (int x = 0; x < 4; x++) {
    for (int z = 0; z < 4; z++) {
      for (int row = 2*x; row < 2*x+2; row++) {

        glBegin(GL_TRIANGLE_STRIP);
        for (int col = 2*z; col < 2*z+2; col++) {
          glTexCoord2f(row/2.0, col/2.0);
          glVertex3f(row, 0.0, col);

          glTexCoord2f((row+1)/2.0, col/2.0);
          glVertex3f(row+1, 0.0, col);

          glTexCoord2f(row/2.0, (col+1)/2.0);
          glVertex3f(row, 0.0, col+1);

          glTexCoord2f((row+1)/2.0, (col+1)/2.0);
          glVertex3f(row+1, 0.0, col+1);
        }
        glEnd();
      }		
    }
  }

  glEndList();
}

void ChessOGL::renderChessBoard()
{
  texMgr.bindTex(boardTexture);
  glPushMatrix();
  glCallList(chessBoardDL);
  glPopMatrix();
}

void ChessOGL::renderPieces()
{
  chessGame->render();
}

void ChessOGL::get3DIntersection(int winx, int winy, double &x, double &y, double &z)
{
  int viewport[4];
  double modelMatrix[16];
  double projMatrix[16];
  double nx, ny, nz;
  double fx, fy, fz;

  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
  gluUnProject((double)winx, (double)(viewport[3] - winy), 0.0, modelMatrix,
    projMatrix, viewport, &nx, &ny, &nz);
  gluUnProject((double)winx, (double)(viewport[3] - winy), 1.0, modelMatrix,
    projMatrix, viewport, &fx, &fy, &fz);

  Vector dir = Vector(fx, fy, fz) - Vector(nx, ny, nz);
  Vector yAxisNormal(0.0, 1.0, 0.0);
  Plane p(yAxisNormal, 0.0);

  double pndotrd = p.N[0]*dir[0] + p.N[1]*dir[1] + p.N[2]*dir[2];
  float point = -(p.N[0] * nx + p.N[1] * ny + p.N[2] * nz + p.D) / pndotrd;

  // get (x, y, z) intersection on (x, z) plane
  x =  nx + (point * dir[0]);
  y =  ny + (point * dir[1]);
  z =  nz + (point * dir[2]);
}
