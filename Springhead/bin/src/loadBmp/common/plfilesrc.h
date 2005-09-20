/*
/--------------------------------------------------------------------
|
|      $Id: plfilesrc.h,v 1.8 2003/11/22 13:11:34 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLFILESRC
#define INCL_PLFILESRC

#ifndef INCL_PLDATASRC
#include "pldatasrc.h"
#endif

#include "config.h"

#include <stdio.h>

class PLIProgressNotification;

//! This is a class which takes a file as a source of picture data.
class PLFileSource : public PLDataSource
{

public:
  //!
  PLFileSource
    ( PLIProgressNotification * pNotification = NULL
    );

  //!
  virtual ~PLFileSource
    ();

  //!
  virtual int Open
    ( const char * pszFName
    );
#ifdef _WINDOWS
  virtual int OpenW
    ( const wchar_t * pszwFName
    );
#endif

  //!
  virtual void Close
    ();

  virtual PLBYTE * ReadNBytes
    ( int n
    );

  //! Read but don't advance file pointer.
  virtual PLBYTE * GetBufferPtr
    ( int MinBytesInBuffer
    );

  virtual PLBYTE * ReadEverything
    ();

private:
  void isBufferOk (int NumBytes);
#ifdef _WINDOWS
  int mapFileHandle();
  int getLastPLError();
  
  HANDLE m_hf;    // File handle.
  HANDLE m_hm;    // Handle to file-mapping object.
#else
  #ifdef HAVE_MMAP
  
  #else  
  bool bytesAvailable
    ( int n
    );

  void fillBuffer
    ( int n = 4096
    );

  FILE * m_pFile;
  PLBYTE * m_pBuffer;

  PLBYTE * m_pReadPos;
  int m_BytesReadFromFile;
  #endif
#endif
  PLBYTE * m_pStartData;
  PLBYTE * m_pCurPos;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plfilesrc.h $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.8  2003/11/22 13:11:34  uzadow
|      Added memory-mapped file support for linux
|
|      Revision 1.7  2003/08/03 12:36:56  uzadow
|      Added unicode support; fixed some header includes.
|
|      Revision 1.6  2003/08/03 12:03:20  uzadow
|      Added unicode support; fixed some header includes.
|
|      Revision 1.5  2003/04/20 22:20:49  uzadow
|      Added check for errors reading memory-mapped files.
|
|      Revision 1.4  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.3  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.2  2001/09/16 20:57:17  uzadow
|      Linux version name prefix changes
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.4  2001/09/13 20:46:45  uzadow
|      Removed 4096-byte limit for fillBuffer that was causing PLPNGEncoder
|      to fail under Linux.
|
|
\--------------------------------------------------------------------
*/
