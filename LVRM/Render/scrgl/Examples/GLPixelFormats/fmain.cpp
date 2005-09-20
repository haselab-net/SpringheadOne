//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <gl\gl.h>
#include <gl\glu.h>

#include "fmain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{ RotX=0.0; RotY=0.0; RotZ=0.0;
  btnList->Click();
  btnCreate->Click();
}
__fastcall TMainForm::~TMainForm()
{ btnDestroy->Click();
}
void __fastcall TMainForm::WndProc(TMessage &msg)
{ if (msg.Msg==WM_SETTINGCHANGE) {btnDestroy->Click(); btnList->Click();btnCreate->Click();}
  TForm::WndProc(msg);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::btnListClick(TObject *Sender)
{ HWND hwnd=Frame->Handle;
  HDC hdc=GetDC(hwnd);
  PIXELFORMATDESCRIPTOR pfd;
  int num=DescribePixelFormat(hdc,1,sizeof(pfd),&pfd);
  int maxqual=0, maxindex=0;
  ListBox->Items->BeginUpdate();
  ListBox->Clear();
  for (int i=0; i<num; i++)
  { DescribePixelFormat(hdc,i,sizeof(pfd),&pfd);
    int bpp=pfd.cColorBits;
    int depth=pfd.cDepthBits;
    bool pal=(pfd.iPixelType==PFD_TYPE_COLORINDEX);
    bool mcd=((pfd.dwFlags & PFD_GENERIC_FORMAT) && (pfd.dwFlags & PFD_GENERIC_ACCELERATED));
    bool soft=((pfd.dwFlags & PFD_GENERIC_FORMAT) && !(pfd.dwFlags & PFD_GENERIC_ACCELERATED));
    bool icd=(!(pfd.dwFlags & PFD_GENERIC_FORMAT) && !(pfd.dwFlags & PFD_GENERIC_ACCELERATED));
    bool opengl=(pfd.dwFlags & PFD_SUPPORT_OPENGL);
    bool window=(pfd.dwFlags & PFD_DRAW_TO_WINDOW);
    bool bitmap=(pfd.dwFlags & PFD_DRAW_TO_BITMAP);
    bool dbuff=(pfd.dwFlags & PFD_DOUBLEBUFFER);
    //
    AnsiString s="";
    s=s+AnsiString(bpp)+"bpp ";
    s=s+AnsiString(depth)+"depth ";
    if (pal) s=s+"pal ";
    if (!opengl) s=s+"X";
    if (window) s=s+"W";
    if (bitmap) s=s+"B";
    if (dbuff) s=s+"D";
    s=s+" ";
    if (mcd) s=s+"MCD ";
    if (icd) s=s+"ICD ";
    if (soft) s=s+"software ";
    //
    int qual=0;
    if (bpp>=16) qual=qual+100; if (bpp>=24) qual=qual+2; if (bpp>=32) qual=qual+2;
    if (depth>=16) qual=qual+10; if (depth>=24) qual=qual-2; if (depth>=32) qual=qual-2;
    if (!pal) qual=qual+50;
    if (window) qual=qual+500;
    if (dbuff) qual=qual+50;
    if (icd) qual=qual+100;
    if (mcd) qual=qual+90;
    //
    int pos=ListBox->Items->Add(s);
    if (qual>maxqual) {maxqual=qual;maxindex=pos;}
  }
  ListBox->Items->Add("--");
  ListBox->Items->Add("MCD: acc. Mini Client Driver");
  ListBox->Items->Add("ICD: acc. Installable Client Driver");
  ListBox->Items->Add("W: support-window");
  ListBox->Items->Add("B: support-bitmap");
  ListBox->Items->Add("D: double-buffered");
  ListBox->ItemIndex=maxindex;
  ListBox->Items->EndUpdate();
  ReleaseDC(hwnd,hdc);
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::btnCreateClick(TObject *Sender)
{ btnDestroy->Click();
  TPanel *p=new TPanel(this); p->Parent=Frame; p->Align=alClient; p->OnMouseDown=RenderMouseDown;
  render=p;
  hdc=GetDC(render->Handle);
  int pf=ListBox->ItemIndex; if (pf<1) pf=1;
  PIXELFORMATDESCRIPTOR pfd; ZeroMemory(&pfd,sizeof(pfd));
  DescribePixelFormat(hdc,pf,sizeof(pfd),&pfd);
  BOOL res=SetPixelFormat(hdc,pf,&pfd);
  if (!res) Application->MessageBox("Failed to set pixel format","Error",MB_OK);
  HGLRC hglrc=wglCreateContext(hdc);
  if (hglrc==NULL && res) Application->MessageBox("Failed to create context","Error",MB_OK);
  BOOL res2=wglMakeCurrent(hdc,hglrc);
  if (res && hglrc!=NULL && !res2) Application->MessageBox("Failed to make current","Error",MB_OK);
  //
  InitGL();
  Timer->Enabled=true;
}
void __fastcall TMainForm::InitGL()
{ glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);  // Superimpose smaller Z values over larger ones
  glShadeModel(GL_SMOOTH); // Smooth shading
  glFrontFace(GL_CCW);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glAlphaFunc(GL_GEQUAL, 0.07f);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST); // Z buffering is go...
  PrepareProjection();
}
void __fastcall TMainForm::PrepareProjection()
{ int width=render->Width, height=render->Height;
  double  znear = 1;     // Put the screen 1 unit in front of the "eye"
  double  zfar  = 50.0;  // It's good to have a small far/near ratio
  double  aspect = ((double)width)/((double)height);
  double  fovX = 15.0;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovX/aspect, aspect, znear, zfar);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TimerTimer(TObject *Sender)
{ HGLRC hglrc=wglGetCurrentContext();
  if (hglrc==NULL) return;
  RotX += 5; RotY += 5; RotZ += 5;
  DrawGL();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DrawGL()
{ glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Wipe the color and Depth Buffers
  glMatrixMode(GL_MODELVIEW); glPushMatrix();
  glLoadIdentity();
  gluLookAt(0,0,-10,  0,0,0,  0,1,0);  // put eye at (0,0,-10) looking at (0,0,0)
  glRotatef(RotX, 1.0f, 0.0f, 0.0f); // rotate about the x unit vector
  glRotatef(RotY, 0.0f, 1.0f, 0.0f); // rotate about the y unit vector
  glRotatef(RotZ, 0.0f, 0.0f, 1.0f); // rotate about the z unit vector
  glBegin(GL_QUADS);
    // Six faces of a cube to draw
    glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    //
    glColor3f(1.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    //
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    //
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glColor3f(1.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    //
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    //
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glColor3f(1.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
  glEnd();
  glPopMatrix();
  SwapBuffers(hdc);
}


void __fastcall TMainForm::btnDestroyClick(TObject *Sender)
{ if (render==NULL) return;
  HGLRC hglrc=wglGetCurrentContext();
  if (hglrc!=NULL)
  { wglMakeCurrent(NULL,NULL);
    wglDeleteContext(hglrc);
  }
  ReleaseDC(render->Handle,hdc); hdc=NULL;
  delete render; render=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ListBoxClick(TObject *Sender)
{ btnCreate->Click();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormResize(TObject *Sender)
{ if (render==NULL) return;
  PrepareProjection();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RenderMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{ TPanel *p=new TPanel(this);
  p->Parent=render;
  p->Left=X; p->Top=Y; p->Width=50; p->Height=50;
  p->OnMouseDown=PanelMouseDown;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PanelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{ delete Sender;
}
//---------------------------------------------------------------------------

