#ifndef PH_JOINTPID_H
#define PH_JOINTPID_H

#include <Physics/PHJoint1D.h>

namespace Spr{;

class PHJointPid:public SGBehaviorEngine{
protected:
	///	前回の誤差
	float error;
	///	積分
	float integratedError;
public:
	SGOBJECTDEF(PHJointPid);
	///	制御対象関節
	UTRef<PHJoint1D> joint;
	///	目標値
	float goal;
	///	PIDの係数
	float proportional, differential, integral;
	///	目標のタイプ 0:位置制御	1：速度制御
	int type;
	
	///	
	PHJointPid():type(0), proportional(0), differential(0), integral(0), goal(0), error(0), integratedError(0){}
	///
	static PHJointPid* Find(PHJoint1D* j, SGScene* scene);
	///	プライオリティ
	virtual int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	シーンをクリアするときの処理
	virtual void Clear(SGScene* s){ joint=NULL; }

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
	///	PID制御をかける．
	void Step(SGScene* s);	

	size_t NReferenceObjects();
	SGObject* ReferenceObject(size_t i);
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
protected:
};


}

#endif