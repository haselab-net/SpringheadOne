#ifndef GRCAMERA_H
#define GRCAMERA_H
#include <SceneGraph/SGFrame.h>

namespace Spr{;

/// �J�����̃f�[�^
class GRCameraData{
public:
	void InitData();
	///	���_�s��
	Affinef view;
	///	@name	�ˉe�s��̗v�f
	//@{
	/// �X�N���[���̃T�C�Y
	Vec2f size;
	/// �J��������̃X�N���[���̂���
	Vec2f center;
	/// �J�����ƃN���b�s���O���ʂƂ̋���
	float front, back;
	///
	GRCameraData();
	//@}
};
/// �J�����̎���
class GRCamera:public SGObject{
public:
	SGOBJECTDEF(GRCamera);
	GRCameraData data;
	///	�J�����ʒu�������t���[��
	UTRef<SGFrame> frPosture;
	///	�Q�ƃt���[���̒ǉ�
	bool AddChildObject(SGObject* obj, SGScene* s);
	virtual size_t NReferenceObjects(){ return frPosture ? 1 : 0;}
	virtual SGObject* ReferenceObject(size_t i){ return i==0 ? (SGObject*)frPosture : (SGObject*)NULL; }
};
class GRCameras:public std::vector< UTRef<GRCamera> >{
};


}
#endif
