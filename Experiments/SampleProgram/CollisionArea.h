// CollisionArea.h: CollisionArea �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef COLLISIONAREA_H
#define COLLISIONAREA_H

#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>

#include <SceneGraph/SGScene.h>
#include <Base/Affine.h>
#include <Base/BaseUtility.h>

#include <Physics/PHHuman.h>
#include <vector>
#include <fstream>


class CollisionArea : public Spr::CDCollisionListener 
{
	Spr::GRRender* render;					//	�����_���[
	Spr::CDCollisionEngine* collision;		//	�Փ˔���G���W��
	//THuman* human;

public:

	CollisionArea();
	virtual ~CollisionArea();

///////////////////////   �֐�   /////////////////////////////////

	void Step(Spr::SGScene* scene,Spr::GRRender* mainrender,Spr::PHHuman& phHuman);

	//CollisionEngine���擾���ACDCollisionListener�̓o�^�̂��߁AAddListener���Ă�
	void ConnectBehaviorEngines(Spr::SGScene* scene,Spr::GRRender* mainrender,Spr::PHHuman& phHuman);
	
	//�ڐG���Ă��钸�_��ۑ�
	void Analyzed(Spr::SGScene* scene, Spr::CDFramePairWithRecord* fr,
		Spr::CDGeometryPair* geom, Spr::CDConvexPairWithRecord* conv, Spr::CDContactAnalysis* analyzer);
	
	//CDCollisionListener�̓o�^
	void AddListener();
	
	//�ڐG���_��`��
	void DrawCollisionVetxs();

	//�ۑ�����Ă���ڐG���_�����
	void ClearCoVetxs();
	
	void ReLoad();

	//�@�Փ˔��肷��Frame�̌���
	void SelectCDFrame(Spr::PHHuman& phHuman);

	bool HasFrame(Spr::SGFrame* f,Spr::PHHuman& phHuman);

	//�`��̂��߂̊֐�
	Spr::GRMaterialData Mat(Spr::Vec4f c){
		return Spr::GRMaterialData(c, c, c,	c, 0.0f);
	}



///////////////////////   �ϐ�   /////////////////////////////////
private:
	
public:
	std::vector<Spr::Vec3f> coVetxs;	//�ڐG�_
	bool bAdd;							//AddListener�o�������ۂ�
	bool bClear;						//coVetxs���N���A����^�C�~���O
	int listenerPos;					//CDContactEngine�̒��ł̃��X�i�̈ʒu
	
};

#endif
