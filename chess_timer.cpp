#include "chess_timer.h"

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

