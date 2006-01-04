/*
/--------------------------------------------------------------------
|
|      $Id: plbmpbase.h,v 1.2 2004/06/15 14:17:11 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLBMPBASE
#define INCL_PLBMPBASE

#include "plbmpinfo.h"
#include "plpoint.h"
#include "pldebug.h"
#include "plpixel32.h"
#include "plpixel24.h"
#include "plpixel16.h"

class PLFilter;

//!
class PLBmpBase : public PLBmpInfo
{

public:

  //! Empty constructor. Constructors in derived classes create a
  //! small empty bitmap to ensure that the object is always in a
  //! sane state.
  PLBmpBase
    ();

  //! Empty destructor.
  virtual ~PLBmpBase
    ();

  //! Test for equality. This function actually tests every pixel, so
  //! it's not fast. It's meant mainly for use in asserts and such.
  bool const operator ==
    ( PLBmpBase const &Other
    );

#ifdef _DEBUG
  virtual void AssertValid
    () const;    // Tests internal object state
#endif

  // PLBmpBase manipulation

  //! Fills the color table with a grayscale palette. This function
  //! is only usable for bitmaps containing a color table. Index 0
  //! contains black (0) and the last index contains white (255). The
  //! alpha channel is set to opaque (255) for every palette entry.
  void SetGrayPalette
    ();

  //! Sets the color table to pPal. The contents of pPal are copied.
  void SetPalette
    ( PLPixel32 * pPal
    );

  //! Sets one entry in the color table. The function may only be
  //! called if there is a color table stored with the bitmap. The
  //! color table entry is set to the red, green, blue, and alpha
  //! values specified.
  void SetPaletteEntry
    ( PLBYTE Entry,
      PLBYTE r,
      PLBYTE g,
      PLBYTE b,
      PLBYTE a
    );

  //! Sets one entry in the color table. The function may only be
  //! called if there is a color table stored with the bitmap. The
  //! color table entry is set to the red, green, blue, and alpha
  //! values specified.
  void SetPaletteEntry
    ( PLBYTE Entry,
      PLPixel32 Value
    );

  //! Replaces the alpha channel of the bitmap with a new one. This
  //! only works for bitmaps with 32 bpp. pAlphaBmp must point to an
  //! 8 bpp bitmap with the same dimensions as the object. The alpha
  //! channel information is physically copied into the bitmap.
  void SetAlphaChannel
    ( PLBmpBase * pAlphaBmp
    );

  //! Slow but simple function to set a single pixel. 32 bpp only.
  void SetPixel
    ( int x,
      int y,
      PLPixel32 pixel
    );

  //! Slow but simple function to get a single pixel. 32 bpp only.
  PLPixel32 GetPixel
    ( int x,
      int y
    ) const;

  //! Find the nearest color to cr in the palette used by this bitmap
  //! Only works for 8 bpp bitmaps.
  PLBYTE FindNearestColor
    ( PLPixel32 cr
    );

  // PLBmpBase information.

  //! Returns number of colors that can be stored by a bitmap with this
  //! color depth.
  int GetNumColors
    ();

  // PLBmpBase direct manipulation

  //! Returns the address of the color table of the bitmap or NULL if
  //! no color table exists. The color table is stored as an array of
  //! consecutive PLPixel32 objects.
  PLPixel32 * GetPalette
    () const;

  //! Returns pointer to an array containing the starting addresses of
  //! the bitmap lines. This array should be used whenever the bitmap
  //! bits need to be manipulated directly.
  PLBYTE ** GetLineArray
    () const;

  //! Returns pointer to an array containing the starting addresses of
  //! the bitmap lines. This array should be used whenever the bitmap
  //! bits need to be manipulated directly.
  PLPixel32 ** GetLineArray32
    () const;

  //! Returns pointer to an array containing the starting addresses of
  //! the bitmap lines. This array should be used whenever the bitmap
  //! bits need to be manipulated directly.
  PLPixel24 ** GetLineArray24
    () const;

  //! Returns pointer to an array containing the starting addresses of
  //! the bitmap lines. This array should be used whenever the bitmap
  //! bits need to be manipulated directly.
  PLPixel16 ** GetLineArray16
    () const;

  //! Returns true if Bmp and this are almost equal. The comparison is done by 
  //! comparing the pixels in the bitmaps component-wise. If all components are 
  //! closer than epsilon, the bitmaps are considered almost equal.
  bool AlmostEqual
    ( const PLBmpBase& Bmp,
      int epsilon
    ) const;

  //! Sets the bitmap resolution in pixels per inch.
  void SetResolution (const PLPoint& Resolution);

protected:
  //! Can be called from internalCreate() to initialize object state.
  virtual void initLocals
    ( PLLONG Width,
      PLLONG Height,
      PLWORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale    
    );

  // Member variables
  PLPixel32 * m_pClrTab;      // Pointer to the color table.
  PLBYTE    ** m_pLineArray;   // Table of the starting addresses of
                             // the lines.
};

inline void PLBmpBase::SetPaletteEntry
    ( PLBYTE Entry,
      PLBYTE r,
      PLBYTE g,
      PLBYTE b,
      PLBYTE a
    )
{
  m_pClrTab[Entry].Set (r, g, b, a);
}

inline void PLBmpBase::SetPaletteEntry
    ( PLBYTE Entry,
      PLPixel32 Value
    )
{
  m_pClrTab[Entry] = Value;
}


inline PLPixel32 PLBmpBase::GetPixel (int x, int y) const
{
  PLASSERT (GetBitsPerPixel() == 32);
  return *((PLPixel32 *)GetLineArray()[y]+x);
}

inline void PLBmpBase::SetPixel (int x, int y, PLPixel32 pixel)
{
  PLASSERT (GetBitsPerPixel() == 32);
  *(GetLineArray32()[y]+x) = pixel;
}

inline void PLBmpBase::SetResolution(const PLPoint& Resolution)
{
  PLASSERT_VALID(this);
  m_Resolution = Resolution;
}

inline int PLBmpBase::GetNumColors
    ()
{
  PLASSERT_VALID (this);

  if (m_bpp == 32)
    return 1 << 24;
   else
    return 1 << m_bpp;
}


// PLBmpBase direct manipulation

inline PLBYTE ** PLBmpBase::GetLineArray
    () const
{
  PLASSERT (m_pLineArray);
  return m_pLineArray;
}

inline PLPixel32 ** PLBmpBase::GetLineArray32
    () const
{
  PLASSERT (m_pLineArray);
  PLASSERT (m_bpp == 32);
  return (PLPixel32**)m_pLineArray;
}

inline PLPixel24 ** PLBmpBase::GetLineArray24
    () const
{
  PLASSERT (m_pLineArray);
  PLASSERT (m_bpp == 24);
  return (PLPixel24**)m_pLineArray;
}

inline PLPixel16 ** PLBmpBase::GetLineArray16
    () const
{
  PLASSERT (m_pLineArray);
  PLASSERT (m_bpp == 16);
  return (PLPixel16**)m_pLineArray;
}

inline PLPixel32 * PLBmpBase::GetPalette
    () const
    // Returns adress of the color table of the bitmap or NULL if no
    // color table exists.
{
  PLASSERT_VALID (this);

  return m_pClrTab;
}

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plbmpbase.h $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.2  2004/06/15 14:17:11  uzadow
|      First working version of PLSubBmp.
|
|      Revision 1.1  2004/06/15 10:46:41  uzadow
|      Initial nonfunctioning version of plbmpbase.
|
|
\--------------------------------------------------------------------
*/
