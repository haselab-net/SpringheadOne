/*
/--------------------------------------------------------------------
|
|      $Id: plfiltergetalpha.cpp,v 1.8 2004/06/15 10:26:13 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plfiltergetalpha.h"
#include "plbitmap.h"


PLFilterGetAlpha::PLFilterGetAlpha() : PLFilter()
{
}

PLFilterGetAlpha::~PLFilterGetAlpha()
{

}

void PLFilterGetAlpha::Apply(PLBmpBase * pBmpSource, PLBmp * pBmpDest) const
{
  // Only works for 32 bpp bitmaps.
  PLASSERT (pBmpSource->GetBitsPerPixel() == 32);

  PLASSERT (pBmpSource->HasAlpha());

  pBmpDest->Create (pBmpSource->GetWidth(), pBmpSource->GetHeight(), 8, false,
                    true, NULL, 0, 
                    pBmpSource->GetResolution());
  PLPixel32 ** pSrcLines = pBmpSource->GetLineArray32();
  PLBYTE ** pDstLines = pBmpDest->GetLineArray();

  for (int y = 0; y<pBmpDest->GetHeight(); ++y)
  { // For each line
    PLPixel32 * pSrcPixel = pSrcLines[y];
    PLBYTE * pDstPixel = pDstLines[y];

    for (int x = 0; x < pBmpDest->GetWidth(); ++x)
    { // For each pixel
      *pDstPixel = pSrcPixel->GetA();
      ++pSrcPixel;
      ++pDstPixel;
    }
  }
}

/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/Filter/plfiltergetalpha.cpp $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.8  2004/06/15 10:26:13  uzadow
|      Initial nonfunctioning version of plbmpbase.
|
|      Revision 1.7  2003/11/05 15:17:26  artcom
|      Added ability to specify initial data in PLBitmap::Create()
|
|      Revision 1.6  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.5  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.4  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.3  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.2  2001/10/06 22:37:08  uzadow
|      Linux compatibility.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.7  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.6  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.5  2000/12/18 22:42:53  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.4  2000/01/16 20:43:15  anonymous
|      Removed MFC dependencies
|
|      Revision 1.3  1999/12/08 16:31:40  Ulrich von Zadow
|      Unix compatibility
|
|      Revision 1.2  1999/10/21 18:48:03  Ulrich von Zadow
|      no message
|
|      Revision 1.1  1999/10/21 16:05:17  Ulrich von Zadow
|      Moved filters to separate directory. Added Crop, Grayscale and
|      GetAlpha filters.
|
|      Revision 1.1  1999/10/19 21:29:44  Ulrich von Zadow
|      Added filters.
|
|
\--------------------------------------------------------------------
*/
