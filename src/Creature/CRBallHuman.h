// CRBallHuman.h: CRBallHuman NXÌC^[tFCX
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//	[solidsÌÔ]
//(ã¼gAÌëj
//0:Waist(),1:Abdomen( ),2:Chest(¹),3:Head(ª),
//(ã¼gAErj
//4:soRUArm(ãr),5:soRFArm,(Or)6:soRHand(è)
//(ã¼gA¶rj
//7:soLUArm(ãr),8:soLFArm(Or),9:soLHand(è)
//(º¼gAEj
//10:soRThigh(åÚ),11:soRLThigh(ºÚ),12:soRHeel(æù),13:soRToe(Üæ)
//(º¼gAEj
//14:soLThigh(åÚ),15:soLLThigh(ºÚ),16:soLHeel(æù),17:soLToe(Üæ)
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//	[jointsÌÔ]
//-------	  ã¼g   --------------
//	(j
//0:joWaist (Ball)
//	(¹j
//1:joChest (Ball)
//	(ñj
//2:joNeck (Ball)
//	(E¨)
//3:joRShoulder (Ball)
//	(EI)
//4:joRElbow (Univ)
//	(Eèñ)
//5:joRWrist (Univ)
//	(¶¨)
//6:joLShoulder (Ball)
//	(¶I)
//7:joLElbow (Univ)
//	(¶èñ)
//8:joLWrist (Univ)
//-------	  º¼g   --------------
//	(EÒÖß)
//9:joRHip (Ball)
//	(EG)
//10:joRKnee (Hinge)
//	(E«ñ)
//11:joRAnkle (Ball)
//	(Ey¥Ü¸)
//12:joRToe (Hinge)
//	(¶ÒÖß)
//13:joLHip (Ball)
//	(¶G)
//14:joLKnee (Hinge)
//	(¶«ñ)
//15:joLAnkle (Ball)
//	(¶y¥Ü¸)
//16:joLToe (Hinge)
//////////////////////////////////////////////////////////////

#ifndef CRBallHuman_H
#define CRBallHuman_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Physics/PHSolid.h>
#include <Physics/PHJointPid.h>
#include <Physics/PHContactEngine.h>

#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>

#include <Base/Affine.h>
#include <vector>

#include "CRSupportArea.h"

#define JOINTS_NUM 17
#define SOLIDS_NUM 18

namespace Spr{;

struct CRBallHumanSolidInfo{
	Vec3f scaleRatio;	// eÊÌg·ÉÎ·éä/Q
	Vec3f scale;		// À¡
	float massRatio;	// eÊÌSÌdÉÎ·é¿Êä

	Vec3f scaleSafeRatio;
	Vec3f massCenterPos;
};

struct CRBallHumanJointInfo{
	Vec3f parentPosScale;	// eeÌ©çÌÖßÊuÌg·ÉÎ·éä
	Vec3f childPosScale;	// eqÌ©çÌÖßÊuÌg·ÉÎ·éä
	float rangeMax;			// Â®æÅål
	float rangeMin;			// Â®æÅ¬l
	float danpa;			// _pW
	float spring;			// olW
	float torqueMax;		// gNÌÅål
	float torqueMin;		// gNÌÅ¬l
	float axis;				// [hÀWnÉÎµÄÌ²Ì³Ìûü
	float initPos;			// î{p¨
	Quaternionf initQt;		// î{p¨(Quaternion)
};

DEF_RECORD(XBallHuman,{
	GUID Guid(){ return WBGuid("48CE97A5-A3C0-446f-A2A0-25D42168A1E4"); }
	FLOAT totalHeight;
	FLOAT totalMass;
});

class CRBallHuman: public SGBehaviorEngine{
public:
	friend class CRBallHumanLoader;
	friend class CRBallHumanSaver;
	SGOBJECTDEF(CRBallHuman);
	PHSolid* test;
	CRSupportArea supportArea;						// ÌSÌÌÀèÌæÌvZ
	std::vector<CRSupportArea> supportObject;		// Ìðx·éSolidÌÚG_©çÀèÌæðvZ

///////////////////////  Ï   /////////////////////////////////
public:
	UTRef<PHJointEngine> jointEngine;				//	WCgGW
	std::vector<PHSolid*> solids;					//	VHÌeÊðû[
	std::vector<PHJointBase*> joints;				//	VHÌeÖßðû[
	std::vector<PHJointPid*> jointPids;				//	VHÌeÖßðû[
	std::vector<PHJointBallPid*> jointBallPids;		//  
	std::vector<unsigned int> supportSolidNum[2];	//@VHðx·éÊÌÔðû[(æù,Üæ)(0:E«,1:¶«j
	std::vector<unsigned int> ankleJointNum[2];		//
	std::vector<int> noUseJoint;					// ÅK»(oX)ÅgíÈ¢ÖßÔ


	CRBallHumanJointInfo	jinfo[JOINTS_NUM];			// VHÌJointîñ
	CRBallHumanSolidInfo	sinfo[SOLIDS_NUM];			// VHÌSolidîñ

protected:
	bool bLoaded;	//@VHª[h(Connct)³ê½©Û©

	//VHÌp[^
	float totalMass;
	float totalHeight;


///////////////////////   Ö   ////////////////////////////////

public:
	CRBallHuman();
	virtual ~CRBallHuman();
	int GetPriority(){return SGBP_CREATURE;}

	///	LµÄ¢éIuWFNgÌ
	virtual size_t NChildObjects(){ return jointEngine ? 0 : 1; }
	///	LµÄ¢éIuWFNg
	virtual SGObject* ChildObject(size_t i){ return jointEngine; }
	///	QÆµÄ¢éIuWFNgÌ
	virtual size_t NReferenceObjects(){ return 0; }
	///	QÆµÄ¢éIuWFNg
	virtual SGObject* ReferenceObject(size_t i){ return NULL; };
	///	qIuWFNgÌÇÁ(LEQÆðâíÈ¢)
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	qIuWFNgÌí
	virtual bool DelChildObject(SGObject* o, SGScene* s){ return false; }

	// î{C^[tF[X
	virtual void Load(SGScene* scene);				// î{ÝèBFrameWorkÌLoadÅÄÎêéB
	virtual void Loaded(SGScene* scene);			// î{ÝèB
	virtual void LoadDerivedModel(SGScene* scene){}	//h¶f(User,VH)ÌÝè

	bool IsLoaded(){ return bLoaded; }				// [hÌmF
	void SetLoaded(bool load){bLoaded = load;}		// ÌSÌðÇÝÜ¸eXgµ½¢ê
	bool HasFrame(SGFrame* f);

	// VHðRg[Â\É·é(Solid,JointÌæ¾)
	virtual bool Connect(UTRef<SGScene> scene);
	void ConnectSolid(const char* name, UTRef<SGScene> scene);
	void ConnectJoint(const char* name, UTRef<SGScene> scene);

	//VHÌeíp[^ÌÝè
	void SetModel(SGScene* scene);				// ÈºÌÝèÖðg¢AVHðg¦éæ¤É·éB
	void SetTotalMass(float mass);
	void SetTotalHeight(float height);
	void SetMass();

	virtual void SetSolidInfo();
	virtual void SetJointInfo();
	virtual void SetJointRange();
	void SetOneJointRange(PHJoint1D* j, float min, float max);

	virtual void SetInertia();
	void SetOneInertia(Spr::PHSolid* solid, Spr::Vec3f Axis);
	void JointPIDMul(PHJointPid* jo, float mulP, float mulD);
	void JointBallPIDMul(PHJointBallPid* jo, float mulP, float mulD);
	virtual void SetJointSpring(float dt);	

	void AddJointPassivityResistance();
	
	// fÌXP[ðZbg
	void SetScale(SGScene* scene);
	void SetSolidScale();
	void SetJointScale();

	// fÌúp¨
	virtual void SetJointInitAngle();

	// VHÌSolid,Jointîñæ¾
	CRBallHumanSolidInfo GetSolidInfo(int i){return sinfo[i];}
	CRBallHumanJointInfo GetJointInfo(int i){return jinfo[i];}
	float GetTotalMass(){return totalMass;}						// VHÌSÌdðæ¾
	Vec3f GetCOG();												// VHÌdSÌÊuæ¾
	Vec3d GetCogVelocity();										// VHÌdSÌ¬xæ¾


	void CalcContactForce();
	virtual void RegistSupportParts(UTRef<SGScene> scene){}		// oXÉ¨¢ÄÌx·éSolidðo^(E«A¶«ÈÇ)
	virtual void SetBodyNum(){}									// oXÈÇÉKvÈSolid,JointÌÔÌo^
protected:
	void LoadX(const XBallHuman& xh);
	void SaveX(XBallHuman& xh) const;

};

}	//	end namespace Spr
#endif // !defined(CRBallHuman_H)
