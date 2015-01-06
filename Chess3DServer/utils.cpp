#include "file_define.h"
#include <stdarg.h>

void log(char* buf, ...)
{
  char tmp_buf[1024], tmp_buf2[1024];
  va_list args;
  FILE* fp;

  time_t tval;
  time(&tval);
  struct tm* currTm = localtime(&tval);

  fp = fopen("log.txt", "a");

  if (!fp) return;

  va_start(args, buf);
  vsprintf(tmp_buf, buf, args);
  va_end(args);

  sprintf(tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d]%s",
	  currTm->tm_year + 1900,
	  currTm->tm_mon + 1,
	  currTm->tm_mday,
	  currTm->tm_hour,
	  currTm->tm_min,
	  currTm->tm_sec,
	  tmp_buf);

  strcpy(tmp_buf, tmp_buf2);
  fprintf(fp, tmp_buf);

#ifndef WIN32
  printf(tmp_buf);
#endif
  fclose(fp);
}

#ifndef WIN32
unsigned long timeGetTime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  unsigned long value = (((tv.tv_sec - 1000000000) * 1000) + (tv.tv_usec / 1000));
  return value;
}
#endif
