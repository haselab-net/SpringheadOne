/*
/--------------------------------------------------------------------
|
|      $Id: plbitmap.cpp,v 1.18 2004/06/15 14:17:11 uzadow Exp $
|      Device independent bitmap class
|
|      Manipulates uncompressed device independent bitmaps
|      of all color depths.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plbitmap.h"
#include "plexcept.h"
#include "plfilterquantize.h"

#include <iostream>

using namespace std;

template<class DestPixelC>
void createTrueColorCopy (PLBmp & rDestBmp, const PLBmpBase & rSrcBmp,
                                      DestPixelC Dummy);

// Creates an empty bitmap.
// Derived classes have to create a small bitmap here so the
// class can assume that a valid bitmap is available all the
// time.
PLBmp::PLBmp ()
: m_LockCount(0),
  m_DitherPaletteType(PLDTHPAL_DEFAULT),   // defaults for 8Bit color quantization
  m_DitherType(PLDTH_NONE)
{
}


PLBmp::~PLBmp
    ()
{
  PLASSERT (m_LockCount == 0);
}

// The dummy parameter is nessesary to work around an MSVC bug. If it isn't
// there, the function will only get instantiated once :-(.
template<class DestPixelC>
void createTrueColorCopy (PLBmp & rDestBmp, const PLBmpBase & rSrcBmp,
                          DestPixelC Dummy)
{
  // Make sure we're using the right template.
  PLASSERT (rDestBmp.GetBitsPerPixel() == sizeof (DestPixelC)*8);

  int BPPSrc = rSrcBmp.GetBitsPerPixel();
  PLBYTE ** pSrcLines = rSrcBmp.GetLineArray();
  DestPixelC ** pDstLines = (DestPixelC**) rDestBmp.GetLineArray();
  int SrcLineLen = rSrcBmp.GetWidth();

  for (int y = 0; y<rSrcBmp.GetHeight(); ++y)
  { // For each line
    DestPixelC * pDstPixel = pDstLines[y];
    switch (BPPSrc)
    {
      case 32:
        {
          PLPixel32 * pSrcPixel = (PLPixel32 *)pSrcLines[y];
          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            *pDstPixel = *pSrcPixel;
            ++pSrcPixel;
            ++pDstPixel;
          }
        }
        break;
      case 24:
        {
          PLPixel24 * pSrcPixel = (PLPixel24 *)pSrcLines[y];
          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            *pDstPixel = *pSrcPixel;
            ++pSrcPixel;
            ++pDstPixel;
          }
        }
        break;
      case 16:
        {
          PLPixel16 * pSrcPixel = (PLPixel16 *)pSrcLines[y];
          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            *pDstPixel = (DestPixelC)*pSrcPixel;
            ++pSrcPixel;
            ++pDstPixel;
          }
        }
        break;
      case 8:
        {
          PLPixel32 *pPal = rSrcBmp.GetPalette();

          PLBYTE * pSrcPixel = pSrcLines[y];

          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            *pDstPixel = pPal[*pSrcPixel];
            ++pSrcPixel;
            ++pDstPixel;
          }
        }
        break;
      case 1:
        {
          PLPixel32 * pPal = rSrcBmp.GetPalette();
          DestPixelC blackDot, whiteDot;
          // if bi-tonal "palette" exists, use it...
          if (pPal)
          {
            whiteDot = pPal[0];
            blackDot = pPal[1];
          }
          else
          {
            whiteDot.Set (255,255,255);
            blackDot.Set (0,0,0);
          }

          // assume msb is leftmost
          PLBYTE * pSrcPixel = pSrcLines[y];

          for (int x = 0; x < SrcLineLen; ++x)
          { // For each pixel
            if (pSrcPixel[x / 8] & (128 >> (x & 7)))  // black pixel
              *pDstPixel = blackDot;
            else
              *pDstPixel = whiteDot;
            pDstPixel++;
          }
        }
        break;
      default:
        PLASSERT (false);
    }
  }
}

#ifdef _DEBUG
void PLBmp::AssertValid () const
{
  PLBmpBase::AssertValid();
  PLASSERT (m_LockCount >= 0);
}
#endif


/////////////////////////////////////////////////////////////////////
// PLBmp creation

void PLBmp::Create (PLLONG Width, PLLONG Height, PLWORD BitsPerPixel,
                    bool bAlphaChannel, bool bIsGreyscale,
                    PLBYTE * pBits, int Stride, const PLPoint& Resolution)
{
  PLASSERT_VALID (this);

  freeMembers ();
  internalCreate (Width, Height, BitsPerPixel, bAlphaChannel, bIsGreyscale);
  m_Resolution = Resolution;

  if (pBits) 
  {
    if (Stride == 0)
    {
      Stride = Width*BitsPerPixel/8;
    }
    for (int y = 0; y<Height; y++) 
    {
      memcpy(GetLineArray()[y], pBits+y*Stride, Width*BitsPerPixel/8);
    }
  }

  PLASSERT_VALID (this);
}

void PLBmp::Create (const PLBmpInfo& Info)
{
  Create (Info.GetWidth(), Info.GetHeight(), Info.GetBitsPerPixel(),
          Info.HasAlpha(), Info.IsGreyscale(), NULL, 0, Info.GetResolution());
}

void PLBmp::CreateCopy (const PLBmpBase & rSrcBmp, int BPPWanted)
{
  PLASSERT_VALID (&rSrcBmp);

  PLASSERT (BPPWanted == 32 || BPPWanted == 24 || BPPWanted == 16 ||
          BPPWanted == 8 || BPPWanted == 1 || BPPWanted == 0);
  int BPPSrc = rSrcBmp.GetBitsPerPixel();
  PLASSERT (BPPSrc == 32 || BPPSrc == 24 || BPPSrc == 16 ||
          BPPSrc == 8 || BPPSrc == 1);

  if (BPPWanted == BPPSrc || BPPWanted == 0)
  {
    if (&rSrcBmp != this)
    {
      // Create empty bitmap.
      freeMembers ();
      internalCopy (rSrcBmp);
    }
  }
  else
  {
    // Can't copy to self while changing bit depth.
    PLASSERT (&rSrcBmp != this);

    bool bDestAlpha = rSrcBmp.HasAlpha() && BPPWanted == 32;
    bool bDestGreyscale = rSrcBmp.IsGreyscale() && BPPWanted == 8;
    Create (rSrcBmp.GetWidth(), rSrcBmp.GetHeight(),
            BPPWanted, bDestAlpha, bDestGreyscale);

    switch (BPPWanted)
    {
      case 32:
        createTrueColorCopy (*this, rSrcBmp, PLPixel32());
        break;

      case 24:
        createTrueColorCopy (*this, rSrcBmp, PLPixel24());
        break;

      case 16:
        createTrueColorCopy (*this, rSrcBmp, PLPixel16());
        break;

      case 8:
        create8BPPCopy (rSrcBmp);
        break;

      case 1:
        create1BPPCopy (rSrcBmp);
        break;
      default:
        PLASSERT(false);
    }
    SetResolution (rSrcBmp.GetResolution());
    PLASSERT_VALID (this);
  }
}

void PLBmp::CreateFilteredCopy (PLBmpBase & rSrcBmp, const PLFilter & rFilter)
{
  PLASSERT_VALID (&rSrcBmp);
  rFilter.Apply (&rSrcBmp, this);
}


/////////////////////////////////////////////////////////////////////
// PLBmp manipulation

void PLBmp::SetQuantizationMode (int DitherType, int DitherPaletteType)
{
  PLASSERT_VALID (this);
  m_DitherType = DitherType;
  m_DitherPaletteType = DitherPaletteType;
}

void PLBmp::ApplyFilter
    ( const PLFilter& Filter
    )
{
  Filter.ApplyInPlace (this);
}

void PLBmp::SetHasAlpha
    (bool b
    )
{
  PLASSERT_VALID (this);
  if (!m_bAlphaChannel && b)
  {
    if (m_bpp == 32) 
    {
      for (int y=0; y < GetHeight(); y++)
      {
        PLPixel32 * pLine = GetLineArray32()[y];
        for (int x=0; x < GetWidth(); x++)
          pLine[x].SetA(0xFF);
      }
    }
    else 
    {
      PLASSERT (m_bpp == 8);
      for (int i=0; i<GetNumColors(); i++)
        ((PLBYTE*)(m_pClrTab+i))[3] = 0xFF;
    }
  }
  m_bAlphaChannel = b;
}

void PLBmp::Lock (bool bReadable, bool bWriteable)
{
  PLASSERT (m_LockCount >= 0);
  PLASSERT (bReadable || bWriteable);
  m_LockCount++;
}

void PLBmp::Unlock
    ()
{
  m_LockCount--;
  PLASSERT (m_LockCount >= 0);
}

/////////////////////////////////////////////////////////////////////
// Local functions

void PLBmp::initLocals (PLLONG Width, PLLONG Height, PLWORD BitsPerPixel,
                        bool bAlphaChannel, bool bIsGreyscale)
{
  SetBmpInfo (PLPoint (Width, Height), BitsPerPixel, PLPoint(0,0),
              bAlphaChannel, bIsGreyscale);
  initLineArray ();

  if (BitsPerPixel < 16)
  { // Color table exists
    SetGrayPalette ();
  }

  PLASSERT_VALID (this);
}


void PLBmp::internalCopy (const PLBmpBase & rSrcBmp)
{
  PLASSERT_VALID(&rSrcBmp);
  // Create empty bitmap.
  internalCreate (rSrcBmp.GetWidth(), rSrcBmp.GetHeight(), 
                  rSrcBmp.GetBitsPerPixel(),
                  rSrcBmp.HasAlpha(), rSrcBmp.IsGreyscale());
  PLBYTE ** pSrcLines = rSrcBmp.GetLineArray();
  PLBYTE ** pDstLines = GetLineArray();
  // Minimum possible line length.
  int LineLen = GetBytesPerLine();

  for (int y = 0; y<GetHeight(); y++)
    memcpy (pDstLines[y], pSrcLines[y], LineLen);

  if (GetBitsPerPixel() <= 8)
    SetPalette (rSrcBmp.GetPalette());

  SetResolution (rSrcBmp.GetResolution());

  PLASSERT_VALID (this);
}

void PLBmp::create8BPPCopy (const PLBmpBase & rSrcBmp)
{
  int BPPSrc = rSrcBmp.GetBitsPerPixel();

  if (BPPSrc == 32) // Conversion 32->8 BPP
  {
    PLFilterQuantize Filter (m_DitherPaletteType, m_DitherType);
    Filter.Apply (&(const_cast<PLBmpBase &>(rSrcBmp)), this);
  }
  else // 1 -> 8
  {
    PLPixel32 *pPal = rSrcBmp.GetPalette();
    // if bi-tonal "palette" exists, use it...
    if (pPal)
    {
      PLBYTE *pWhite = (PLBYTE *) pPal;
      PLBYTE *pBlack = (PLBYTE *) (pPal+1);
      SetPaletteEntry(0,
                      pWhite[PL_RGBA_RED],pWhite[PL_RGBA_GREEN],pWhite[PL_RGBA_BLUE],
                      255);
      SetPaletteEntry(1,
                      pBlack[PL_RGBA_RED],pBlack[PL_RGBA_GREEN],pBlack[PL_RGBA_BLUE],
                      255);
    }
    else
    {
      SetPaletteEntry(0,255,255,255,255);
      SetPaletteEntry(1,0,0,0,255);
    }

    int SrcLineLen = rSrcBmp.GetWidth();
    // assume msb is leftmost
    for (int y = 0; y<GetHeight(); ++y)
    { // For each line
      PLBYTE * pSrcPixel = rSrcBmp.GetLineArray()[y];
      PLBYTE * pDstPixel = GetLineArray()[y];
      for (int x = 0; x < SrcLineLen; ++x)  // For each pixel
        pDstPixel[x] = pSrcPixel[x / 8] & (128 >> (x & 7)) ? 1 : 0;
    }
  }
}

void PLBmp::create1BPPCopy (const PLBmpBase & rSrcBmp)
{
  int BPPSrc = rSrcBmp.GetBitsPerPixel();
  PLBYTE ** pSrcLines = rSrcBmp.GetLineArray();
  PLBYTE ** pDstLines = GetLineArray();
  int SrcLineLen = GetWidth();

  SetPaletteEntry(0,255,255,255,255);
  SetPaletteEntry(1,0,0,0,255);

  // downgrade to monochrome
  PLPixel32 *pPal = rSrcBmp.GetPalette();
  PLBYTE *pRGBA;
  int DstLineLen = GetBytesPerLine();

  for (int y = 0; y < GetHeight(); ++y)
  { // For each line
    PLBYTE * pSrcPixel = pSrcLines[y];
    PLBYTE * pDstPixel = pDstLines[y];
    // fill with background (index 0) color
    memset(pDstPixel,0,DstLineLen);

    for (int x = 0; x < SrcLineLen; ++x)  // For each pixel
    {
      pRGBA = BPPSrc == 8 ? (PLBYTE*) &pPal[*pSrcPixel] : pSrcPixel;
      // the following criterion supposedly finds "dark" pixels; it may
      // need some twiddling and maybe use the alpha channel as well
      if (pRGBA[PL_RGBA_RED] < 128 &&
          pRGBA[PL_RGBA_GREEN] < 128 &&
          pRGBA[PL_RGBA_BLUE] < 128 )
        pDstPixel[x / 8] |= 128 >> (x & 7);
      pSrcPixel += BPPSrc == 8 ? 1 : sizeof(PLPixel32);
    }
  }
}

/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plbitmap.cpp $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.18  2004/06/15 14:17:11  uzadow
|      First working version of PLSubBmp.
|
|      Revision 1.17  2004/06/15 10:26:05  uzadow
|      Initial nonfunctioning version of plbmpbase.
|
|      Revision 1.16  2004/06/09 21:34:53  uzadow
|      Added 16 bpp support to plbitmap, planybmp and pldirectfbbmp
|
|      Revision 1.15  2003/11/05 15:17:23  artcom
|      Added ability to specify initial data in PLBitmap::Create()
|
|      Revision 1.14  2003/07/29 21:27:41  uzadow
|      Fixed PLDirectFBBmp::GetBytesPerLine(), im2* Makefiles
|
|      Revision 1.13  2003/02/24 16:19:49  uzadow
|      Added url source (linux only), improved configure support for libungif detection.
|
|      Revision 1.12  2003/02/15 21:26:58  uzadow
|      Added win32 version of url data source.
|
|      Revision 1.11  2002/11/02 20:33:27  uzadow
|      Added gif transparency handling (Mark Richarme)
|
|      Revision 1.10  2002/08/05 19:06:30  uzadow
|      no message
|
|      Revision 1.9  2002/08/04 21:20:41  uzadow
|      no message
|
|      Revision 1.8  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.7  2002/03/31 13:36:41  uzadow
|      Updated copyright.
|
|      Revision 1.6  2001/10/21 17:12:39  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|      Revision 1.5  2001/10/16 17:12:26  uzadow
|      Added support for resolution information (Luca Piergentili)
|
|      Revision 1.4  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.3  2001/09/30 19:54:17  uzadow
|      Improved AlmostEqual();
|
|      Revision 1.2  2001/09/28 19:50:56  uzadow
|      Added some 24 bpp stuff & other minor features.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.31  2001/09/15 14:30:20  uzadow
|      Fixed PLPixel32 initialization bug.
|
|      Revision 1.30  2001/09/13 20:47:36  uzadow
|      Removed commented-out lines.
|
|      Revision 1.29  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.28  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.27  2000/12/18 22:42:52  uzadow
|      Replaced RGBAPIXEL with PLPixel32.
|
|      Revision 1.26  2000/12/03 21:15:43  uzadow
|      Fixed png handling in configure; test/ adapted to linux
|
|      Revision 1.25  2000/11/21 20:18:03  uzadow
|      Added operator ==
|
|      Revision 1.24  2000/11/02 21:28:47  uzadow
|      Fixed copy constructors.
|
|      Revision 1.23  2000/09/26 14:28:47  Administrator
|      Added Threshold filter
|
|      Revision 1.22  2000/09/26 12:14:51  Administrator
|      Refactored quantization.
|
|      Revision 1.21  2000/09/01 14:30:03  Administrator
|      no message
|
|      Revision 1.20  2000/09/01 14:19:46  Administrator
|      no message
|
|      Revision 1.19  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|      Revision 1.18  2000/05/28 11:08:51  Ulrich von Zadow
|      Corrected alpha channel bug in CreateCopy.
|
|      Revision 1.17  2000/03/31 12:20:05  Ulrich von Zadow
|      Video invert filter (beta)
|
|      Revision 1.16  2000/03/31 11:53:29  Ulrich von Zadow
|      Added quantization support.
|
|      Revision 1.15  2000/01/16 20:43:12  anonymous
|      Removed MFC dependencies
|
|      Revision 1.14  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.13  1999/12/31 17:59:54  Ulrich von Zadow
|      Corrected error in PLBmp::SetPixel for 1 bpp.
|
|      Revision 1.12  1999/12/30 15:54:47  Ulrich von Zadow
|      Added PLWinBmp::FromClipBoard() and CreateFromHBitmap().
|
|      Revision 1.11  1999/12/10 01:27:26  Ulrich von Zadow
|      Added assignment operator and copy constructor to
|      bitmap classes.
|
|      Revision 1.10  1999/12/08 15:39:45  Ulrich von Zadow
|      Unix compatibility changes
|
|      Revision 1.9  1999/12/02 17:07:34  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.8  1999/11/08 22:10:53  Ulrich von Zadow
|      no message
|
|      Revision 1.7  1999/10/22 21:25:51  Ulrich von Zadow
|      Removed buggy octree quantization
|
|      Revision 1.6  1999/10/21 18:47:26  Ulrich von Zadow
|      Added Rotate, GetPixel, SetPixel and FindNearestColor
|
|      Revision 1.5  1999/10/21 16:05:43  Ulrich von Zadow
|      Moved filters to separate directory. Added Crop, Grayscale and
|      GetAlpha filters.
|
|      Revision 1.4  1999/10/19 21:29:44  Ulrich von Zadow
|      Added filters.
|
|      Revision 1.3  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
\--------------------------------------------------------------------
*/

