/*
/--------------------------------------------------------------------
|
|      $Id: plsubbmp.cpp,v 1.1 2004/06/15 14:17:11 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plsubbmp.h"
#include "plrect.h"
#include "plexcept.h"

#include <iostream>

using namespace std;

PLSubBmp::PLSubBmp
  ()
{
  m_pLineArray=0;
}


PLSubBmp::~PLSubBmp
    ()
{
  // Free the memory.
  freeMembers();
}
  
void PLSubBmp::Create
    ( PLBmpBase & SrcBmp,
      const PLRect & SrcRect
    )
{
  PLASSERT (!m_pLineArray);

  int bpp = SrcBmp.GetBitsPerPixel();
  if (bpp <= 8)
    m_pClrTab = new PLPixel32 [1 << bpp];
  else
    m_pClrTab = NULL;
  
  initLocals (SrcRect.Width(), SrcRect.Height(), 
          bpp, SrcBmp.HasAlpha(),
          SrcBmp.IsGreyscale());
  if (bpp <= 8)
    SetPalette (SrcBmp.GetPalette());
              
  m_pLineArray = new PLBYTE * [m_Size.y];
  PLBYTE** ppSrcLines = SrcBmp.GetLineArray();
  int XOfs = SrcRect.tl.x*(bpp/8);
  for (int y=0; y<m_Size.y; y++)
    m_pLineArray[y] = ppSrcLines[SrcRect.tl.y+y]+XOfs;
    
  PLASSERT_VALID(this);
}    
    
void PLSubBmp::Create
    ( int Width,
      int Height,
      int bpp,
      bool bHasAlpha,
      PLBYTE * pBits,
      int Stride
    )
{
  PLASSERT (!m_pLineArray);
  
  if (bpp <= 8)
    m_pClrTab = new PLPixel32 [1 << bpp];
  else
    m_pClrTab = NULL;
  initLocals (Width, Height, bpp, bHasAlpha, false);
  if (bpp <= 8)
    SetGrayPalette ();
          
  m_pLineArray = new PLBYTE * [m_Size.y];
  for (int y=0; y<m_Size.y; y++)
    m_pLineArray[y] = pBits+Stride*y;

  PLASSERT_VALID(this);
}    


/////////////////////////////////////////////////////////////////////
// Local functions


void PLSubBmp::freeMembers
    ()
{
  if (m_pClrTab)
  {
    delete [] m_pClrTab;
    m_pClrTab = NULL;
  }

  if (m_pLineArray)
  {
    delete [] m_pLineArray;
    m_pLineArray = NULL;
  }
}

/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plsubbmp.cpp $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.1  2004/06/15 14:17:11  uzadow
|      First working version of PLSubBmp.
|
|
\--------------------------------------------------------------------
*/
