#ifndef _KMERU_H
#define _KMERU_H
/********************************************************/
/*	bitmap�t�@�C�����������߂̒�`��֐��Ȃǂ��W�߂�	*/
/*		�C���N���[�h�t�@�C��							*/
/*	kmenu.h												*/
/*														*/
/********************************************************/
#ifdef _cplusplus
extern "C"
{
#endif

#define	LEVEL	256	/* �Z�x�̃��x���̐� */
#define	STDOUT	stdout
#define	LOW	0	/* �Q�l�摜�̍� */
#define	HIGH	255	/* �Q�l�摜�̔� */
#define PI	3.14159265	/* �� */

/********************************************************/
/*	�e��ϐ��̐錾										*/
/********************************************************/

/* Header��ǂ�ŋL�������Ă������ߍ\���̂̒�` */
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


/* �e�J���[�p���b�g��ǂނ��߂̍\���̂̒�` */
typedef struct tagRGBQuad
{
	unsigned char	Blue;
	unsigned char	Green;
	unsigned char	Red;
}RGBQuad;


/* �e�s�N�Z���̃f�[�^�����邽�߂̍\���̂̒�` */
typedef struct tagBMP
{
        unsigned char   Blue;
        unsigned char   Green;
        unsigned char   Red;
}BMP;

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


/************************/
/*      �֐��̐錾      */
/************************/

/************************************/
/* setwebmp.c�Œ�`����֐�			*/
/************************************/
extern unsigned char ReadByte( FILE *fp );
extern int getmem( int ma, long width, long height );
extern void imgread( int m, char fname[] );

#ifdef _cplusplus
}
#endif

#endif



 
   



