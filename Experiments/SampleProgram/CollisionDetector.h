// CollisionDetector.h: CollisionDetector �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>

#include <SceneGraph/SGScene.h>
#include <Base/Affine.h>
#include <Base/BaseUtility.h>
#include <vector>

class CollisionDetector : public Spr::CDCollisionListener 
{
	Spr::GRRender* render;					//	�����_���[
	Spr::CDCollisionEngine* collision;		//	�Փ˔���G���W��
public:
	CollisionDetector();
	virtual ~CollisionDetector();
	void ConnectBehaviorEngines(Spr::SGScene* scene,Spr::GRRender* mainrender);
	//�ڐG���Ă��钸�_��ۑ�
	void Analyzed(Spr::SGScene* scene, Spr::CDFramePairWithRecord* fr,
		Spr::CDGeometryPair* geom, Spr::CDConvexPairWithRecord* conv, Spr::CDContactAnalysis* analyzer);
	//CDCollisionListener�̓o�^
	//void AddListener(Spr::SGScene* scene,Spr::CDCollisionEngine* collision);
	void AddListener();
	//�ڐG���_��`��
	void DrawCollisionVetxs();
	//�ۑ�����Ă���ڐG���_�����
	void ClearCoVetxs();
	
	Spr::GRMaterialData Mat(Spr::Vec4f c){
		return Spr::GRMaterialData(c, c, c,	c, 0.0f);
	}
	std::vector<Spr::Vec3f> coVetxs;
	bool bAdd;
};

#endif
