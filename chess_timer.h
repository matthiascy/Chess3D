/*
 * Wrapper for the high-resolution timer. Can't be used if the hi-res timer
 * doesn't exist
 */
#ifndef __CHESS_TIMER_H__
#define __CHESS_TIMER_H__

#include <Windows.h>

class ChessHiResTimer {
public:
  ChessHiResTimer() {};
  ~ChessHiResTimer() {};

/*
 *
 * If the hi-res timer is present, the tick rate is stored and the function
 * returns true. Otherwise, the function returns false, and the timer should
 * not be used.
 */
bool init();

float getElapsedSeconds(unsigned long elapsedFrames = 1);

/*
 * Returns the average frames per second over elapsedFrames, which defaults to
 * one. If this is not called every frame, the client should track the number
 * of frames itself, and reset the value after this is called.
 */
float getFPS(unsigned long elapsedFrames = 1);

/*
 * Used to lock the frame rate to a set amount. This will block until enough
 * time has passed to ensure that the fps won't go over the requested amount.
 * Note that this can only keep the fps from going above the specified level;
 * it can still drop below it. It is assumed that if used, this function will
 * be called every frame. The value returned is the instantaneous fps, which
 * will be <= targetFPS.
 */
float lockFPS(unsigned char targetFPS);

private:
  LARGE_INTEGER startTime;
  LARGE_INTEGER ticksPerSecond;

};

#endif // __CHESS_TIMER_H__