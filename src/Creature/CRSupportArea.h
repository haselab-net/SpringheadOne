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

//#include "CRHuman.h"

namespace Spr{;

class CRHuman;

class CRSupportArea : public CDCollisionListener
{
public:
	CRSupportArea();
	virtual ~CRSupportArea();

	CDCollisionEngine* collision;		//	�Փ˔���G���W��

private:
	std::vector<Vec3f> coVetxs;			// �ڐG�_
	std::vector<Vec3f> supportVetxs;	// ����̈�̓_
	bool bAdd;							// AddListener�o�������ۂ�
	bool bClear;						// coVetxs���N���A����^�C�~���O
	int listenerPos;					// CDContactEngine�̒��ł̃��X�i�̈ʒu
	

public:
	void Step();
	void Load(SGScene* scene,CRHuman* crHuman);

	//	CollisionEngine���擾���ACDCollisionListener�̓o�^ 
	void AddListener(SGScene* scene);

	//�ڐG���Ă��钸�_��ۑ�
	void Analyzed(SGScene* scene, CDFramePairWithRecord* fr,
		CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	
	void DrawCollisionVetxs(GRRender* render);				// �ڐG���_��`��
	void ClearCoVetxs();									// �ۑ�����Ă���ڐG���_�����
	void SelectCDFrame(CRHuman* crHuman);					// �Փ˔��肷��Frame�̌���
	bool HasFrame(SGFrame* f,CRHuman* crHuman);				// Frame��VH���ǂ����m�F
	void CalcSupportArea();									// ����̈�̌v�Z
	std::vector<Vec3f> GetSupportArea(){ return coVetxs;}	// ����̈���擾

	// CalcSupportArea�Ŏg���ϐ�
	typedef struct
	{
		double			x;
		double			y;
		int				n;
	} M_point;

	int point_number;		// �}�E�X�̓_�̌� 
	int search_number;		// ���߂�_�̌� 
	M_point M_vec_draw;		// �}�E�X�̈ʒu
	std::vector<M_point> M_vec;	// �}�E�X�̓_�̗����@�������̓f�[�^�ɂȂ�?
	std::vector<M_point> S_vec;	// �O�`�ƂȂ�_�̍��W
	Vec2f coCog;				//����̈�̒��S�i�d�S�j


};		

}		//end namespace Spr
#endif // !defined(CR_SUPPORT_AREA_H)
