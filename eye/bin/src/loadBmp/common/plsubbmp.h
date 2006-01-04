/*
/--------------------------------------------------------------------
|
|      $Id: plsubbmp.h,v 1.1 2004/06/15 14:17:11 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLSUBBMP
#define INCL_PLSUBBMP

#ifndef INCL_PLBITMAP
#include "plbitmap.h"
#endif

class PLRect;


//! 
class PLSubBmp : public PLBmpBase
{

public:
  //! Creates an empty bitmap.
  PLSubBmp
    ();

  //! Destroys the bitmap.
  virtual ~PLSubBmp
    ();

  virtual void Create
    ( PLBmpBase & SrcBmp,
      const PLRect & SrcRect
    );
    
  virtual void Create
    ( int Width,
      int Height,
      int bpp,
      bool bHasAlpha,
      PLBYTE * pBits,
      int Stride
    );

private:

  // Deletes memory allocated by member variables.
  virtual void freeMembers
    ();
};



#endif
/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plsubbmp.h $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.1  2004/06/15 14:17:11  uzadow
|      First working version of PLSubBmp.
|
|
\--------------------------------------------------------------------
*/
