/*
/--------------------------------------------------------------------
|
|      $Id: plfilterfliprgb.cpp,v 1.3 2004/06/14 12:51:35 artcom Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfilterfliprgb.h"
#include "plbitmap.h"


PLFilterFlipRGB::PLFilterFlipRGB()
  : PLFilter()
{
}

PLFilterFlipRGB::~PLFilterFlipRGB()
{

}

void PLFilterFlipRGB::ApplyInPlace(PLBmp * pBmp) const
{
  // Only 24 and 32 bpp supported.
  PLASSERT (pBmp->GetBitsPerPixel() == 24 ||
            pBmp->GetBitsPerPixel() == 32);

  PLBYTE ** pLineArray = pBmp->GetLineArray();
  for (int y = 0; y < pBmp->GetHeight(); y++)
  {
    PLBYTE * pLine = pLineArray[y];
    if (pBmp->GetBitsPerPixel() == 24) 
    {
      for (int x = 0; x < pBmp->GetWidth(); x++) 
      { 
        PLBYTE tmp = pLine[x*3+PL_RGBA_RED];
        pLine[x*3+PL_RGBA_RED] = pLine[x*3+PL_RGBA_BLUE];
        pLine[x*3+PL_RGBA_BLUE] = tmp;
      }
    } 
    else
    {
      for (int x = 0; x < pBmp->GetWidth(); x++) 
      { 
        PLBYTE tmp = pLine[x*4+PL_RGBA_RED];
        pLine[x*4+PL_RGBA_RED] = pLine[x*4+PL_RGBA_BLUE];
        pLine[x*4+PL_RGBA_BLUE] = tmp;
      } 
    }
  }
}

/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/Filter/plfilterfliprgb.cpp $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.3  2004/06/14 12:51:35  artcom
|      Performance improvement
|
|      Revision 1.2  2003/07/29 21:27:41  uzadow
|      Fixed PLDirectFBBmp::GetBytesPerLine(), im2* Makefiles
|
|      Revision 1.1  2003/07/27 18:08:38  uzadow
|      Added plfilterfliprgb
|
|
\--------------------------------------------------------------------
*/
