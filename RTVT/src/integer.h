/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef _FF_INTEGER
#define _FF_INTEGER

#ifdef _WIN32	/* FatFs development platform */

#include <windows.h>
#include <tchar.h>

#else			/* Embedded platform */

/* This type MUST be 8 bit */
typedef volatile unsigned char	BYTE;

/* These types MUST be 16 bit */
typedef volatile short			SHORT;
typedef volatile unsigned short	WORD;
typedef volatile unsigned short	WCHAR;

/* These types MUST be 16 bit or 32 bit */
typedef volatile int				INT;
typedef volatile unsigned int	UINT;

/* These types MUST be 32 bit */
typedef volatile long			LONG;
typedef volatile unsigned long	DWORD;

#endif

#endif
