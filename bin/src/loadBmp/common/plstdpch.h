/*
/--------------------------------------------------------------------
|
|      $Id: plstdpch.h,v 1.8 2004/06/06 12:56:38 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

//! \file
//! Contains most system-specific includes and definitions. On windows
//! systems, it corresponds to stdafx.h. On other systems, the
//! appropriate data types and macros are declared here.

#ifndef INCL_PLSTDPCH
#define INCL_PLSTDPCH

#include "plobject.h"

#ifdef _WINDOWS
  #define WIN32_LEAN_AND_MEAN  /* Prevent including <winsock*.h> in <windows.h> */
  #define VC_EXTRALEAN  // Exclude rarely-used stuff from Windows headers
  #include <windows.h>
  #include <stdlib.h> 
  #include <malloc.h>  
#else
  #include <stdlib.h>
#endif

#include "pldebug.h"

#endif // #ifndef _STDPCH_H_

/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plstdpch.h $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.8  2004/06/06 12:56:38  uzadow
|      Doxygenified documentation.
|
|      Revision 1.7  2004/04/18 12:34:45  uzadow
|      Changes for new libcurl version.
|
|      Revision 1.6  2003/02/15 21:26:58  uzadow
|      Added win32 version of url data source.
|
|      Revision 1.5  2002/11/06 21:54:49  uzadow
|      cygwin build stuff, removed LSB_FIRST.
|
|      Revision 1.4  2002/02/24 13:00:40  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.3  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.2  2001/10/06 15:32:22  uzadow
|      Removed types LPBYTE, DWORD, UCHAR, VOID and INT from the code.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.14  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.13  2000/11/07 15:40:46  jmbuena
|      Changes related to paintlibdefs.h and pixeldefs.h
|
|      Revision 1.12  2000/10/31 11:27:04  jmbuena
|      Removed PAINTLIB_INTERNAL and moved win definitions to windefs.h
|
|      Revision 1.11  2000/10/30 13:10:57  jmbuena
|      Only include config.h when PAINTLIB_INTERNAL is defined
|
|      Revision 1.10  2000/10/24 16:46:34  uzadow
|      Fixed build problems
|
|      Revision 1.9  2000/10/24 12:02:24  jmbuena
|      Include recursion protection
|
|      Revision 1.8  2000/05/27 16:34:05  Ulrich von Zadow
|      Linux compatibility changes
|
|      Revision 1.7  2000/03/31 11:53:30  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.6  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  1999/12/08 15:43:58  Ulrich von Zadow
|      Changed ASSERT and PLASSERT_VALID so that they
|      disappear correctly in release mode.
|
|      Revision 1.4  1999/11/22 14:59:37  Ulrich von Zadow
|      no message
|
|      Revision 1.3  1999/10/03 18:50:52  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
