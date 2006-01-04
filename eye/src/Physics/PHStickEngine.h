#pragma once
#include "PHSolid.h"
#include <SceneGraph/SGScene.h>
#include <Physics/PHConvexPair.h>
#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>

namespace Spr{;


class PHStick: public SGObject{
public:
	SGOBJECTDEF(PHStick);
	UTRef<SGFrame> frame[2];
	float force;
	bool AddChildObject(SGObject* o, SGScene* s);
};
class PHContactEngine;
/**	�������G���W��	*/
class PHStickEngine:public SGBehaviorEngine, CDCollisionListener{
public:
protected:
	int listenerPos;
	int framePairRecordPos;
	PHContactEngine* contactEngine;
public:	
	///	���������X�g
	typedef std::vector< UTRef<PHStick> > Sticks;
	Sticks sticks;

	SGOBJECTDEF(PHStickEngine);
	bool AddChildObject(SGObject* o, SGScene* s);
	void After(SGScene* scene, CDFramePairWithRecord* fr);
	///
	int GetPriority() const { return SGBP_STICKENGINE; }
	///	���Ԃ� dt �i�߂�D(�������Ȃ��CListener �� Collide �����ׂĂ̏���������D)
	virtual void Step(SGScene* s){}
	///	sticks���N���A����D
	virtual void Clear(SGScene* s);
	///	���[�h�������̏����CCollisionEngine �ɃR�[���o�b�N��o�^����DCollisionEngine���Ȃ���΍��D
	virtual void Loaded(SGScene* scene);
};


}
