#ifndef __incl_System_h
#define __incl_System_h

#ifdef WIN32

#include <windows.h>
#pragma warning(disable : 4996) // deprecated

#else

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

#endif

#endif
