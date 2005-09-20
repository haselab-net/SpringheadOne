///////////////////////////////////////////////////////////////////////////////
//
//	Bitmap�t�@�C���������݃w�b�_
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <gl/gl.h>

///////////////////////////////////////////////////////////////////////////////
//Bitmap�t�@�C��
//BitmapHeader�\����
typedef struct _BitmapHeader{
	char	distinct1;
	char	distinct2;
	int		filesize;
	short	reserve1;
	short	reserve2;
	int		offset;
}BitmapHeader;
//BitmapInfoHeader�\����
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
//�s�N�Z���f�[�^���r�b�g�}�b�v�ɏ�������
int WriteBitmap(const char*, GLubyte*, int width, int height);
///////////////////////////////////////////////////////////////////////////////
//�r�b�g�}�b�v�t�@�C������s�N�Z���f�[�^��Ԃ��D
unsigned char* ReadBitmap(const char*);
///////////////////////////////////////////////////////////////////////////////
//�r�b�g�}�b�v�t�@�C������e�N�X�`���ԍ���Ԃ��D
unsigned int GenTextureBitmap(const char*filename);
