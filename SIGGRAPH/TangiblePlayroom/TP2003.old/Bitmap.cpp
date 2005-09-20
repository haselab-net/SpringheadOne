///////////////////////////////////////////////////////////////////////////////
//
//	Bitmapファイル書き込み&読み込み
//
///////////////////////////////////////////////////////////////////////////////

#define TEX_SIZE 256

#include "Bitmap.h"


///////////////////////////////////////////////////////////////////////////////
//ビットマップファイルのサイズチェック
int CheckSize(unsigned int pict_size){
	unsigned int i=2;
	unsigned int max_texture = TEX_SIZE*TEX_SIZE;
	//サイズチェック
	for(i=2;i<=max_texture;i*=2){
		if( i == pict_size){
			return 1;
		}
	}
	return 0;
};
///////////////////////////////////////////////////////////////////////////////
//Bitmap構造体初期化
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
//Bitmapヘッダ書き込み
void WriteHeader(BitmapHeader *header,FILE *fp){
	fwrite(&(header->distinct1), sizeof(char),1,fp);
	fwrite(&(header->distinct2), sizeof(char),1,fp);
	fwrite(&(header->filesize), sizeof(int),1,fp);
	fwrite(&(header->reserve1), sizeof(short),1,fp);
	fwrite(&(header->reserve2), sizeof(short),1,fp);
	fwrite(&(header->offset), sizeof(int),1,fp);
}
///////////////////////////////////////////////////////////////////////////////
//BitmapInfoヘッダ書き込み
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
//Bitmapヘッダ読み込み
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
//BitmapInfoヘッダ読み込み
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
//Bitmapヘッダチェック
int CheckHeaders(BitmapHeader *header,BitmapInfoHeader *info){
	//ヘッダのBMの文字
	if( header->distinct1!='B' || header->distinct2!='M' )return 0;
	//レイヤー１
	if( info->plane !=1 )return 0;
	//圧縮なし
	if( info->compression !=0 ) return 0;
	//色数を返す
	if( info->bits ==24 || info->bits ==8 )return info->bits;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//ピクセルデータをBitmapファイルへ書き込む
int WriteBitmap(const char* filename, GLubyte* data, int width, int height){
	FILE *fp;
	BitmapHeader header;
	BitmapInfoHeader info;
	int i=0;
	int j=0;
	int x;
	int y;
	// ファイルオープン
	if( ( fp = fopen(filename, "wb") )==NULL){
		return -1;
	}
	//ヘッダ構造体の初期化
	InitHeaders(&header, &info);
	//Bitmapサイズ
	info.width = width;
	info.height = height;
	//ファイル容量
	header.filesize = 
		width*height*3*sizeof(GLubyte)		//ビット情報量
		+ 14								//BitmapHeader構造体サイズ
		+ 40;								//BitmapInfoHeader構造体サイズ
	//ヘッダ書き込み
	WriteHeader(&header,fp);
	WriteInfoHeader(&info,fp);
	// イメージデータ書き込み
	for( y=0 ; y < height ; y++ ){
		for( x=0 ; x < width ; x++ ){
			j=fwrite((data+x*3+3*y*width+2),sizeof(GLubyte),1,fp);
			j=fwrite((data+x*3+3*y*width+1),sizeof(GLubyte),1,fp);
			j=fwrite((data+x*3+3*y*width),sizeof(GLubyte),1,fp);
		}
	}
	// ファイルクローズ
	fclose(fp);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//ビットマップファイルからピクセルデータを返す．
unsigned char* ReadBitmap(const char*filename){
	int i,j;
	FILE *fp;
	//各ピクセルの輝度データ
	unsigned char* bitdata;
	//
	unsigned char* color_pallete_r;
	unsigned char* color_pallete_g;
	unsigned char* color_pallete_b;
	unsigned char* color_pallete_v;
	//ビットマップヘッダ
	BitmapHeader header;
	BitmapInfoHeader info;
	//色数
	unsigned int color_bit;
	// ファイルオープン
	if( ( fp = fopen(filename, "rb") )==NULL){
		return NULL;
	}
	//ヘッダ読み込み
	if( !ReadHeader(&header,fp) ){
		fclose(fp);
		return NULL;
	}
	//ヘッダ読み込み
	if( !ReadInfoHeader(&info,fp) ){
		fclose(fp);
		return NULL;
	}
	//ヘッダチェック
	if( !(color_bit = CheckHeaders(&header,&info)) ){
		fclose(fp);
		return NULL;
	}
	//ビットマップファイルサイズチェック
	if( !CheckSize(info.width*info.height) ){
		fclose(fp);
		return NULL;
	}
	//カラービット数で処理を分ける
	if(color_bit==8)goto bit_8;
	if(color_bit==24)goto bit_24;
	else{
		fclose(fp);
		return NULL;
	}
/////////////////////////////////////////////////////////////////////
// 8bit(256色)ビットマップの処理
bit_8:
	unsigned char pallete_num;
	//カラーパレット読み込み
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
	//ビットデータ読み込み
	bitdata = (unsigned char*)malloc(3*info.width*info.height*sizeof(unsigned char));
	for(i=0;i<info.width;i++){
		for(j=0;j<info.height;j++){
			fread( &pallete_num, sizeof(unsigned char),1,fp);
			*(bitdata+i+j*info.width)  = *(color_pallete_b+pallete_num); 
			*(bitdata+i+j*info.width+1)= *(color_pallete_g+pallete_num); 
			*(bitdata+i+j*info.width+2)= *(color_pallete_b+pallete_num); 
		}
	}
	//カラーパレットメモリ開放
	free(color_pallete_r);
	free(color_pallete_g);
	free(color_pallete_b);
	free(color_pallete_v);
	free(bitdata);
	// ファイルクローズ
	fclose(fp);
	return NULL;
/////////////////////////////////////////////////////////////////////
// 24bit(1677万色)ビットマップの処理
bit_24:
	unsigned char red,green,blue;
	//ビットデータ読み込み
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
	//メモリ開放
	free(bitdata);
	// ファイルクローズ
	fclose(fp);
	return NULL;
}
unsigned int GenTextureBitmap(const char*filename){
	unsigned int tex_number;
	int i,j;
	FILE *fp;
	//各ピクセルの輝度データ
	unsigned char* bitdata;
	//
	unsigned char* color_pallete_r;
	unsigned char* color_pallete_g;
	unsigned char* color_pallete_b;
	unsigned char* color_pallete_v;
	//ビットマップヘッダ
	BitmapHeader header;
	BitmapInfoHeader info;
	//色数
	unsigned int color_bit;
	// ファイルオープン
	if( ( fp = fopen(filename, "rb") )==NULL){
		return NULL;
	}
	//ヘッダ読み込み
	if( !ReadHeader(&header,fp) ){
		fclose(fp);
		return NULL;
	}
	//ヘッダ読み込み
	if( !ReadInfoHeader(&info,fp) ){
		fclose(fp);
		return NULL;
	}
	//ヘッダチェック
	if( !(color_bit = CheckHeaders(&header,&info)) ){
		fclose(fp);
		return NULL;
	}
	//ビットマップファイルサイズチェック
	if( !CheckSize(info.width*info.height) ){
		fclose(fp);
		return NULL;
	}
	//カラービット数で処理を分ける
	if(color_bit==8)goto bit_8;
	else if(color_bit==24)goto bit_24;
	//一応例外処理
	else{
		fclose(fp);
		return NULL;
	}
/////////////////////////////////////////////////////////////////////
// 8bit(256色)ビットマップの処理
bit_8:
	unsigned char pallete_num;
	//カラーパレット読み込み
	//メモリ確保
	color_pallete_r = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_g = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_b = (unsigned char*)malloc(256*sizeof(unsigned char));
	color_pallete_v = (unsigned char*)malloc(256*sizeof(unsigned char));
	//パレット読み込み
	for(i=0;i<256;i++){
		fread( (color_pallete_r+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_g+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_b+i), sizeof(unsigned char),1,fp);
		fread( (color_pallete_v+i), sizeof(unsigned char),1,fp);
	}
	//ビットデータ読み込み
	bitdata = (unsigned char*)malloc(3*info.width*info.height*sizeof(unsigned char));
	for(j=(info.height-1);j>=0;j--){
		for(i=(info.width-1);i>=0;i--){
			fread( &pallete_num, sizeof(unsigned char),1,fp);
			*(bitdata+3*i+3*j*info.width)  = *(color_pallete_b+pallete_num); 
			*(bitdata+3*i+3*j*info.width+1)= *(color_pallete_g+pallete_num); 
			*(bitdata+3*i+3*j*info.width+2)= *(color_pallete_r+pallete_num); 
		}
	}
	//カラーパレットメモリ開放
	free(color_pallete_r);
	free(color_pallete_g);
	free(color_pallete_b);
	free(color_pallete_v);
	//テクスチャ作成
	glGenTextures(1, &tex_number);
	glBindTexture(GL_TEXTURE_2D, tex_number);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, 
		info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitdata);
	free(bitdata);
	// ファイルクローズ
	fclose(fp);
	return tex_number;
/////////////////////////////////////////////////////////////////////
// 24bit(1677万色)ビットマップの処理
bit_24:
	unsigned char red,green,blue;
	//ビットデータ読み込み
	//メモリ確保
	bitdata = (unsigned char*)malloc(3*info.width*info.height*sizeof(unsigned char));
	//読み込み
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
	//テクスチャ作成
	glGenTextures(1, &tex_number);
	glBindTexture(GL_TEXTURE_2D, tex_number);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, 
		info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitdata);
	//メモリ開放
	free(bitdata);
	// ファイルクローズ
	fclose(fp);
	return tex_number;
}