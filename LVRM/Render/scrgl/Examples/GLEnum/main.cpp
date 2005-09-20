//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "main.h"
#include "prof.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scrgl"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------




__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{ 
}
void __fastcall TMainForm::FormCreate(TObject *Sender)
{  Set8087CW(0x133f);  // reprogram the FPU so it doesn't raise exceptions
  SetEnvironmentVariable("FX_GLIDE_NO_SPLASH","1"); // stop 3dfx from displaying a spinning logo
  SglDriverList->Search->Insert(0,"c:\\progra~1\\3dfx\\opengl\\3dfxopengl.dll");
  DriverList->Lines=SglDriverList->Search;
  WNDCLASS   wndclass;
  wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wndclass.lpfnWndProc   = (WNDPROC)DefWindowProc;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = HInstance;
  wndclass.hIcon         = NULL;
  wndclass.hCursor       = LoadCursor (NULL,IDC_ARROW);
  wndclass.hbrBackground = NULL;
  wndclass.lpszMenuName  = NULL;
  wndclass.lpszClassName = "LuglWindow";
  ATOM res=RegisterClass(&wndclass);
  if (!res) Application->MessageBox("Error registering class","Error",MB_OK);
  //
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RefreshClick(TObject *Sender)
{ SglDriverList->Search=DriverList->Lines;
  Draw();
}
//---------------------------------------------------------------------------
AnsiString __fastcall StringFromFlags(DWORD d)
{ AnsiString s="";
  if (d&PFD_DRAW_TO_WINDOW) s=s+"draw-to-window ";
  if (d&PFD_DRAW_TO_BITMAP) s=s+"draw-to-bitmap ";
  if (d&PFD_SUPPORT_GDI) s=s+"support-gdi ";
  if (d&PFD_SUPPORT_OPENGL) s=s+"support-opengl ";
  if (d&PFD_GENERIC_ACCELERATED) s=s+"generic-acc ";
  if (d&PFD_GENERIC_FORMAT) s=s+"generic-fmt ";
  if (d&PFD_NEED_PALETTE) s=s+"need-pal ";
  if (d&PFD_NEED_SYSTEM_PALETTE) s=s+"need-sys-pal ";
  if (d&PFD_DOUBLEBUFFER) s=s+"dblbuff ";
  if (d&PFD_STEREO) s=s+"stereo ";
  if (d&PFD_SWAP_LAYER_BUFFERS) s=s+"swap-layer-bufs ";
  return s;
}

AnsiString __fastcall AccFromFlags(DWORD d)
{ bool genacc= (d&PFD_GENERIC_ACCELERATED)!=0;
  bool genfmt= (d&PFD_GENERIC_FORMAT)!=0;
  if (!genfmt && !genacc) return "ICD";
  if (!genfmt &&  genacc) return "???";
  if ( genfmt && !genacc) return "software";
  if ( genfmt &&  genacc) return "MCD";
  return "";
}

AnsiString __fastcall PixelTypeFromString(BYTE i)
{ if (i==PFD_TYPE_RGBA) return "RGBA";
  if (i==PFD_TYPE_COLORINDEX) return "COLORINDEX";
  return "?";
}


void __fastcall TMainForm::Draw()
{ TSglDriverStrings *s=SglDriverList->Items;
  Grid->RowCount=500;
  Grid->ColCount=35;
  Grid->FixedRows=1;
  Grid->FixedCols=0;
  TStringList *sl=new TStringList();
  sl->Add("Works?");
  sl->Add("FileName");
  sl->Add("CompanyName");
  sl->Add("FileDescription");
  sl->Add("FileVersion");
  sl->Add("3dfx?");
  sl->Add("nVersion");
  sl->Add("dwFlags");
  sl->Add("Acceleration");
  sl->Add("iPixelType");
  sl->Add("cColorBits");
  sl->Add("cRedBits");
  sl->Add("cRedShift");
  sl->Add("cGreenBits");
  sl->Add("cGreenShift");
  sl->Add("cBlueBits");
  sl->Add("cBlueShift");
  sl->Add("cAlphaBits");
  sl->Add("cAlphaShift");
  sl->Add("cAccumBits");
  sl->Add("cAccumRedBits");
  sl->Add("cAccumGreenBits");
  sl->Add("cAccumBlueBits");
  sl->Add("cAccumAlphaBits");
  sl->Add("cDepthBits");
  sl->Add("cStencilBits");
  sl->Add("cAuxBuffers");
  sl->Add("iLayerType");
  sl->Add("dwLayerMask");
  sl->Add("dwVisibleMask");
  sl->Add("dwDamageMask");
  sl->Add("VENDOR");
  sl->Add("RENDERER");
  sl->Add("VERSION");
  sl->Add("EXTENSIONS");
  Grid->Rows[0]=sl;
  delete sl;
  TCursor oldc=Screen->Cursor; Screen->Cursor=crHourGlass;
  TProgressForm *prog=new TProgressForm(this); Enabled=false; prog->Show();
  int row=1;
  for (int driver=0; driver<s->Count && !prog->FCancelled; driver++)
  { TSglDriverDetails *item=s->Objects[driver];
    HWND hwnd=CreateWindowEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW,"LuglWindow","Lugl",
      WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
      0, 0, 0,0,
      GetDesktopWindow(),NULL,HInstance,NULL);
    HDC hdc=GetDC(hwnd);
    Application->ProcessMessages(); Sleep(200);
    prog->Label->Caption="Loading "+ExtractFileName(item->FileName)+"...";
    sglLoadLibrary(item->FileName);
    PIXELFORMATDESCRIPTOR pfd; ZeroMemory(&pfd,sizeof(pfd)); pfd.nSize=sizeof(pfd); pfd.nVersion=1;
    int num=sglDescribePixelFormat(hdc,1,sizeof(pfd),&pfd);
    ReleaseDC(hwnd,hdc);
    DestroyWindow(hwnd);
    for (int p=1; p<=num && !prog->FCancelled; p++)
    { HWND hwnd=CreateWindowEx(0,"LuglWindow","Lugl",
        WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
        0, 0, TestPanel->Width, TestPanel->Height,
        TestPanel->Handle,NULL,HInstance,NULL);
      HDC hdc=GetDC(hwnd);
      PIXELFORMATDESCRIPTOR pfd; ZeroMemory(&pfd,sizeof(pfd)); pfd.nSize=sizeof(pfd); pfd.nVersion=1;
      sglDescribePixelFormat(hdc,p,sizeof(pfd),&pfd);
      BOOL MadeCurrent=FALSE; HGLRC hglrc=NULL;
      BOOL SetFormat=sglSetPixelFormat(hdc,p,&pfd);
      AnsiString works;
      if (!SetFormat) works="NO-setpfdX";
      else
      { hglrc=sglCreateContext(hdc);
        if (hglrc==NULL) works="NO-createX";
        { MadeCurrent=sglMakeCurrent(hdc,hglrc);
          if (!MadeCurrent) works="NO-makecurX";
          else works="Yes!";
        }
      }
      Application->ProcessMessages(); Sleep(200);
      TStringList *sl=new TStringList();
      sl->Add(works);
      sl->Add(item->FileName);
      sl->Add(item->CompanyName);
      sl->Add(item->FileDescription);
      sl->Add(item->FileVersion);
      sl->Add( (item->Uses3dfxStyle ? "3dfx" : "-"));
      sl->Add( AnsiString((int)pfd.nVersion));
      sl->Add( StringFromFlags(pfd.dwFlags));
      sl->Add( AccFromFlags(pfd.dwFlags));
      sl->Add( PixelTypeFromString(pfd.iPixelType));
      sl->Add( AnsiString((int)pfd.cColorBits));
      sl->Add( AnsiString((int)pfd.cRedBits));
      sl->Add( AnsiString((int)pfd.cRedShift));
      sl->Add( AnsiString((int)pfd.cGreenBits));
      sl->Add( AnsiString((int)pfd.cGreenShift));
      sl->Add( AnsiString((int)pfd.cBlueBits));
      sl->Add( AnsiString((int)pfd.cBlueShift));
      sl->Add( AnsiString((int)pfd.cAlphaBits));
      sl->Add( AnsiString((int)pfd.cAlphaShift));
      sl->Add( AnsiString((int)pfd.cAccumBits));
      sl->Add( AnsiString((int)pfd.cAccumRedBits));
      sl->Add( AnsiString((int)pfd.cAccumGreenBits));
      sl->Add( AnsiString((int)pfd.cAccumBlueBits));
      sl->Add( AnsiString((int)pfd.cAccumAlphaBits));
      sl->Add( AnsiString((int)pfd.cDepthBits));
      sl->Add( AnsiString((int)pfd.cStencilBits));
      sl->Add( AnsiString((int)pfd.cAuxBuffers));
      sl->Add( AnsiString((int)pfd.iLayerType));
      sl->Add( "0x"+IntToHex((int)pfd.dwLayerMask,8));
      sl->Add( "0x"+IntToHex((int)pfd.dwVisibleMask,8));
      sl->Add( "0x"+IntToHex((int)pfd.dwDamageMask,8));
      sl->Add( AnsiString((char*)sglGetString(GL_VENDOR)));
      sl->Add( AnsiString((char*)sglGetString(GL_RENDERER)));
      sl->Add( AnsiString((char*)sglGetString(GL_VERSION)));
      sl->Add( AnsiString((char*)sglGetString(GL_EXTENSIONS)));
      Grid->Rows[row]=sl;
      delete sl;
      AnsiString desc=ExtractFileName(item->FileName)+" - "+AccFromFlags(pfd.dwFlags)+" "+AnsiString((char*)sglGetString(GL_RENDERER))+" "+(int)pfd.cColorBits+"bpp "+(int)pfd.cDepthBits+"depth";
      prog->Label->Caption=desc;
      if (MadeCurrent) sglMakeCurrent(NULL,NULL);
      if (hglrc!=NULL) sglDeleteContext(hglrc);
      ReleaseDC(hwnd,hdc);
      DestroyWindow(hwnd);
      row++;
      prog->ProgressBar->Position=(row % (prog->ProgressBar->Max-1))+1;
    }
    sglFreeLibrary();
  }
  Grid->RowCount=row;
  delete prog;
  Enabled=true;
  Screen->Cursor=oldc;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::btnSaveClick(TObject *Sender)
{ bool res=SaveDialog->Execute();
  if (!res) return;
  AnsiString fn=SaveDialog->FileName;
  TStringList *fs=new TStringList();
  for (int r=0; r<Grid->RowCount; r++)
  { AnsiString s="";
    for (int c=0; c<34; c++)
    { AnsiString t=Grid->Cells[c][r];
      for (int i=1; i<=t.Length(); i++) {if (t[i]==',') t[i]='-';}
      s=s+t+",";
    }
    fs->Add(s);
  }
  fs->SaveToFile(fn);
  delete fs;
}
//---------------------------------------------------------------------------

