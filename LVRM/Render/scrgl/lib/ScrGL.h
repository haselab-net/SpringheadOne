#ifndef __ScrGL_h
#define __ScrGL_h
#include <gl\gl.h>



namespace Scrgl {
typedef void (__fastcall *SGLDEBUG)(AnsiString s,void*ctxt);
void __fastcall PACKAGE SetMessageProc(SGLDEBUG msgproc,SGLDEBUG errproc,void*ctxt);
}


bool __fastcall PACKAGE sglIsDriver3dfx(AnsiString fn);

class PACKAGE TSglDriverDetails : public TObject
{
public:
  __fastcall TSglDriverDetails();
  AnsiString FileName;
  AnsiString CompanyName;
  AnsiString FileDescription;
  AnsiString FileVersion;
  bool Uses3dfxStyle;
};



class PACKAGE TSglDriverStrings : public TStringList
{
public:
  __fastcall TSglDriverStrings();
  __fastcall ~TSglDriverStrings();
  __property TSglDriverDetails* Objects[int Index] = {read=GetObject,write=SetObject};
  int __fastcall IndexOfFileName(AnsiString fn);
  void __fastcall Delete(int i);
  void __fastcall Clear();
protected:
  TSglDriverDetails* __fastcall GetObject(int i); void __fastcall SetObject(int i,TSglDriverDetails *d);
};



class PACKAGE TSglDriverList : public TComponent
{
public:
  __fastcall TSglDriverList(TComponent *Owner);
  __fastcall ~TSglDriverList();
__published:
  __property TStrings* Search = {read=GetSearchPath,write=SetSearchPath};
public:
  __property TSglDriverStrings* Items = {read=GetDrivers};
protected:
  TStringList *FSearchPath;
  TStrings* __fastcall GetSearchPath();
  void __fastcall SetSearchPath(TStrings *s);
  void __fastcall SearchPathChanged(TObject *Sender);
  TSglDriverStrings *FDrivers; bool FDriversValid; 
  TSglDriverStrings* __fastcall GetDrivers();
};



enum TGLFlag {f_DOUBLEBUFFER, f_DRAW_TO_BITMAP, f_DRAW_TO_WINDOW,
              f_GENERIC_ACCELERATED, f_GENERIC_FORMAT, f_NEED_PALETTE,
              f_NEED_SYSTEM_PALETTE, f_SUPPORT_GDI, f_SUPPORT_OPENGL, f_STEREO,
              f_SWAP_COPY, f_SWAP_EXCHANGE, f_SWAP_LAYER_BUFFERS};

typedef Set<TGLFlag, f_DOUBLEBUFFER, f_SWAP_LAYER_BUFFERS> TGLFlags;

enum TGLPixelType {pt_TYPE_RGBA, pt_TYPE_COLORINDEX};


typedef void __fastcall (__closure *TGLErrorEvent)(System::TObject* Sender,AnsiString err);
typedef void __fastcall (__closure *TGLPixelFormatEvent)(System::TObject* Sender,HDC hdc,int &num,int bpp,int depth,int dbl,int acc);


class PACKAGE TSglPanel : public TWinControl
{ public:
  __fastcall TSglPanel(TComponent *Owner);
  __fastcall ~TSglPanel();
  void __fastcall CreateWindowHandle(const TCreateParams &p);
  void __fastcall DestroyWindowHandle();
__published:
  __property AnsiString glDriver = {read=FglDriver,write=SetglDriver};
  __property int glBpp = {read=FglBpp,write=SetglBpp,default=-1};
  __property int glDepth = {read=FglDepth,write=SetglDepth,default=16};
  __property int glDblBuff = {read=FglDblBuff,write=SetglDblBuff,default=1};
  __property int glAcc = {read=FglAcc,write=SetglAcc,default=1};
  __property int ScreenWidth = {read=FScreenWidth,write=SetScreenWidth,default=-1};
  __property int ScreenHeight = {read=FScreenHeight,write=SetScreenHeight,default=-1};
  __property int ScreenBpp = {read=FScreenBpp,write=SetScreenBpp,default=-1};
  __property TGLPixelFormatEvent OnGLPixelFormat = {read=FOnGLPixelFormat,write=FOnGLPixelFormat,default=NULL};
  __property TNotifyEvent OnGLInit = {read=FOnGLInit,write=FOnGLInit,default=NULL};
  __property TNotifyEvent OnGLPaint = {read=FOnGLPaint,write=FOnGLPaint,default=NULL};
  __property TNotifyEvent OnGLDestroy = {read=FOnGLDestroy,write=FOnGLDestroy,default=NULL};
  __property TGLErrorEvent OnGLError = {read=FOnGLError,write=FOnGLError,default=NULL};
  __property TNotifyEvent OnResize = {read=FOnResize,write=FOnResize,default=NULL};
  __property TColor Color = {read=FColor,write=SetColor,default=clNone};
  __property Visible;
  __property Align;
  __property TabStop;
public:
  void __fastcall GLHandleNeeded();
  bool __fastcall GLHandleAllocated();
  __property HGLRC GLHandle = {read=GetGLHandle};
  __property HDC GLCanvasHandle = {read=GetGLCanvasHandle};
  __property bool IsOn = {read=GetIsOn};
  void __fastcall SetScreenDimensions(int w,int h,int b);
  void __fastcall SwapBuffers();
  void __fastcall SetCurrent();
protected:
  int FScreenWidth,FScreenHeight,FScreenBpp;
  void __fastcall SetScreenWidth(int w);
  void __fastcall SetScreenHeight(int h);
  void __fastcall SetScreenBpp(int b);
  void __fastcall ChangeScreenMode();
  void __fastcall ResetScreenMode();
  void __fastcall EnterMode();
  void __fastcall ExitMode();
  void __fastcall CreateParams(TCreateParams &p);
  void __fastcall WndProc(TMessage &msg);
  HGLRC __fastcall GetGLHandle();
  HDC __fastcall GetGLCanvasHandle();
  void __fastcall InvalidateGLHandle();
  void __fastcall GLError(AnsiString s);
  void __fastcall ClipMouse();
  void __fastcall UnclipMouse();
  bool __fastcall GetIsOn();
  void __fastcall CheckIfMoved();
  int FSuppressedWidth, FSuppressedHeight;
  TAlign FSuppressedAlign;
  bool FIsSuppressed;
  HINSTANCE FhGLib;
  HDC Fhdc; HGLRC Fhgl;
  bool FIsMouseClipped;
  RECT FOldMouseClip;
  int FOldMouseSpeed;
  bool FIsOutModed; // have we changed screen mode?
  int FCurrentModeWidth,FCurrentModeHeight,FCurrentModeBpp;
  TPoint FLastPoint; // where was the top-left-corner last time we checked?
  bool FPixelFormatSullied; // a hwnd can only have SetPixelFormat called on it once
  //
  AnsiString FglDriver;
  int FglBpp, FglDepth, FglAcc, FglDblBuff;
  TColor FColor;
  TNotifyEvent FOnGLPaint;
  TNotifyEvent FOnGLInit;
  TNotifyEvent FOnGLDestroy;
  TGLErrorEvent FOnGLError;
  TGLPixelFormatEvent FOnGLPixelFormat;
  TNotifyEvent FOnResize;
  //
  void __fastcall SetglBpp(const int bpp);
  void __fastcall SetglDepth(const int depth);
  void __fastcall SetglDblBuff(const int dblbuff);
  void __fastcall SetglAcc(const int acc);
  void __fastcall SetglDriver(AnsiString s);
  void __fastcall SetColor(const TColor c);
  //
  HWND hwndSubclass;
  bool GiveUpTrying;
public:
  LRESULT __fastcall SubclassFormProc(FARPROC OldWndProc,HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
__published:
  __property OnMouseMove;
  __property OnMouseDown;
  __property OnMouseUp;
};





// SUBSTGL OVERVIEW
//
// Dynamic linking is good. In a screensaver we might want to link with opengl32.dll
// for running in the preview in display properties, and with 3dfxgl.dll for
// running full-screen. So how do we achieve dynamic linking? read on...
//
// If we made a call to any of the standard functions (glBegin, SetPixelFormat,
// wglCreateContext, gluLookAt, ...) then BCB would automatically link us with
// OPENGL32.LIB. And that will force us to use OPENGL32.DLL. This is bad.
// Therefore, we only ever make calls to sglBegin, sglSetPixelFormat,
// sglCreateContext, sgluLookAt and so on. Any call that begins with sgl is
// part of SubstGL and will work properly dynamically.
//
// Before making any sgl calls, you must
//   hlib=sglLoadLibrary("3dfxgl.dll");
// which will use the standard conventions for finding DLL paths. After the end, use
//   sglFreeeLibrary(hlib);
//
// These following are the windows calls that you must change over to SubstGL.
// I have listed them here because you might have been fooled by their names
// into thinking that they are unrelated to GL.
//   sglSwapBuffers
//   sglChoosePixelFormat
//   sglDescribePixelFormat
//   sglSetPixelFormat
//
// A MiniGL has been produced for quake and halflife and others, which is smaller
// and implements only some of the functions. It would give you more flexibility
// in your distribution if you too restricted yourself to calls in the MiniGL.
// Control this with Project|Options|Defines, and define SUBST_MINIGLONLY
// Users of your program can put whichever ICD (installable client driver) into
// your directory that they wish. Or you could include some yourself.
//
// If you had made any calls to the glu functions, then this would implictly
// have linked with GLU32.DLL. But this library implicitly links into OPENGL32.DLL.
// Therefore we are not allowed to call any glu functions at all.
// A few sglu functions have been reimplemented below: sgluLookAt, sgluPickMatrix,
// sgluPerspective, sgluUnProject. Their source code was taken from Mesa, a free
// OpenGL implementation, thereby forcing ScrGL to use the GNU public library
// license. If you don't want to be saddled by this license, you must delete
// those functions and figure out workarounds.
//
//
// PROBLEMS? WHAT NEXT?
//
// This code is only partially developed. More functions need to be added.
// I am adding them in an ad-hoc manner, as I go along.
// At the bottom of this file is a list of MiniGL functions that have not
// yet been added. If you attempt to call any of them, you will get a link
// error.
// I've had problems with some functions not working with the MiniGL driver.
// I don't know why, but will try to document those that I find.
// If you have any questions, email ljw1004@cam.ac.uk or visit my web page
// at www.wischik.com/lu
//



HINSTANCE PACKAGE sglLoadLibrary(AnsiString fn);
void PACKAGE sglFreeLibrary();
bool PACKAGE sglUses3dfxStyle();
int PACKAGE sglChoosePixelFormatEx(HDC hdc,int *bpp,int *depth,int *dbl,int *acc);


void            APIENTRY PACKAGE sglAlphaFunc(GLenum func, GLclampf ref);
GLboolean       APIENTRY PACKAGE sglAreTexturesResident(GLsizei n, const GLuint *textures, GLboolean *residences);
// glArrayElementEXT
void            APIENTRY PACKAGE sglBegin(GLenum mode);
void            APIENTRY PACKAGE sglBindTexture(GLenum target, GLuint texture);
void            APIENTRY PACKAGE sglBlendFunc(GLenum sfactor, GLenum dfactor);
void            APIENTRY PACKAGE sglClear(GLbitfield mask);
void            APIENTRY PACKAGE sglClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void            APIENTRY PACKAGE sglClearDepth(GLclampd depth);
void            APIENTRY PACKAGE sglColor3f(GLfloat red, GLfloat green, GLfloat blue);
void            APIENTRY PACKAGE sglColor3fv(const GLfloat *v);
void            APIENTRY PACKAGE sglColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void            APIENTRY PACKAGE sglColor4fv(const GLfloat *v);
void            APIENTRY PACKAGE sglColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void            APIENTRY PACKAGE sglColor4ubv (const GLubyte *v);
// glColorPointerEXT
void            APIENTRY PACKAGE sglCullFace(GLenum mode);
void            APIENTRY PACKAGE sglDepthFunc(GLenum func);
void            APIENTRY PACKAGE sglDepthMask (GLboolean flag);
void            APIENTRY PACKAGE sglDepthRange (GLclampd zNear, GLclampd zFar);
void            APIENTRY PACKAGE sglDisable(GLenum cap); // TEXTURE_2D, BLEND, DEPTH_TEST, CULL_FACE, ALPHA_TEST only
void            APIENTRY PACKAGE sglDisableClientState (GLenum array);
// glDrawArraysEXT
void            APIENTRY PACKAGE sglDrawBuffer (GLenum mode);
void            APIENTRY PACKAGE sglDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
// glEdgeFlagPointerEXT
void            APIENTRY PACKAGE sglEnable(GLenum cap); // TEXTURE_2D, BLEND, DEPTH_TEST, CULL_FACE, ALPHA_TEST only
void            APIENTRY PACKAGE sglEnableClientState (GLenum array);
void            APIENTRY PACKAGE sglEnd(void);
void            APIENTRY PACKAGE sglFinish(void);
void            APIENTRY PACKAGE sglFlush();
void            APIENTRY PACKAGE sglFrontFace(GLenum mode);
void            APIENTRY PACKAGE sglFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void            APIENTRY PACKAGE sglGenTextures (GLsizei n, GLuint *textures);
GLenum          APIENTRY PACKAGE sglGetError(void);
void            APIENTRY PACKAGE sglGetFloatv(GLenum pname, GLfloat *params);
void            APIENTRY PACKAGE sglGetIntegerv(GLenum pname, GLint *params);
// glGetPintervEXT
const GLubyte * APIENTRY PACKAGE sglGetString(GLenum name);
// glIndexPointerEXT
void            APIENTRY PACKAGE sglInterleavedArrays (GLenum format, GLsizei stride, const GLvoid *pointer);
GLboolean       APIENTRY PACKAGE sglIsTexture (GLuint texture);
void            APIENTRY PACKAGE sglLoadIdentity(void);
void            APIENTRY PACKAGE sglLoadMatrixf(const GLfloat *m);
void            APIENTRY PACKAGE sglMatrixMode(GLenum mode); // PROJECTION, MODELVIEW only
void            APIENTRY PACKAGE sglMultMatrixd(const GLdouble *m);
void            APIENTRY PACKAGE sglMultMatrixf(const GLfloat *m);
void            APIENTRY PACKAGE sglOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void            APIENTRY PACKAGE sglPixelStoref (GLenum pname, GLfloat param);
void            APIENTRY PACKAGE sglPixelStorei(GLenum pname, GLint param);
void            APIENTRY PACKAGE sglPolygonMode (GLenum face, GLenum mode);
void            APIENTRY PACKAGE sglPopMatrix(void);
void            APIENTRY PACKAGE sglPrioritizeTextures (GLsizei n, const GLuint *textures, const GLclampf *priorities);
void            APIENTRY PACKAGE sglPushMatrix(void);
void            APIENTRY PACKAGE sglReadBuffer (GLenum mode);
void            APIENTRY PACKAGE sglReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void            APIENTRY PACKAGE sglRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void            APIENTRY PACKAGE sglRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void            APIENTRY PACKAGE sglScaled (GLdouble x, GLdouble y, GLdouble z);
void            APIENTRY PACKAGE sglScalef(GLfloat x, GLfloat y, GLfloat z);
void            APIENTRY PACKAGE sglScissor (GLint x, GLint y, GLsizei width, GLsizei height);
void            APIENTRY PACKAGE sglShadeModel(GLenum mode);
void            APIENTRY PACKAGE sglTexCoord2f(GLfloat s, GLfloat t);
void            APIENTRY PACKAGE sglTexCoord2fv(const GLfloat *v);
// glTextCoordPointerEXT
void            APIENTRY PACKAGE sglTexEnvf(GLenum target, GLenum pname, GLfloat param); // MODULATE REPLACE, DECAL
void            APIENTRY PACKAGE sglTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
void            APIENTRY PACKAGE sglTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void            APIENTRY PACKAGE sglTexParameterf(GLenum target, GLenum pname, GLfloat param);
void            APIENTRY PACKAGE sglTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
void            APIENTRY PACKAGE sglTranslated(GLdouble x, GLdouble y, GLdouble z);
void            APIENTRY PACKAGE sglTranslatef(GLfloat x, GLfloat y, GLfloat z);
void            APIENTRY PACKAGE sglVertex2f(GLfloat x, GLfloat y);
void            APIENTRY PACKAGE sglVertex2fv (const GLfloat *v);
void            APIENTRY PACKAGE sglVertex3f(GLfloat x, GLfloat y, GLfloat z);
void            APIENTRY PACKAGE sglVertex3fv(const GLfloat *v);
// glVertexPointerEXT
void            APIENTRY PACKAGE sglViewport(GLint x, GLint y, GLsizei width, GLsizei height);




// STUFF NOT IN THE MINIGL:
#ifndef SUBST_MINIGLONLY
void            APIENTRY PACKAGE sglCallList(GLuint list);
void            APIENTRY PACKAGE sglColor3d(GLdouble red, GLdouble green, GLdouble blue);
void            APIENTRY PACKAGE sglDeleteLists(GLuint list,GLsizei range);
void            APIENTRY PACKAGE sglEndList();
GLuint          APIENTRY PACKAGE sglGenLists(GLsizei range);
void            APIENTRY PACKAGE sglGetDoublev(GLenum pname,GLdouble *params);
void            APIENTRY PACKAGE sglHint(GLenum target, GLenum mode);
void            APIENTRY PACKAGE sglInitNames();
GLboolean       APIENTRY PACKAGE sglIsEnabled(GLenum cap);
void            APIENTRY PACKAGE sglNewList(GLuint list, GLenum mode);
void            APIENTRY PACKAGE sglPolygonOffset(GLfloat factor, GLfloat units);
void            APIENTRY PACKAGE sglPopName();
void            APIENTRY PACKAGE sglPushName(GLuint name);
GLint           APIENTRY PACKAGE sglRenderMode(GLenum mode);
void            APIENTRY PACKAGE sglSelectBuffer(GLsizei size, GLuint *buffer);
void            APIENTRY PACKAGE sglTexCoord3f(GLfloat s, GLfloat t, GLfloat r);
void            APIENTRY PACKAGE sglTexCoord3fv(const GLfloat *v);
void            APIENTRY PACKAGE sglVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void            APIENTRY PACKAGE sglVertex3d(GLdouble x, GLdouble y,GLdouble z);
void            APIENTRY PACKAGE sglPushAttrib(GLbitfield mask);
void            APIENTRY PACKAGE sglPopAttrib();
void            APIENTRY PACKAGE sglVertex2i(GLint x, GLint y);
void            APIENTRY PACKAGE sglRectf(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2);
#endif




// Use these instead of the wgl* functions
HGLRC APIENTRY PACKAGE sglCreateContext(HDC hdc);
BOOL  APIENTRY PACKAGE sglDeleteContext(HGLRC hglrc);
BOOL  APIENTRY PACKAGE sglMakeCurrent(HDC hdc, HGLRC hglrc);
BOOL  APIENTRY PACKAGE sglSwapLayerBuffers(HDC hdc, UINT fuPlanes);

// use these functions instead of the GDI ones
BOOL  APIENTRY PACKAGE sglSwapBuffers(HDC hdc);
int   APIENTRY PACKAGE sglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR * ppfd);
int   APIENTRY PACKAGE sglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
BOOL  APIENTRY PACKAGE sglSetPixelFormat(HDC hdc, int iPixelFormat, CONST PIXELFORMATDESCRIPTOR * ppfd);



GLint APIENTRY PACKAGE sgluUnProject(GLdouble winx,GLdouble winy,GLdouble winz,
                            const GLdouble model[16],const GLdouble proj[16],
                            const GLint viewport[4],
                            GLdouble *objx,GLdouble *objy,GLdouble *objz);
void APIENTRY PACKAGE sgluPickMatrix( GLdouble x, GLdouble y,
                             GLdouble width, GLdouble height,
                             const GLint viewport[4] );
void APIENTRY PACKAGE sgluPerspective( GLdouble fovy, GLdouble aspect,
                              GLdouble zNear, GLdouble zFar );
void APIENTRY PACKAGE sgluLookAt( GLdouble eyex, GLdouble eyey, GLdouble eyez,
                         GLdouble centerx, GLdouble centery, GLdouble centerz,
                         GLdouble upx, GLdouble upy, GLdouble upz );
void APIENTRY PACKAGE sgluOrtho2D( GLdouble left, GLdouble right,
                          GLdouble bottom, GLdouble top );


// MINIGL COMMENTS
// Ryan's web site (http://home.bc.rogers.wave.ca/borealis/ryan.html) has a list
// of those calls which are supported by the MiniGL. I have taken the list from
// that.
//
// The following calls are part of the MiniGL but have not been implemented.
// If you try to use them, you will get a linker error.
// sglAreTexturesResident
// sglArrayElementEXT
// sglColor4ub, glColor4ubv
// sglColorPointerEXT
// sglDepthMask
// sglDepthRange
// sglDisableClientState
// sglDrawArraysEXT
// sglDrawBuffer
// sglDrawElements
// sglEdgeFlagPointerEXT
// sglEnableClientState
// sglGenTextures
// sglGetPintervEXT
// sglIndexPointerEXT
// sglIsTexture
// sglInterleavedArrays
// sglPixelStoref
// sglPolygonMode
// sglPrioritize
// sglReadBuffer
// sglReadPixels
// sglRotated
// sglScaled
// sglScissor
// sglTextCoordPointerEXT
// sglTexEnvfv
// sglTexParameterfv
// sglVertex2fv
// sglVertexPointerEXT


#endif