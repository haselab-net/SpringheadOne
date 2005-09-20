#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "Bitmap.h"

///////////////////////////////////////////////////////////////////////////////
//�f�t�H���g�E�B���h�E�T�C�Y
#define WIDTH	300
#define HEIGHT	300

//GLUT�������R�}���h
void Graphics_Init(int argc, char** argv);

//OpenGL�������ƃV�[���̃��[�h
void Graphics_Setup (void);

//�e�����֐��̓o�^�ƃ��C���̃��[�v
void Graphics_StartRender(void) ;

//void SpritePNG(unsigned int id, Vec3f pos,float r, float alpha);
void Texture_Setup();


#endif