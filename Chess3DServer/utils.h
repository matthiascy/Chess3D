#ifndef __UTILS_H__
#define __UTILS_H__

void log(char* buf, ...);

#ifndef WIN32
unsigned long timeGetTime();
#endif

#endif
