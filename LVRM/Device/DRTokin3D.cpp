#include "DRTokin3D.h"
#include "WinBasis/WBWin.h"
#include "TKINPUT.h"
#include "TKINPUT_I.C"
namespace LVRM {;

DRTokin3D::DVInputPort::DVInputPort(DRTokin3D* r, int c):ch(c), realDevice(r){
	sprintf(name, "%s input port ch %d", realDevice->Name(), ch);
}

DRTokin3D::DRTokin3D(){
	sprintf(name, "Tokin 3D motion sensor");
	intf = NULL;
	CoInitialize(NULL);
}
DRTokin3D::~DRTokin3D(){
	if (intf){
		long retinfo;
		((ITk3dInput*)intf)->CloseDevice(&retinfo);
		((ITk3dInput*)intf)->Release();
	}
	CoUninitialize();
}
bool DRTokin3D::Init(){
	if ((ITk3dInput*)intf) return true;
	HRESULT hr = CoCreateInstance(CLSID_Tk3dInput, NULL, CLSCTX_INPROC, IID_ITk3dInput, &intf);
	if ( FAILED( hr )){
		DSTR.setf(std::ios::hex, std::ios::basefield);
		DSTR << "Failed to create ITk3dInput interface. result:" << hr << std::endl;
		return false;
	}
	// Access the IMath interface
	long retinfo;
	((ITk3dInput*)intf)->CreateDevice(&retinfo);
//	if (!retinfo) return false;
	//	DSTR << "CreateDevice :retinfo = 0x" << std::ios::hex << retinfo << std::endl;
	((ITk3dInput*)intf)->Poll( 0, &retinfo );
	//	DSTR << "Poll(0):retinfo = 0x" << std::ios::hex << retinfo << std::endl;
	return true;
}
void DRTokin3D::Register(DVVirtualDevicePool& vpool){
	vpool.Register(new DVOrientation(this));
	for(int i=0; i<5; i++){
		vpool.Register(new DVInputPort(this, i));
	}
}
void DRTokin3D::GetMatrix(Matrix3f& ori){
	TK3DISTATE tk3dIState;
	ZeroMemory( &tk3dIState, sizeof(TK3DISTATE) );
	long retinfo;
	((ITk3dInput*)intf)->GetDeviceData( 0, &tk3dIState, &retinfo);
	Quaternionf q;
	q = Quaternionf::Rot(tk3dIState.Alfa, 'y') * Quaternionf::Rot(tk3dIState.Beta, 'z') * Quaternionf::Rot(tk3dIState.Gamma, 'x');
	ori = Matrix3f(q);
}
Vec3f DRTokin3D::GetEuler(){
	TK3DISTATE tk3dIState;
	ZeroMemory( &tk3dIState, sizeof(TK3DISTATE) );
	long retinfo;
	((ITk3dInput*)intf)->GetDeviceData( 0, &tk3dIState, &retinfo);
	Vec3f euler;
	euler.Z() = tk3dIState.Alfa;
	euler.Y() = tk3dIState.Beta;
	euler.X() = tk3dIState.Gamma;
	return euler;
}
int DRTokin3D::GetPort(int ch){
	TK3DISTATE tk3dIState;
	ZeroMemory( &tk3dIState, sizeof(TK3DISTATE) );
	long retinfo;
	((ITk3dInput*)intf)->GetDeviceData( 0, &tk3dIState, &retinfo);
	int button = tk3dIState.Button;
	if (ch==0) return (button&0x01)!=0;
	if (ch==1) return (button&0x02)!=0;
	if (ch==2) return (button&0x04)!=0;
	if (ch==3) return (button&0x08)!=0;
	if (ch==4) return (button&0x10)!=0;
	return 0;
}
}	//	namespace LVRM
