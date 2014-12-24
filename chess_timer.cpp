/*
 * @file:        chess_timer.cpp
 * @author:      matthiascy(matthias_cy@outlook.com)
 * @date:        2014-12-20
 * @description: Wrapper for the high-resolution timer. Can't be used if the
 *               hi-res timer doesn't exist.
 */
#include "chess_timer.h"

/* 
 * if the hi-res timer is present, the tick rate is stored and the function
 * returns true. Otherwise, the function returns false, and the timer should
 * not be used.
 */
bool ChessHiResTimer::initialize()
{
  if (!QueryPerformanceFrequency(&ticksPerSecond)) {
    // system doesn't support hi-res timer
    return false;
  } else {
    QueryPerformanceCounter(&startTime);
    return true;
  }
}

float ChessHiResTimer::getElapsedSeconds(unsigned long elapsedFrames /*= 1*/)
{
  static LARGE_INTEGER s_lastTime = startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float seconds =  ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart) / (float)ticksPerSecond.QuadPart;

  // reset the timer
  s_lastTime = currentTime;

  return seconds;
}

/*
 * returns the average frames per seconds over elapsedFrames, which defaults to
 * one. If this is not called every frame, the client should track the number
 * of frames itself, and reset the value after this is called.
 */
float ChessHiResTimer::getFPS(unsigned long elapsedFrames /*= 1*/)
{
  static LARGE_INTEGER s_lastTime = startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float fps = (float)elapsedFrames * (float)ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);

  // reset the timer
  s_lastTime = currentTime;

  return fps;
}

/*
 * used to lock the frame rate to a set amount. This will block until enough
 * time has passed to ensure that the fps won't go over the requested amount.
 * Nore that this can only keep the fps from going aboue the specified level;
 * it can still drop below it. It is assumed that if used, this function will
 * be called every frame. The value returned is the instantaneous fps, which
 * will be <= targetFPS
 */
float ChessHiResTimer::lockFPS(unsigned char targetFPS)
{
  if (targetFPS == 0)
    targetFPS = 1;

  static LARGE_INTEGER s_lastTime = startTime;
  LARGE_INTEGER currentTime;
  float   fps;

  // delay to maintain a constant frame rate
  do {
    QueryPerformanceCounter(&currentTime);
    fps = (float)ticksPerSecond.QuadPart/((float)(currentTime.QuadPart - s_lastTime.QuadPart));
  } while (fps > (float)targetFPS);

  // reset the timer
  s_lastTime = startTime;

  return fps;
}

