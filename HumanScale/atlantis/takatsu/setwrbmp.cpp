/********************************************/
/*	setwrbmp.c                              */
/*	DIBファイルの読み込みと書き出しの関数群	*/
/********************************************/
#include <stdio.h>
#include <stdlib.h>

#include "kmenu.h"

/*******************************************************/
/*  Headerの構造体の宣言  */
BMPFileHeader	bf[25]; /*10->25(12.5)*/
BMPInfoHeader	bi[25]; /*10->25(12.5)*/

/*　構造体RGBQuadへのポインタ変数の宣言　*/
RGBQuad			*aColors[25]; /*10->25(12.5)*/

/*　構造体BMPへのポインタ変数の宣言　*/
BMP	*images[25]; /*10->25(12.5)*/
           
/*  ファイルへの書き出しをしてもいいかを与えるフラッグ用の変数の宣言  */
unsigned int	wokflg[25]; /*10->25(12.5)*/

/*	ヒストグラム用配列の宣言	*/
unsigned int	hist[25][256]; /*10->25(12.5)*/

/******************************************************/

/* ファイルから８ビット読み込むための関数 */
unsigned char ReadByte( FILE *fp )
{
	return fgetc( fp );
}


/* ファイルから１６ビット読み込むための関数 */
unsigned int ReadWord( FILE *fp )
{
	unsigned int	temp;

	temp = ReadByte( fp );
	return temp |= (ReadByte( fp ) << 8 );
}




/* ファイルから32ビット読み込むための関数 */
unsigned long ReadDword( FILE *fp )
{
	unsigned int	temp;

	temp = ReadWord( fp );
	return temp |= ( ReadWord( fp ) << 16 );
}



/********************************************************/
/*	ヘッダーデータの読み込み							*/
/********************************************************/

/* BMPファイルからHeaderの内容を読んで、変数に代入する関数 */
void BmpHead( int m, FILE *fp )
{
	/*  BITMAPFILEHEADER の部分　*/
	bf[m].Type		= ReadWord( fp );  /*  ファイルのタイプ		*/
	bf[m].Size		= ReadDword( fp ); /*  ファイルのサイズ		*/
	bf[m].Reserved1		= ReadWord( fp );  /*  予約（必ず０）		*/
	bf[m].Reserved2		= ReadWord( fp );  /*  予約（必ず０）		*/
	bf[m].OffBits		= ReadDword( fp ); /*  データまでのオフセット	*/


	/*  BITMAPINFOHEADER の部分  */
	bi[m].Size		= ReadDword( fp ); /*  構造体のバイト数		*/
	bi[m].Width		= ReadDword( fp ); /*  ビットマップの幅		*/
	bi[m].Height		= ReadDword( fp ); /*  ビットマップの高さ	*/
	bi[m].Planes		= ReadWord( fp );  /*  プレーン数（必ず１）	*/
	bi[m].BitCount		= ReadWord( fp );  /*  ピクセル当たりのビット数	*/
	bi[m].Compression	= ReadDword( fp ); /*  圧縮タイプ		*/
	bi[m].SizeImage		= ReadDword( fp ); /*  イメージのサイズ		*/
	bi[m].XPelsPerMeter	= ReadDword( fp ); /*  水平解像度		*/
	bi[m].YPelsPerMeter	= ReadDword( fp ); /*  垂直解像度		*/
	bi[m].ClrUsed		= ReadDword( fp ); /*  カラーインデックスの個数	*/
	bi[m].ClrImportant	= ReadDword( fp ); /*  カラーインデックスの個数	*/
}



/********************************************************/
/*	カラーパレットデータの読み込み						*/
/********************************************************/

/* BMPファイルからカラーパレット情報をメモリに読み込む関数 */
void GetColor( int m, FILE *fp, int num )
{
	int	i;

	if( ( aColors[m] =  (RGBQuad *)calloc( num * 3, sizeof( unsigned char ) ) ) == NULL ) {
		printf("メモリの取得に失敗しました\n");
		exit(1);
	}

	for( i = 0 ; i < num ; i++) {
		aColors[m][i].Blue	= ReadByte( fp );  /* rgbBlue	*/
		aColors[m][i].Green	= ReadByte( fp );  /* rgbGreen	*/
		aColors[m][i].Red	= ReadByte( fp );  /* rgbRed	*/
				  	  ReadByte( fp );  /* rgbReserved*/
	}
}




/* カラーパレット情報の大きさを得る関数 */
int ColorTable( int m, FILE *fp )
{
	int	num, count;

	count = bi[m].BitCount;

	if( bi[m].ClrUsed == 0 ) {
		switch( count ) {
			case 1:	num =	2	; GetColor( m, fp, num );	break;
			case 4:	num =	16	; GetColor( m, fp, num );	break;
			case 8:	num =	256	; GetColor( m, fp, num );	break;
			case 24: num =	0;					break;

			default: printf("エラー　bi[%d].BitCount = %04X\n\n", m, bi[m].BitCount ); exit(1);
		}
	}
	else {
		num = bi[m].ClrUsed;
		GetColor( m, fp, num );
	}

	return num;
}
/****************************************************/
/*	画像用のメモリを確保する関数					*/
/*													*/
/*	getmem(int ma, int width, int height)			*/
/*	ma:画像バッファ番号								*/
/*	width:画像の横の長さ							*/
/*	height:画像の縦の長さ							*/
/****************************************************/
int getmem( int ma, long width, long height )
{
	long	memnum;

	memnum = width * height * 3;
	printf("画像の横の長さ = %ld, 画像の縦の長さ = %ld,\n",width, height); 

	/* メモリの確保 */
	if( ( images[ma] = (tagBMP *)calloc( memnum, sizeof(char) ) ) == NULL ) {
		return(0);
	}
	return(1);
}

/****************************************************/
/*	画像データの読み出し							*/
/****************************************************/

/* モノクロ画像の場合の、データの読み出しのための関数 */
void BitMap0( int m, FILE *fp )
{
	unsigned int temp;
	int i, j, k, a, b, c, bit;

	for( i=0 ; i<bi[m].Height ; i++ ) {
		a = ( (int)(bi[m].Width / 32.) + 1 ) * 4;
		if( (a * 8) == ((int)bi[m].Width + 32) )
			a -= 4;
		b = (int)(bi[m].Width / 8.);
		c = (int)bi[m].Width - b * 8;

		for( j=0 ; j < a ; j++ ) {
			temp = ReadByte( fp );
			if( j < b ) {
				for( k=0 ; k<8 ; k++ ) {

/*					if(temp & 0x80)
						bit = 1;
					else
*/						bit = 0;

					images[m][ i*bi[m].Width+(j * 8 + k) ].Blue  = aColors[m][ bit ].Blue;
					images[m][ i*bi[m].Width+(j * 8 + k) ].Green = aColors[m][ bit ].Green;
					images[m][ i*bi[m].Width+(j * 8 + k) ].Red   = aColors[m][ bit ].Red;
					temp <<= 1;
				}
			}
			else if( j == b ) {
				for( k=0 ; k<c ; k++ ) {

/*					if( temp & 0x80 )
						bit = 1;
					else
*/						bit = 0;

					images[m][ i*bi[m].Width+(j * 8 + k) ].Blue  = aColors[m][ bit ].Blue;
					images[m][ i*bi[m].Width+(j * 8 + k) ].Green = aColors[m][ bit ].Green;
					images[m][ i*bi[m].Width+(j * 8 + k) ].Red   = aColors[m][ bit ].Red;
					temp <<= 1;
				}
			}
		}
	}
}

/* 16色の場合の、データの読み出しのための関数 */
void BitMap4( int m, FILE *fp )
{
	unsigned int temp;
	int i, j, a, b, c;

	for( i=0 ; i<bi[m].Height ; i++ ) {
		a = ( (int)(bi[m].Width / 8.) + 1 ) * 4;
		if( (a*2) == (int)bi[m].Width + 8)
			a -= 4;
		b = (int)(bi[m].Width / 2.);
		c = (int)bi[m].Width - b * 2;

		for( j=0 ; j<a ; j++ ) {
			temp = ReadByte( fp );
			if( j < b) {
				images[m][ i*bi[m].Width+ j*2 ].Blue	= aColors[m][ temp >> 4 ].Blue;
				images[m][ i*bi[m].Width+ j*2 ].Green	= aColors[m][ temp >> 4 ].Green;
				images[m][ i*bi[m].Width+ j*2 ].Red	= aColors[m][ temp >> 4 ].Red;
				images[m][ i*bi[m].Width+ j*2 +1 ].Blue	= aColors[m][ temp & 0x0f ].Blue;
				images[m][ i*bi[m].Width+ j*2 +1 ].Green= aColors[m][ temp & 0x0f ].Green;
				images[m][ i*bi[m].Width+ j*2 +1 ].Red	= aColors[m][ temp & 0x0f ].Red;
			}
			else if( (j == b) && (c == 1) ) {
				images[m][ i*bi[m].Width+ j*2 ].Blue	= aColors[m][ temp >> 4 ].Blue;
				images[m][ i*bi[m].Width+ j*2 ].Green	= aColors[m][ temp >> 4 ].Green;
				images[m][ i*bi[m].Width+ j*2 ].Red	= aColors[m][ temp >> 4 ].Red;
			}
		}
	}
}

/* 256色の場合の、データの読み出しのための関数 */
void BitMap8( int m, FILE *fp )
{
	int i, j, a, b, temp;

	for( i=0 ; i<bi[m].Height ; i++ ) {
		a = ( (int)(bi[m].Width / 4.) + 1 ) * 4;
		if( a == (int)bi[m].Width + 4)
			a -= 4;
		b = (int)bi[m].Width;

		for( j=0 ; j<a ; j++ ) {
			temp = (int)ReadByte( fp );
			if( j < b) {
				images[m][ i*bi[m].Width+j ].Blue	= aColors[m][ temp ].Red; //Blue
				images[m][ i*bi[m].Width+j ].Green	= aColors[m][ temp ].Green;
				images[m][ i*bi[m].Width+j ].Red	= aColors[m][ temp ].Blue; //Red
			}
		}
	}
}

/* フルカラーの場合の、データの読み出しのための関数 */

void BitMap24( int m, FILE *fp )
{
	int i, j, a, b, d, e, temp;


	for( i = 0; i < (int)bi[m].Height; i++ ) {
		a = ( (int)(bi[m].Width * 3 / 4.) + 1 ) * 4;
		if( a == (int)bi[m].Width * 3 + 4)
			a -= 4;
		b = (int)bi[m].Width * 3;
		d = 1;

		for( j = 0; j < a; j++ ) {
			temp = (int)ReadByte(fp);
			if( j < b ) {
				e = (int)(j / 3.);
				switch(d){
				case 1:	/* 青の情報の読み込み */
					images[m][ i*bi[m].Width+e ].Red	= temp; //Blue
					d = 2;
					break;
				case 2:	/* 緑の情報の読み込み */
					images[m][ i*bi[m].Width+e ].Green	= temp;
					d = 3;
					break;
				case 3:	/* 赤の情報の読み込み */
					images[m][ i*bi[m].Width+e ].Blue	= temp; //Red
					d = 1;
				}
			}
		}
	}
}

/* bmpファイルの使っている色数に応じて、読み出す関数を選択するための関数 */
void BitMapData( int m, FILE *fp, int count ){
	switch( count ){
	case 1:	/* モノクロ */
		BitMap0( m, fp );
		break;
	case 4:	/* 16色 */
		BitMap4( m, fp );
		break;
	case 8:	/* 256色 */
		BitMap8( m, fp );
		break;
	case 24:/* フルカラー */
		BitMap24( m, fp );
		break;
	}
}

/********************************************************/
/*	BMPファイルのデータをメモリに読み込む関数			*/
/*														*/
/*	imgread(int m, char fname[])						*/
/*	m:画像を読み込むバッファの番号						*/
/*	fname[]:ファイル名									*/
/*														*/
/*	注意												*/
/*	　圧縮のかかっている画像は扱えない。				*/
/*														*/
/********************************************************/
void imgread( int m, char fname[] )
{
	FILE	*fp;
	int	num;
	/*  与えられたファイルがないとき 警告を発して終了する  */
	if( (fp = fopen( fname, "rb" ) ) == NULL ) {
		printf("\n(%s)のファイルが存在しません!\n\n", fname );
		return;
	}
	/*  Headerを読んで 値を構造体に代入する */
	BmpHead( m, fp );

	/* 圧縮されていた場合、取り扱いをやめて終了する */
	if( bi[m].Compression != 0 ) {
		printf("このファイルは圧縮されています。取り扱えません\n\n");
		return;
	}

	/*  カラーテーブルを読む  */
	num = ColorTable( m, fp );

	/*  画像データ用メモリの確保  */
	if( getmem(m, bi[m].Width, bi[m].Height) == 0) {
		printf("画像データ用メモリの確保に失敗しました\n");
		return;
	}
	/* 各ピクセルのデータを読む -->  *images[ ] へ  */
	BitMapData( m, fp, bi[m].BitCount);

	printf("ファイル %s をバッファ%dに読み込みました\n\n", fname, m);
	fclose( fp );

	/* データのあるなしを表す変数にあることを示す */
	wokflg[m] = 1;
}

/************************************************/
/*	bitmapファイルへメモリ上の画像データを		*/
/*	セーブするための関数群						*/
/************************************************/

/*  値が固定でないものにたいして、配列を求める関数  */
void getnums( long n, char buff[] )
{
	char	st1[10], st2[5];

	sprintf(st1, "%08lX", n);

	st2[0] = st1[6];
	st2[1] = st1[7];
	st2[2] = 0;
	sscanf(st2, "%x", &buff[0]);

	st2[0] = st1[4];
	st2[1] = st1[5];
	st2[2] = 0;
	sscanf(st2, "%x", &buff[1]);

	st2[0] = st1[2];
	st2[1] = st1[3];
	st2[2] = 0;
	sscanf(st2, "%x", &buff[2]);

	st2[0] = st1[0];
	st2[1] = st1[1];
	st2[2] = 0;
	sscanf(st2, "%x", &buff[3]);

} 

/*  Header の情報を８ビット単位のファイルへの書き込む順にならべる関数 */
void gethead( int m, char head[] )
{
	char	bufffs[5], buffiw[5], buffih[5];
	long	imgsize, a;

	/*  値が固定でないものにたいして、配列を求める  */
	a = ( (int)(bi[m].Width * 3 / 4.) + 1 ) * 4;
	if( a == ((bi[m].Width * 3 / 4.) + 1 ) * 4)
		a -= 4;
	imgsize = a * bi[m].Height * 3 + 54;
	getnums( imgsize, bufffs );
	getnums( bi[m].Width, buffiw );
	getnums( bi[m].Height, buffih );



	/*  配列に値を代入  */
	head[0] = 'B';	head[1] = 'M';					/* Type		*/
	head[2]=bufffs[0]; head[3]=bufffs[1]; head[4]=bufffs[2]; head[5]=bufffs[3];	/* Size		*/
	head[6] = 0;	head[7] = 0;					/* REserved1	*/


	head[8] = 0;	head[9] = 0;					/* Reserved2	*/
	head[10] = 0x36;head[11] = 0;	head[12] = 0;	head[13] = 0;	/* OffBits	*/
	head[14] = 0x28;head[15] = 0;	head[16] = 0;	head[17] = 0;	/* Size		*/
	head[18]=buffiw[0]; head[19]=buffiw[1]; head[20]=buffiw[2]; head[21]=buffiw[3];	/* Width	*/
	head[22]=buffih[0]; head[23]=buffih[1]; head[24]=buffih[2]; head[25]=buffih[3];	/* Height	*/
	head[26] = 0x01;head[27] = 0;					/* Planes	*/
	head[28] = 0x18;head[29] = 0;					/* BitCount	*/
	head[30] = 0;	head[31] = 0;	head[32] = 0;	head[33] = 0;	/* Compression	*/
	head[34] = 0;	head[35] = 0;	head[36] = 0;	head[37] = 0;	/* SizeImage	*/
	head[38] = 0;	head[39] = 0;	head[40] = 0;	head[41] = 0;	/* XPelsPerMeter*/
	head[42] = 0;	head[43] = 0;	head[44] = 0;	head[45] = 0;	/* YPelsPerMeter*/
	head[46] = 0;	head[47] = 0;	head[48] = 0;	head[49] = 0;	/* ClrUsed	*/
	head[50] = 0;	head[51] = 0;	head[52] = 0;	head[53] = 0;	/* ClrImportant	*/

}
