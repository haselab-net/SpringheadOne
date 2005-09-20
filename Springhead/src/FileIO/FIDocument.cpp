#include "FileIO.h"
#pragma hdrstop
#include <SceneGraph/SGFrame.h>
#include <SceneGraph/SGScene.h>

namespace Spr{;
//----------------------------------------------------------------------------
//	FIDocNodeBase
void FIDocNodeBase::Print(std::ostream& os, int indent) const {
	os << UTPadding(indent) << ((FIDocNodeBase*)this)->GetName().c_str() << " : " << ((FIDocNodeBase*)this)->GetType().c_str() << std::endl;
	PrintChildren(os, indent+2);
}
void FIDocNodeBase::PrintChildren(std::ostream& os, int indent)const {
	for(int i=0; i<NChildren(); ++i){
		((FIDocNodeBase*)this)->Child(i)->Print(os, indent);
	}
}
void FIDocNodeBase::GetDataImp(void* c, int sz, UTString id){
	FITypeDesc* td = GetTypeDesc();
	if (td){
		char* ptr = (char*)c;
		td->Read(ptr, this, id);
	}else{
		FIDocNodePos* ctx = ReadContext(id);
		bool rv = Read(ctx, (char*)c, sz);
        assert(rv);
		ReleaseContext(ctx);
	}
}
void FIDocNodeBase::SetDataImp(const void* c, size_t sz){
	FITypeDesc* td = GetTypeDesc();
	if (td){
		const char* ptr = (const char*)c;
		td->Write(this, ptr);
	}else{
		FIDocNodePos* ctx = WriteContext();
		Write(ctx, (char*)c, sz, "", "");
		ReleaseContext(ctx);
	}
}


void FIDocNodes::Print(std::ostream& os, int indent) const {
	for(const_iterator it = begin(); it != end(); ++it){
		(*it)->Print(os, indent);
	}
}

//----------------------------------------------------------------------------
//	FIDocNode
void FIDocNode::SetType(FITypeDesc* t){
	typeDesc = t;
}

UTString FIDocNode::GetType(){
	if (typeDesc) return typeDesc->GetTypeName();
	return "";
}
class FIDNContext:public FIDocNodePos{
public:
	int pos;
	FIDNContext(): pos(0) {}
};

FIDocNodePos* FIDocNode::ReadContextImp(FIString id){
	FIDNContext* ctx = new FIDNContext;
	if (id.length()){
		assert(0);	//	not yet implemented.
		FITypeDesc* td = GetTypeDesc();
		assert(td);
		FITypeDesc::Composit& comp = td->GetComposition();
		for(FITypeDesc::Composit::iterator it = comp.begin(); it != comp.end(); ++it){
			if (it->name.compare(id)==0) return ctx;
	//		if (!it->Read(, this, ctx)) break;
		}
	}
	return ctx;
}
bool FIDocNode::Read(FIDocNodePos* ctxBase, char* ptr, int sz){
	FIDNContext* ctx = (FIDNContext*)ctxBase;
	if (data.size() < size_t(ctx->pos+sz) ) return false;
	memcpy(ptr, &*(data.begin() + ctx->pos), sz);
	ctx->pos += sz;
	return true;
}
void FIDocNode::Write(FIDocNodePos* ctxBase, const char* ptr, int sz, FIString fn, FIString type){
	FIDNContext* ctx = (FIDNContext*)ctxBase;
	if (data.size() < size_t(ctx->pos+sz)) data.resize(ctx->pos+sz);
	memcpy(&*(data.begin()+ctx->pos), ptr, sz);
	ctx->pos += sz;
}

FIDocNodeBase* FIDocNode::FindChild(UTString type){
	for(int i=0; i<NChildren(); ++i){
		if (Children()[i]->GetType().compare(type)){
			return Children()[i];
		}
	}
	return NULL;
}

}	//	namespace Spr
