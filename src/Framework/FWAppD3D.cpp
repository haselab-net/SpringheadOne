#include "Framework.h"
#pragma hdrstop
#include "FWAppD3D.h"

namespace Spr{;
void FWAppD3D::Init(HWND hw){
	FWApp::Init(hw);
	if (initialLoadFile.length()){
		Load(initialLoadFile);
		initialLoadFile = "";
	}
}
void FWAppD3D::LoadImp(std::string fn){
	bBusy = true;
	cs.Enter();
	if (!render){
		initialLoadFile = fn;
		return;
	}
	lastLoadFile = fn;

	scene->Clear();
	scene->GetRenderers().Add(render);

	fileIO.Load(lastLoadFile.c_str(), scene);
//	scene->GetWorld()->Print(DSTR);
	ConnectHis();
	cs.Leave();
	bBusy = false;
}
void FWAppD3D::SaveImp(std::string fn){
	bBusy = true;
	cs.Enter();
	fileIO.Save(fn.c_str(), scene);
	cs.Leave();
	bBusy = false;
}

}
