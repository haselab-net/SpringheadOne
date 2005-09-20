
#include "glutils.h"
#include <stdio.h>

///
/// GLUTで開いたWindowのGL-Contextをメインにする
///
void MakeGlutWindowCurrent(void)
{
    static int glutWinId = glutGetWindow();

    glutSetWindow( glutWinId );
}

///
/// WGL関係のエラーをチェックする
/// 
void wglGetLastError(void)
{
    DWORD err = GetLastError();
    switch(err)
    {
    case ERROR_INVALID_PIXEL_FORMAT:
        //cerr << "Win32 Error:  ERROR_INVALID_PIXEL_FORMAT\n";
		printf("Win32 Error:  ERROR_INVALID_PIXEL_FORMAT\n");
        break;
    case ERROR_NO_SYSTEM_RESOURCES:
        //cerr << "Win32 Error:  ERROR_NO_SYSTEM_RESOURCES\n";
		printf("Win32 Error:  ERROR_NO_SYSTEM_RESOURCES\n");
        break;
    case ERROR_INVALID_DATA:
        //cerr << "Win32 Error:  ERROR_INVALID_DATA\n";
		printf("Win32 Error:  ERROR_INVALID_DATA\n");
        break;
    case ERROR_INVALID_WINDOW_HANDLE:
        //cerr << "Win32 Error:  ERROR_INVALID_WINDOW_HANDLE\n";
		printf("Win32 Error:  ERROR_INVALID_WINDOW_HANDLE\n");
        break;
    case ERROR_RESOURCE_TYPE_NOT_FOUND:
        //cerr << "Win32 Error:  ERROR_RESOURCE_TYPE_NOT_FOUND\n";
		printf("Win32 Error:  ERROR_RESOURCE_TYPE_NOT_FOUND\n");
        break;
    case ERROR_SUCCESS:
        // no error
        break;
    default:
        //cerr << "Win32 Error:  " << err << endl;
		printf("Win32 Error:  \n");
        break;
    }
    SetLastError(0);
}


///
/// 画面上に文字を表示するルーチン
///
void drawtext(void)
{
	
    int ww = glutGet( (GLenum)GLUT_WINDOW_WIDTH );
    int wh = glutGet( (GLenum)GLUT_WINDOW_HEIGHT );

    glDisable( GL_DEPTH_TEST );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0, ww-1, 0, wh-1 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();



	int x = 20;
	int y = wh-22;
    glRasterPos2i( x, y );

	char string[1024] = "This demo shows how to use a p-buffer for off-screen\n"
                        "rendering, and it shares textures and display lists\n"
						"with the main window.  It renders a glutWireTeapot()\n"
						"to the 512x512 pbuffer, then does a fast\n"
						"glCopyTexSubImage2D() with GENERATE_MIPMAPS turned on,\n"
						"and trilinear and anisotropic filtering.\n\n"

						"Quality filtering with high resolution, dynamic\n"
						"textures -- all generated on-card!\n\n"

                        "Press the 'ESC' key to exit.";
                        
	char *p;

    for ( p = string; *p; p++ )
		{
        if ( *p == '\n' )
            {
            y = y - 14;
            glRasterPos2i( x, y );
            continue;
            }
        //glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *p );
		}

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    glEnable( GL_DEPTH_TEST );
}
