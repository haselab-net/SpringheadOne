#include "FileIO.h"
#pragma hdrstop
#include <crtdbg.h>

namespace Spr{;

class FIDocConverter_Key: public FIDocConvBase{
public:
	UTString type;
	UTString GetType() const { return type; }
};
void FIDocConverter::Convert(FIDocNodeBase* f){
	static UTRef<FIDocConverter_Key> key;
	if (!key) key = new FIDocConverter_Key;
	key->type = f->GetType();
	std::pair<Convs::iterator, Convs::iterator> r = convs.equal_range(key);
	from = f;
	for(Convs::iterator it = r.first; it != r.second; ++it){
		(*it)->Pre(this);
		_CrtCheckMemory();
	}
	_CrtCheckMemory();
	for(int i=0; i < f->NChildren(); ++i){
		Convert(f->GetChild(i));
	}
	from = f;
	for(Convs::iterator it = r.first; it != r.second; ++it){
		(*it)->Post(this);
	}
}
void FIDocConverter::Register(FIDocConvBase* f){
	convs.insert(f);
}

FIDocNodeBase* FIDocConverter::From(){
	return from;
}
FIDocNodeBase* FIDocConverter::To(){
	return to;
}
void FIDocConverter::UpTo(){
	to = to->GetParent();
}
void FIDocConverter::ChangeTo(FIString t){
}
FIDocNodeBase* FIDocConverter::CreateNode(FIString t){
	FIDocNode* node =  new FIDocNode;
	FITypeDesc* td = typeDb ? typeDb->Find(t) : NULL;
	if (!td) td = new FITypeDesc(t);
	node->SetTypeDesc(td);
	if (to) to->AddChild(node);
	to = node;
	if (!root) root = to;
	return node;
}

}	//	namespace Spr
