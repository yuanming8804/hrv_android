#include <wchar.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// 修正BCB6的tchar.h只有_TCHAR却没有TCHAR的问题.
//#if defined(__BORLANDC__) && !defined(_TCHAR_DEFINED)
//	typedef _TCHAR    TCHAR, *PTCHAR;
//	typedef _TCHAR    TBYTE, *PTBYTE;
//	#define _TCHAR_DEFINED
//#endif    // #if defined(__BORLANDC__) && !defined(_TCHAR_DEFINED)

#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef unsigned long ULONG;


typedef CHAR *PCHAR, *LPCH, *PCH;
typedef const CHAR *LPCCH, *PCCH;
typedef const CHAR *LPCSTR, *PCSTR;

typedef char TCHAR, *PTCHAR;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

typedef void *HANDLE;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

inline int mul_div(int number, int numerator, int denominator)
{
    long long ret = number;
    ret *= numerator;
    ret /= denominator;
    return (int) ret;
}

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif

//#ifndef NULL
//#ifdef __cplusplus
//#define NULL    0
//#else
//#define NULL    ((void *)0)
//#endif
//#endif
