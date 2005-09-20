//
//
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PBuffer.h"
#include <stdio.h>


#define MAX_ATTRIBS 32
#define MAX_FORMATS 32

//
//
//
////////////////////////////////////////////////////////////////////////////////
PBuffer::PBuffer( int width, int height, int bpp, bool isFloat, bool dBuffer, bool hasDepth, bool hasStencil, bool texture, bool share) :
   m_buffer(0), m_RC(0), m_DC(0), m_prevRC(0), m_prevDC(0)
{
   int iAttribs[2*MAX_ATTRIBS];
   float fAttribs[2*MAX_ATTRIBS];
   int niAttribs = 0;
   int nfAttribs = 0;
   int pformats[MAX_FORMATS];
   unsigned int nformat;

   HGLRC rc = wglGetCurrentContext();
   HDC hdc = wglGetCurrentDC();

   //full float buffer
   niAttribs = 0;
   nfAttribs = 0;

   memset( iAttribs, 0, sizeof(int)*2*MAX_ATTRIBS);
   memset( fAttribs, 0, sizeof(int)*2*MAX_ATTRIBS);

   iAttribs[ niAttribs*2] = WGL_DRAW_TO_PBUFFER_ARB;
   iAttribs[ niAttribs*2 + 1] = 1;
   niAttribs++;
   iAttribs[ niAttribs*2] = WGL_PIXEL_TYPE_ARB;
   iAttribs[ niAttribs*2 + 1] = (isFloat) ? WGL_TYPE_RGBA_FLOAT_ATI : WGL_TYPE_RGBA_ARB;
   niAttribs++;
   iAttribs[ niAttribs*2] = WGL_DOUBLE_BUFFER_ARB;
   iAttribs[ niAttribs*2 + 1] = (dBuffer) ? 1 : 0;
   niAttribs++;
   iAttribs[ niAttribs*2] = WGL_SUPPORT_OPENGL_ARB;
   iAttribs[ niAttribs*2 + 1] = 1;
   niAttribs++;
   iAttribs[ niAttribs*2] = WGL_RED_BITS_ARB;
   iAttribs[ niAttribs*2 + 1] = bpp / 4;
   niAttribs++;
   iAttribs[ niAttribs*2] = WGL_GREEN_BITS_ARB;
   iAttribs[ niAttribs*2 + 1] = bpp / 4;
   niAttribs++;
   iAttribs[ niAttribs*2] = WGL_BLUE_BITS_ARB;
   iAttribs[ niAttribs*2 + 1] = bpp / 4;
   niAttribs++;
   iAttribs[ niAttribs*2] = WGL_ALPHA_BITS_ARB;
   iAttribs[ niAttribs*2 + 1] = bpp / 4;
   niAttribs++;
   iAttribs[ niAttribs*2] = WGL_COLOR_BITS_ARB;
   iAttribs[ niAttribs*2 + 1] = bpp;
   niAttribs++;

   if (hasDepth)
   {
      iAttribs[ niAttribs*2] = WGL_DEPTH_BITS_ARB;
      iAttribs[ niAttribs*2 + 1] = 24;
      niAttribs++;
   }

   if (hasStencil)
   {
      iAttribs[ niAttribs*2] = WGL_STENCIL_BITS_ARB;
      iAttribs[ niAttribs*2 + 1] = 8;
      niAttribs++;
   }

   if (texture)
   {
      iAttribs[ niAttribs*2] = WGL_BIND_TO_TEXTURE_RGBA_ARB;
      iAttribs[ niAttribs*2 + 1] = GL_TRUE;
      niAttribs++;
   }

   if (wglChoosePixelFormatARB( hdc, iAttribs, fAttribs, MAX_FORMATS, pformats, &nformat))
   {
      niAttribs = 0;
      if (texture)
      {
         iAttribs[niAttribs++] = WGL_TEXTURE_FORMAT_ARB;
         iAttribs[niAttribs++] = WGL_TEXTURE_RGBA_ARB;
         iAttribs[niAttribs++] = WGL_TEXTURE_TARGET_ARB;
         iAttribs[niAttribs++] = WGL_TEXTURE_2D_ARB;
		 iAttribs[niAttribs++] = WGL_MIPMAP_TEXTURE_ARB;
		 iAttribs[niAttribs++] = TRUE;
      }
      iAttribs[niAttribs] = 0;
      m_buffer = wglCreatePbufferARB( hdc, pformats[0], width, height, iAttribs);

	  //cerr << "m_buffer finish. (" << m_buffer << "(" << endl;
	  printf("m_buffer finish......\n");

      if (m_buffer)
      {
		//cerr << "m_buffer\n";
		  printf("m_buffer\n");
         m_DC = wglGetPbufferDCARB( m_buffer);

         if (m_DC)
         {
			//cerr << "m_DC\n";
			 printf("m_DC\n");
            m_RC = wglCreateContext( m_DC);

            if (m_RC)
            {
               if (share)
               {
                  wglShareLists( rc, m_RC);
               }
            }
            else //floatRC
            {
               wglReleasePbufferDCARB( m_buffer, m_DC);
               m_DC = NULL;
               wglDestroyPbufferARB( m_buffer);
               m_buffer = NULL;
            }
         }
         else //floatDC
         {
            wglDestroyPbufferARB( m_buffer);
            m_buffer = NULL;
         }
      }
      else //floatBuffer
      {
         //nothing presently
      }
   }

   InitTexture();
}

//
//
//
////////////////////////////////////////////////////////////////////////////////
PBuffer::~PBuffer()
{
   if (m_RC)
   {
      wglDeleteContext( m_RC);
      m_RC = NULL;
   }

   if (m_DC)
   {
		wglReleasePbufferDCARB( m_buffer,m_DC );
      m_DC = NULL;
   }

   if (m_buffer)
   {
		wglDestroyPbufferARB( m_buffer);
      m_buffer = NULL;
   }
}

//
//
//
////////////////////////////////////////////////////////////////////////////////
bool PBuffer::IsValid()
{
   return (m_RC != NULL);
}

//
//
//
////////////////////////////////////////////////////////////////////////////////
void PBuffer::Bind()
{
   m_prevDC = wglGetCurrentDC();
   m_prevRC = wglGetCurrentContext();


   wglMakeCurrent( m_DC, m_RC);
}

//
//
//
////////////////////////////////////////////////////////////////////////////////
void PBuffer::Unbind()
{
   wglMakeCurrent( m_prevDC, m_prevRC);
}

//
//
//
////////////////////////////////////////////////////////////////////////////////
void PBuffer::BindAsTexture( int buffer)
{
	int lost = 0;
    wglQueryPbufferARB( m_buffer, WGL_PBUFFER_LOST_ARB, &lost );
	if(lost != 0){
		//cerr << " PBuffer was lost!\n";
		printf(" PBuffer was lost!\n");
	}

	if(!wglBindTexImageARB( m_buffer, buffer)){
		//cerr << "pbuffer: wglBindTexImageARB() failed\n";
		printf("pbuffer: wglBindTexImageARB() failed\n");
		exit(-1);
	}
}

//
//
//
////////////////////////////////////////////////////////////////////////////////
void PBuffer::ReleaseTexture( int buffer)
{
	if(!wglReleaseTexImageARB( m_buffer, buffer)){
		//cerr << "pbuffer' wglReleaseTexImageARB() failed\n";
		printf("pbuffer: wglReleaseTexImageARB() failed\n");
		exit(-1);
	}
}

//
//
//
////////////////////////////////////////////////////////////////////////////////
void PBuffer::Swap()
{
   wglSwapLayerBuffers( m_DC, WGL_SWAP_MAIN_PLANE);
}
////////////////////////////////////////////////////////////////////////////////
void PBuffer::InitTexture()
{
	glEnable(GL_TEXTURE_2D);

    glGenTextures( 1, &texture_id );
    glBindTexture( GL_TEXTURE_2D, texture_id );

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
 //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	
	//	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, P_WIDTH, P_HEIGHT, 0, GL_RGB, GL_FLOAT, 0 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                     GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                     GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE ); 
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
/*
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
                     GL_CLAMP_TO_EDGE_EXT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
                     GL_CLAMP_TO_EDGE_EXT );

  */
}
