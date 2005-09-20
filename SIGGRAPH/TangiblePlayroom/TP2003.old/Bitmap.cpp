///////////////////////////////////////////////////////////////////////////////
//
//	Bitmap�t�@�C����������&�ǂݍ���
//
///////////////////////////////////////////////////////////////////////////////

#define TEX_SIZE 256

#include "Bitmap.h"


///////////////////////////////////////////////////////////////////////////////
//�r�b�g�}�b�v�t�@�C���̃T�C�Y�`�F�b�N
int CheckSize(unsigned int pict_size){
	unsigned int i=2;
	unsigned int max_texture = TEX_SIZE*TEX_SIZE;
	//�T�C�Y�`�F�b�N
	for(i=2;i<=max_texture;i*=2){
		if( i == pict_size){
			return 1;
		}
	}
	return 0;
};
///////////////////////////////////////////////////////////////////////////////
//Bitmap�\���̏�����
void InitHeaders(BitmapHeader *header,BitmapInfoHeader *info){
	header->distinct1 = 'B';
	header->distinct2 = 'M';
	header->filesize = 0;
	header->reserve1 = 0;
	header->reserve2 = 0;
	header->offset = 54;

	info->header = 40;
	info->width = 0;
	info->height = 0;
	info->plane = 1;
	info->bits = 24;
	info->compression = 0;
	info->comp_image_size = 0;
	info->x_resolution = 0;
	info->y_resolution = 0;
	info->pallet_num = 0;
	info->important_pallet_num = 0;
};
///////////////////////////////////////////////////////////////////////////////
//Bitmap�w�b�_��������
void WriteHeader(BitmapHeader *header,FILE *fp){
	fwrite(&(header->distinct1), sizeof(char),1,fp);
	fwrite(&(header->distinct2), sizeof(char),1,fp);
	fwrite(&(header->filesize), sizeof(int),1,fp);
	fwrite(&(header->reserve1), sizeof(short),1,fp);
	fwrite(&(header->reserve2), sizeof(short),1,fp);
	fwrite(&(header->offset), sizeof(int),1,fp);
}
///////////////////////////////////////////////////////////////////////////////
//BitmapInfo�w�b�_��������
void WriteInfoHeader(BitmapInfoHeader *info,FILE *fp){
	fwrite(&(info->header), sizeof(int),1,fp);
	fwrite(&(info->width), sizeof(int),1,fp);
	fwrite(&(info->height), sizeof(int),1,fp);
	fwrite(&(info->plane), sizeof(short),1,fp);
	fwrite(&(info->bits), sizeof(short),1,fp);
	fwrite(&(info->compression), sizeof(int),1,fp);
	fwrite(&(info->comp_image_size), sizeof(int),1,fp);
	fwrite(&(info->x_resolution), sizeof(int),1,fp);
	fwrite(&(info->y_resolution), sizeof(int),1,fp);
	fwrite(&(info->pallet_num), sizeof(int),1,fp);
	fwrite(&(info->important_pallet_num), sizeof(int),1,fp);
}
///////////////////////////////////////////////////////////////////////////////
//Bitmap�w�b�_�ǂݍ���
int ReadHeader(BitmapHeader *header,FILE *fp){
	if( fread(&(header->distinct1), sizeof(char),1,fp)<1 )return 0;
	if( fread(&(header->distinct2), sizeof(char),1,fp)<1 )return 0;
	if( fread(&(header->filesize), sizeof(int),1,fp)<1   )return 0;
	if( fread(&(header->reserve1), sizeof(short),1,fp)<1 )return 0;
	if( fread(&(header->reserve2), sizeof(short),1,fp)<1 )return 0;
	if( fread(&(header->offset), sizeof(int),1,fp)<1     )return 0;
	return 1;
}
///////////////////////////////////////////////////////////////////////////////
//BitmapInfo�w�b�_�ǂݍ���
int ReadInfoHeader(BitmapInfoHeader *info,FILE *fp){
	if( fread(&(info->header), sizeof(int),1,fp)<1 )return 0;
	if( fread(&(info->width), sizeof(int),1,fp)<1 )return 0;
	if( fread(&(info->height), sizeof(int),1,fp)<1 )return 0;
	if( fread(&(info->plane), sizeof(short),1,fp)<1 )return 0;
	if( fread(&(info->bits), sizeof(short),1,fp)<1 )return 0;
	if( fread(&(info->compression), sizeof(int),1,fp)<1 )return 0;
	if( fread(&(info->comp_image_size), sizeof(int),1,fp)<1 )return 0;
	if( fread(&(info->x_resolution), sizeof(int),1,fp)<1 )return 0;
	if( fread(&(info->y_resolution), sizeof(int),1,fp)<1 )return 0;
	if( fread(&(info->pallet_num), sizeof(int),1,fp)<1 )return 0;
	if( fread(&(info->important_pallet_num), sizeof(int),1,fp)<1 )return 0;
	return 1;
}
///////////////////////////////////////////////////////////////////////////////
//Bitmap�w�b�_�`�F�b�N
int CheckHeaders(BitmapHeader *header,BitmapInfoHeader *info){
	//�w�b�_��BM�̕���
	if( header->distinct1!='B' || header->distinct2!='M' )return 0;
	//���C���[�P
	if( info->plane !=1 )return 0;
	//���k�Ȃ�
	if( info->compression !=0 ) return 0;
	//�F����Ԃ�
	if( info->bits ==24 || info->bits ==8 )return info->bits;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//�s�N�Z���f�[�^��Bitmap�t�@�C���֏�������
int WriteBitmap(const char* filename, GLubyte* data, int width, int height){
	FILE *fp;
	BitmapHeader header;
	BitmapInfoHeader info;
	int i=0;
	int j=0;
	int x;
	int y;
	// �t�@�C���I�[�v��
	if( ( fp = fopen(filename, "wb") )==NULL){
		return -1;
	}
	//�w�b�_�\���̂̏�����
	InitHeaders(&header, &info);
	//Bitmap�T�C�Y
	info.width = width;
	info.height = height;
	//�t�@�C���e��
	header.filesize = 
		width*height*3*sizeof(GLubyte)		//�r�b�g����
		+ 14								//BitmapHeader�\���̃T�C�Y
		+ 40;								//BitmapInfoHeader�\���̃T�C�Y
	//�w�b�_��������
	WriteHeader(&header,fp);
	WriteInfoHeader(&info,fp);
	// �C���[�W�f�[�^��������
	for( y=0 ; y < height ; y++ ){
		for( x=0 ; x < width ; x++ ){
			j=fwrite((data+x*3+3*y*width+2),sizeof(GLubyte),1,fp);
			j=fwrite((data+x*3+3*y*width+1),sizeof(GLubyte),1,fp);
			j=fwrite((data+x*3+3*y*width),sizeof(GLubyte),1,fp);
		}
	}
	// �t�@�C���N���[�Y
	fclose(fp);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//�r�b�g�}�b�v�t�@�C������s�N�Z���f�[�^��Ԃ��D
unsigned char* ReadBitmap(const char*filename){
	int i,j;
	FILE *fp;
	//�e�s�N�Z���̋P�x�f�[�^
	unsigned char* bitdata;
	//
	unsigned char* color_pallete_r;
	unsigned char* color_pallete_g;
	unsigned char* color_pallete_b;
	unsigned char* color_pallete_v;
	//�r�b�g�}�b�v�w�b�_
	BitmapHeader header;
	BitmapInfoHeader info;
	//�F��
	unsigned int color_bit;
	// �t�@�C���I�[�v��
	if( ( fp = fopen(filename, "rb") )==NULL){
		return NULL;
	}
	//�w�b�_�ǂݍ���
	if( !ReadHeader(&header,fp) ){
		fclose(fp);
		return NULL;
	}
	//�w�b�_�ǂݍ���
	if( !ReadInfoHeader(&info,fp) ){
		fclose(fp);
		return NULL;
	}
	//�w�b�_�`�F�b�N
	if( !(color_bit = CheckHeaders(&header,&info)) ){
		fclose(fp);
		return NULL;
	}
	//�r�b�g�}�b�v�t�@�C���T�C�Y�`�F�b�N
	if( !CheckSize(info.width*info.height) ){
		fclose(fp);
		return NULL;
	}
	//�J���[�r�b�g���ŏ����𕪂���
	if(color_bit==8)goto bit_8;
	if(color_bit==24)goto bit_24;
	else{
		fclose(fp);
		return NULL;
	}
/////////////////////////////////////////////////////////////////////
// 8bit(256�F)�r�b�g�}�b�v�̏���
bit_8:
	unsigned char pallete_num;
	//�J���[�p���b�g�ǂݍ���
	color_pallete_r = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_g = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_b = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_v = (unsigned char*)malloc(256*sizeof(unsigned char));
	for(i=0;i<256;i++){
		fread( (color_pallete_r+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_g+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_b+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_v+i), sizeof(unsigned char),1,fp);
	}
	//�r�b�g�f�[�^�ǂݍ���
	bitdata = (unsigned char*)malloc(3*info.width*info.height*sizeof(unsigned char));
	for(i=0;i<info.width;i++){
		for(j=0;j<info.height;j++){
			fread( &pallete_num, sizeof(unsigned char),1,fp);
			*(bitdata+i+j*info.width)  = *(color_pallete_b+pallete_num); 
			*(bitdata+i+j*info.width+1)= *(color_pallete_g+pallete_num); 
			*(bitdata+i+j*info.width+2)= *(color_pallete_b+pallete_num); 
		}
	}
	//�J���[�p���b�g�������J��
	free(color_pallete_r);
	free(color_pallete_g);
	free(color_pallete_b);
	free(color_pallete_v);
	free(bitdata);
	// �t�@�C���N���[�Y
	fclose(fp);
	return NULL;
/////////////////////////////////////////////////////////////////////
// 24bit(1677���F)�r�b�g�}�b�v�̏���
bit_24:
	unsigned char red,green,blue;
	//�r�b�g�f�[�^�ǂݍ���
	bitdata = (unsigned char*)malloc(3*info.width*info.height*sizeof(unsigned char));
	for(i=0;i<info.width;i++){
		for(j=0;j<info.height;j++){
			fread( &red, sizeof(unsigned char),1,fp);
			fread( &green, sizeof(unsigned char),1,fp);
			fread( &blue, sizeof(unsigned char),1,fp);
			*(bitdata+i+j*info.width)  = blue;
			*(bitdata+i+j*info.width+1)= green; 
			*(bitdata+i+j*info.width+2)= red;
		}
	}
	//�������J��
	free(bitdata);
	// �t�@�C���N���[�Y
	fclose(fp);
	return NULL;
}
unsigned int GenTextureBitmap(const char*filename){
	unsigned int tex_number;
	int i,j;
	FILE *fp;
	//�e�s�N�Z���̋P�x�f�[�^
	unsigned char* bitdata;
	//
	unsigned char* color_pallete_r;
	unsigned char* color_pallete_g;
	unsigned char* color_pallete_b;
	unsigned char* color_pallete_v;
	//�r�b�g�}�b�v�w�b�_
	BitmapHeader header;
	BitmapInfoHeader info;
	//�F��
	unsigned int color_bit;
	// �t�@�C���I�[�v��
	if( ( fp = fopen(filename, "rb") )==NULL){
		return NULL;
	}
	//�w�b�_�ǂݍ���
	if( !ReadHeader(&header,fp) ){
		fclose(fp);
		return NULL;
	}
	//�w�b�_�ǂݍ���
	if( !ReadInfoHeader(&info,fp) ){
		fclose(fp);
		return NULL;
	}
	//�w�b�_�`�F�b�N
	if( !(color_bit = CheckHeaders(&header,&info)) ){
		fclose(fp);
		return NULL;
	}
	//�r�b�g�}�b�v�t�@�C���T�C�Y�`�F�b�N
	if( !CheckSize(info.width*info.height) ){
		fclose(fp);
		return NULL;
	}
	//�J���[�r�b�g���ŏ����𕪂���
	if(color_bit==8)goto bit_8;
	else if(color_bit==24)goto bit_24;
	//�ꉞ��O����
	else{
		fclose(fp);
		return NULL;
	}
/////////////////////////////////////////////////////////////////////
// 8bit(256�F)�r�b�g�}�b�v�̏���
bit_8:
	unsigned char pallete_num;
	//�J���[�p���b�g�ǂݍ���
	//�������m��
	color_pallete_r = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_g = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_b = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_v = (unsigned char*)malloc(256*sizeof(unsigned char));
	//�p���b�g�ǂݍ���
	for(i=0;i<256;i++){
		fread( (color_pallete_r+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_g+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_b+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_v+i), sizeof(unsigned char),1,fp);
	}
	//�r�b�g�f�[�^�ǂݍ���
	bitdata = (unsigned char*)malloc(3*info.width*info.height*sizeof(unsigned char));
	for(j=(info.height-1);j>=0;j--){
		for(i=(info.width-1);i>=0;i--){
			fread( &pallete_num, sizeof(unsigned char),1,fp);
			*(bitdata+3*i+3*j*info.width)  = *(color_pallete_b+pallete_num); 
			*(bitdata+3*i+3*j*info.width+1)= *(color_pallete_g+pallete_num); 
			*(bitdata+3*i+3*j*info.width+2)= *(color_pallete_r+pallete_num); 
		}
	}
	//�J���[�p���b�g�������J��
	free(color_pallete_r);
	free(color_pallete_g);
	free(color_pallete_b);
	free(color_pallete_v);
	//�e�N�X�`���쐬
	glGenTextures(1, &tex_number);
	glBindTexture(GL_TEXTURE_2D, tex_number);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, 
		info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitdata);
	free(bitdata);
	// �t�@�C���N���[�Y
	fclose(fp);
	return tex_number;
/////////////////////////////////////////////////////////////////////
// 24bit(1677���F)�r�b�g�}�b�v�̏���
bit_24:
	unsigned char red,green,blue;
	//�r�b�g�f�[�^�ǂݍ���
	//�������m��
	bitdata = (unsigned char*)malloc(3*info.width*info.height*sizeof(unsigned char));
	//�ǂݍ���
	for(j=(info.height-1);j>=0;j--){
		for(i=(info.width-1);i>=0;i--){
			fread( &red, sizeof(unsigned char),1,fp);
			fread( &green, sizeof(unsigned char),1,fp);
			fread( &blue, sizeof(unsigned char),1,fp);
			*(bitdata+3*i+3*j*info.width)  = blue;
			*(bitdata+3*i+3*j*info.width+1)= green; 
			*(bitdata+3*i+3*j*info.width+2)= red;
		}
	}
	//�e�N�X�`���쐬
	glGenTextures(1, &tex_number);
	glBindTexture(GL_TEXTURE_2D, tex_number);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, 
		info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitdata);
	//�������J��
	free(bitdata);
	// �t�@�C���N���[�Y
	fclose(fp);
	return tex_number;
}