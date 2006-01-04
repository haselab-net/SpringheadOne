#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <vcl.h>
#include "SHBox.h"
#include "SHScene.h"
#include "SHCamera.h"
#include <SceneGraph/SGScene.h>
#include <HIS/HIMouse.h>
#include <fstream>
#include <iomanip>


#pragma resource "*.res"
#pragma package(smart_init)



using namespace Spr;
//---------------------------------------------------------------------------
// ValidCtrCheck は、定義されたコンポーネントが純粋仮想関数を含む
// 抽象クラスではないことを確認するために定義されています。
//
//std::ofstream file("debug.txt");

static inline void ValidCtrCheck(TSHBox *)
{
	new TSHBox(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSHBox::TSHBox(TComponent* Owner)
	: TWinControl(Owner), scene(NULL), camera(NULL){
	camera = new TSHCamera(this);
}
__fastcall TSHBox::~TSHBox(){
	if (scene){
		std::vector<TSHBox*>::iterator it = std::find(scene->boxes.begin(), scene->boxes.end(), this);
		scene->boxes.erase(it);
	}
	delete camera;
	camera = NULL;
}
void __fastcall TSHBox::WndProc(Messages::TMessage &Message){
/*	TMessage* msg = &Message;
	::file << "WND:" << std::setbase(std::ios::hex) << msg->Msg;
	::file << " W" << msg->WParam << " L" << msg->LParam;
	::file << " Res" << msg->Result << std::endl;
	::file.flush();
*/
	TWinControl::WndProc(Message);
}
void __fastcall TSHBox::DefaultHandler(void *Message){
	TMessage* msg = (TMessage*)Message;
	switch (msg->Msg){
		case WM_PAINT:
			if (Scene){
				Scene->Render(this);
				ValidateRect(Handle, NULL);
			}else{
				TWinControl::DefaultHandler(Message);
			}
			break;
        case WM_LBUTTONDOWN:
            if (Scene){
                mouse.OnButtonDown(msg->LParamLo, msg->LParamHi);
				TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_MBUTTONDOWN:
            if (Scene){
                mouse.OnButtonDown(msg->LParamLo, msg->LParamHi);
				TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_RBUTTONDOWN:
            if (Scene){
                mouse.OnButtonDown(msg->LParamLo, msg->LParamHi);
				TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_LBUTTONUP:
            if (Scene){
                mouse.OnButtonUp();
				TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_MBUTTONUP:
            if (Scene){
                mouse.OnButtonUp();
				TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_RBUTTONUP:
            if (Scene){
                mouse.OnButtonUp();
				TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_MOUSEMOVE:
            if (Scene){
              	if(mouse.IsGood() ){
                    mouse.SetAxis(camera->Imp->data.view);
		            mouse.OnButtonMove(msg->WParam, msg->LParamLo, msg->LParamHi);
                    mouse.Update(0.005);
                    camera->Imp->data.view = mouse.GetAxis();
            		scene->NotifyChange();
                }
                TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_MOUSEWHEEL:
            if (Scene){
              	if(mouse.IsGood() ){
                    mouse.SetAxis(camera->Imp->data.view);
                    mouse.OnWheel(msg->WParamLo, msg->WParamHi/10);
                    camera->Imp->data.view = mouse.GetAxis();
            		scene->NotifyChange();
                }
                TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_LBUTTONDBLCLK:
            if (Scene){
            	if (msg->WParam == MK_SHIFT + MK_LBUTTON ){
            		// Shift + 左ダブルクリック時はカメラをロード時の状態に初期化
            		mouse.OnShiftLButtonDClick();
                    camera->Imp->data.view = mouse.GetAxis();
            	}
	            else if (msg->WParam == MK_LBUTTON) {
            		// 左ダブルクリック時はポインタの位置を原点に初期化
            		mouse.OnLButtonDClick();
            	}
                scene->NotifyChange();
				TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
        case WM_RBUTTONDBLCLK:
            if (Scene){
                if (msg->WParam == MK_RBUTTON) {
            		// 右ダブルクリック時はポインタの姿勢を水平に初期化
            		mouse.OnRButtonDClick();
            	}
                scene->NotifyChange();
				TWinControl::DefaultHandler(Message);
            }else{
                TWinControl::DefaultHandler(Message);
            }
            break;
 		default:
			TWinControl::DefaultHandler(Message);
		break;
	}
}

void __fastcall TSHBox::SetScene(TSHScene* s){
	if (scene == s) return;
	if (scene){
		std::vector<TSHBox*>::iterator it = std::find(scene->boxes.begin(), scene->boxes.end(), this);
		scene->boxes.erase(it);
	}
	scene = s;
	if (scene){
		scene->boxes.push_back(this);
		Invalidate();
	}
	if (!camera) return;
	camera->Imp = NULL;
	camera->Scene = scene;
	if (scene){
	    GRCamera* cam = DCAST(GRCamera, Scene->Imp->FindObject(cameraName.c_str()));
		if (!cam) Scene->Imp->GetRenderers().Find(cam);
		camera->Imp = cam;
    	// マウスの初期化
	    mouse.Init();
        mouse.SetScale(0.02f, 0.008f, 0.07f, 0.008f);
		if (camera->Imp){
			mouse.SetAxis(camera->Imp->data.view);
			mouse.SetAxisInit(camera->Imp->data.view);
		}
	}
}
void __fastcall TSHBox::SetCamera(TSHCamera* c){
	if (camera == c) return;
	if (c && c->Imp){
		camera->Imp = c->Imp;
		Name = c->Name;
	}
}
void __fastcall TSHBox::Loaded(void){
    if (!scene) return;
	scene->Imp->FindObject(camera->Imp, CameraName.c_str());
	if (!camera->Imp){
		camera->Imp = new Spr::GRCamera;
		scene->Imp->GetRenderers().Set(camera->Imp);
		camera->Name="NewCamera";
		scene->Imp->GetWorld()->AddChildObject(camera->Imp, scene->Imp);
	}
}
AnsiString __fastcall TSHBox::GetCameraName(){
	if (camera && camera->Imp) return camera->Imp->GetName();
	else return cameraName;
}
void __fastcall TSHBox::SetCameraName(AnsiString n){
	cameraName = n;
    if (Scene){
        GRCamera* cam = DCAST(GRCamera, Scene->Imp->FindObject(cameraName.c_str()));
        if (!cam){
            Scene->Imp->GetRenderers().Find(cam);
		}
        camera->Imp = cam;
    }
}

//---------------------------------------------------------------------------
namespace Shbox
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSHBox)};
		RegisterComponents("SpringHead", classes, 0);
	}
}
//---------------------------------------------------------------------------


