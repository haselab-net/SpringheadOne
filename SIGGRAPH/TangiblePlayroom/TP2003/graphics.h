#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "Bitmap.h"

///////////////////////////////////////////////////////////////////////////////
//デフォルトウィンドウサイズ
#define WIDTH	300
#define HEIGHT	300

//GLUT初期化コマンド
void Graphics_Init(int argc, char** argv);

//OpenGL初期化とシーンのロード
void Graphics_Setup (void);

//各処理関数の登録とメインのループ
void Graphics_StartRender(void) ;

//void SpritePNG(unsigned int id, Vec3f pos,float r, float alpha);
void Texture_Setup();


#endif