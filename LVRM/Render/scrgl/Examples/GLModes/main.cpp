//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scrgl"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------

// MsgProc -- we set this up as a callback. All messages
// from the SglPanel package will get sent here.
void __fastcall MsgProc(AnsiString s,void*ctxt)
{ TStrings *strings = (TStrings*)ctxt;
  strings->Add(s);
  OutputDebugString(s.c_str());
}

__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{ SglSetMessageProc(MsgProc,MsgProc,Log->Lines);
  RotX=0; RotY=0; RotZ=0;
  GenerateModeList();
}
__fastcall TMainForm::~TMainForm()
{ SglSetMessageProc(NULL,NULL,NULL);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SglPanelGLInit(TObject *Sender)
{ sglClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  sglClearDepth(1.0);
  sglDepthFunc(GL_LEQUAL);  // Superimpose smaller Z values over larger ones
  sglShadeModel(GL_SMOOTH); // Smooth shading
  sglFrontFace(GL_CCW);
  sglPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  sglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  sglAlphaFunc(GL_GEQUAL, 0.07f);
  sglDisable(GL_ALPHA_TEST);
  sglDisable(GL_BLEND);
  sglDisable(GL_CULL_FACE);
  sglEnable(GL_DEPTH_TEST); // Z buffering is go...
  PrepareProjection();
}
void __fastcall TMainForm::PrepareProjection()
{ int width=SglPanel->Width, height=SglPanel->Height;
  double  znear = 1;     // Put the screen 1 unit in front of the "eye"
  double  zfar  = 50.0;  // It's good to have a small far/near ratio
  double  aspect = ((double)width)/((double)height);
  double  fovX = 15.0;
  sglViewport(0, 0, width, height);
  sglMatrixMode(GL_PROJECTION);
  sglLoadIdentity();
  sgluPerspective(fovX/aspect, aspect, znear, zfar);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Timer1Timer(TObject *Sender)
{ RotX += 5; RotY += 5; RotZ += 5;
  if (SglPanel!=NULL) SglPanel->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SglPanelGLPaint(TObject *Sender)
{ sglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Wipe the color and Depth Buffers
  sglMatrixMode(GL_MODELVIEW); sglPushMatrix();
  sglLoadIdentity();
  sgluLookAt(0,0,-10,  0,0,0,  0,1,0);  // put eye at (0,0,-10) looking at (0,0,0)
  sglRotatef(RotX, 1.0f, 0.0f, 0.0f); // rotate about the x unit vector
  sglRotatef(RotY, 0.0f, 1.0f, 0.0f); // rotate about the y unit vector
  sglRotatef(RotZ, 0.0f, 0.0f, 1.0f); // rotate about the z unit vector
  sglBegin(GL_QUADS);
    // Six faces of a cube to draw
    sglColor3f(0.0f, 0.0f, 0.0f); sglVertex3f(-1.0f, -1.0f, -1.0f);
    sglColor3f(0.0f, 1.0f, 0.0f); sglVertex3f(-1.0f,  1.0f, -1.0f);
    sglColor3f(1.0f, 1.0f, 0.0f); sglVertex3f( 1.0f,  1.0f, -1.0f);
    sglColor3f(1.0f, 0.0f, 0.0f); sglVertex3f( 1.0f, -1.0f, -1.0f);
    //
    sglColor3f(1.0f, 0.0f, 1.0f); sglVertex3f( 1.0f, -1.0f,  1.0f);
    sglColor3f(1.0f, 1.0f, 1.0f); sglVertex3f( 1.0f,  1.0f,  1.0f);
    sglColor3f(0.0f, 1.0f, 1.0f); sglVertex3f(-1.0f,  1.0f,  1.0f);
    sglColor3f(0.0f, 0.0f, 1.0f); sglVertex3f(-1.0f, -1.0f,  1.0f);
    //
    sglColor3f(0.0f, 1.0f, 0.0f); sglVertex3f(-1.0f,  1.0f, -1.0f);
    sglColor3f(0.0f, 1.0f, 1.0f); sglVertex3f(-1.0f,  1.0f,  1.0f);
    sglColor3f(1.0f, 1.0f, 1.0f); sglVertex3f( 1.0f,  1.0f,  1.0f);
    sglColor3f(1.0f, 1.0f, 0.0f); sglVertex3f( 1.0f,  1.0f, -1.0f);
    //
    sglColor3f(1.0f, 0.0f, 0.0f); sglVertex3f( 1.0f, -1.0f, -1.0f);
    sglColor3f(1.0f, 0.0f, 1.0f); sglVertex3f( 1.0f, -1.0f,  1.0f);
    sglColor3f(0.0f, 0.0f, 1.0f); sglVertex3f(-1.0f, -1.0f,  1.0f);
    sglColor3f(0.0f, 0.0f, 0.0f); sglVertex3f(-1.0f, -1.0f, -1.0f);
    //
    sglColor3f(0.0f, 0.0f, 1.0f); sglVertex3f(-1.0f, -1.0f,  1.0f);
    sglColor3f(0.0f, 1.0f, 1.0f); sglVertex3f(-1.0f,  1.0f,  1.0f);
    sglColor3f(0.0f, 1.0f, 0.0f); sglVertex3f(-1.0f,  1.0f, -1.0f);
    sglColor3f(0.0f, 0.0f, 0.0f); sglVertex3f(-1.0f, -1.0f, -1.0f);
    //
    sglColor3f(1.0f, 0.0f, 0.0f); sglVertex3f( 1.0f, -1.0f, -1.0f);
    sglColor3f(1.0f, 1.0f, 0.0f); sglVertex3f( 1.0f,  1.0f, -1.0f);
    sglColor3f(1.0f, 1.0f, 1.0f); sglVertex3f( 1.0f,  1.0f,  1.0f);
    sglColor3f(1.0f, 0.0f, 1.0f); sglVertex3f( 1.0f, -1.0f,  1.0f);
  sglEnd();
  sglPopMatrix();
  SglPanel->SwapBuffers();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::x3dfxClick(TObject *Sender)
{ GenerateModeList();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::xIgnoreBppClick(TObject *Sender)
{ GenerateModeList();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SglPanelGLError(TObject *Sender, AnsiString err)
{ Application->MessageBox(err.c_str(),"Error",MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ModeListClick(TObject *Sender)
{ int i=ModeList->ItemIndex; if (i==-1) return;
  AnsiString s=ModeList->Items->Strings[i];
  char *c=s.c_str();
  AnsiString xs="", ys="", bs=""; i=1;
  while (*c!='x' && *c!=0)
  { xs=xs+AnsiString(*c);
    c++;
  }
  if (*c=='x') c++;
  while (*c!='x' && *c!=0) {ys=ys+AnsiString(*c); c++;} if (*c=='x') c++;
  while (*c!='x' && *c!=0) {bs=bs+AnsiString(*c); i++;}
  int x,y,b=-1;
  try
  { x=xs.Trim().ToInt(); y=ys.Trim().ToInt();
    if (bs.Trim().Length()==0) b=-1; else b=bs.Trim().ToInt();
  }
  catch (...) {return;}
  if (SglPanel!=NULL) SglPanel->SetScreenDimensions(x,y,b);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::GenerateModeList()
{ ModeList->Clear();
  if (x3dfx->Checked)
  { AnsiString s=""; if (!xIgnoreBpp->Checked) s="x16";
    ModeList->Items->Add("512x384"+s);
    ModeList->Items->Add("640x480"+s);
    ModeList->Items->Add("800x600"+s);
    ModeList->Items->Add("1024x768"+s);
    return;
  }
  int width[1000],height[1000],bpp[1000]; int count=0;
  int i=0; for (BOOL res=true; res; i++)
  { DEVMODE dm; ZeroMemory(&dm,sizeof(dm));
    res=EnumDisplaySettings(NULL,i,&dm);
    if (res)
    { bool add=true;
      for (int i=0; i<count; i++)
      { if (xIgnoreBpp->Checked||x3dfx->Checked)
        { if (width[i]==(int)dm.dmPelsWidth && height[i]==(int)dm.dmPelsHeight) add=false;
        }
        else
        { if (width[i]==(int)dm.dmPelsWidth && height[i]==(int)dm.dmPelsHeight && bpp[i]==(int)dm.dmBitsPerPel) add=false;
        }
      }
      if (add)
      { width[count]=dm.dmPelsWidth;
        height[count]=dm.dmPelsHeight;
        bpp[count]=dm.dmBitsPerPel;
        count++;
      }
    }
  }
  for (int i=0; i<count; i++)
  { int w=width[i], h=height[i], b=bpp[i];
    AnsiString s=AnsiString("")+w+"x"+h;
    if (!xIgnoreBpp->Checked) s=s+"x"+b;
    ModeList->Items->Add(s);
  }
}
//---------------------------------------------------------------------------


