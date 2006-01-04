/*
/--------------------------------------------------------------------
|
|      $Id: pldatasrc.cpp,v 1.5 2003/08/03 12:03:20 uzadow Exp $
|      Data Source Base Class
|
|      This is a base class for a source of picture data.
|      It defines methods to open, close, and read from data sources.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"

#include "pldatasrc.h"
#include "plprognot.h"
#include <string.h>


PLDataSource::PLDataSource
    ( 
      PLIProgressNotification * pNotification
    )
    : PLObject(),
      m_pszName(NULL),
#ifdef _WINDOWS
        m_pszwName(NULL),
        m_bNameIsWide(false),
#endif
      m_FileSize(0),
      m_BytesRead(0),
      m_bSrcLSBFirst(true),   // Source byte order: true for intel order,
                              // false for Motorola et al. (MSB first).
      m_pNotification(pNotification)

{
}


PLDataSource::~PLDataSource
    ()
{
}


void PLDataSource::Open
    ( const char * pszName,
      int    FileSize
    )
{
  // Data source may not be open already!
  PLASSERT (!m_FileSize);

  m_pszName = new char [strlen (pszName)+1];
  strcpy (m_pszName, pszName);
  m_FileSize = FileSize;
  m_BytesRead = 0;
#ifdef _WINDOWS
  m_bNameIsWide = false;
#endif
}

#ifdef _WINDOWS
void PLDataSource::OpenW
    ( const wchar_t * pszwName,
      int    FileSize
    )
{
  // Data source may not be open already!
  PLASSERT (!m_FileSize);

  m_pszwName = new wchar_t [wcslen (pszwName)+1];
  wcscpy (m_pszwName, pszwName);
  m_pszName = new char[strlen("Wide")+1];
  strcpy(m_pszName,"Wide");
  m_FileSize = FileSize;
  m_BytesRead = 0;
  m_bNameIsWide = true;
}
#endif

void PLDataSource::Close
    ()
{
  if (m_pNotification)
    // be smart and tell the world: ich habe fertig!
    m_pNotification->OnProgress( 1);
  delete [] m_pszName;
  m_pszName = NULL;
#ifdef _WINDOWS
  delete [] m_pszwName;
  m_pszwName = NULL;
  m_bNameIsWide = false;
#endif
  m_FileSize = 0;
}


char * PLDataSource::GetName
    ()
{
  return m_pszName;
}

#ifdef _WINDOWS
wchar_t * PLDataSource::GetNameW
    ()
{
  return m_pszwName;
}
#endif

PLBYTE * PLDataSource::ReadNBytes
    ( int n
    )
{
  int OldBytesRead = m_BytesRead;
  m_BytesRead += n;
  if (m_BytesRead/1024 > OldBytesRead/1024 && m_pNotification)
    m_pNotification->OnProgress (double(m_BytesRead)/m_FileSize);
  CheckEOF();
  return NULL;
}

// Jo Hagelberg 15.4.99:
// for use by other libs that handle progress internally (eg libjpeg)
void PLDataSource::OProgressNotification
    ( double part
    )
{
  if( m_pNotification)
    m_pNotification->OnProgress( part);
}
/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/pldatasrc.cpp $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.5  2003/08/03 12:03:20  uzadow
|      Added unicode support; fixed some header includes.
|
|      Revision 1.4  2002/11/18 14:44:39  uzadow
|      Added PNG compression support, fixed docs.
|
|      Revision 1.3  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.8  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.7  2001/01/21 14:28:21  uzadow
|      Changed array cleanup from delete to delete[].
|
|      Revision 1.6  2000/01/16 20:43:13  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.4  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/
