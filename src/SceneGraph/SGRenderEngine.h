#ifndef SGRENDERENGINE_H
#define SGRENDERENGINE_H

#include "SGEngine.h"

namespace Spr {;

///	����G���W���̊�{�N���X�D
class SGRenderEngine:public SGEngine{
	SGOBJECTDEF(SGRenderEngine);
public:
	///	�����_�����O���s��
	virtual void Render(SGScene* s){}
};

///	SGEngine�̔z��
class SGRenderEngines: public SGEngines{
public:
	///@name �����_�����O����
	//@{
	///	(�O���t�B�b�N�X)�����_�����O�D
	virtual void Render(SGScene* s);
	//@}
};

}
#endif
