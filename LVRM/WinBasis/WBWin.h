#ifndef WINCMP_CWIN_H
#define WINCMP_CWIN_H

//	include <afxwin.h> or <windows.h>
#ifndef _WIN32_WINNT
 #define _WIN32_WINNT 0x0400
#endif
#ifdef _AFXDLL
 #define USEMFC
#endif
#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B
#ifdef USEMFC
 #include <afxwin.h>
#else
 #ifndef STRICT
 #define STRICT
 #endif
 #include <windows.h>
#endif

#include "WBWinTypes.h"

#ifdef _MSC_VER
#undef min
#undef max
#endif

#endif