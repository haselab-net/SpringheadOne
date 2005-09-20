#include "Framework.h"
#pragma hdrstop
#include "FWFileIOD3D.h"
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>

namespace Spr{;

bool FWFileIOD3D::PrepareLoader(){
	if (loader) return true;
	loader = new FILoadScene;
	REGISTER_LOADER(loader, SceneGraph);
	REGISTER_LOADER(loader, ImpD3D);
	REGISTER_LOADER(loader, Collision);
	REGISTER_LOADER(loader, Physics);
	REGISTER_LOADER(loader, Creature);
	REGISTER_LOADER(loader, Graphics);
	return true;
}
bool FWFileIOD3D::Load(const char* fn, SGScene* scene, SGObject* obj){
	if (!PrepareLoader()) return false;
	
	D3FileDoc fileDoc;
	fileDoc.Load(fn);

	loader->Load(scene, obj, &fileDoc);
	return true;
}
bool FWFileIOD3D::PrepareSaver(){
	if (saver) return true;
	saver = new D3SaveScene;
	REGISTER_SAVER(saver, SceneGraph);
	REGISTER_SAVER(saver, ImpD3D);
	REGISTER_SAVER(saver, Collision);
	REGISTER_SAVER(saver, Physics);
	REGISTER_SAVER(saver, Creature);
	REGISTER_SAVER(saver, Graphics);
	return true;
}
bool FWFileIOD3D::Save(const char* fn, SGScene* scene, SGObject* obj){
	if (!PrepareSaver()) return false;
	UTRef<D3DocNode> doc = new D3DocNode("ROOT");
	saver->Save(doc, scene, obj);
	D3FileDoc fileDoc;
	for (int i=0; i<doc->NChildren(); ++i){
		fileDoc.Save(fn, doc->Child(i));
	}
	return true;
}

}
