// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//	2001 Shoichi Hasegawa
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.
#include "node.h"
#include "type.h"

namespace LVRM {
//----------------------------------------------------------------------------
//	TNode
void TNode::SetElm(const char *fieldName, TField *f) {
	nodeElms[fieldName] = *f;
	for(TField::iterator it = f->begin(); it != f->end(); ++it){
		TFieldBaseSts* fbSts = dynamic_cast<TFieldBaseSts*>(&**it);
		if (fbSts){
			TSts* sts = fbSts->GetSts();
			for(TSts::iterator its = sts->begin(); its != sts->end(); ++its){
				(*its)->SetParent(this);
			}
		}
	}
}
TField* TNode::GetField(const char* fieldName){
	TField* field = NULL;
	TNodeElms::iterator it = nodeElms.find(fieldName);
	if(it != nodeElms.end()) field = &it->second;
	if (!field){
		TTypeProto* tp = (TTypeProto*)GetType();
		it = tp->defaults.find(fieldName);
		if(it != tp->defaults.end()) field = &it->second;
	}
	int nest = 0;
	while (field && field->size()){
		TField* ref = field->back()->GetRefer(nest++);
		if (ref) field = ref;
		else break;
	}
	return field;
}
void TNode::GetPosture(Affinef& posture){
	TNode* pn = this;
	pn = pn->GetParent();
	while(pn){
		float		center[] = {0, 0, 0};
		float		translation[] = {0, 0, 0};
		float		rotation[] = {0, 0, 1, 0};
		float		scale[] = {1, 1, 1};
		float		scaleOrientation[] = {0, 0, 1, 0};
		
		CopyElm(pn, "center", center);
		CopyElm(pn, "translation", translation);
		CopyElm(pn, "rotation", rotation);
		CopyElm(pn, "scale", scale);
		CopyElm(pn, "scaleOrientation", scaleOrientation);
		
		posture.Pos() -= (Vec3f&) center;
		if(scaleOrientation[3]){
			Affinef rot(- 180/M_PI * scaleOrientation[3], (Vec3f&)scaleOrientation);
			posture = rot * posture;
		}
		if(scale[0] != 1 || scale[1] != 1 || scale[2] != 1){
			posture.Ex() *= scale[0];
			posture.Ey() *= scale[1];
			posture.Ez() *= scale[2];
		}
		if(scaleOrientation[3]){
			Affinef rot(180/M_PI * scaleOrientation[3], (Vec3f&)scaleOrientation);
			posture = rot * posture;
		}
		posture.Pos() += (Vec3f&) center;
		if(rotation[3]){
			Affinef rot(180/M_PI * rotation[3], (Vec3f&)rotation);
			posture = rot * posture;
		}
		posture.Pos() += (Vec3f&) center;
		posture.Pos() += (Vec3f&) translation;
		pn = pn->GetParent();
	}
}

//----------------------------------------------------------------------------
//	TFieldbaseRefer
TField* TFieldBaseRefer::GetRefer(int nest){
	if (TType::protoStack.size() > nest){
		TNode* pnProto = TType::protoStack.end()[-1-nest];
		TNodeElms::iterator it = pnProto->NodeElms().find(value);
		if(it != pnProto->NodeElms().end()) return &it->second;

		TTypeProto* tp = (TTypeProto*)pnProto->GetType();
		it = tp->defaults.find(value);
		if(it != tp->defaults.end()) return &it->second;
	}else{
		cerr << "IS statement was used without PROTO" << endl;
	}
	return NULL;
}

}	//	namespace LVRM
