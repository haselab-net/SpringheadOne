#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include <basepch.h>
#include <DesignEditors.hpp>
#include "SHComponent.h"
#include "SHScene.h"
#include "SHBox.h"
#include "SHCamera.h"
#include <SceneGraph/SGScene.h>
using namespace Spr;

#pragma package(smart_init)
void __fastcall TSHComponent::Loaded(){
	if (!imp){
		imp = Scene->Imp->FindObject(Name.c_str());
	}
}
void __fastcall TSHComponent::SetObject(SGObject* o){
	if (imp == o) return;
	if (imp) imp->userData = NULL;
	imp = o;
	if (imp){
		imp->userData = this;
		if (strlen(imp->GetName()) == 0){
			imp->SetName(Name.c_str(), scene->Imp);
		}
		while(Name != imp->GetName()){
			try{
				Name = imp->GetName();
			}catch(const EComponentError &E){
				imp->SetName(Name.c_str(), scene->Imp);
			}
		}
	}
	NotifyChange();
}
void __fastcall TSHComponent::SetName(const AnsiString n){
	if (Name == n) return;
	if (imp){
		imp->SetName(n.c_str(), scene->Imp);
		try{
			TComponent::SetName(imp->GetName());
		}
		catch(const EComponentError	&E){
			imp->SetName(Name.c_str(), scene->Imp);
			throw E;
		}
		if (scene) scene->NotifyChangeName(imp);
	}else{
		TComponent::SetName(n);
	}
}
void __fastcall TSHComponent::NotifyChange(TObject* o){
	if (scene) scene->NotifyChange(this);
}
