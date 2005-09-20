/********************************************/
/*	setwrbmp.c                              */
/*	DIB�t�@�C���̓ǂݍ��݂Ə����o���̊֐��Q	*/
/********************************************/
#include <stdio.h>
#include <stdlib.h>

#include "kmenu.h"

/*******************************************************/
/*  Header�̍\���̂̐錾  */
BMPFileHeader	bf[25]; /*10->25(12.5)*/
BMPInfoHeader	bi[25]; /*10->25(12.5)*/

/*�@�\����RGBQuad�ւ̃|�C���^�ϐ��̐錾�@*/
RGBQuad			*aColors[25]; /*10->25(12.5)*/

/*�@�\����BMP�ւ̃|�C���^�ϐ��̐錾�@*/
BMP	*images[25]; /*10->25(12.5)*/
           
/*  �t�@�C���ւ̏����o�������Ă���������^����t���b�O�p�̕ϐ��̐錾  */
unsigned int	wokflg[25]; /*10->25(12.5)*/

/*	�q�X�g�O�����p�z��̐錾	*/
unsigned int	hist[25][256]; /*10->25(12.5)*/

/******************************************************/

/* �t�@�C������W�r�b�g�ǂݍ��ނ��߂̊֐� */
unsigned char ReadByte( FILE *fp )
{
	return fgetc( fp );
}


/* �t�@�C������P�U�r�b�g�ǂݍ��ނ��߂̊֐� */
unsigned int ReadWord( FILE *fp )
{
	unsigned int	temp;

	temp = ReadByte( fp );
	return temp |= (ReadByte( fp ) << 8 );
}




/* �t�@�C������32�r�b�g�ǂݍ��ނ��߂̊֐� */
unsigned long ReadDword( FILE *fp )
{
	unsigned int	temp;

	temp = ReadWord( fp );
	return temp |= ( ReadWord( fp ) << 16 );
}



/********************************************************/
/*	�w�b�_�[�f�[�^�̓ǂݍ���							*/
/********************************************************/

/* BMP�t�@�C������Header�̓��e��ǂ�ŁA�ϐ��ɑ������֐� */
void BmpHead( int m, FILE *fp )
{
	/*  BITMAPFILEHEADER �̕����@*/
	bf[m].Type		= ReadWord( fp );  /*  �t�@�C���̃^�C�v		*/
	bf[m].Size		= ReadDword( fp ); /*  �t�@�C���̃T�C�Y		*/
	bf[m].Reserved1		= ReadWord( fp );  /*  �\��i�K���O�j		*/
	bf[m].Reserved2		= ReadWord( fp );  /*  �\��i�K���O�j		*/
	bf[m].OffBits		= ReadDword( fp ); /*  �f�[�^�܂ł̃I�t�Z�b�g	*/


	/*  BITMAPINFOHEADER �̕���  */
	bi[m].Size		= ReadDword( fp ); /*  �\���̂̃o�C�g��		*/
	bi[m].Width		= ReadDword( fp ); /*  �r�b�g�}�b�v�̕�		*/
	bi[m].Height		= ReadDword( fp ); /*  �r�b�g�}�b�v�̍���	*/
	bi[m].Planes		= ReadWord( fp );  /*  �v���[�����i�K���P�j	*/
	bi[m].BitCount		= ReadWord( fp );  /*  �s�N�Z��������̃r�b�g��	*/
	bi[m].Compression	= ReadDword( fp ); /*  ���k�^�C�v		*/
	bi[m].SizeImage		= ReadDword( fp ); /*  �C���[�W�̃T�C�Y		*/
	bi[m].XPelsPerMeter	= ReadDword( fp ); /*  �����𑜓x		*/
	bi[m].YPelsPerMeter	= ReadDword( fp ); /*  �����𑜓x		*/
	bi[m].ClrUsed		= ReadDword( fp ); /*  �J���[�C���f�b�N�X�̌�	*/
	bi[m].ClrImportant	= ReadDword( fp ); /*  �J���[�C���f�b�N�X�̌�	*/
}



/********************************************************/
/*	�J���[�p���b�g�f�[�^�̓ǂݍ���						*/
/********************************************************/

/* BMP�t�@�C������J���[�p���b�g�����������ɓǂݍ��ފ֐� */
void GetColor( int m, FILE *fp, int num )
{
	int	i;

	if( ( aColors[m] =  (RGBQuad *)calloc( num * 3, sizeof( unsigned char ) ) ) == NULL ) {
		printf("�������̎擾�Ɏ��s���܂���\n");
		exit(1);
	}

	for( i = 0 ; i < num ; i++) {
		aColors[m][i].Blue	= ReadByte( fp );  /* rgbBlue	*/
		aColors[m][i].Green	= ReadByte( fp );  /* rgbGreen	*/
		aColors[m][i].Red	= ReadByte( fp );  /* rgbRed	*/
				  	  ReadByte( fp );  /* rgbReserved*/
	}
}




/* �J���[�p���b�g���̑傫���𓾂�֐� */
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

			default: printf("�G���[�@bi[%d].BitCount = %04X\n\n", m, bi[m].BitCount ); exit(1);
		}
	}
	else {
		num = bi[m].ClrUsed;
		GetColor( m, fp, num );
	}

	return num;
}
/****************************************************/
/*	�摜�p�̃��������m�ۂ���֐�					*/
/*													*/
/*	getmem(int ma, int width, int height)			*/
/*	ma:�摜�o�b�t�@�ԍ�								*/
/*	width:�摜�̉��̒���							*/
/*	height:�摜�̏c�̒���							*/
/****************************************************/
int getmem( int ma, long width, long height )
{
	long	memnum;

	memnum = width * height * 3;
	printf("�摜�̉��̒��� = %ld, �摜�̏c�̒��� = %ld,\n",width, height); 

	/* �������̊m�� */
	if( ( images[ma] = (tagBMP *)calloc( memnum, sizeof(char) ) ) == NULL ) {
		return(0);
	}
	return(1);
}

/****************************************************/
/*	�摜�f�[�^�̓ǂݏo��							*/
/****************************************************/

/* ���m�N���摜�̏ꍇ�́A�f�[�^�̓ǂݏo���̂��߂̊֐� */
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

/* 16�F�̏ꍇ�́A�f�[�^�̓ǂݏo���̂��߂̊֐� */
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

/* 256�F�̏ꍇ�́A�f�[�^�̓ǂݏo���̂��߂̊֐� */
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

/* �t���J���[�̏ꍇ�́A�f�[�^�̓ǂݏo���̂��߂̊֐� */

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
				case 1:	/* �̏��̓ǂݍ��� */
					images[m][ i*bi[m].Width+e ].Red	= temp; //Blue
					d = 2;
					break;
				case 2:	/* �΂̏��̓ǂݍ��� */
					images[m][ i*bi[m].Width+e ].Green	= temp;
					d = 3;
					break;
				case 3:	/* �Ԃ̏��̓ǂݍ��� */
					images[m][ i*bi[m].Width+e ].Blue	= temp; //Red
					d = 1;
				}
			}
		}
	}
}

/* bmp�t�@�C���̎g���Ă���F���ɉ����āA�ǂݏo���֐���I�����邽�߂̊֐� */
void BitMapData( int m, FILE *fp, int count ){
	switch( count ){
	case 1:	/* ���m�N�� */
		BitMap0( m, fp );
		break;
	case 4:	/* 16�F */
		BitMap4( m, fp );
		break;
	case 8:	/* 256�F */
		BitMap8( m, fp );
		break;
	case 24:/* �t���J���[ */
		BitMap24( m, fp );
		break;
	}
}

/********************************************************/
/*	BMP�t�@�C���̃f�[�^���������ɓǂݍ��ފ֐�			*/
/*														*/
/*	imgread(int m, char fname[])						*/
/*	m:�摜��ǂݍ��ރo�b�t�@�̔ԍ�						*/
/*	fname[]:�t�@�C����									*/
/*														*/
/*	����												*/
/*	�@���k�̂������Ă���摜�͈����Ȃ��B				*/
/*														*/
/********************************************************/
void imgread( int m, char fname[] )
{
	FILE	*fp;
	int	num;
	/*  �^����ꂽ�t�@�C�����Ȃ��Ƃ� �x���𔭂��ďI������  */
	if( (fp = fopen( fname, "rb" ) ) == NULL ) {
		printf("\n(%s)�̃t�@�C�������݂��܂���!\n\n", fname );
		return;
	}
	/*  Header��ǂ�� �l���\���̂ɑ������ */
	BmpHead( m, fp );

	/* ���k����Ă����ꍇ�A��舵������߂ďI������ */
	if( bi[m].Compression != 0 ) {
		printf("���̃t�@�C���͈��k����Ă��܂��B��舵���܂���\n\n");
		return;
	}

	/*  �J���[�e�[�u����ǂ�  */
	num = ColorTable( m, fp );

	/*  �摜�f�[�^�p�������̊m��  */
	if( getmem(m, bi[m].Width, bi[m].Height) == 0) {
		printf("�摜�f�[�^�p�������̊m�ۂɎ��s���܂���\n");
		return;
	}
	/* �e�s�N�Z���̃f�[�^��ǂ� -->  *images[ ] ��  */
	BitMapData( m, fp, bi[m].BitCount);

	printf("�t�@�C�� %s ���o�b�t�@%d�ɓǂݍ��݂܂���\n\n", fname, m);
	fclose( fp );

	/* �f�[�^�̂���Ȃ���\���ϐ��ɂ��邱�Ƃ����� */
	wokflg[m] = 1;
}

/************************************************/
/*	bitmap�t�@�C���փ�������̉摜�f�[�^��		*/
/*	�Z�[�u���邽�߂̊֐��Q						*/
/************************************************/

/*  �l���Œ�łȂ����̂ɂ������āA�z������߂�֐�  */
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

/*  Header �̏����W�r�b�g�P�ʂ̃t�@�C���ւ̏������ޏ��ɂȂ�ׂ�֐� */
void gethead( int m, char head[] )
{
	char	bufffs[5], buffiw[5], buffih[5];
	long	imgsize, a;

	/*  �l���Œ�łȂ����̂ɂ������āA�z������߂�  */
	a = ( (int)(bi[m].Width * 3 / 4.) + 1 ) * 4;
	if( a == ((bi[m].Width * 3 / 4.) + 1 ) * 4)
		a -= 4;
	imgsize = a * bi[m].Height * 3 + 54;
	getnums( imgsize, bufffs );
	getnums( bi[m].Width, buffiw );
	getnums( bi[m].Height, buffih );



	/*  �z��ɒl����  */
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
