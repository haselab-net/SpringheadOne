//---------------------------------------------------------------------------
#ifndef TSHBoxH
#define TSHBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include "SHCamera.h"
#include "SHScene.h"
#include <HIS/HIMouse.h>
#pragma link "SpringheadBCB.lib"
#pragma link "C:\\DXSDK\\lib\\bcb\\d3d8.lib"
#pragma link "C:\\DXSDK\\lib\\bcb\\d3dx8d.lib"
#pragma link "C:\\DXSDK\\lib\\bcb\\d3dxof.lib"

//---------------------------------------------------------------------------
class PACKAGE TSHBox : public TWinControl
{
	AnsiString cameraName;
	AnsiString __fastcall GetCameraName();
	void __fastcall SetCameraName(AnsiString n);
	void __fastcall SetCamera(TSHCamera* c);
protected:
    /// マウス
    Spr::HIMouse mouse;
	TSHScene* scene;
	TSHCamera* camera;
	void __fastcall SetScene(TSHScene* s);
	//	TSHBox上でのマウスの操作などのメッセージを受け取るために使用する．
	virtual void __fastcall DefaultHandler(void *Message);
	virtual void __fastcall WndProc(Messages::TMessage &Message);
	virtual void __fastcall Loaded();
public:
	__fastcall TSHBox(TComponent* Owner);
	__fastcall ~TSHBox();
__published:
	__property TSHScene* Scene={read=scene, write=SetScene};
	__property TSHCamera* Camera={read=camera, write=SetCamera, stored=false};
	__property AnsiString CameraName={read=GetCameraName, write=SetCameraName};
	__property Align;


};
//---------------------------------------------------------------------------
#endif

