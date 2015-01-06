#ifndef __FILE_DEFINE_H__
#define __FILE_DEFINE_H__

#define SERVER_PORT 1994
#define MAX_LISTEN 20

#if defined (WIN32)

#include <windows.h>
#include <winsock.h>
#include <mmsystem.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"

#else

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define TRUE  1
#define FALSE 0
#define INVALID_SOCKET -1

typedef bool           BOOL;
typedef unsigned char  BYTE;
typedef unsigned long  DWORD;
typedef unsigned short WORD;

#endif
