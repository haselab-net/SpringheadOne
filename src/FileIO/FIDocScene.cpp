#include "FileIO.h"
#pragma hdrstop
#include <SceneGraph/SGFrame.h>
#include <SceneGraph/SGScene.h>
#pragma hdrstop

namespace Spr{;

SGOBJECTIMP(FILink, SGObject);
void FIObjectAdder::Add(SGObject* obj){
	objects.Push(obj);
}
void FIObjectAdder::Add(UTString name){
	assert(name.length());
	UTRef<FILink> link = new FILink;
	link->linkName = name;
	objects.Push(link);
}
void FIObjectAdder::Link(SGScene* s){
	for(SGObjects::iterator it = objects.begin(); it != objects.end(); ++it){
		FILink* link = DCAST(FILink, *it);
		if (link){
			SGObjectNames::TNameMap::iterator itMap = s->GetNameMap().find(link->linkName);
			if (itMap != s->GetNameMap().end()) link->linkName = itMap->second;
			SGObject* obj = s->FindObject(link->linkName);
			assert(obj);
			*it = obj;
		}
	}
}



SGObject* FIReferenceBase::Find(SGScene* s){
	return s->FindObject(name);
}

void FILoadScene::Load(SGScene* s, SGObject* obj, FIFileDoc* fileDoc){
	fileName = fileDoc->GetFileName();
	Load(s, obj, fileDoc->GetRoots());
}
void FILoadScene::Load(SGScene* s, FIFileDoc* fileDoc){
	fileName = fileDoc->GetFileName();
	Load(s, fileDoc->GetRoots());
}
void FILoadScene::Load(SGScene* s, FIDocNodes& docs){
	Load(s, NULL, docs); 
}
///	ドキュメントオブジェクトからシーングラフを作成
void FILoadScene::Load(SGScene* s, SGObject* obj, FIDocNodes& docsIn){
	scene = s;
    if (!obj) obj = scene;
	adderStack.Push();
	adderStack.Top().containers.Push(obj);
//	docsIn.Print(DSTR);
	for(FIDocNodes::iterator it = docsIn.begin(); it != docsIn.end(); ++it){
		docs.Push(*it);
		LoadRecursive();
		docs.Pop();
	}
	adders.Push(adderStack.Pop());
	SolveReferences();
	AddObjects();
	s->Loaded(s);
}
void FILoadScene::SolveReferences(){
	for(FIObjectAdders::iterator it = adders.begin(); it != adders.end(); ++it){
		it->Link(scene);
	}
}
void FILoadScene::AddObjects(){
	assert(adderStack.size() == 0);
	assert(containers.size() == 0);
	for(FIObjectAdders::iterator it = adders.begin(); it != adders.end(); ++it){
		for(SGObjects::iterator t = it->containers.begin(); t != it->containers.end(); ++t){
			for(SGObjects::iterator o = it->objects.begin(); o != it->objects.end(); ++o){
				(*t)->AddChildObject(*o, scene);
			}
		}
	}
	adders.clear();
}
void FILoadScene::AddObject(SGObject* obj){
	adderStack.Top().Add(obj);
}
void FILoadScene::AddObject(UTString name){
	adderStack.Top().Add(name);
}
void FILoadScene::CancelAddObject(){
	adderStack.Top().objects.pop_back();
}
void FILoadScene::LoadRecursive(){
	if (docs.Top()->IsReference()){		//	参照の場合，参照を追加．
		UTString name = docs.Top()->GetName();
		if (!name.length()){
			DSTR << "nameless link:";
			docs.end()[-2]->Print(DSTR);
		}
		AddObject(name);
	}else{								//	データの場合，ロードする．
//		DSTR << "doc:" << docs.Top()->GetName() << ":" << docs.Top()->GetType() << "-" << docs.Top()->NChildren() << std::endl;
		std::pair<FILoders::iterator,FILoders::iterator> range;
		range = loaders.EqualRange(docs.Top()->GetType());
		for(FILoders::iterator it = range.first; it != range.second; ++it) (*it)->Load(this);
		bool shouldPop = PushContainer();
		for(int i=0; i<docs.Top()->NChildren(); ++i){
			docs.Push( docs.Top()->GetChild(i) );
			LoadRecursive();
			docs.Pop();
		}
		if(shouldPop) PopContainer();
		for(FILoders::iterator it = range.second; it != range.first;){
			--it;
			(*it)->Loaded(this);
		}
	}
}
bool FILoadScene::PushContainer(){
	if (!containers.size()) return false;
	adderStack.Push();
	adderStack.Top().containers = containers;
	containers.clear();
	return true;
}
void FILoadScene::PopContainer(){
	adders.Push(adderStack.Pop());
}
void FISaveScene::Save(FIDocNodeBase* doc, SGScene* s, SGObject* obj){
	docs.push_back(doc);
	scene = s;
	if (!obj) obj = scene;
	SaveRecursive(obj);
	savedObjects.clear();
}
void FISaveScene::SaveRecursive(SGObject* obj){
	FISavers::iterator it = savers.Find(obj->GetTypeInfo()->ClassName());
	if (it != savers.end()){
		(*it)->Save(this, obj);
	}
}
void FISaveScene::SaveReference(SGObject* obj){
	FISavers::iterator it = savers.Find(obj->GetTypeInfo()->ClassName());
	if (it != savers.end()){
		UTRef<FIDocNodeBase> doc = CreateDocNode("REF");
		assert(strlen(obj->GetName()));
		doc->SetName(obj->GetName());
		docs.Top()->AddChild(doc);
	}
}

}
