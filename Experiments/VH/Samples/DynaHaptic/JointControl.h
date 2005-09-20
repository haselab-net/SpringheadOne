// JointControl.h: JointControl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef JOINTCONTROL_H
#define JOINTCONTROL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Physics/PHJointPid.h>
#include <Physics/PHJoint.h>
#include <Physics/PHJointMulti.h>
namespace Spr{;

class JointControl  
{
public:
	JointControl();
	virtual ~JointControl();

	void Load(SGScene* scene);
	void GetJointData();

	//UTRef<PHJoint1D> jo1;
	UTRef<PHJointUniversal> jo1;

};

}		// end namespace
#endif // !defined(JOINTCONTROL_H)
