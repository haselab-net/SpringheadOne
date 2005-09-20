#include <vcl.h>
#pragma hdrstop
#include <gl\gl.h>
#include <math.h>
#include <float.h>
#include "ScrGL.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

namespace Scrgl
{
unsigned int debugStartTime=0;
SGLDEBUG debugExternMsgProc=NULL;
SGLDEBUG debugExternErrProc=NULL;
void* debugContext;
}
enum TDebugLevel {dlMsg,dlErr};


void __fastcall Scrgl::SetMessageProc(SGLDEBUG msgproc,SGLDEBUG errproc,void*ctxt)
{ Scrgl::debugExternMsgProc=msgproc;
  Scrgl::debugExternErrProc=errproc;
  Scrgl::debugContext=ctxt;
}

inline void __fastcall Debug(TDebugLevel etl,AnsiString s)
{ if (Scrgl::debugStartTime==0) Scrgl::debugStartTime=GetTickCount();
  if (etl==dlMsg && Scrgl::debugExternMsgProc==NULL) return;
  if (etl==dlErr && Scrgl::debugExternErrProc==NULL) return;
  unsigned int CurrentTime=GetTickCount()-Scrgl::debugStartTime;
  AnsiString t="sgl:"+AnsiString((int)CurrentTime)+" - "+s;
  if (etl==dlMsg) Scrgl::debugExternMsgProc(t,Scrgl::debugContext);
  else Scrgl::debugExternErrProc(t,Scrgl::debugContext);
}



HINSTANCE sglInst = NULL;
AnsiString sglFile = "";
bool sglBypassGDI = false;


AnsiString __fastcall GetLastErrorString()
{ LPVOID lpMsgBuf;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),
    MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&lpMsgBuf,0,NULL);
  AnsiString s=AnsiString((char*)lpMsgBuf);
  LocalFree(lpMsgBuf);
  return s;
}



AnsiString __fastcall ExtractDirFromFilePath(AnsiString s)
{ AnsiString t=ExtractFilePath(s);
  if (t[t.Length()]=='\\') t=t.SubString(1,t.Length()-1);
  return t;
}

// GLDriver - Just a single string. Setting it will deduce whether it uses full style
// GLFullMode - One of 512x384, 640x480, 800x600, 1024x768
// If the driver turned out to be a 3dfx driver, then we will ignore
// the current width, height, align and go into full-screen mode.
// Otherwise we will respect the current dimensions.


__fastcall TSglDriverDetails::TSglDriverDetails()
{ FileName=""; CompanyName=""; FileDescription=""; FileVersion="";
  Uses3dfxStyle=false;
}



bool __fastcall GetDriverDetails(AnsiString afn,TSglDriverDetails *d)
{ char *fn=afn.c_str();
  // It would be nice to load each of them just to check that they were actually
  // opengl dlls. But this gives an error in some cases.
  // It's not wise to load a hardware driver if you don't actually have that
  // hardware on your machine, and don't intend to use it!
  d->FileName=afn;
  d->CompanyName="";
  d->FileDescription="";
  d->FileVersion="";
  d->Uses3dfxStyle=false;
  DWORD dummy; DWORD vis=GetFileVersionInfoSize(fn,&dummy);
  if (vis==0) return true;
  void *vData;
  vData=(void *)new char[(UINT)vis];
  if (!GetFileVersionInfo(fn,dummy,vis,vData)) {delete vData;return true;}
  char vn[100];
  strcpy(vn,"\\VarFileInfo\\Translation");
  LPVOID transblock; UINT vsize;
  BOOL res=VerQueryValue(vData,vn,&transblock,&vsize);
  if (!res) {delete vData;return true;}
  // Swap the words so wsprintf will print the lang-charset in the correct format.
  DWORD *dw=(DWORD*)transblock; DWORD dwhi=HIWORD(*dw); DWORD dwlo=LOWORD(*dw); *dw = dwhi | (dwlo << 16);
  char *info;
  info="CompanyName"; wsprintf(vn,"\\StringFileInfo\\%08lx\\%s",*(DWORD *)transblock,info);
  char *CompanyName=NULL; VerQueryValue(vData,vn,(LPVOID*)&CompanyName,&vsize);
  info="FileDescription"; wsprintf(vn,"\\StringFileInfo\\%08lx\\%s",*(DWORD *)transblock,info);
  char *FileDescription=NULL; VerQueryValue(vData,vn,(LPVOID*)&FileDescription,&vsize);
  info="FileVersion"; wsprintf(vn,"\\StringFileInfo\\%08lx\\%s",*(DWORD *)transblock,info);
  char *FileVersion=NULL; VerQueryValue(vData,vn,(LPVOID*)&FileVersion,&vsize);
  d->CompanyName=CompanyName;
  d->FileDescription=FileDescription;
  d->FileVersion=FileVersion;
  delete vData;
  AnsiString de=d->FileDescription, ce=d->CompanyName;
  int i1=de.Pos(ce);
  if (i1!=0)
  { int i2=i1+ce.Length();
    de=de.SubString(1,i1-1)+de.SubString(i2+1,de.Length()-i2-1);
    d->FileDescription=de;
  }
  AnsiString b=d->CompanyName+" "+d->FileDescription+" "+d->FileVersion;
  b=b.LowerCase();
  int pos=b.Pos("3dfx"); d->Uses3dfxStyle=(pos!=0);
  return true;
}

bool __fastcall sglIsDriver3dfx(AnsiString fn)
{ TSglDriverDetails *d=new TSglDriverDetails();
  bool res=GetDriverDetails(fn,d);
  bool u=false; if (res) u=d->Uses3dfxStyle;
  delete d;
  return u;
}


__fastcall TSglDriverStrings::TSglDriverStrings() : TStringList() {}
TSglDriverDetails* __fastcall TSglDriverStrings::GetObject(int i)
{ return (TSglDriverDetails*)TStringList::GetObject(i);
}
__fastcall TSglDriverStrings::~TSglDriverStrings()
{ for (int i=0; i<Count; i++) Objects[i]=NULL;
}
void __fastcall TSglDriverStrings::Delete(int i)
{ Objects[i]=NULL;
  TStringList::Delete(i);
}
void __fastcall TSglDriverStrings::Clear()
{ for (int i=0; i<Count; i++) Objects[i]=NULL;
  TStringList::Clear();
}
void __fastcall TSglDriverStrings::SetObject(int i,TSglDriverDetails *newd)
{ TSglDriverDetails *d=Objects[i];
  if (d!=NULL) delete d;
  TStringList::PutObject(i,newd);
}
int __fastcall TSglDriverStrings::IndexOfFileName(AnsiString fn)
{ fn=fn.UpperCase();
  AnsiString efn=ExtractFileName(fn);
  for (int i=0; i<Count; i++)
  { TSglDriverDetails *d=Objects[i];
    AnsiString icdfn=d->FileName.UpperCase();
    AnsiString eicdfn=ExtractFileName(d->FileName);
    if (fn==icdfn || efn==eicdfn) return i;
  }
  return -1;
}


class PACKAGE TSglDriverShieldedStringList : public TStringList
{
public:
  __fastcall TSglDriverShieldedStringList(TNotifyEvent aChange) {FScrChange=aChange;}
protected:
  TNotifyEvent FScrChange;
  void __fastcall Changed()
  { TStringList::Changed();
    if (FScrChange!=NULL) FScrChange(this);
  }
};



__fastcall TSglDriverList::TSglDriverList(TComponent *Owner) : TComponent(Owner)
{ FDrivers=new TSglDriverStrings(); FDriversValid=false;
  FSearchPath=new TSglDriverShieldedStringList(&SearchPathChanged);
  FSearchPath->Duplicates=dupIgnore;
  FSearchPath->Add("<project>\\opengl*.dll");
  FSearchPath->Add("<project>\\3dfx*gl*.dll");
  FSearchPath->Add("<windows>\\opengl*.dll");
  FSearchPath->Add("<system>\\opengl*.dll");
}
__fastcall TSglDriverList::~TSglDriverList()
{ if (FDrivers!=NULL) delete FDrivers; FDrivers=NULL;
  if (FSearchPath!=NULL) delete FSearchPath; FSearchPath=NULL;
}
TStrings* __fastcall TSglDriverList::GetSearchPath() {return FSearchPath;}
void __fastcall TSglDriverList::SetSearchPath(TStrings *s) {FSearchPath->Assign(s);}
void __fastcall TSglDriverList::SearchPathChanged(TObject *Sender)
{ FDriversValid=false;
}

AnsiString GlobalVendor="", GlobalRenderer="", GlobalVersion="";
bool GlobalVersionSucc=false, GlobalVersionTried=false;

void __fastcall sglGetGlobalVersion()
{ if (GlobalVersionTried) return;
  if (sglInst!=NULL) return;
  Debug(dlMsg,"Temporarily loading opengl32 to find its vendor information");
  sglLoadLibrary("opengl32.dll");
  WNDCLASS   wndclass; ZeroMemory(&wndclass,sizeof(wndclass));
  BOOL res=GetClassInfo(HInstance,"ScrGL.OpenGL32.PixelFormat",&wndclass);
  if (res==0)
  { wndclass.style         = CS_OWNDC;
    wndclass.lpfnWndProc   = DefWindowProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = HInstance;
    wndclass.hIcon         = NULL;
    wndclass.hCursor       = LoadCursor (NULL,IDC_ARROW);
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = "ScrGL.OpenGL32.PixelFormat";
    RegisterClass(&wndclass);
  }
  HWND hwnd=CreateWindowEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW,"ScrGL.OpenGL32.PixelFormat","ScrGL.PixelFormatWindow",
             WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
             0, 0, 0,0,
             NULL,NULL,HInstance,NULL);
  if (hwnd!=NULL) ShowWindow(hwnd,SW_SHOWNOACTIVATE);
  HDC hdc=NULL; if (hwnd!=NULL) hdc=GetDC(hwnd);
  int bpp=-1, depth=-1, acc=1, dbl=-1;
  int pf=0; if (hdc!=NULL) pf=sglChoosePixelFormatEx(hdc,&bpp,&depth,&dbl,&acc);
  if (pf!=0)
  { PIXELFORMATDESCRIPTOR pfd; ZeroMemory(&pfd,sizeof(pfd));
    sglDescribePixelFormat(hdc,pf,sizeof(pfd),&pfd);
    BOOL res=sglSetPixelFormat(hdc,pf,&pfd);
    if (res)
    { HGLRC hglrc=sglCreateContext(hdc);
      if (hglrc)
      { BOOL res=sglMakeCurrent(hdc,hglrc);
        if (res)
        { sglMakeCurrent(NULL,NULL);
        }
        sglDeleteContext(hglrc);
      }
    }
  }
  if (hdc!=NULL) ReleaseDC(hwnd,hdc);
  if (hwnd!=NULL) DestroyWindow(hwnd);
  sglFreeLibrary();
  Debug(dlMsg,"Finished temporary load. Vendor is "+GlobalVendor+".");
}





TSglDriverStrings* __fastcall TSglDriverList::GetDrivers()
{ if (FDriversValid) return FDrivers;
  if (!GlobalVersionTried && sglInst==NULL) sglGetGlobalVersion();
  FDriversValid=true;
  FDrivers->Clear(); char c[MAX_PATH];
  AnsiString project=ExtractDirFromFilePath(ExtractFilePath(Application->ExeName));
  GetWindowsDirectory(c,MAX_PATH); AnsiString windows=c;
  GetSystemDirectory(c,MAX_PATH); AnsiString system=c;
  for (int i=0; i<FSearchPath->Count; i++)
  { AnsiString search=FSearchPath->Strings[i]; int pos;
    pos=search.Pos("<project>"); if (pos!=0) search=search.SubString(1,pos-1)+project+search.SubString(pos+9,search.Length()-pos-7);
    pos=search.Pos("<windows>"); if (pos!=0) search=search.SubString(1,pos-1)+windows+search.SubString(pos+9,search.Length()-pos-7);
    pos=search.Pos("<system>"); if (pos!=0) search=search.SubString(1,pos-1)+system+search.SubString(pos+8,search.Length()-pos-6);
    AnsiString dir=ExtractFilePath(search);
    WIN32_FIND_DATA fd; HANDLE hFind=FindFirstFile(search.c_str(),&fd);
    while (hFind!=INVALID_HANDLE_VALUE)
    { TSglDriverDetails *d=new TSglDriverDetails();
      bool lres=GetDriverDetails(dir+fd.cFileName,d);
      if (!lres) delete d;
      else
      { if (FDrivers->IndexOfFileName(d->FileName)==-1)
        { AnsiString sDesc=ExtractFileName(d->FileName)+" - "+d->CompanyName+" "+d->FileDescription+" "+d->FileVersion;
          if (AnsiString(fd.cFileName).LowerCase()=="opengl32.dll" && GlobalVersionSucc)
          { sDesc=ExtractFileName(d->FileName)+" - "+AnsiString(GlobalVendor)+" "+AnsiString(GlobalRenderer)+" "+AnsiString(GlobalVersion);
          }
          FDrivers->AddObject(sDesc,d);
        }
      }
      bool res=FindNextFile(hFind,&fd); if (!res) {FindClose(hFind); hFind=INVALID_HANDLE_VALUE;}
    }
  }
  return FDrivers;
}



__fastcall TSglPanel::TSglPanel(TComponent *Owner) : TWinControl(Owner)
{ Debug(dlMsg,"Starting constructor");
  ControlStyle << csOpaque;
  FColor=clNone; TWinControl::Color=FColor;
  Width=100; Height=100;
  //
  FIsMouseClipped=false;
  Fhgl=NULL; Fhdc=NULL;
  FglBpp=-1; FglDepth=16; FglDblBuff=1; FglAcc=1; FglDriver="opengl32.dll";
  FOnGLPaint=NULL; FOnGLInit=NULL; FOnGLDestroy=NULL; FOnResize=NULL; FOnGLPixelFormat=NULL;
  FhGLib=NULL;  GiveUpTrying=false;
  hwndSubclass=NULL;
  FScreenWidth=-1; FScreenHeight=-1; FScreenBpp=-1;
  FIsSuppressed=false; FIsOutModed=false;
  FCurrentModeWidth=-1; FCurrentModeHeight=-1; FCurrentModeBpp=-1;
  FLastPoint=Point(0,0);
  FPixelFormatSullied=false;
  Debug(dlMsg,"Done constructor");
}
void __fastcall TSglPanel::CreateParams(TCreateParams &p)
{ TWinControl::CreateParams(p);
  p.WindowClass.style |= CS_OWNDC; strcat(p.WinClassName,"Owndc");
}
void __fastcall TSglPanel::CreateWindowHandle(const TCreateParams &p)
{ TWinControl::CreateWindowHandle(p);
  FPixelFormatSullied=false;
}
void __fastcall TSglPanel::DestroyWindowHandle()
{ Debug(dlMsg,"DestroyWindowHandle... but first, destroying the GL handle");
  TControlState cs=ControlState; cs << csDestroyingHandle; ControlState=cs;
  InvalidateGLHandle();
  // You might think that we should not
  // call this function, and just leave the job to DestroyWnd.
  // But that won't work! e.g. the Matrox ICD gives an exception
  // if you try to delete its context during WM_DESTROY processing.
  // We set the control-state at the beginning (even though
  // the same thing is done at the start of the ancestor method)
  // to ensure that the flag was set during the InvalidateGLHandle
  TWinControl::DestroyWindowHandle();
}
__fastcall TSglPanel::~TSglPanel()
{ Debug(dlMsg,"~TSglPanel... commencing destructor.");
  ResetScreenMode();
  if (HandleAllocated()) DestroyWnd();
  // DestroyWnd invokes DestroyWindowHandle, which in turn
  // calls InvalidateGLHandle, then ReleaseDC
  // We're calling it there instead of relying upon ancestor destructor,
  // in order that we can free the library right here:
  if (FhGLib!=NULL) sglFreeLibrary(); FhGLib=NULL;
  Debug(dlMsg,"~TSglPanel. Finished destructor.");
}

void __fastcall TSglPanel::SetglDriver(AnsiString s)
{ if (ExtractFileName(FglDriver).UpperCase()==ExtractFileName(s).UpperCase()) return;
  InvalidateGLHandle();
  if (FhGLib!=NULL) sglFreeLibrary(); FhGLib=NULL;
  FglDriver=s;
}

void __fastcall TSglPanel::SetScreenDimensions(int w,int h,int b)
{ if (FScreenWidth==w && FScreenHeight==h && FScreenBpp==b) return;
  FScreenWidth=w; FScreenHeight=h; FScreenBpp=b;
  InvalidateGLHandle();
}
void __fastcall TSglPanel::SetScreenWidth(int w)
{ if (FScreenWidth==w) return;
  FScreenWidth=w;
  InvalidateGLHandle();
}
void __fastcall TSglPanel::SetScreenHeight(int w)
{ if (FScreenHeight==w) return;
  FScreenHeight=w;
  InvalidateGLHandle();
}
void __fastcall TSglPanel::SetScreenBpp(int w)
{ if (FScreenBpp==w) return;
  FScreenBpp=w;
  InvalidateGLHandle();
}


// If you're using a 3dfx style card, then immediately upon creating the glContext
// we do some magic:
// we clip the mouse, and scale it, so that the mouse can never leave its region
LRESULT CALLBACK ScrGLSubclassFormProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{ TSglPanel *scr=(TSglPanel*)GetProp(hwnd,"ScrGLSubclass");
  FARPROC OldWndProc=(FARPROC)GetProp(hwnd,"ScrGLOldWndProc");
  if (scr!=NULL) return scr->SubclassFormProc(OldWndProc,hwnd,msg,wParam,lParam);
  if (OldWndProc==NULL) return DefWindowProc(hwnd,msg,wParam,lParam);
  else return CallWindowProc(OldWndProc,hwnd,msg,wParam,lParam);
}
void __fastcall TSglPanel::EnterMode()
{ if (hwndSubclass!=NULL) return;
  TCustomForm *f=GetParentForm(this);
  HWND hwnd; if (f==NULL) hwnd=Handle; else hwnd=f->Handle;
  TSglPanel *alreadygl=(TSglPanel*)GetProp(hwnd,"ScrGLSubclass");
  if (alreadygl!=NULL) return;
  SetProp(hwnd,"ScrGLSubclass",this);
  // we only need to add ourselves into the chain if we are not already there
  LONG AlreadyInChain=(LONG)GetProp(hwnd,"ScrGLOldWndProc");
  if (AlreadyInChain==NULL)
  { LONG OldWndProc=SetWindowLong(hwnd,GWL_WNDPROC,(LONG)ScrGLSubclassFormProc);
    SetProp(hwnd,"ScrGLOldWndProc",(void*)OldWndProc);
  } 
  hwndSubclass=hwnd;
  // and the subclass has been installed!
  // Now to clip the mouse
  if (sglUses3dfxStyle()) ClipMouse();
}
void __fastcall TSglPanel::ClipMouse()
{ if (FIsMouseClipped) return;
  int sw=GetSystemMetrics(SM_CXSCREEN), sh=GetSystemMetrics(SM_CYSCREEN);
  int cw=FSuppressedWidth, ch=FSuppressedHeight;
  // First, we'll adjust the mouse coordinate so that it is now in the same
  // place with respect to our window, as it used to be with respect to the screen.
  POINT tl; tl.x=0; tl.y=0; ::ClientToScreen(Handle,&tl);
  POINT pt; GetCursorPos(&pt);
  pt.x=pt.x*cw/sw+tl.x; pt.y=pt.y*ch/sh+tl.y;
  SetCursorPos(pt.x,pt.y);
  // Then we set the clip rectangle to our window
  GetClipCursor(&FOldMouseClip);
  RECT rc; pt.x=0; pt.y=0; POINT pts=ClientToScreen(pt); rc.left=pts.x; rc.top=pts.y;
  pt.x=FSuppressedWidth; pt.y=FSuppressedHeight; pts=ClientToScreen(pt); rc.right=pts.x; rc.bottom=pts.y;
  ClipCursor(&rc);
  // Finally we adjust speed so that it moves at the same speed with respect to
  // our window as it used to with respect to the screen. The three params are:
  // mouse threshold 1 (outside of which speed is doubled), mouse threshold 2
  // (outside of which speed is doubled again), and base speed. Speed is often
  // an integer of the order of 1 or 2 or 3. We won't be doing much here, but at
  // least we're giving it our best shot! We're also not adjusting the thresholds.
  // They didn't seem to make much difference.
  DWORD param[3]; SystemParametersInfo(SPI_GETMOUSE,0,&param,FALSE); FOldMouseSpeed=param[2];
  param[2]=param[2]*cw/sw; SystemParametersInfo(SPI_SETMOUSE,0,&param,FALSE);
  FIsMouseClipped=true;
}
void __fastcall TSglPanel::UnclipMouse()
{ if (!FIsMouseClipped) return;
  int sw=GetSystemMetrics(SM_CXSCREEN), sh=GetSystemMetrics(SM_CYSCREEN);
  int cw=FSuppressedWidth, ch=FSuppressedHeight;
  // First restore the mouse's speed
  DWORD param[3]; SystemParametersInfo(SPI_GETMOUSE,0,&param,FALSE);
  param[2]=FOldMouseSpeed; SystemParametersInfo(SPI_SETMOUSE,0,&param,FALSE);
  // Then restore its clip rectangle
  //ClipCursor(&FOldMouseClip);
  RECT rc; rc.left=0; rc.top=0; rc.right=1152; rc.bottom=864;
  ClipCursor(&rc);
  // Finally move it to the same place wrt. the screen as it used to be wrt. the window
  POINT tl; tl.x=0; tl.y=0; ::ClientToScreen(Handle,&tl);
  POINT pt; GetCursorPos(&pt);
  pt.x=(pt.x-tl.x)*sw/cw; pt.y=(pt.y-tl.y)*sh/ch;
  SetCursorPos(pt.x,pt.y);
  FIsMouseClipped=false;
}

void __fastcall TSglPanel::ExitMode()
{ UnclipMouse();
  if (FIsSuppressed)
  { Width=FSuppressedWidth; Height=FSuppressedHeight; Align=FSuppressedAlign;
    FIsSuppressed=false;
  }
  HWND hwnd=hwndSubclass;
  if (hwnd==NULL) return;
  TSglPanel *alreadyscr=(TSglPanel*)GetProp(hwnd,"ScrGLSubclass");
  if (alreadyscr!=this) return; // has been snitched by a different ScrGL!
  LONG prevwndproc=(LONG)GetProp(hwnd,"ScrGLOldWndProc");
  LONG currentwndproc=GetWindowLong(hwnd,GWL_WNDPROC);
  if (currentwndproc==(LONG)ScrGLSubclassFormProc) SetWindowLong(hwnd,GWL_WNDPROC,prevwndproc);
  RemoveProp(hwnd,"ScrGLSubclass");
  if (currentwndproc==(LONG)ScrGLSubclassFormProc) RemoveProp(hwnd,"ScrGLOldWndProc");
  else return; // someone else had hooked into the chain after us
}

void __fastcall TSglPanel::ResetScreenMode()
{ if (!FIsOutModed) return;
  ChangeDisplaySettings(NULL,NULL);
  FIsOutModed=false;
  FCurrentModeWidth=-1; FCurrentModeHeight=-1; FCurrentModeBpp=-1;
}
void __fastcall TSglPanel::ChangeScreenMode()
{ TCustomForm *f=GetParentForm(this); // we will only change screen if we can resize our parent form
  if (f==NULL) return;
  DEVMODE dm; ZeroMemory(&dm,sizeof(dm)); dm.dmSize=sizeof(dm);
  dm.dmFields=0;
  if (FScreenWidth!=-1) {dm.dmFields |= DM_PELSWIDTH; dm.dmPelsWidth=FScreenWidth;}
  if (FScreenHeight!=-1) {dm.dmPelsHeight=FScreenHeight;  dm.dmFields |= DM_PELSHEIGHT;}
  if (FScreenBpp!=-1) {dm.dmBitsPerPel=FScreenBpp;  dm.dmFields |= DM_BITSPERPEL;}
  LONG res=::ChangeDisplaySettings(&dm,CDS_TEST);
  if (res!=DISP_CHANGE_SUCCESSFUL) return;
  ChangeDisplaySettings(&dm,CDS_FULLSCREEN);
  f->SetBounds(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
  FIsOutModed=true;
  FCurrentModeWidth=FScreenWidth; FCurrentModeHeight=FScreenHeight; FCurrentModeBpp=FScreenBpp;
}



void __fastcall TSglPanel::InvalidateGLHandle()
{ GiveUpTrying=false;
  if (Fhgl!=NULL)
  { sglMakeCurrent(NULL, NULL);
    sglDeleteContext(Fhgl); Fhgl=NULL;
  }
  ExitMode();
}
bool __fastcall TSglPanel::GLHandleAllocated()
{ if (Fhgl==NULL) return false;
  else return true;
}
void __fastcall TSglPanel::GLHandleNeeded()
{ if (GiveUpTrying) return;
  if (Fhgl!=NULL) return;
  if (FPixelFormatSullied)
  { bool oldvisible=Visible;
    DestroyWnd(); CreateWnd();
    Visible=false; if (oldvisible) Visible=true; // otherwise it didn't become visible again properly
    FPixelFormatSullied=false;
    Fhdc=NULL;
    // That's because Windows doesn't allow more than one pixel format
    // allocated in the lifetime of a window.
  }
  HDC hdc=GLCanvasHandle; if (hdc==NULL) return;
  // The help says to do Set8087CW(0x133f);
  // but its the first one that actually works better! (with _Set8087 I was still
  // getting access violations when running in the IDE)
  _control87(MCW_EM, MCW_EM); // reprogram the FPU so it doesn't raise exceptions
  // Now to turn off the splash-screen for 3dfx.
  // We might as well do it regardless of whether it's a 3dfx card or not.
  // (but it must be done before trying to load the library)
  SetEnvironmentVariable("FX_GLIDE_NO_SPLASH","1"); // stop 3dfx from displaying a spinning logo
  // Now to do stuff in preparation of changing screen mode. We do this before
  // creating the context so that the context only gets created with
  // screen mode and bpp (for opengl32) and panel-dimensions (for 3dfx) in place.
  // In fact, we do it before even loading the library so the library doesn't
  // try to initialise itself to the wrong bit depth.
  bool is3dfx=sglIsDriver3dfx(FglDriver);
  if (is3dfx && !FIsSuppressed)
  { FSuppressedWidth=Width; FSuppressedHeight=Height; FSuppressedAlign=Align;
    int w,h; if (FScreenHeight<440) {w=512; h=384;}
    else if (FScreenHeight<540) {w=640; h=480;}
    else if (FScreenHeight<680) {w=800; h=600;}
    else {w=1024;h=758;}
    Align=alNone; Width=w; Height=h; FIsSuppressed=true;
    if ((Width!=FSuppressedWidth || Height!=FSuppressedHeight) && FOnResize!=NULL) FOnResize(this);
  }
  else if (!is3dfx && (FScreenWidth!=FCurrentModeWidth || FScreenHeight!=FCurrentModeHeight || FScreenBpp!=FCurrentModeBpp))
  { Debug(dlMsg,"About to change screen mode");
    if (FScreenWidth==-1 && FScreenHeight==-1 && FScreenBpp==-1) ResetScreenMode();
    else ChangeScreenMode();
    Debug(dlMsg,"Changed screen mode");
  }
  Debug(dlMsg,"About to load library");
  if (FhGLib==NULL)
  { FhGLib=sglLoadLibrary(FglDriver);
  }
  if (FhGLib==NULL) {GiveUpTrying=true;GLError("Error loading library "+FglDriver);return;}
  Debug(dlMsg,"Loaded library");
  // set up pixel format
  Debug(dlMsg,"Choosing pixel format");
  int bpp=FScreenBpp, depth=FglDepth, dbl=FglDblBuff, acc=FglAcc;
  int pixelformat=sglChoosePixelFormatEx(hdc,&bpp,&depth,&dbl,&acc);
  if (FOnGLPixelFormat!=NULL) FOnGLPixelFormat(this,hdc,pixelformat,bpp,depth,dbl,acc);
  if (pixelformat==0) {GiveUpTrying=true;GLError("ChoosePixelFormat failed");return;}
  Debug(dlMsg,"Got pixel format; setting it...");
  PIXELFORMATDESCRIPTOR pfd; ZeroMemory(&pfd,sizeof(pfd));  pfd.nSize=sizeof(pfd); pfd.nVersion=1;
  sglDescribePixelFormat(hdc,pixelformat,sizeof(pfd),&pfd);
  bool res=sglSetPixelFormat(hdc,pixelformat,&pfd);
  if (!res) {GiveUpTrying=true;GLError("SetPixelFormat failed");return;}
  FPixelFormatSullied=true;
  Debug(dlMsg,"Set pixel format; creating context...");
  Fhgl=sglCreateContext(hdc);
  if (Fhgl==NULL) {GiveUpTrying=true;GLError("Error creating context: "+GetLastErrorString());return;}
  // Sometimes it seems to work even with a null context... ???
  Debug(dlMsg,"Created context. Making it current...");
  sglMakeCurrent(hdc,Fhgl);
  Debug(dlMsg,"Made it current");
  if (Color!=clNone)
  { DWORD rgb=ColorToRGB(Color);
    int red=(rgb&0x000000FF)>>0;
    int green=(rgb&0x0000FF00)>>8;
    int blue=(rgb&0x00FF0000)>>16;
    sglClearColor(((float)red)/256.0,((float)green)/256.0,((float)blue)/256.0,0.0);
  }
  Debug(dlMsg,"Entering mode...");
  EnterMode();
  Debug(dlMsg,"Entered mode. Initing...");
  if (FOnGLInit!=NULL) FOnGLInit(this);
  Debug(dlMsg,"Done init");
}

void __fastcall TSglPanel::SwapBuffers()
{ sglSwapBuffers(GLCanvasHandle);
}


HDC __fastcall TSglPanel::GetGLCanvasHandle()
{ if (Fhdc!=NULL) return Fhdc;
  Fhdc=GetDC(Handle);
  return Fhdc;
}

bool __fastcall TSglPanel::GetIsOn()
{ return (Fhgl!=NULL);
}

void __fastcall TSglPanel::GLError(AnsiString s)
{ Debug(dlErr,s);
  if (FOnGLError!=NULL) FOnGLError(this,s);
}

void __fastcall TSglPanel::SetColor(const TColor c)
{ if (FColor==c) return;
  FColor=c; TWinControl::Color=c;
  if (Color!=clNone && Fhgl!=NULL)
  { DWORD rgb=ColorToRGB(Color);
    int red=(rgb&0x000000FF)>>0;
    int green=(rgb&0x0000FF00)>>8;
    int blue=(rgb&0x00FF0000)>>16;
    sglClearColor(((float)red)/256.0,((float)green)/256.0,((float)blue)/256.0,0.0);
  }
  // For some reason, at design time, using just Repaint or Invalidate
  // didn't end up doing anything. It seemed that the message was being
  // set but the colour didn't change.
  if (HandleAllocated())
  { ::InvalidateRect(Handle,NULL,TRUE);
    UpdateWindow(Handle);
  }
}

void __fastcall TSglPanel::SetglBpp(int bpp)
{ if (FScreenBpp==bpp) return;
  FScreenBpp=bpp; InvalidateGLHandle();
}
void __fastcall TSglPanel::SetglDepth(int depth)
{ if (FglDepth==depth) return;
  FglDepth=depth; InvalidateGLHandle();
}
void __fastcall TSglPanel::SetglDblBuff(int db)
{ if (FglDblBuff==db) return;
  FglDblBuff=db; InvalidateGLHandle();
}
void __fastcall TSglPanel::SetglAcc(int a)
{ if (FglAcc==a) return;
  FglAcc=a; InvalidateGLHandle();
}

LRESULT __fastcall TSglPanel::SubclassFormProc(FARPROC OldWndProc,HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{ if (msg==WM_ACTIVATE)
  { if (sglUses3dfxStyle())
    { BOOL fActive=LOWORD(wParam);
      if (fActive) ClipMouse(); else UnclipMouse();
    }
  }
  else if (msg==CM_RELEASE)
  { // Normal behaviour of a form upon CM_RELEASE
    // is to hide itself first (and ask questions later :)
    // We must get out of change-mode before the form is hidden!
    ResetScreenMode();
  }
  else if (msg==WM_DESTROY)
  { InvalidateGLHandle(); // If the parent is getting
    // WM_DESTROY, then it's a certainty that the child too will
    // get sent a WM_DESTROY. However, there is a quirk in the
    // Matrox ICD under NT: it crashes if you try to release
    // your context inside the WM_DESTROY processing. Therefore,
    // we're releasing it here earlier.
    ResetScreenMode();
  }
  else if (msg==WM_SIZE)
  { CheckIfMoved();
  }
  return CallWindowProc(OldWndProc,hwnd,msg,wParam,lParam);
}





void __fastcall TSglPanel::WndProc(TMessage &msg)
{ TComponent *test; if (Owner==NULL) test=this; else test=Owner;
  bool des=test->ComponentState.Contains(csDesigning);
  if (des)
  { TWinControl::WndProc(msg);
    return;
  }
  if (msg.Msg==WM_DESTROY)
  { InvalidateGLHandle();
    if (Fhdc!=NULL) ReleaseDC(Handle,Fhdc); Fhdc=NULL;
    if (FIsOutModed) ResetScreenMode();
  }
  if (msg.Msg==WM_SIZE)
  { if (Fhgl==NULL) return;
    FLastPoint=Point(0,0); ClientToScreen(FLastPoint);
    sglViewport( 0, 0, Width, Height );
    if (FOnResize!=NULL) FOnResize(this);
  }
  if (msg.Msg==WM_ERASEBKGND) return;
  if (msg.Msg==WM_PAINT)
  { CheckIfMoved();
    GLHandleNeeded();
    PAINTSTRUCT ps; BeginPaint(Handle,&ps); EndPaint(Handle,&ps);
    if (Fhgl!=NULL)
    { if (Color!=clNone) sglClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      if (FOnGLPaint!=NULL) FOnGLPaint(this);
    }
  }

  TWinControl::WndProc(msg);
}

void __fastcall TSglPanel::CheckIfMoved()
{ if (Fhgl==NULL) return;
  TPoint pt=Point(0,0); ::ClientToScreen(Handle,&pt);
  if (pt.x!=FLastPoint.x || pt.y!=FLastPoint.y)
  { Debug(dlMsg,"The window has moved!");
    FLastPoint.x=pt.x; FLastPoint.y=pt.y;
    sglViewport( 0, 0, Width, Height );
    if (FOnResize!=NULL) FOnResize(this);
  }
}
HGLRC __fastcall TSglPanel::GetGLHandle(){
	return Fhgl;
}
void __fastcall TSglPanel::SetCurrent(){
	if (GetGLCanvasHandle() && GetGLHandle()){
		sglMakeCurrent(GetGLCanvasHandle(), GetGLHandle());
	}
}

// Code copied from OpenGL.c
// Part of the Plasma Game Server
// written/copyright Ryan Haksi, 1998
// As Ryan says:
//   'If you use this file as part of a publically released project, please
//    grant me the courtesy of a mention in the credits section somewhere.
//    Other than that opengl.h and opengl.c are yours to do with what you wish
//    Current email address(es): cryogen@unix.infoserve.net
//    Current web page address: http://home.bc.rogers.wave.ca/borealis/ryan.html'
// Many thanks to Ryan. Credits to him will duly appear in the about box.


int sglChoosePixelFormatEx(HDC hdc,int *p_bpp,int *p_depth,int *p_dbl,int *p_acc)
{ int wbpp; if (p_bpp==NULL) wbpp=-1; else wbpp=*p_bpp;
  int wdepth; if (p_depth==NULL) wdepth=16; else wdepth=*p_depth;
  int wdbl; if (p_dbl==NULL) wdbl=-1; else wdbl=*p_dbl;
  int wacc; if (p_acc==NULL) wacc=1; else wacc=*p_acc;
  PIXELFORMATDESCRIPTOR pfd; ZeroMemory(&pfd,sizeof(pfd)); pfd.nSize=sizeof(pfd); pfd.nVersion=1;
  int num=sglDescribePixelFormat(hdc,1,sizeof(pfd),&pfd);
  if (num==0) return 0;
  unsigned int maxqual=0; int maxindex=0;
  int max_bpp, max_depth, max_dbl, max_acc;
  for (int i=1; i<=num; i++)
  { ZeroMemory(&pfd,sizeof(pfd)); pfd.nSize=sizeof(pfd); pfd.nVersion=1;
    sglDescribePixelFormat(hdc,i,sizeof(pfd),&pfd);
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
    unsigned int q=0;
    if (opengl && window) q=q+0x8000;
    if (wdepth==-1 || (wdepth>0 && depth>0)) q=q+0x4000;
    if (wdbl==-1 || (wdbl==0 && !dbuff) || (wdbl==1 && dbuff)) q=q+0x2000;
    if (wacc==-1 || (wacc==0 && soft) || (wacc==1 && (mcd || icd))) q=q+0x1000;
    if (mcd || icd) q=q+0x0040; if (icd) q=q+0x0002;
    if (wbpp==-1 || (wbpp==bpp)) q=q+0x0800;
    if (bpp>=16) q=q+0x0020; if (bpp==16) q=q+0x0008;
    if (wdepth==-1 || (wdepth==depth)) q=q+0x0400;
    if (depth>=16) q=q+0x0010; if (depth==16) q=q+0x0004;
    if (!pal) q=q+0x0080;
    if (bitmap) q=q+0x0001;
    if (q>maxqual) {maxqual=q; maxindex=i;max_bpp=bpp; max_depth=depth; max_dbl=dbuff?1:0; max_acc=soft?0:1;}
  }
  if (maxindex==0) return maxindex;
  if (p_bpp!=NULL) *p_bpp=max_bpp;
  if (p_depth!=NULL) *p_depth=max_depth;
  if (p_dbl!=NULL) *p_dbl=max_dbl;
  if (p_acc!=NULL) *p_acc=max_acc;
  return maxindex;
}

void            (APIENTRY *fnglAlphaFunc)(GLenum func, GLclampf ref);
void            (APIENTRY *fnglBegin)(GLenum mode);
void            (APIENTRY *fnglBindTexture)(GLenum target, GLuint texture);
void            (APIENTRY *fnglBlendFunc)(GLenum sfactor, GLenum dfactor);
void            (APIENTRY *fnglClear)(GLbitfield mask);
void            (APIENTRY *fnglClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void            (APIENTRY *fnglClearDepth)(GLclampd depth);
void            (APIENTRY *fnglColor3f)(GLfloat red, GLfloat green, GLfloat blue);
void            (APIENTRY *fnglColor3fv)(const GLfloat *v);
void            (APIENTRY *fnglColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void            (APIENTRY *fnglColor4fv)(const GLfloat *v);
void            (APIENTRY *fnglCullFace)(GLenum mode);
void            (APIENTRY *fnglDepthFunc)(GLenum func);
void            (APIENTRY *fnglDisable)(GLenum cap);
void            (APIENTRY *fnglEnable)(GLenum cap);
void            (APIENTRY *fnglEnd)(void);
void            (APIENTRY *fnglFinish)(void);
void            (APIENTRY *fnglFrontFace)(GLenum mode);
void            (APIENTRY *fnglFrustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLenum          (APIENTRY *fnglGetError)(void);
void            (APIENTRY *fnglGetFloatv)(GLenum pname, GLfloat *params);
void            (APIENTRY *fnglGetIntegerv)(GLenum pname, GLint *params);
const GLubyte * (APIENTRY *fnglGetString)(GLenum name);
void            (APIENTRY *fnglHint)(GLenum target, GLenum mode);
GLboolean       (APIENTRY *fnglIsEnabled)(GLenum cap);
void            (APIENTRY *fnglLoadIdentity)(void);
void            (APIENTRY *fnglLoadMatrixf)(const GLfloat *m);
void            (APIENTRY *fnglMatrixMode)(GLenum mode);
void            (APIENTRY *fnglMultMatrixf)(const GLfloat *m);
void            (APIENTRY *fnglOrtho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void            (APIENTRY *fnglPixelStorei)(GLenum pname, GLint param);
void            (APIENTRY *fnglPolygonOffset)(GLfloat factor, GLfloat units);
void            (APIENTRY *fnglPopMatrix)(void);
void            (APIENTRY *fnglPushMatrix)(void);
void            (APIENTRY *fnglRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void            (APIENTRY *fnglScalef)(GLfloat x, GLfloat y, GLfloat z);
void            (APIENTRY *fnglShadeModel)(GLenum mode);
void            (APIENTRY *fnglTexCoord2f)(GLfloat s, GLfloat t);
void            (APIENTRY *fnglTexCoord2fv)(const GLfloat *v);
void            (APIENTRY *fnglTexCoord3f)(GLfloat s, GLfloat t, GLfloat r);
void            (APIENTRY *fnglTexCoord3fv)(const GLfloat *v);
void            (APIENTRY *fnglTexEnvf)(GLenum target, GLenum pname, GLfloat param);
void            (APIENTRY *fnglTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void            (APIENTRY *fnglTexParameterf)(GLenum target, GLenum pname, GLfloat param);
void            (APIENTRY *fnglTranslatef)(GLfloat x, GLfloat y, GLfloat z);
void            (APIENTRY *fnglVertex2f)(GLfloat x, GLfloat y);
void            (APIENTRY *fnglVertex3f)(GLfloat x, GLfloat y, GLfloat z);
void            (APIENTRY *fnglVertex3fv)(const GLfloat *v);
void            (APIENTRY *fnglVertex4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void            (APIENTRY *fnglViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
void            (APIENTRY *fnglCallList)(GLuint list);
void            (APIENTRY *fnglFlush)(void);
void            (APIENTRY *fnglNewList)(GLuint list, GLenum mode);
void            (APIENTRY *fnglPushName)(GLuint name);
void            (APIENTRY *fnglColor3d)(GLdouble red, GLdouble green, GLdouble blue);
void            (APIENTRY *fnglVertex3d)(GLdouble x, GLdouble y,GLdouble z);
void            (APIENTRY *fnglPopName)(void);
void            (APIENTRY *fnglEndList)(void);
void            (APIENTRY *fnglSelectBuffer)(GLsizei size, GLuint *buffer);
GLint           (APIENTRY *fnglRenderMode)(GLenum mode);
void            (APIENTRY *fnglInitNames)(void);
void            (APIENTRY *fnglGetDoublev)(GLenum pname,GLdouble *params);
void            (APIENTRY *fnglMultMatrixd)(const GLdouble *m);
void            (APIENTRY *fnglTranslated)(GLdouble x, GLdouble y, GLdouble z);
void            (APIENTRY *fnglDeleteLists)(GLuint list,GLsizei range);
GLuint          (APIENTRY *fnglGenLists)(GLsizei range);
void            (APIENTRY *fnglPushAttrib)(GLbitfield mask);
void            (APIENTRY *fnglPopAttrib)(void);
void            (APIENTRY *fnglVertex2i)(GLint x, GLint y);
void            (APIENTRY *fnglRectf)(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2);


HGLRC (APIENTRY *fnwglCreateContext)(HDC hdc);
BOOL (APIENTRY *fnwglDeleteContext)(HGLRC hglrc);
BOOL (APIENTRY *fnwglMakeCurrent)(HDC hdc, HGLRC hglrc);
BOOL (APIENTRY *fnwglGetCurrentContext)();
BOOL (APIENTRY *fnwglSwapLayerBuffers)(HDC hdc, UINT fuPlanes);

BOOL (APIENTRY *fnSwapBuffers)(HDC hdc);
int (APIENTRY *fnChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR * ppfd);
int (APIENTRY *fnDescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
BOOL (APIENTRY *fnSetPixelFormat)(HDC hdc, int iPixelFormat, CONST PIXELFORMATDESCRIPTOR * ppfd);


bool sglUses3dfxStyle()
{ return sglBypassGDI;
}

HINSTANCE sglLoadLibrary(AnsiString fn)
{ // The 3dfxgl.dll minidriver bypasses the GDI entirely
  // Ryan does a proper failover, but the GDI failover call for SwapBuffers is
  // REALLY bloody slow, so best to avoid it alltogether. (when using winglide
  // for debugging Ryan was going from 8 fps to 5fps, thats 37.5%!!!!)
  if (sglInst!=NULL) sglFreeLibrary();
  sglFile = fn;
  sglInst = LoadLibrary(fn.c_str());
  if (sglInst == NULL) return NULL;
  sglBypassGDI = (fn.LowerCase().Pos("3dfx")!=0);

  // load all the function pointers
  (void*)fnglAlphaFunc     = GetProcAddress(sglInst, "glAlphaFunc");
  (void*)fnglBegin         = GetProcAddress(sglInst, "glBegin");
  (void*)fnglBindTexture   = GetProcAddress(sglInst, "glBindTexture");
  (void*)fnglBlendFunc     = GetProcAddress(sglInst, "glBlendFunc");
  (void*)fnglClear         = GetProcAddress(sglInst, "glClear");
  (void*)fnglClearColor    = GetProcAddress(sglInst, "glClearColor");
  (void*)fnglClearDepth    = GetProcAddress(sglInst, "glClearDepth");
  (void*)fnglColor3f       = GetProcAddress(sglInst, "glColor3f");
  (void*)fnglColor3fv      = GetProcAddress(sglInst, "glColor3fv");
  (void*)fnglColor4f       = GetProcAddress(sglInst, "glColor4f");
  (void*)fnglColor4fv      = GetProcAddress(sglInst, "glColor4fv");
  (void*)fnglCullFace      = GetProcAddress(sglInst, "glCullFace");
  (void*)fnglDepthFunc     = GetProcAddress(sglInst, "glDepthFunc");
  (void*)fnglDisable       = GetProcAddress(sglInst, "glDisable");
  (void*)fnglEnable        = GetProcAddress(sglInst, "glEnable");
  (void*)fnglEnd           = GetProcAddress(sglInst, "glEnd");
  (void*)fnglFinish        = GetProcAddress(sglInst, "glFinish");
  (void*)fnglFrontFace     = GetProcAddress(sglInst, "glFrontFace");
  (void*)fnglFrustum       = GetProcAddress(sglInst, "glFrustum");
  (void*)fnglGetError      = GetProcAddress(sglInst, "glGetError");
  (void*)fnglGetFloatv     = GetProcAddress(sglInst, "glGetFloatv");
  (void*)fnglGetIntegerv   = GetProcAddress(sglInst, "glGetIntegerv");
  (void*)fnglGetString     = GetProcAddress(sglInst, "glGetString");
  (void*)fnglHint          = GetProcAddress(sglInst, "glHint");
  (void*)fnglIsEnabled     = GetProcAddress(sglInst, "glIsEnabled");
  (void*)fnglLoadIdentity  = GetProcAddress(sglInst, "glLoadIdentity");
  (void*)fnglLoadMatrixf   = GetProcAddress(sglInst, "glLoadMatrixf");
  (void*)fnglMatrixMode    = GetProcAddress(sglInst, "glMatrixMode");
  (void*)fnglMultMatrixf   = GetProcAddress(sglInst, "glMultMatrixf");
  (void*)fnglOrtho         = GetProcAddress(sglInst, "glOrtho");
  (void*)fnglPixelStorei   = GetProcAddress(sglInst, "glPixelStorei");
  (void*)fnglPolygonOffset = GetProcAddress(sglInst, "glPolygonOffset");
  (void*)fnglPopMatrix     = GetProcAddress(sglInst, "glPopMatrix");
  (void*)fnglPushMatrix    = GetProcAddress(sglInst, "glPushMatrix");
  (void*)fnglRotatef       = GetProcAddress(sglInst, "glRotatef");
  (void*)fnglScalef        = GetProcAddress(sglInst, "glScalef");
  (void*)fnglShadeModel    = GetProcAddress(sglInst, "glShadeModel");
  (void*)fnglTexCoord2f    = GetProcAddress(sglInst, "glTexCoord2f");
  (void*)fnglTexCoord2fv   = GetProcAddress(sglInst, "glTexCoord2fv");
  (void*)fnglTexCoord3f    = GetProcAddress(sglInst, "glTexCoord3f");
  (void*)fnglTexCoord3fv   = GetProcAddress(sglInst, "glTexCoord3fv");
  (void*)fnglTexEnvf       = GetProcAddress(sglInst, "glTexEnvf");
  (void*)fnglTexImage2D    = GetProcAddress(sglInst, "glTexImage2D");
  (void*)fnglTexParameterf = GetProcAddress(sglInst, "glTexParameterf");
  (void*)fnglTranslatef    = GetProcAddress(sglInst, "glTranslatef");
  (void*)fnglVertex2f      = GetProcAddress(sglInst, "glVertex2f");
  (void*)fnglVertex3f      = GetProcAddress(sglInst, "glVertex3f");
  (void*)fnglVertex3fv     = GetProcAddress(sglInst, "glVertex3fv");
  (void*)fnglVertex4f      = GetProcAddress(sglInst, "glVertex4f");
  (void*)fnglViewport      = GetProcAddress(sglInst, "glViewport");
  (void*)fnglCallList      = GetProcAddress(sglInst, "glCallList");
  (void*)fnglFlush         = GetProcAddress(sglInst, "glFlush");
  (void*)fnglNewList       = GetProcAddress(sglInst, "glNewList");
  (void*)fnglPushName      = GetProcAddress(sglInst, "glPushName");
  (void*)fnglColor3d       = GetProcAddress(sglInst, "glColor3d");
  (void*)fnglVertex3d      = GetProcAddress(sglInst, "glVertex3d");
  (void*)fnglPopName       = GetProcAddress(sglInst, "glPopName");
  (void*)fnglEndList       = GetProcAddress(sglInst, "glEndList");
  (void*)fnglSelectBuffer  = GetProcAddress(sglInst, "glSelectBuffer");
  (void*)fnglRenderMode    = GetProcAddress(sglInst, "glRenderMode");
  (void*)fnglInitNames     = GetProcAddress(sglInst, "glInitNames");
  (void*)fnglGetDoublev    = GetProcAddress(sglInst, "glGetDoublev");
  (void*)fnglMultMatrixd   = GetProcAddress(sglInst, "glMultMatrixd");
  (void*)fnglTranslated    = GetProcAddress(sglInst, "glTranslated");
  (void*)fnglDeleteLists   = GetProcAddress(sglInst, "glDeleteLists");
  (void*)fnglGenLists      = GetProcAddress(sglInst, "glGenLists");
  (void*)fnglPushAttrib    = GetProcAddress(sglInst, "glPushAttrib");
  (void*)fnglPopAttrib     = GetProcAddress(sglInst, "glPopAttrib");
  (void*)fnglVertex2i      = GetProcAddress(sglInst, "glVertex2i");
  (void*)fnglRectf         = GetProcAddress(sglInst, "glRectf");
  //
  (void*)fnwglCreateContext = GetProcAddress(sglInst, "wglCreateContext");
  (void*)fnwglDeleteContext = GetProcAddress(sglInst, "wglDeleteContext");
  (void*)fnwglMakeCurrent = GetProcAddress(sglInst, "wglMakeCurrent");
  (void*)fnwglGetCurrentContext = GetProcAddress(sglInst, "wglGetCurrentContext");
  (void*)fnwglSwapLayerBuffers = GetProcAddress(sglInst, "wglSwapLayerBuffers");
  //
  (void*)fnSwapBuffers = GetProcAddress(sglInst, "wglSwapBuffers");
  (void*)fnChoosePixelFormat = GetProcAddress(sglInst, "wglChoosePixelFormat");
  (void*)fnDescribePixelFormat = GetProcAddress(sglInst, "wglDescribePixelFormat");
  (void*)fnSetPixelFormat = GetProcAddress(sglInst, "wglSetPixelFormat");
  //
  bool succ=true;
  AnsiString fail="";
  //
  if (fnwglCreateContext==NULL) {fail=fail+"glCreateContext"; succ=false;}
  if (fnwglDeleteContext==NULL) {fail=fail+"glDeleteContext"; succ=false;}
  if (fnwglMakeCurrent==NULL) {fail=fail+"glMakeCurrent"; succ=false;}
  if (fnwglGetCurrentContext==NULL) {fail=fail+"glGetCurrentContext"; succ=false;}
  if (fnwglSwapLayerBuffers==NULL) {fail=fail+"glSwapLayerBuffers"; succ=false;}
  // And the windows functions we won't complain about, since
  // we'll generally be calling the Windows API versions of these first.
  // SwapBuffers, ChoosePixelFormat, DescribePixelFormat, SetPixelFormat
  //
  // *** Must change this to allow pass if we've defined minigl
  if (fnglAlphaFunc==NULL) {fail=fail+"glAlphaFunc."; succ=false;}
  if (fnglBegin==NULL) {fail=fail+"glBegin."; succ=false;}
  if (fnglBindTexture==NULL) {fail=fail+"glBindTexture."; succ=false;}
  if (fnglBlendFunc==NULL) {fail=fail+"glBlendFunc."; succ=false;}
  if (fnglClear==NULL) {fail=fail+"glClear."; succ=false;}
  if (fnglClearColor==NULL) {fail=fail+"glClearColor."; succ=false;}
  if (fnglClearDepth==NULL) {fail=fail+"glClearDepth."; succ=false;}
  if (fnglColor3f==NULL) {fail=fail+"glColor3f."; succ=false;}
  if (fnglColor3fv==NULL) {fail=fail+"glColor3fv."; succ=false;}
  if (fnglColor4f==NULL) {fail=fail+"glColor4f."; succ=false;}
  if (fnglColor4fv==NULL) {fail=fail+"glColor4fv."; succ=false;}
  if (fnglCullFace==NULL) {fail=fail+"glCullFace."; succ=false;}
  if (fnglDepthFunc==NULL) {fail=fail+"glDepthFunc."; succ=false;}
  if (fnglDisable==NULL) {fail=fail+"glDisable."; succ=false;}
  if (fnglEnable==NULL) {fail=fail+"glEnable."; succ=false;}
  if (fnglEnd           ==NULL) {fail=fail+"glEnd."; succ=false;}
  if (fnglFinish        ==NULL) {fail=fail+"glFinish."; succ=false;}
  if (fnglFrontFace     ==NULL) {fail=fail+"glFrontFace."; succ=false;}
  if (fnglFrustum       ==NULL) {fail=fail+"glFrustrum."; succ=false;}
  if (fnglGetError      ==NULL) {fail=fail+"glGetError."; succ=false;}
  if (fnglGetFloatv     ==NULL) {fail=fail+"glGetFloatv."; succ=false;}
  if (fnglGetIntegerv   ==NULL) {fail=fail+"glGetIntegerv."; succ=false;}
  if (fnglGetString==NULL) {fail=fail+"glGetString."; succ=false;}
  if (fnglHint==NULL) {fail=fail+"glHint."; succ=false;}
  if (fnglIsEnabled==NULL) {fail=fail+"glIsEnabled."; succ=false;}
  if (fnglLoadIdentity==NULL) {fail=fail+"glLoadIdentity."; succ=false;}
  if (fnglLoadMatrixf==NULL) {fail=fail+"glLoadMatrixf."; succ=false;}
  if (fnglMatrixMode==NULL) {fail=fail+"glMatrixMode."; succ=false;}
  if (fnglMultMatrixf==NULL) {fail=fail+"glMultMatrixf."; succ=false;}
  if (fnglOrtho==NULL) {fail=fail+"glOrtho."; succ=false;}
  if (fnglPixelStorei==NULL) {fail=fail+"glPixelStorei."; succ=false;}
  if (fnglPolygonOffset==NULL) {fail=fail+"glPolygonOffset."; succ=false;}
  if (fnglPopMatrix==NULL) {fail=fail+"glPopMatric."; succ=false;}
  if (fnglPushMatrix==NULL) {fail=fail+"glPushMatrix."; succ=false;}
  if (fnglRotatef==NULL) {fail=fail+"glRotatef."; succ=false;}
  if (fnglScalef==NULL) {fail=fail+"glScalef."; succ=false;}
  if (fnglShadeModel==NULL) {fail=fail+"glShadeModel."; succ=false;}
  if (fnglTexCoord2f==NULL) {fail=fail+"glTexCoord2f."; succ=false;}
  if (fnglTexCoord2fv==NULL) {fail=fail+"glTexCoord2fv."; succ=false;}
  if (fnglTexCoord3f==NULL) {fail=fail+"glTexCoord3f."; succ=false;}
  if (fnglTexCoord3fv==NULL) {fail=fail+"glTexCoord3fv."; succ=false;}
  if (fnglTexEnvf==NULL) {fail=fail+"glTexEnvf."; succ=false;}
  if (fnglTexImage2D==NULL) {fail=fail+"glTexImage2D."; succ=false;}
  if (fnglTexParameterf==NULL) {fail=fail+"glTexParameterf."; succ=false;}
  if (fnglTranslatef==NULL) {fail=fail+"glTranslatef."; succ=false;}
  if (fnglVertex2f==NULL) {fail=fail+"glVertex2f."; succ=false;}
  if (fnglVertex3f==NULL) {fail=fail+"glVertex3f."; succ=false;}
  if (fnglVertex3fv==NULL) {fail=fail+"glVertex3fv."; succ=false;}
  if (fnglVertex4f==NULL) {fail=fail+"glVertex4f."; succ=false;}
  if (fnglViewport==NULL) {fail=fail+"glViewport."; succ=false;}
  if (fnglCallList==NULL) {fail=fail+"glCallList."; succ=false;}
  if (fnglFlush==NULL) {fail=fail+"glFlush."; succ=false;}
  if (fnglNewList==NULL) {fail=fail+"glNewList."; succ=false;}
  if (fnglPushName==NULL) {fail=fail+"glPushName."; succ=false;}
  if (fnglColor3d==NULL) {fail=fail+"glColor3d."; succ=false;}
  if (fnglVertex3d==NULL) {fail=fail+"glVertex3d."; succ=false;}
  if (fnglPopName==NULL) {fail=fail+"glPopName."; succ=false;}
  if (fnglEndList==NULL) {fail=fail+"glEndList."; succ=false;}
  if (fnglSelectBuffer==NULL) {fail=fail+"glSelectBuffer."; succ=false;}
  if (fnglRenderMode==NULL) {fail=fail+"glRenderMode."; succ=false;}
  if (fnglInitNames==NULL) {fail=fail+"glInitNames."; succ=false;}
  if (fnglGetDoublev==NULL) {fail=fail+"glGetDoublev."; succ=false;}
  if (fnglMultMatrixd==NULL) {fail=fail+"glMultMatrixd."; succ=false;}
  if (fnglTranslated==NULL) {fail=fail+"glTranslated."; succ=false;}
  if (fnglDeleteLists==NULL) {fail=fail+"glDeleteLists."; succ=false;}
  if (fnglGenLists==NULL) {fail=fail+"glGenLists."; succ=false;}
  if (fnglPushAttrib==NULL) {fail=fail+"glPushAttrib."; succ=false;}
  if (fnglPopAttrib==NULL) {fail=fail+"glPopAttrib."; succ=false;}
  if (fnglVertex2i==NULL) {fail=fail+"glVertex2i."; succ=false;}
  if (fnglRectf==NULL) {fail=fail+"glRectf."; succ=false;}
  //
  if (!succ)
  { FreeLibrary(sglInst);
    sglInst=NULL;
    Debug(dlMsg,"Failed to load "+fn+": "+fail);
    return NULL;
  }

  return sglInst;
}

void sglFreeLibrary()
{ if (sglInst!=NULL) FreeLibrary(sglInst);
  sglInst = NULL;
}



void PACKAGE APIENTRY sglAlphaFunc(GLenum func, GLclampf ref)         {(*fnglAlphaFunc)(func,ref);}
void PACKAGE APIENTRY sglBegin(GLenum mode)                           {(*fnglBegin)(mode);}
void PACKAGE APIENTRY sglBindTexture(GLenum target, GLuint texture)   {(*fnglBindTexture)(target,texture);}
void PACKAGE APIENTRY sglBlendFunc(GLenum sfactor, GLenum dfactor)    {(*fnglBlendFunc)(sfactor,dfactor);}
void PACKAGE APIENTRY sglClear(GLbitfield mask)                       {(*fnglClear)(mask);}
void PACKAGE APIENTRY sglClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha){(*fnglClearColor)(red,green,blue,alpha);}
void PACKAGE APIENTRY sglClearDepth (GLclampd depth)                  {(*fnglClearDepth)(depth);}
void PACKAGE APIENTRY sglColor3f(GLfloat red, GLfloat green, GLfloat blue) {(*fnglColor3f)(red,green,blue);}
void PACKAGE APIENTRY sglColor3fv(const GLfloat *v)                   {(*fnglColor3fv)(v);}
void PACKAGE APIENTRY sglColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {(*fnglColor4f)(red,green,blue,alpha);}
void PACKAGE APIENTRY sglColor4fv(const GLfloat *v)                   {(*fnglColor4fv)(v);}
void PACKAGE APIENTRY sglCullFace(GLenum mode)                        {(*fnglCullFace)(mode);}
void PACKAGE APIENTRY sglDepthFunc(GLenum func)                       {(*fnglDepthFunc)(func);}
void PACKAGE APIENTRY sglDisable(GLenum cap)                          {(*fnglDisable)(cap);}
void PACKAGE APIENTRY sglEnable(GLenum cap)                           {(*fnglEnable)(cap);}
void PACKAGE APIENTRY sglEnd(void)                                    {(*fnglEnd)();}
void PACKAGE APIENTRY sglFinish(void)                                 {(*fnglFinish)();}
void PACKAGE APIENTRY sglFrontFace(GLenum mode)                       {(*fnglFrontFace)(mode);}
void PACKAGE APIENTRY sglFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {(*fnglFrustum)(left,right,bottom,top,zNear,zFar);}
GLenum PACKAGE APIENTRY sglGetError(void)                             {return (*fnglGetError)();}
void PACKAGE APIENTRY sglGetFloatv(GLenum pname, GLfloat *params)     {(*fnglGetFloatv)(pname, params);}
void PACKAGE APIENTRY sglGetIntegerv(GLenum pname, GLint *params)     {(*fnglGetIntegerv)(pname,params);}
const GLubyte * PACKAGE APIENTRY sglGetString(GLenum name)            {return (*fnglGetString)(name);}
void PACKAGE APIENTRY sglHint(GLenum target, GLenum mode)             {(*fnglHint)(target,mode);}
GLboolean PACKAGE APIENTRY sglIsEnabled(GLenum cap)                   {return (*fnglIsEnabled)(cap);}
void PACKAGE APIENTRY sglLoadIdentity(void)                           {(*fnglLoadIdentity)();}
void PACKAGE APIENTRY sglLoadMatrixf(const GLfloat *m)                {(*fnglLoadMatrixf)(m);}
void PACKAGE APIENTRY sglMatrixMode(GLenum mode)                      {(*fnglMatrixMode)(mode);}
void PACKAGE APIENTRY sglMultMatrixf(const GLfloat *m)                {(*fnglMultMatrixf)(m);}
void PACKAGE APIENTRY sglOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {(*fnglOrtho)(left,right,bottom,top,zNear,zFar);}
void PACKAGE APIENTRY sglPixelStorei (GLenum pname, GLint param)      {(*fnglPixelStorei)(pname, param);}
void PACKAGE APIENTRY sglPolygonOffset(GLfloat factor, GLfloat units) {(*fnglPolygonOffset)(factor,units);}
void PACKAGE APIENTRY sglPopMatrix(void)                              {(*fnglPopMatrix)();}
void PACKAGE APIENTRY sglPushMatrix(void)                             {(*fnglPushMatrix)();}
void PACKAGE APIENTRY sglRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {(*fnglRotatef)(angle,x,y,z);}
void PACKAGE APIENTRY sglScalef(GLfloat x, GLfloat y, GLfloat z)      {(*fnglScalef)(x,y,z);}
void PACKAGE APIENTRY sglShadeModel (GLenum mode)                     {(*fnglShadeModel)(mode);}
void PACKAGE APIENTRY sglTexCoord2f(GLfloat s, GLfloat t)             {(*fnglTexCoord2f)(s,t);}
void PACKAGE APIENTRY sglTexCoord2fv(const GLfloat *v)                {(*fnglTexCoord2fv)(v);}
void PACKAGE APIENTRY sglTexCoord3f(GLfloat s, GLfloat t, GLfloat r)  {(*fnglTexCoord3f)(s,t,r);}
void PACKAGE APIENTRY sglTexCoord3fv(const GLfloat *v)                {(*fnglTexCoord3fv)(v);}
void PACKAGE APIENTRY sglTexEnvf(GLenum target, GLenum pname, GLfloat param) {(*fnglTexEnvf)(target,pname,param);}
void PACKAGE APIENTRY sglTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) {(*fnglTexImage2D)(target,level,internalformat,width,height,border,format,type,pixels);}
void PACKAGE APIENTRY sglTexParameterf(GLenum target, GLenum pname, GLfloat param) {(*fnglTexParameterf)(target,pname,param);}
void PACKAGE APIENTRY sglTranslatef(GLfloat x, GLfloat y, GLfloat z)  {(*fnglTranslatef)(x,y,z);}
void PACKAGE APIENTRY sglVertex2f(GLfloat x, GLfloat y)               {(*fnglVertex2f)(x,y);}
void PACKAGE APIENTRY sglVertex3f(GLfloat x, GLfloat y, GLfloat z)    {(*fnglVertex3f)(x,y,z);}
void PACKAGE APIENTRY sglVertex3fv(const GLfloat *v)                  {(*fnglVertex3fv)(v);}
void PACKAGE APIENTRY sglVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {(*fnglVertex4f)(x,y,z,w);}
void PACKAGE APIENTRY sglViewport(GLint x, GLint y, GLsizei width, GLsizei height) {(*fnglViewport)(x,y,width,height);}
// LJW:
void PACKAGE APIENTRY sglCallList(GLuint list)                        {(*fnglCallList)(list);}
void PACKAGE APIENTRY sglFlush()                                      {(*fnglFlush)();}
void PACKAGE APIENTRY sglNewList(GLuint list,GLenum mode)             {(*fnglNewList)(list,mode);}
void PACKAGE APIENTRY sglPushName(GLuint name)                        {(*fnglPushName)(name);}
void PACKAGE APIENTRY sglColor3d(GLdouble red, GLdouble green, GLdouble blue) {(*fnglColor3d)(red,green,blue);}
void PACKAGE APIENTRY sglVertex3d(GLdouble x, GLdouble y,GLdouble z)  {(*fnglVertex3d)(x,y,z);}
void PACKAGE APIENTRY sglPopName()                                    {(*fnglPopName)();}
void PACKAGE APIENTRY sglEndList()                                    {(*fnglEndList)();}
void PACKAGE APIENTRY sglSelectBuffer(GLsizei size, GLuint *buffer)   {(*fnglSelectBuffer)(size,buffer);}
GLint PACKAGE APIENTRY sglRenderMode(GLenum mode)                     {return (*fnglRenderMode)(mode);}
void PACKAGE APIENTRY sglInitNames()                                  {(*fnglInitNames)();}
void PACKAGE APIENTRY sglGetDoublev(GLenum pname,GLdouble *params)    {(*fnglGetDoublev)(pname,params);}
void PACKAGE APIENTRY sglMultMatrixd(const GLdouble *m)               {(*fnglMultMatrixd)(m);}
void PACKAGE APIENTRY sglTranslated(GLdouble x, GLdouble y, GLdouble z) {(*fnglTranslated)(x,y,z);}
void PACKAGE APIENTRY sglDeleteLists(GLuint list,GLsizei range)       {(*fnglDeleteLists)(list,range);}
GLuint PACKAGE APIENTRY sglGenLists(GLsizei range)                    {return (*fnglGenLists)(range);}
void PACKAGE APIENTRY sglPushAttrib(GLbitfield mask)                  {(*fnglPushAttrib)(mask);}
void PACKAGE APIENTRY sglPopAttrib()                                  {(*fnglPopAttrib)();}
void PACKAGE APIENTRY sglVertex2i(GLint x, GLint y)                   {(*fnglVertex2i)(x,y);}
void PACKAGE APIENTRY sglRectf(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2) {(*fnglRectf)(x1,y1,x2,y2);}






// These are wgl functions
HGLRC PACKAGE APIENTRY sglCreateContext(HDC hdc)                     {return (*fnwglCreateContext)(hdc);}
BOOL PACKAGE APIENTRY sglDeleteContext(HGLRC hglrc)                  {return (*fnwglDeleteContext)(hglrc);}
BOOL PACKAGE APIENTRY sglSwapLayerBuffers(HDC hdc, UINT fuPlanes)    {return (*fnwglSwapLayerBuffers)(hdc, fuPlanes);}

BOOL PACKAGE APIENTRY sglMakeCurrent(HDC hdc, HGLRC hglrc)
{ BOOL mcres=  (*fnwglMakeCurrent)(hdc, hglrc);
  if (mcres && hglrc!=NULL && !GlobalVersionTried && sglFile.LowerCase()=="opengl32.dll")
  { Debug(dlMsg,"sglMakeCurrent for the first time... taking the opportunity to get vendor information.");
    GlobalVendor=(char*)sglGetString(GL_VENDOR);
    GlobalRenderer=(char*)sglGetString(GL_RENDERER);
    GlobalVersion=(char*)sglGetString(GL_VERSION);
    GlobalVersionSucc=true;
    GlobalVersionTried=true;
  }
  return mcres;
}




// These are windows functions. SwapBuffers is specially optimised not to call GDI
BOOL PACKAGE APIENTRY sglSwapBuffers(HDC hdc)
{ if (!sglBypassGDI) {int retValue=SwapBuffers(hdc); if (retValue) return retValue;}
  if (!sglInst || !fnSwapBuffers) return FALSE; else return (*fnSwapBuffers)(hdc);
}
int PACKAGE APIENTRY sglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR * ppfd)
{ if (fnChoosePixelFormat!=NULL) return (*fnChoosePixelFormat)(hdc, ppfd);
  return ChoosePixelFormat(hdc, ppfd);
}
int PACKAGE APIENTRY sglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd)
{ if (fnDescribePixelFormat!=NULL) return (*fnDescribePixelFormat)(hdc,iPixelFormat,nBytes,ppfd);
  return DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd);
}
BOOL PACKAGE APIENTRY sglSetPixelFormat(HDC hdc, int iPixelFormat, CONST PIXELFORMATDESCRIPTOR * ppfd)
{ if (sglBypassGDI && fnSetPixelFormat!=NULL) return (*fnSetPixelFormat)(hdc, iPixelFormat, ppfd);
  // apparently, for 3dfx, we must call the 3dfx's version of this;
  // but for opengl32.dll we must call the plain windows function and not the one
  // we find in opengl32.dll!!!
  return SetPixelFormat(hdc, iPixelFormat, ppfd);
}





// The code for these sglu* functions comes from Mesa.
// Their inclusion in here forces ScrGL to be a gnu library license thing.
// If you don't want the license, then you must delete these from the code.

void PACKAGE APIENTRY sgluOrtho2D( GLdouble left, GLdouble right,
                          GLdouble bottom, GLdouble top )
{
   sglOrtho( left, right, bottom, top, -1.0, 1.0 );
}


void PACKAGE APIENTRY sgluLookAt( GLdouble eyex, GLdouble eyey, GLdouble eyez,
                         GLdouble centerx, GLdouble centery, GLdouble centerz,
                         GLdouble upx, GLdouble upy, GLdouble upz )
{
   GLdouble m[16];
   GLdouble x[3], y[3], z[3];
   GLdouble mag;

   /* Make rotation matrix */

   /* Z vector */
   z[0] = eyex - centerx;
   z[1] = eyey - centery;
   z[2] = eyez - centerz;
   mag = sqrt( z[0]*z[0] + z[1]*z[1] + z[2]*z[2] );
   if (mag) {  /* mpichler, 19950515 */
      z[0] /= mag;
      z[1] /= mag;
      z[2] /= mag;
   }

   /* Y vector */
   y[0] = upx;
   y[1] = upy;
   y[2] = upz;

   /* X vector = Y cross Z */
   x[0] =  y[1]*z[2] - y[2]*z[1];
   x[1] = -y[0]*z[2] + y[2]*z[0];
   x[2] =  y[0]*z[1] - y[1]*z[0];

   /* Recompute Y = Z cross X */
   y[0] =  z[1]*x[2] - z[2]*x[1];
   y[1] = -z[0]*x[2] + z[2]*x[0];
   y[2] =  z[0]*x[1] - z[1]*x[0];

   /* mpichler, 19950515 */
   /* cross product gives area of parallelogram, which is < 1.0 for
    * non-perpendicular unit-length vectors; so normalize x, y here
    */

   mag = sqrt( x[0]*x[0] + x[1]*x[1] + x[2]*x[2] );
   if (mag) {
      x[0] /= mag;
      x[1] /= mag;
      x[2] /= mag;
   }

   mag = sqrt( y[0]*y[0] + y[1]*y[1] + y[2]*y[2] );
   if (mag) {
      y[0] /= mag;
      y[1] /= mag;
      y[2] /= mag;
   }

#define M(row,col)  m[col*4+row]
   M(0,0) = x[0];  M(0,1) = x[1];  M(0,2) = x[2];  M(0,3) = 0.0;
   M(1,0) = y[0];  M(1,1) = y[1];  M(1,2) = y[2];  M(1,3) = 0.0;
   M(2,0) = z[0];  M(2,1) = z[1];  M(2,2) = z[2];  M(2,3) = 0.0;
   M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
#undef M
   sglMultMatrixd( m );

   /* Translate Eye to Origin */
   sglTranslated( -eyex, -eyey, -eyez );

}




void PACKAGE APIENTRY sgluPerspective( GLdouble fovy, GLdouble aspect,
                              GLdouble zNear, GLdouble zFar )
{
   GLdouble xmin, xmax, ymin, ymax;

   ymax = zNear * tan( fovy * M_PI / 360.0 );
   ymin = -ymax;

   xmin = ymin * aspect;
   xmax = ymax * aspect;

   sglFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}



void PACKAGE APIENTRY sgluPickMatrix( GLdouble x, GLdouble y,
                             GLdouble width, GLdouble height,
                             const GLint viewport[4] )
{
   GLfloat m[16];
   GLfloat sx, sy;
   GLfloat tx, ty;

   sx = viewport[2] / width;
   sy = viewport[3] / height;
   tx = (viewport[2] + 2.0 * (viewport[0] - x)) / width;
   ty = (viewport[3] + 2.0 * (viewport[1] - y)) / height;

#define M(row,col)  m[col*4+row]
   M(0,0) = sx;   M(0,1) = 0.0;  M(0,2) = 0.0;  M(0,3) = tx;
   M(1,0) = 0.0;  M(1,1) = sy;   M(1,2) = 0.0;  M(1,3) = ty;
   M(2,0) = 0.0;  M(2,1) = 0.0;  M(2,2) = 1.0;  M(2,3) = 0.0;
   M(3,0) = 0.0;  M(3,1) = 0.0;  M(3,2) = 0.0;  M(3,3) = 1.0;
#undef M

   sglMultMatrixf( m );
}



/*
 * Perform a 4x4 matrix multiplication  (product = a x b).
 * Input:  a, b - matrices to multiply
 * Output:  product - product of a and b
 */
static void matmul( GLdouble *product, const GLdouble *a, const GLdouble *b )
{
   /* This matmul was contributed by Thomas Malik */
   GLdouble temp[16];
   GLint i;

#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define T(row,col)  temp[(col<<2)+row]

   /* i-te Zeile */
   for (i = 0; i < 4; i++)
     {
	T(i, 0) = A(i, 0) * B(0, 0) + A(i, 1) * B(1, 0) + A(i, 2) * B(2, 0) + A(i, 3) * B(3, 0);
	T(i, 1) = A(i, 0) * B(0, 1) + A(i, 1) * B(1, 1) + A(i, 2) * B(2, 1) + A(i, 3) * B(3, 1);
	T(i, 2) = A(i, 0) * B(0, 2) + A(i, 1) * B(1, 2) + A(i, 2) * B(2, 2) + A(i, 3) * B(3, 2);
	T(i, 3) = A(i, 0) * B(0, 3) + A(i, 1) * B(1, 3) + A(i, 2) * B(2, 3) + A(i, 3) * B(3, 3);
     }

#undef A
#undef B
#undef T
   memcpy( product, temp, 16*sizeof(GLdouble) );
}

/*
 * Compute inverse of 4x4 transformation matrix.
 * Code contributed by Jacques Leroy jle@star.be
 * Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
 */
static GLboolean invert_matrix( const GLdouble *m, GLdouble *out )
{
/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { GLdouble *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

 GLdouble wtmp[4][8];
 GLdouble m0, m1, m2, m3, s;
 GLdouble *r0, *r1, *r2, *r3;

 r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

 r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
 r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
 r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

 r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
 r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
 r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

 r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
 r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
 r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

 r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
 r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
 r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

 /* choose pivot - or die */
 if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
 if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
 if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
 if (0.0 == r0[0])  return GL_FALSE;

 /* eliminate first variable     */
 m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
 s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
 s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
 s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
 s = r0[4];
 if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r0[5];
 if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r0[6];
 if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r0[7];
 if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
 if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
 if (0.0 == r1[1])  return GL_FALSE;

 /* eliminate second variable */
 m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
 r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
 r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
 s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
 if (0.0 == r2[2])  return GL_FALSE;

 /* eliminate third variable */
 m3 = r3[2]/r2[2];
 r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
 r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
 r3[7] -= m3 * r2[7];

 /* last check */
 if (0.0 == r3[3]) return GL_FALSE;

 s = 1.0/r3[3];              /* now back substitute row 3 */
 r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

 m2 = r2[3];                 /* now back substitute row 2 */
 s  = 1.0/r2[2];
 r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
 r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
 m1 = r1[3];
 r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
 r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
 m0 = r0[3];
 r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
 r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

 m1 = r1[2];                 /* now back substitute row 1 */
 s  = 1.0/r1[1];
 r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
 r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
 m0 = r0[2];
 r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
 r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

 m0 = r0[1];                 /* now back substitute row 0 */
 s  = 1.0/r0[0];
 r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
 r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

 MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
 MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
 MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
 MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
 MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
 MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
 MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
 MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];

 return GL_TRUE;

#undef MAT
#undef SWAP_ROWS
}


/*
 * Transform a point (column vector) by a 4x4 matrix.  I.e.  out = m * in
 * Input:  m - the 4x4 matrix
 *         in - the 4x1 vector
 * Output:  out - the resulting 4x1 vector.
 */
static void transform_point( GLdouble out[4], const GLdouble m[16],
			     const GLdouble in[4] )
{
#define M(row,col)  m[col*4+row]
   out[0] = M(0,0) * in[0] + M(0,1) * in[1] + M(0,2) * in[2] + M(0,3) * in[3];
   out[1] = M(1,0) * in[0] + M(1,1) * in[1] + M(1,2) * in[2] + M(1,3) * in[3];
   out[2] = M(2,0) * in[0] + M(2,1) * in[1] + M(2,2) * in[2] + M(2,3) * in[3];
   out[3] = M(3,0) * in[0] + M(3,1) * in[1] + M(3,2) * in[2] + M(3,3) * in[3];
#undef M
}


/* transformation du point ecran (winx,winy,winz) en point objet */
GLint PACKAGE APIENTRY sgluUnProject(GLdouble winx,GLdouble winy,GLdouble winz,
                            const GLdouble model[16],const GLdouble proj[16],
                            const GLint viewport[4],
                            GLdouble *objx,GLdouble *objy,GLdouble *objz)
{
    /* matrice de transformation */
    GLdouble m[16], A[16];
    GLdouble in[4],out[4];

    /* transformation coordonnees normalisees entre -1 et 1 */
    in[0]=(winx-viewport[0])*2/viewport[2] - 1.0;
    in[1]=(winy-viewport[1])*2/viewport[3] - 1.0;
    in[2]=2*winz - 1.0;
    in[3]=1.0;

    /* calcul transformation inverse */
    matmul(A,proj,model);
    invert_matrix(A,m);

    /* d'ou les coordonnees objets */
    transform_point(out,m,in);
    if (out[3]==0.0)
       return GL_FALSE;
    *objx=out[0]/out[3];
    *objy=out[1]/out[3];
    *objz=out[2]/out[3];
    return GL_TRUE;
}







//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*) {return 1;}
namespace Scrgl
{ void __fastcall PACKAGE Register()
  { TComponentClass classes[] = {__classid(TSglPanel),
                                 __classid(TSglDriverList)};
    RegisterComponents("ScrPlus",classes,(sizeof(classes)/sizeof(classes[0]))-1);
  }
}



