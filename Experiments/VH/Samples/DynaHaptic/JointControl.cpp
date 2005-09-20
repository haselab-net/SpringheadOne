// JointControl.cpp: JointControl クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JointControl.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

namespace Spr{;

JointControl::JointControl()
{
	jo1 = NULL;
}

JointControl::~JointControl()
{

}

void JointControl::Load(SGScene* scene){
	//jo1 = DCAST(PHJoint1D,  scene->FindObject("jo1"));
	jo1 = DCAST(PHJointUniversal,  scene->FindObject("jo1"));

}
void JointControl::GetJointData(){
	if(jo1){
		DSTR << "角度1:" << jo1->GetJointPosition(0) << std::endl;;
		DSTR << "角度2:" << jo1->GetJointPosition(1) << std::endl;;
	}
}

}		// end namespace