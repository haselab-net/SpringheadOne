#ifndef _KMERU_H
#define _KMERU_H
/********************************************************/
/*	bitmapファイルを扱うための定義や関数などを集めた	*/
/*		インクルードファイル							*/
/*	kmenu.h												*/
/*														*/
/********************************************************/
#ifdef _cplusplus
extern "C"
{
#endif

#define	LEVEL	256	/* 濃度のレベルの数 */
#define	STDOUT	stdout
#define	LOW	0	/* ２値画像の黒 */
#define	HIGH	255	/* ２値画像の白 */
#define PI	3.14159265	/* π */

/********************************************************/
/*	各種変数の宣言										*/
/********************************************************/

/* Headerを読んで記憶させておくため構造体の定義 */
typedef struct tagBMPHeader
{
	int	Type;
	long	Size;
	int	Reserved1;
	int	Reserved2;
	long	OffBits;
}BMPFileHeader;


typedef struct tagBMPInfoHeader
{
	long	Size;
	long	Width;
	long	Height;
	int	Planes;
	int	BitCount;
	long	Compression;
	long	SizeImage;
	long	XPelsPerMeter;
	long	YPelsPerMeter;
	long	ClrUsed;
	long	ClrImportant;
}BMPInfoHeader;


/* 各カラーパレットを読むための構造体の定義 */
typedef struct tagRGBQuad
{
	unsigned char	Blue;
	unsigned char	Green;
	unsigned char	Red;
}RGBQuad;


/* 各ピクセルのデータを入れるための構造体の定義 */
typedef struct tagBMP
{
        unsigned char   Blue;
        unsigned char   Green;
        unsigned char   Red;
}BMP;






/************************/
/*      関数の宣言      */
/************************/

/************************************/
/* setwebmp.cで定義する関数			*/
/************************************/
extern unsigned char ReadByte( FILE *fp );
extern int getmem( int ma, long width, long height );
extern void imgread( int m, char fname[] );

/*  Headerの構造体の宣言  */
extern BMPFileHeader	bf[25]; /*10->25(12.5)*/
extern BMPInfoHeader	bi[25]; /*10->25(12.5)*/

/*　構造体RGBQuadへのポインタ変数の宣言　*/
extern RGBQuad			*aColors[25]; /*10->25(12.5)*/

/*　構造体BMPへのポインタ変数の宣言　*/
extern BMP	*images[25]; /*10->25(12.5)*/        

/*  ファイルへの書き出しをしてもいいかを与えるフラッグ用の変数の宣言  */
extern unsigned int	wokflg[25]; /*10->25(12.5)*/

/*	ヒストグラム用配列の宣言	*/
extern unsigned int	hist[25][256]; /*10->25(12.5)*/



#ifdef _cplusplus
}
#endif

#endif



 
   



