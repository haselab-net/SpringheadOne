/*
/--------------------------------------------------------------------
|
|      $Id: plsdlbmp.h,v 1.3 2004/06/15 14:32:22 uzadow Exp $
|
|      Copyright (c) 1996-2003 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLSDLBMP
#define INCL_PLSDLBMP

#ifndef INCL_PLBITMAP
#include "plbitmap.h"
#endif

#include <SDL/SDL.h>

//! Manipulates uncompressed bitmaps stored as SDL surfaces.
//! 32 bpp only. 
class PLSDLBmp : public PLBmp
{

public:
  //! Creates an empty bitmap.
  PLSDLBmp
    ( int SurfFlags = SDL_SWSURFACE
    );

  //! Destroys the bitmap.
  virtual ~PLSDLBmp
    ();

  //! Copy constructor
  PLSDLBmp
    ( const PLBmpBase &Orig
    );

  //! Copy constructor
  PLSDLBmp
    ( const PLSDLBmp &Orig
    );

  //! Attach to an existing SDL surface. After the attach, the 
  //! bitmap object besomes the owner of the surface. Any previous contents
  //! are destroyed.
  void Attach
    ( SDL_Surface * pSurface
    );

#ifdef _DEBUG
  virtual void AssertValid
    () const;    // Tests internal object state
#endif

  //! Assignment operator.
  PLSDLBmp &operator=
    ( const PLBmpBase &Orig
    );

  //! Assignment operator.
  PLSDLBmp &operator=
    ( const PLSDLBmp &Orig
    );

  // PLSDLBmp information

  //! Returns a pointer to the internal SDL surface.
  //! Using this pointer, you can bring SDL and paintlib data out of sync
  //! easily. Handle with care.
  SDL_Surface * GetSurface
    ();

  //! Returns the amount of memory used by the object.
  long GetMemUsed
    ();

  //! Returns number of bytes used per line.
  long GetBytesPerLine
    ();

  // Static functions

  //! Returns memory needed by a bitmap with the specified attributes.
  static long GetMemNeeded
    ( PLLONG width,
      PLLONG height,
      PLWORD BitsPerPixel
    );

  //! Returns memory needed by bitmap bits.
  static long GetBitsMemNeeded
    ( PLLONG width,
      PLLONG height,
      PLWORD BitsPerPixel
    );


protected:
  // Local functions

  // Create a new bitmap with uninitialized bits. Assumes no memory
  // is allocated yet.
  virtual void internalCreate
    ( PLLONG Width,
      PLLONG Height,
      PLWORD BitsPerPixel,
      bool bAlphaChannel,
      bool bIsGreyscale
    );

  // Deletes memory allocated by member variables.
  virtual void freeMembers
    ();

  // Initializes internal table of line addresses.
  virtual void initLineArray
    ();

private:  
  SDL_Surface * m_pSurface;        // Pointer to the bits.
  int m_SurfFlags;
};

// Note that _both_ these copy constructors are needed. If only the
// second one is there, MSVC generates a default copy
// constructor anyway :-(.
inline PLSDLBmp::PLSDLBmp
    ( const PLSDLBmp &Orig
    )
    : PLBmp ()
{
  m_SurfFlags = Orig.m_SurfFlags;
  internalCopy (Orig);
}

inline PLSDLBmp::PLSDLBmp
    ( const PLBmpBase &Orig
    )
    : PLBmp ()
{
  m_SurfFlags = SDL_SWSURFACE;
  internalCopy (Orig);
}

inline PLSDLBmp & PLSDLBmp::operator=
    ( const PLBmpBase &Orig
    )
{
  PLBmp::operator=(Orig);
  return *this;
}

inline PLSDLBmp & PLSDLBmp::operator=
    ( const PLSDLBmp &Orig
    )
{
  m_SurfFlags = Orig.m_SurfFlags;
  PLBmp::operator=(Orig);
  return *this;
}


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plsdlbmp.h $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.3  2004/06/15 14:32:22  uzadow
|      Linux build fix.
|
|      Revision 1.2  2003/03/08 15:15:51  uzadow
|      Added PLSDLBmp::Attach().
|
|      Revision 1.1  2003/03/08 14:32:08  uzadow
|      Added support for bitmaps in SDL surface format.
|
|
\--------------------------------------------------------------------
*/
