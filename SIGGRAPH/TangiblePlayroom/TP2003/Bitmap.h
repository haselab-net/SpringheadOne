///////////////////////////////////////////////////////////////////////////////
//
//	Bitmapファイル書き込みヘッダ
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <gl/gl.h>

///////////////////////////////////////////////////////////////////////////////
//Bitmapファイル
//BitmapHeader構造体
typedef struct _BitmapHeader{
	char	distinct1;
	char	distinct2;
	int		filesize;
	short	reserve1;
	short	reserve2;
	int		offset;
}BitmapHeader;
//BitmapInfoHeader構造体
typedef struct _BitmapInfoHeader{
	int		header;
	int		width;
	int		height;
	short	plane;
	short	bits;
	int		compression;
	int		comp_image_size;
	int		x_resolution;
	int		y_resolution;
	int		pallet_num;
	int		important_pallet_num;
}BitmapInfoHeader;

///////////////////////////////////////////////////////////////////////////////
//ピクセルデータをビットマップに書き込む
int WriteBitmap(const char*, GLubyte*, int width, int height);
///////////////////////////////////////////////////////////////////////////////
//ビットマップファイルからピクセルデータを返す．
unsigned char* ReadBitmap(const char*);
///////////////////////////////////////////////////////////////////////////////
//ビットマップファイルからテクスチャ番号を返す．
unsigned int GenTextureBitmap(const char*filename);
