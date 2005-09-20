/*------------------------------
File : TextureImage.cpp
------------------------------*/
/*

http://alb.hp.infoseek.co.jp/opengl/texture.shtml  
------------------------------
File : main.cpp
------------------------------
#include "TextureImage.h"

TextureImage *texture;

void init(){
  texture = new TextureImage();
  texture->IMGLoad( "sample.png" );
}

void draw(){
  glBindTexture( GL_TEXTURE_2D, texture->texID );
  // ポリゴン描画
}
...

使い方のキモは、IMGLoad でテクスチャを読み込んだら、
 glBindTexture で texID を渡してやるというコトだけです♪
 コソパイルは -lglut -lSDL -lSDL_image あたりをつければ良いかと♪
 注意深くみてくれた人は気付いたかもしれませんが、 
 画像データを 32bit RGBA フォーマットで扱っているので、 
 透過 PNG なんかもちゃんと使えます♪ 
 自分で動作を確認した画像フォーマットは BMP JPG PNG TGA で、
 PNG と TGA は透過部分もキッチリ扱えてました♪ 
*/

#include "TextureImage.h"

void TextureImage::IMGLoad( char *filename ){
  SDL_Surface *Image=NULL;
  SDL_Surface *tmpImage;

  if( Image=IMG_Load( filename ) ){
    tmpImage = SDL_CreateRGBSurface( SDL_SWSURFACE, 0, 0, 32, 
				     0x000000ff, 
				     0x0000ff00, 
				     0x00ff0000, 
				     0xff000000 );
    Image = SDL_ConvertSurface( Image, tmpImage->format, SDL_SWSURFACE );
    SDL_FreeSurface( tmpImage );

    glGenTextures( 1, &texID );
    glBindTexture( GL_TEXTURE_2D, texID );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, Image->w, Image->h, 0,
		  GL_RGBA, GL_UNSIGNED_BYTE, Image->pixels );

    SDL_FreeSurface( Image );
 
  }
}
