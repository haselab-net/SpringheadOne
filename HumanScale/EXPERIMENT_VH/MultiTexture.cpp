/*-----------------------------------------------
			MultiTexture.cpp
            Written by Steven Jones
-----------------------------------------------*/

#include "multitexture.h"

PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	 = NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		 = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;

bool InitMultiTextures(void)
{
	// Initialize the multitexturing functions
	glActiveTextureARB		 = (PFNGLACTIVETEXTUREARBPROC)		 wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB	 = (PFNGLMULTITEXCOORD2FARBPROC)	 wglGetProcAddress("glMultiTexCoord2fARB");
	glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");

	// Make sure that an appropriate version of OpenGL is installed.
	// If the extension functions could not be found then the function pointers will be NULL
	if(!glActiveTextureARB || !glMultiTexCoord2fARB)
		return false;
	else
		return true;
}