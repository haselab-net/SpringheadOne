#ifndef WINCMP_CWIN_H
#define WINCMP_CWIN_H

//	include <afxwin.h> or <windows.h>
#ifndef _WIN32_WINNT
 #define _WIN32_WINNT 0x0400
#endif

#ifdef _AFXDLL
 #define USEMFC
#endif

#ifdef __BORLANDC__
 #pragma pack(push)
#endif


#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#ifdef USEMFC
 #include <afxwin.h>
#else
 #ifdef __BORLANDC__
  #undef STRICT
  #include <winsock2.h>
 #else
  #ifndef STRICT
   #define STRICT
  #endif
   #if _MSC_VER >= 1300
    #include <winsock2.h>
   #endif
 #endif
 #include <unknwn.h>  //これをincludeしないと非MFCアプリではIUnknownが宣言されない
 #include <windows.h>
#endif

#ifdef __BORLANDC__
 #pragma pack(pop)
#endif

#include "WBWinTypes.h"

#ifdef _MSC_VER
#undef min
#undef max
#endif

#endif