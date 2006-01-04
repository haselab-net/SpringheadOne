/*
/--------------------------------------------------------------------
|
|      $Id: plfilterfliprgb.h,v 1.2 2004/06/06 12:56:38 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#if !defined(INCL_PLFILTERFLIPRGB)
#define INCL_PLFILTERFLIPRGB

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "plfilter.h"

class PLBmp;

//! Filter that flips the R and B components of a bitmap.
class PLFilterFlipRGB : public PLFilter
{
public:
  PLFilterFlipRGB();
  virtual ~PLFilterFlipRGB();
  virtual void ApplyInPlace(PLBmp * pBmp) const;

private:
};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/Filter/plfilterfliprgb.h $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.2  2004/06/06 12:56:38  uzadow
|      Doxygenified documentation.
|
|      Revision 1.1  2003/07/27 18:08:38  uzadow
|      Added plfilterfliprgb
|
|
|
\--------------------------------------------------------------------
*/
