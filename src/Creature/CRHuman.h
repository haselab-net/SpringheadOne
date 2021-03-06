// CRHuman.h: CRHuman NXÌC^[tFCX
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
//0:joWaist1(-X), 1:joWaist2(Z), 2:joWaist3(-Y)
//	(¹j
//3:joChest1(-X), 4:joChest2(Z), 5:joChest3(-Y)
//	(ñj
//6:joNeck1(-X), 7:joNeck2(Z), 8:joNeck3(-Y)
//	(E¨)
//9:joRShoulder1(X), 10:joRShoulder2(Z), 11:joRShoulder3(-Y)
//	(EI)
//12:joRElbow1(X), 13:joRElbow2(-Y)
//	(Eèñ)
//14:joRWrist1(-Z), 15:joRWrist2(X)
//	(¶¨)
//16:joLShoulder1(X), 17:joLShoulder2(-Z), 18:joLShoulder3(Y)
//	(¶I)
//19:joLElbow1(X), 20:joLElbow2(Y)
//	(¶èñ)
//21:joLWrist1(Z), 22:joLWrist2(X)
//-------	  º¼g   --------------
//	(EÒÖß)
//23:joRHip1(X), 24:joRHip2(Z), 25:joRHip3(-Y)
//	(EG)
//26:joRKnee(-X)
//	(E«ñ)
//27:joRAnkle1(X), 28:joRAnkle2(Z), 29:joRAnkle3(-Y)
//	(Ey¥Ü¸)
//30:joRToe(-X)
//	(¶ÒÖß)
//31:joLHip1(X), 32:joLHip2(-Z), 33:joLHip3(Y)
//	(¶G)
//34:joLKnee(-X)
//	(¶«ñ)
//35:joLAnkle1(X), 36:joLAnkle2(-Z), 37:joLAnkle3(Y)
//	(¶y¥Ü¸)
//38:joLToe(-X)
//////////////////////////////////////////////////////////////

#ifndef CRHuman_H
#define CRHuman_H

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

#define JOINT_NUM 39
#define SOLID_NUM 18

namespace Spr{;

struct CRHumanSolidInfo{
	Vec3f scaleRatio;	// eÊÌg·ÉÎ·éä/Q
	Vec3f scale;		// À¡
	float massRatio;	// eÊÌSÌdÉÎ·é¿Êä

	Vec3f scaleSafeRatio;
	Vec3f massCenterPos;
};

struct CRHumanJointInfo{
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
};


DEF_RECORD(XHuman,{
	GUID Guid(){ return WBGuid("48CE97A5-A3C0-446f-A2A0-25D42168A1E4"); }
	FLOAT totalHeight;
	FLOAT totalMass;
});

class CRHuman: public SGBehaviorEngine{
public:
	friend class CRHumanLoader;
	friend class CRHumanSaver;
	SGOBJECTDEF(CRHuman);
	PHSolid* test;
	CRSupportArea supportArea;						// ÌSÌÌÀèÌæÌvZ
	std::vector<CRSupportArea> supportObject;		// Ìðx·éSolidÌÚG_©çÀèÌæðvZ

///////////////////////  Ï   /////////////////////////////////
public:
	UTRef<PHJointEngine> jointEngine;				//	WCgGW
	std::vector<PHSolid*> solids;					//	VHÌeÊðû[
	std::vector<PHJointBase*> joints;				//	VHÌeÖßðû[
	std::vector<PHJointPid*> jointPids;				//	VHÌeÖßðû[
	std::vector<unsigned int> supportSolidNum[2];	//@VHðx·éÊÌÔðû[(æù,Üæ)(0:E«,1:¶«j
	std::vector<unsigned int> ankleJointNum[2];		//
	std::vector<int> noUseJoint;					// ÅK»(oX)ÅgíÈ¢ÖßÔ


	CRHumanJointInfo	jinfo[JOINT_NUM];			// VHÌJointîñ
	CRHumanSolidInfo	sinfo[SOLID_NUM];			// VHÌSolidîñ

protected:
	bool bLoaded;	//@VHª[h(Connct)³ê½©Û©

	//VHÌp[^
	float totalMass;
	float totalHeight;


///////////////////////   Ö   ////////////////////////////////

public:
	CRHuman();
	virtual ~CRHuman();
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
	virtual void SetJointSpring(float dt);	

	void AddJointPassivityResistance();
	
	// fÌXP[ðZbg
	void SetScale(SGScene* scene);
	void SetSolidScale();
	void SetJointScale();

	// fÌúp¨
	virtual void SetJointInitAngle();

	// VHÌSolid,Jointîñæ¾
	CRHumanSolidInfo GetSolidInfo(int i){return sinfo[i];}
	CRHumanJointInfo GetJointInfo(int i){return jinfo[i];}
	float GetTotalMass(){return totalMass;}						// VHÌSÌdðæ¾
	Vec3f GetCOG();												// VHÌdSÌÊuæ¾
	Vec3d GetCogVelocity();										// VHÌdSÌ¬xæ¾


	void CalcContactForce();
	virtual void RegistSupportParts(UTRef<SGScene> scene){}		// oXÉ¨¢ÄÌx·éSolidðo^(E«A¶«ÈÇ)
	virtual void SetBodyNum(){}									// oXÈÇÉKvÈSolid,JointÌÔÌo^
protected:
	void LoadX(const XHuman& xh);
	void SaveX(XHuman& xh) const;

};

}	//	end namespace Spr
#endif // !defined(CRHuman_H)
