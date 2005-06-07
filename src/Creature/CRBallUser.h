// CRBallUser.h: CRBallUser �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////
#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>
#include <Creature/CRBallPuppet.h>
#include <SceneGraph/SGScene.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include <HIS/HISpidar4.h>

#if !defined(CRBALLUSER_H)
#define CRBALLUSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Spr{;

class CRBallUser: public CRBallPuppet{
public:
	SGOBJECTDEF(CRBallUser);
	CRBallUser();

	void Load(SGScene* scene);						// ��{�ݒ�BFrameWork��Load�ŌĂ΂��B
	bool Connect(UTRef<SGScene> scene);

	void SetSprings();
	void SetFixedPos();
	void SetSpidarPos(std::vector<HISpidar4*> spidars);
	void SetExpectedPos(int count, float dt);
	void SetSpidarForce(CRBallPuppet* puppet, SGScene* scene, std::vector<HISpidar4*> spidars);

	bool bSpidarForce;
};

}

#endif // !defined(CRBALLUSER)
