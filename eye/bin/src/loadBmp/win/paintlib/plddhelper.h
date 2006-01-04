/*
/--------------------------------------------------------------------
|
|      $Id: plddhelper.h,v 1.2 2002/03/31 13:36:42 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

char * GetDDErrorText
    (HRESULT hr
    );

void CheckForDDError 
    ( HRESULT hr
    );

/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/win/paintlib/plddhelper.h $
 * 
 * 1     04/07/12 13:35 Hase
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.1  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|
\--------------------------------------------------------------------
*/
