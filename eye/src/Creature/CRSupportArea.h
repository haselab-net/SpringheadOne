// CRSupportArea.h: CRSaftyArea �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CR_SUPPORT_AREA_H
#define CR_SUPPORT_AREA_H

#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <Base/Affine.h>
#include <Base/BaseUtility.h>
#include <vector>
#include <fstream>
#include <Physics/PHContactEngine.h>

namespace Spr{;

class CRHuman;

class CRContactDetector : public CDCollisionListener, public UTRefCount{
public:
	struct CRContactPair{
		CRContactPair(PHContactEngine::FramePairRecord* f, float s): fpr(f), sign(s){}
		PHContactEngine::FramePairRecord* fpr;
		float sign;
	};
protected:
	SGFrames subjects;						///<	���̗��Ȃǎx������̂̍��̂̃��X�g
	std::set<SGFrame*> excepts;				///<	�̂ȂǁC���O���ׂ����̂̃��X�g
	CDCollisionEngine* collision;			///<	�Փ˔���G���W��
	std::vector<CRContactPair> contactPairs;///<	�ڐG�͌v�Z�̂��߂̐ڐG�y�A
	Vec3f contactForce, contactTorque;		///<	�ڐG�́C�ڐG�g���N
public:
	CRContactDetector(){}
	virtual ~CRContactDetector(){}
	virtual void AddSubject(SGFrame* f);///<	
	virtual void AddExcept(SGFrame* f);	///<	
	virtual void Loaded(SGScene* s);	///<	���[�h��̏���=����������
	std::vector<Vec3f> vtxs;			///<	�Փ˓_
	virtual void BeforeAll(SGScene* s){ vtxs.clear(); }
	virtual void AfterAll(SGScene* s){ CalcContactForce(); }
	//	�ڐG���Ă��钸�_��vtxs�ɕۑ�
	virtual void Analyzed(SGScene* scene, CDFramePairWithRecord* fr,
		CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	//	�ڐG�͂̌v�Z
	void CalcContactForce();
	Vec3f GetContactForce(){ return contactForce; }
	Vec3f GetContactTorque(){ return contactTorque; }
	Vec3f GetApplicationPoint(float y=0);						//	��p�_
};


class CRSupportArea : public CDCollisionListener{
public:
	CRSupportArea();
	virtual ~CRSupportArea();
	typedef std::vector< UTRef<CRContactDetector> > CREnumContacts;
	class CRSupportVtxs:public std::vector<Vec3f>{
	public:
		Vec3f ClosestPoint(Vec3f p);
	};
protected:
	CREnumContacts contacts;
	CRSupportVtxs supportVtxs;	// ����̈�̓_
	Vec3f contactForce, contactTorque;
public:
	void Step(){
		CalcSupportArea();
		CalcForce();
	}
	void SetContacts(CRContactDetector* contact);				//	����̈�ɂȂ�ڐG��
	void DrawCollisionVetxs(GRRender* render);					//	����̈�ƐڐG���_��`��
	CRSupportVtxs& GetSupportArea(){ return supportVtxs;}	// ����̈���擾
	Vec3f GetContactForce(){ return contactForce; }
	Vec3f GetContactTorque(){ return contactTorque; }
	Vec3f GetApplicationPoint(float y=0);						//	��p�_
protected:
	void CalcForce();
	void CalcSupportArea();										//	����̈�̌v�Z
};		

}		//end namespace Spr
#endif // !defined(CR_SUPPORT_AREA_H)
