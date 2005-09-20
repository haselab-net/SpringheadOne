//
//
//
//
// PBuffer
//
//  This is a simple conveniance class for using PBuffers.
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PBUFFER_H
#define PBUFFER_H

//#include <iostream.h> //なぜかエラー・・・
#include <gl/glut.h>
#include "ATIExtensions.h"

class PBuffer
{
   public:
      //create a Pbuffer with the correct attributes, share allows it to share lists with the current context
      PBuffer( int width, int height, int bpp, bool isFloat, bool dBuffer, bool hasDepth, bool hasStencil, bool texture, bool share);
      ~PBuffer();

      //is this buffer usable
      bool IsValid();
      
      //make it available for rendering
      void Bind();
      void Unbind();

      //make it available as a texture if applicable 
      void BindAsTexture( int buffer);
      void ReleaseTexture( int buffer);

      //swap if applicable
      void Swap();

	  //Texture ID
	  GLuint texture_id;

	  GLfloat mat_amb_diff_teapot[4];

   private:
      HPBUFFERARB m_buffer;

      HGLRC m_RC;

      HDC m_DC;

      //used to restore the previous RC when unbinding
      HGLRC m_prevRC;
      HDC m_prevDC;

	  //PBuffer用テクスチャオブジェクト生成
	  void InitTexture();
};


#endif //PBUFFER_H

