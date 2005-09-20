#include "FileIO.h"
#pragma hdrstop
#include <SceneGraph/SGFrame.h>
#include <SceneGraph/SGScene.h>

namespace Spr{;
//----------------------------------------------------------------------------
//	FIDocNodeBase
bool FIDocNodeBase::GetWholeData(void* data, size_t sz){
	FIIterator it = FirstAttr();
	char* ptr = (char*)data;
	size_t pos = 0;
	while(!IsLast(it)){
		if (pos >= sz) return false;
		size_t s = sz-pos;
		it.GetDataImp(ptr, s);
		pos += s;
	}
	return true;
}
bool FIDocNodeBase::SetWholeData(const void* data, size_t sz){
	FirstAttr().SetDataImp(data, sz);
	return true;
}
void FIDocNodeBase::Print(std::ostream& os, int indent) const {
	os << UTPadding(indent) << ((FIDocNodeBase*)this)->GetName().c_str() << " : " << ((FIDocNodeBase*)this)->GetType().c_str() << std::endl;
//	PrintAttr(os, indent+2);
	PrintChildren(os, indent+2);
}
void FIDocNodeBase::PrintAttr(std::ostream& os, int indent)const {
	FIIterator it = FirstAttr();
	while(!IsLast(it)){
		const FIDocNodeBase* node = it.GetNode();
		if (node){
			node->Print(os, indent+2);
			++it;
		}else{
			os << UTPadding(indent) << it.GetType() << " " << it.GetId() << " = ";
			size_t sz = it.GetElementSize();
			size_t n = it.GetNElement();
			char* data = new char[sz];
			for(unsigned i=0; i<n; ++i){
				it.GetDataImp(data, sz);
				for(size_t i=0; i<sz; ++i){
					if (isprint(data[i])) os << data[i];
					else os << "?";
				}
				os << " ";
				++it;
			}
			if (n == 0){
				os << "(null)";
				++it;
			}
			os << std::endl;
			delete data;
		}
	}
}
void FIDocNodeBase::PrintChildren(std::ostream& os, int indent)const {
	for(int i=0; i<NChildren(); ++i){
		((FIDocNodeBase*)this)->Child(i)->Print(os, indent);
	}
}
void FIDocNodes::Print(std::ostream& os, int indent) const {
	for(const_iterator it = begin(); it != end(); ++it){
		(*it)->Print(os, indent);
	}
}

//----------------------------------------------------------------------------
//	FIDocNode
FIString FIDocNode::GetType() const {
	return type;
}
void FIDocNode::SetType(FIString t){
	type = t;
}

FIIterator FIDocNode::FirstAttr() const {
	return FIIterator( new TIteratorImp((FIDocNode*)this, attrs.begin(), 0) );
}
FIIterator FIDocNode::LastAttr() const {
	return FIIterator( new TIteratorImp((FIDocNode*)this, attrs.end()-1, attrs.end()[-1].Size()) );
}
FIIterator FIDocNode::FindAttr(FIString id, size_t pos) const {
	return FIIterator( new TIteratorImp((FIDocNode*)this, attrs.Find(id), pos) );
}
bool FIDocNode::IsLast(const FIIterator& itBase) const {
	TIteratorImp* it = (TIteratorImp*)itBase.imp;
	if (it->it==attrs.end() || it->pos == it->it->Size() && it->it == attrs.end()-1) return true;
	return false;
}
FIDocNodeBase* FIDocNode::SetNewNode(FIString id, FIString type){
	FIDocNode* newNode = new FIDocNode;
	newNode->SetType(type);
	newNode->SetName(id);
	TAttrs::iterator it = attrs.Find(id);
	if (it != attrs.end()){
		it->nodes.push_back(newNode);
	}else{
		attrs.push_back(TAttr(id));
		attrs.back().nodes.push_back(newNode);
	}
	return newNode;	
}
FIDocNode::TAttrs::iterator FIDocNode::TAttrs::Find(FIString key) const {
	TAttr k;
	k.id = key;
	TAttrs::const_iterator it = std::find(begin(), end(), k);
	return (TAttrs::iterator&)it;
}


//----------------------------------------------------------------------------
//	FIDocNode::TIteratorImp

bool FIDocNode::TIteratorImp::Validate() const {
	if (pos < it->Size()) return true;
	while(pos == it->Size()){
		if (it == node->attrs.end()-1) return false;
		++ ((FIDocNode::TIteratorImp*)this)->it;
		(size_t&)pos = 0;
	}
	return true;
}
/*
bool FIDocNode::TIteratorImp::ValidateNode(){
	if (pos < it->Size()){
		if (it->nodes.size()) return true;
		return false;
	}else{
		if (it == node->attrs.end()-1) return false;
		TAttrs::iterator n = it;
		++n;
		if (n->nodes.size()){
			it = n;
			pos = 0;
			return true;
		}
	}
	return false;
}
bool FIDocNode::TIteratorImp::ValidateData(){
	if (pos < it->Size()){
		if (it->data.Size()) return true;
		return false;
	}else{
		if (it == node->attrs.end()-1) return false;
		TAttrs::iterator n = it;
		++n;
		if (n->data.Size()){
			it = n;
			pos = 0;
			return true;
		}
	}
	return false;
}
*/
FIString FIDocNode::TIteratorImp::GetId() const{
	if (Validate()) return it->id;
	else return FIString("");
}
FIString FIDocNode::TIteratorImp::GetType() const{
	if (Validate()) return it->data.type;
	else return FIString("");
}
size_t FIDocNode::TIteratorImp::GetElementSize() const{
	if (Validate()) return it->data.ElementSize();
	else return 0;
}
size_t FIDocNode::TIteratorImp::GetNElement() const{
	if (Validate()) return it->data.Size();
	else return 0;
}
bool FIDocNode::TIteratorImp::SetDataImp(const void* data, size_t& sz, size_t esz, FIString id, FIString type){
	if (it == node->attrs.end()){
		node->attrs.push_back(TAttr());
		it = node->attrs.end()-1;
	}
	if (it->nodes.size()!=0 || it->id.compare(id)!=0) return false;
	if (!esz) esz = sz;

	assert(it->data.type.length()==0 || it->data.type.compare(type)==0);
	assert(it->data.Size() == 0 || it->data.elementSize == esz);
	it->data.type = type;
	it->data.ElementSize(esz);
	size_t nWrite = sz / esz;
	it->data.Resize(pos + nWrite);
	sz = nWrite*esz;
	memcpy(it->data.Data(pos), data, sz);
	return true;
}
bool FIDocNode::TIteratorImp::GetDataImp(void* data, size_t& sz){
	if (!Validate()) return false;
	size_t esz = it->data.ElementSize();
	size_t n = esz ? sz/esz : 0;
	if (n > it->data.Size()) n = it->data.Size();
	if (n==0) return false;
	sz = n * esz;
	memcpy(data, it->data.Begin(), sz);
	return true;
}
bool FIDocNode::TIteratorImp::SetNode(FIDocNodeBase* n){
	if (Validate()){
		it->nodes[pos] = (FIDocNode*)n;
		return true;
	}else if(it->data.Size()==0){
		it->nodes.push_back((FIDocNode*)n);
	}
	return false;
}
FIDocNodeBase* FIDocNode::TIteratorImp::GetNode(){
	if (Validate() && it->nodes.size()) return it->nodes[pos];
	return NULL;
}

}	//	namespace Spr
