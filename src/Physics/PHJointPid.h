#ifndef PH_JOINTPID_H
#define PH_JOINTPID_H

#include <Physics/PHJoint1D.h>
#include <Physics/PHJointMulti.h>

namespace Spr{;

class PHJointPid:public SGBehaviorEngine{
protected:
	///	前回の目標値
	float lastGoal;
	///	積分
	float integratedError;
public:
	SGOBJECTDEF(PHJointPid);
	///	制御対象関節
	UTRef<PHJointBase> joint;
	///	対象の軸
	int axis;
	///	目標値
	float goal;
	///	PIDの係数
	float proportional, differential, integral;
	///	目標のタイプ 0:位置制御	1：速度制御
	int type;
	/// proportional,differential,integralによるトルク(PID制御)
	float p_torque, d_torque, i_torque;

	/// p_torque, d_torque,i_torqueの取得
	float GetProportionalTorque(){ return p_torque; }
	float GetDifferentialTorque(){ return d_torque; }
	float GetIntegralTorque(){ return i_torque; }
	
	///	
	PHJointPid():type(0), proportional(0), differential(0), integral(0), goal(0), lastGoal(0), integratedError(0), axis(0){}
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

	///
	double GetJointTorque(){ return joint->GetJointTorque(axis); }

	size_t NReferenceObjects();
	SGObject* ReferenceObject(size_t i);
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
protected:
};

class PHJointBallPid:public SGBehaviorEngine{
protected:
	///	前回のゴール
	Quaterniond lastGoal;
	///	積分
	Vec3f integratedError;
public:
	SGOBJECTDEF(PHJointBallPid);
	///	制御対象関節
	UTRef<PHJointBall> joint;
	///	目標値
	Quaternionf goal;
	///	PIDの係数
	float proportional, differential, integral;
	/// proportional,differential,integralによるトルク(PID制御)
	Vec3f p_torque, d_torque, i_torque;

	/// p_torque, d_torque,i_torqueの取得
	Vec3f GetProportionalTorque(){ return p_torque; }
	Vec3f GetDifferentialTorque(){ return d_torque; }
	Vec3f GetIntegralTorque(){ return i_torque; }
	
	///	
	PHJointBallPid():proportional(0), differential(0), integral(0){}
	///
	static PHJointBallPid* Find(PHJointBall* j, SGScene* scene);
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

	///
	Vec3f GetJointTorque(){ return joint->GetTorque(); }

	size_t NReferenceObjects();
	SGObject* ReferenceObject(size_t i);
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);	
};


}

#endif