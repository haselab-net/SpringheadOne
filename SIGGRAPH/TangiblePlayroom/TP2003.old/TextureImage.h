/*------------------------------
File : TextureImage.h
 http://alb.hp.infoseek.co.jp/opengl/texture.shtml
------------------------------*/
#ifndef TEXTUREIMAGE_H
#define TEXTUREIMAGE_H

#include <iostream>
#include <GL/glut.h>
#include <SDL_image.h>
//#include <SDL/SDL_image.h>


class TextureImage{
 public:
  GLuint texID;
  void IMGLoad( char *filename );
};

#endif
