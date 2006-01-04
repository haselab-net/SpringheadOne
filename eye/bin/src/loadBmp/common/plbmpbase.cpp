/*
/--------------------------------------------------------------------
|
|      $Id: plbmpbase.cpp,v 1.3 2004/06/15 14:17:11 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plbmpbase.h"
#include "Filter/plfilter.h"

#include <iostream>

using namespace std;

// Creates an empty bitmap.
// Derived classes have to create a small bitmap here so the
// class can assume that a valid bitmap is available all the
// time.
PLBmpBase::PLBmpBase ()
{
}


PLBmpBase::~PLBmpBase
    ()
{
}

bool const PLBmpBase::operator ==
    ( PLBmpBase const &Other
    )
{
  return AlmostEqual (Other, 0);
}

#ifdef _DEBUG
void PLBmpBase::AssertValid () const
{
  if (m_Size != PLPoint(0,0))
  {
    // Color table only if 8 bpp.
    PLASSERT ((m_bpp > 8) == (m_pClrTab == NULL));
    PLASSERT (m_Size.y >= 0);
    PLASSERT (m_Size.x >= 0);

    // Greyscale only if 8 bpp
    PLASSERT ((m_bpp <= 8) || !m_bIsGreyscale);
  }
}
#endif

/////////////////////////////////////////////////////////////////////
// PLBmpBase manipulation

void PLBmpBase::SetGrayPalette ()
// Fills the color table with a grayscale palette.
{
  PLASSERT (m_pClrTab); // Bitmap must contain a palette!

  int i;
  int NumColors = GetNumColors();
  double ColFactor = 255/(NumColors-1);

  for (i=0; i<NumColors; i++)
    SetPaletteEntry (i, int(i*ColFactor), int(i*ColFactor), int(i*ColFactor), 0xFF);
}

void PLBmpBase::SetPalette (PLPixel32 * pPal)
{
  PLASSERT (m_pClrTab); // Bitmap must contain a palette!

  memcpy (m_pClrTab, pPal, GetNumColors() * sizeof(PLPixel32));
}


void PLBmpBase::SetAlphaChannel (PLBmpBase * pAlphaBmp)
// Replaces the alpha channel with a new one.
{
  PLBYTE * pAlphaLine;
  int x,y;
  PLPixel32 ** pLineArray;
  PLBYTE ** pAlphaLineArray;

  PLASSERT_VALID (this);
  // Current bitmap must be 32 bpp.
  PLASSERT (GetBitsPerPixel() == 32);

  PLASSERT_VALID (pAlphaBmp);
  // Alpha channel must be 8 bpp.
  PLASSERT (pAlphaBmp->GetBitsPerPixel() == 8);

  // The two bitmaps must have the same dimensions
  PLASSERT (pAlphaBmp->GetWidth() == GetWidth());
  PLASSERT (pAlphaBmp->GetHeight() == GetHeight());

  pLineArray = GetLineArray32();
  pAlphaLineArray = pAlphaBmp->GetLineArray();

  for (y=0; y < GetHeight(); y++)
  {
    PLPixel32 * pLine = pLineArray[y];
    pAlphaLine = pAlphaLineArray[y];
    for (x=0; x < GetWidth(); x++)
    {
      pLine[x].SetA(pAlphaLine[x]);
    }
  }

  m_bAlphaChannel = true;

  PLASSERT_VALID (this);
}

PLBYTE PLBmpBase::FindNearestColor (PLPixel32 col)
{
  PLPixel32 * pPalette = GetPalette();
  PLASSERT (pPalette);

  int d1;
  int dMin = 100000;
  int i;
  int index;
  for (i = 0; i<GetNumColors(); i++)
  {
    d1 = col.BoxDist (pPalette[i]);
    if (d1 < dMin)
    {
      dMin = d1;
      index = i;
    }
  }
  return index;
}

bool PLBmpBase::AlmostEqual
    ( const PLBmpBase& Bmp,
      int epsilon
    ) const
    // TODO: Fix 24 and 16 bpp versions!
{
  if (GetWidth() != Bmp.GetWidth() ||
      GetHeight() != Bmp.GetHeight() ||
      HasAlpha() != Bmp.HasAlpha() ||
      IsGreyscale() != Bmp.IsGreyscale() ||
      GetBitsPerPixel() != Bmp.GetBitsPerPixel())
    return false;

  if (m_Resolution != Bmp.GetResolution())
    return false;

  PLBYTE ** ppLines1 = GetLineArray();
  PLBYTE ** ppLines2 = Bmp.GetLineArray();
  int y,x;
  for (y=0; y<GetHeight(); y++)
    for (x=0; x<GetWidth(); x++)
      switch (GetBitsPerPixel())
      {
        case 8:
          if (abs (ppLines1[y][x] - ppLines2[y][x]) > epsilon)
            return false;
          break;
        case 16:
          if (((PLPixel16*)(ppLines1[y]))[x] != ((PLPixel16*)(ppLines2[y]))[x])
            return false;
          break;
        case 24:
          if (ppLines1[y][x*3+PL_RGBA_RED] != ppLines2[y][x*3+PL_RGBA_RED] ||
              ppLines1[y][x*3+PL_RGBA_GREEN] != ppLines2[y][x*3+PL_RGBA_GREEN] ||
              ppLines1[y][x*3+PL_RGBA_BLUE] != ppLines2[y][x*3+PL_RGBA_BLUE])
            return false;
          break;
        case 32:
          if (abs (ppLines1[y][x*4+PL_RGBA_RED] - ppLines2[y][x*4+PL_RGBA_RED]) > epsilon ||
              abs (ppLines1[y][x*4+PL_RGBA_GREEN] - ppLines2[y][x*4+PL_RGBA_GREEN]) > epsilon ||
              abs (ppLines1[y][x*4+PL_RGBA_BLUE] - ppLines2[y][x*4+PL_RGBA_BLUE]) > epsilon)
            return false;
          if (HasAlpha() &&
              abs (ppLines1[y][x*4+3] - ppLines2[y][x*4+3]) > epsilon)
            return false;
          break;
        default:
          // Unsupported BPP.
          PLASSERT (false);
      }

  // Test if the palettes are the same for paletted bitmaps.
  if (GetBitsPerPixel() == 8)
  {
    int i;
    PLPixel32 * pPal1 = GetPalette();
    PLPixel32 * pPal2 = Bmp.GetPalette();
    for (i=0; i<255; i++)
    {
      if (abs (pPal1[i].GetR() - pPal2[i].GetR()) > epsilon ||
          abs (pPal1[i].GetG() - pPal2[i].GetG()) > epsilon ||
          abs (pPal1[i].GetB() - pPal2[i].GetB()) > epsilon)
        return false;
    }
  }

  return true;
}


void PLBmpBase::initLocals (PLLONG Width, PLLONG Height, PLWORD BitsPerPixel,
                        bool bAlphaChannel, bool bIsGreyscale)
{
  SetBmpInfo (PLPoint (Width, Height), BitsPerPixel, PLPoint(0,0),
              bAlphaChannel, bIsGreyscale);

  if (BitsPerPixel < 16)
  { // Color table exists
    SetGrayPalette ();
  }

  PLASSERT_VALID (this);
}

/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plbmpbase.cpp $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.3  2004/06/15 14:17:11  uzadow
|      First working version of PLSubBmp.
|
|      Revision 1.2  2004/06/15 11:18:17  uzadow
|      First working version of PLBmpBase.
|
|      Revision 1.1  2004/06/15 10:46:41  uzadow
|      Initial nonfunctioning version of plbmpbase.
|
|
\--------------------------------------------------------------------
*/

