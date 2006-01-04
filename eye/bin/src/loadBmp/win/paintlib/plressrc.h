/*
/--------------------------------------------------------------------
|
|      $Id: plressrc.h,v 1.4 2003/08/03 12:03:22 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_RESSRC
#define INCL_RESSRC

#ifndef INCL_PLDATASRC
#include "pldatasrc.h"
#endif
#include "plpaintlibdefs.h"

//! This is a class which takes a windows resource as a source of
//! picture data.
class PLResourceSource : public PLDataSource
{
public:
  //!
  PLResourceSource
    ();

  //!
  virtual ~PLResourceSource
    ();

  //!
  virtual int Open
    ( HINSTANCE lh_ResInst, int ResourceID,
      const char * pResType = NULL
    );

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

  //! This is a legacy routine that interferes with progress notifications.
  //! Don't call it!
  virtual PLBYTE * ReadEverything
    ();

private:
  HRSRC   m_hRsrc;
  HGLOBAL m_hGlobal;
  PLBYTE * m_pCurPos;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/win/paintlib/plressrc.h $
 * 
 * 1     04/07/12 13:35 Hase
|      Revision 1.4  2003/08/03 12:03:22  uzadow
|      Added unicode support; fixed some header includes.
|
|      Revision 1.3  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2000/01/16 20:43:18  anonymous
|      Removed MFC dependencies
|
|      Revision 1.4  2000/01/11 22:07:11  Ulrich von Zadow
|      Added instance handle parameter.
|
|      Revision 1.3  1999/11/02 21:20:14  Ulrich von Zadow
|      AfxFindResourceHandle statt AfxGetInstanceHandle
|      verwendet.
|
|
\--------------------------------------------------------------------
*/
