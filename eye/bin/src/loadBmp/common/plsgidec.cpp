/*
/--------------------------------------------------------------------
|
|      $Id: plsgidec.cpp,v 1.1 2003/06/01 21:12:06 uzadow Exp $
|      SGI rgb Decoder Class
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"
#include "plsgidec.h"
#include "plexcept.h"

const PLWORD SGI_MAGIC = 474;

const int STORAGE_VERBATIM = 0;
const int STORAGE_RLE      = 1;

const int CMAP_NORMAL      = 0;
const int CMAP_DITHERED    = 1;  // not supported
const int CMAP_SCREEN      = 2;  // not supported
const int CMAP_COLORMAP    = 3;  // not supported


PLSGIDecoder::PLSGIDecoder
    ()
    : PLPicDecoder()
{
}


PLSGIDecoder::~PLSGIDecoder
    ()
{
}

void PLSGIDecoder::Open (PLDataSource * pDataSrc)
{
  Trace (2, "Decoding SGI.\n");

  readHeader (&m_Header, pDataSrc);

  int DestBPP;
  bool bAlpha = false;

  switch (m_Header.ZSize)
  {
    case 1:
      DestBPP = 8;
      break;
    case 2:
      throw PLTextException (PL_ERRFORMAT_NOT_SUPPORTED, "Two-channel SGI RGB files are not supported.");
      break;
    case 3:
      DestBPP = 32;
      break;
    case 4:
      // Image has alpha channel
      DestBPP = 32;
      bAlpha = true;
  }
  if (m_Header.Dimension == 1)
  {
    throw PLTextException (PL_ERRFORMAT_NOT_SUPPORTED, "One-dimensional SGI RGB files are not supported.");
  }
  if (m_Header.Storage == STORAGE_RLE)
  {
    throw PLTextException (PL_ERRFORMAT_NOT_SUPPORTED, "Compressed SGI RGB files are not supported.");
  }
  
  bool bGreyscale = (m_Header.ZSize == 1 && m_Header.Colormap == CMAP_NORMAL);
  SetBmpInfo (PLPoint (m_Header.XSize, m_Header.YSize), DestBPP,
              PLPoint(0, 0), bAlpha, bGreyscale);
}

void PLSGIDecoder::GetImage (PLBmp & Bmp)
{
  Bmp.Create(*this);
  if (GetBitsPerPixel() == 8) 
  {
    PLBYTE ** ppLine = Bmp.GetLineArray();
    for (int y = 0; y < GetHeight(); y++) 
    {
      memcpy(ppLine[GetHeight()-y], m_pDataSrc->ReadNBytes(GetWidth()), GetWidth());
    }
  } 
  else
  { 
    PLBYTE ** ppLine = Bmp.GetLineArray();
    for (int z = 0; z < m_Header.ZSize; z++)
    {
      for (int y = GetHeight()-1; y >=0 ; y--) 
      {
        PLBYTE * pLine = ppLine[y];
        for (int x = 0; x < GetWidth(); x++)
        {
          switch (z)
          {
            case 0:
              pLine[x*4+PL_RGBA_RED] = ReadByte (m_pDataSrc);
              break;
            case 1:
              pLine[x*4+PL_RGBA_GREEN] = ReadByte (m_pDataSrc);
              break;
            case 2:
              pLine[x*4+PL_RGBA_BLUE] = ReadByte (m_pDataSrc);
              break;
            case 3:
              pLine[x*4+PL_RGBA_ALPHA] = ReadByte (m_pDataSrc);
              break;
          }
        }
      }
    }
  }
}


void PLSGIDecoder::readHeader
    ( SGIHeader * pHeader,
      PLDataSource * pDataSrc
    )
{
    pHeader->Magic     = ReadMWord (pDataSrc);
    pHeader->Storage   = ReadByte (pDataSrc);
    pHeader->bpc       = ReadByte (pDataSrc);
    pHeader->Dimension = ReadMWord (pDataSrc);
    pHeader->XSize     = ReadMWord (pDataSrc);
    pHeader->YSize     = ReadMWord (pDataSrc);
    pHeader->ZSize     = ReadMWord (pDataSrc);
    pHeader->PixMin    = ReadMLong (pDataSrc);
    pHeader->PixMax    = ReadMLong (pDataSrc);
    ReadMLong(pDataSrc);
    memcpy (pHeader->Name, (char *)pDataSrc->ReadNBytes(80), 80);
    pHeader->Colormap  = ReadMLong (pDataSrc);
    pDataSrc->Skip(404);

    if (pHeader->Magic != SGI_MAGIC)
    {
      throw PLTextException (PL_ERRWRONG_SIGNATURE, "SGI decoder invoked for non-SGI file.");
    }
}


/*
/--------------------------------------------------------------------
|
|      $Log: /Project/Springhead/bin/src/loadBmp/common/plsgidec.cpp $
 * 
 * 1     04/07/12 13:34 Hase
|      Revision 1.1  2003/06/01 21:12:06  uzadow
|      Added sgi rgb file format.
|
|
|
\--------------------------------------------------------------------
*/
