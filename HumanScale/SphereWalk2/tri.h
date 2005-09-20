#ifndef _TRI_H
#define _TRI_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "kmenu.h"

#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>

#define num 60
unsigned int texid[64];
const int MAX_LOD = 1;
const double TriPI = 3.14159265;
const double TEXSIZE = 2048;   // テクスチャサイズを変えるときには変更が必要！

#ifdef  __cplusplus
extern "C" {
#endif

//extern 
void CImage(int imagenum);
//extern 
void myinit(void);
void TexMap(int mode, float* x, float* y, float X, float Y, float Z);
void WriteSphere(void);

#ifdef __cplusplus
}
#endif
#endif