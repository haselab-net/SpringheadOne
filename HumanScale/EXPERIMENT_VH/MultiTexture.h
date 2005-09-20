/*-----------------------------------------------
			MultiTexture.h
            Written by Steven Jones
-----------------------------------------------*/

#ifndef ___MULTITEXTURE_H___
#define ___MULTITEXTURE_H___

//#include "stdafx.h"						// For the OpenGL includes shown below
/*
#include <gl\gl.h>							// Header File For The OpenGL32 Library
#include <gl\glu.h>							// Header File For The GLu32 Library
#include <gl\glext.h>
*/
// Constants ////////////////////////////////////////////////////////////////////////////
#include <gl/glext.h>

// Multitexture constants
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1

#define GL_COMBINE_ARB						0x8570
#define GL_RGB_SCALE_ARB					0x8573


// Functions ////////////////////////////////////////////////////////////////////////////

typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC)     (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC)       (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum target);

bool InitMultiTextures(void);


// External Function Pointers ///////////////////////////////////////////////////////////

extern PFNGLMULTITEXCOORD2FARBPROC	   glMultiTexCoord2fARB;
extern PFNGLACTIVETEXTUREARBPROC 	   glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;

#endif // ___MULTITEXTURE_H___