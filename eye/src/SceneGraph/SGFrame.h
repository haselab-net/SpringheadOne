#ifndef SG_FRAME_H
#define SG_FRAME_H

#include "SGObject.h"

namespace Spr {;
///	�o�E���f�B���O�{�b�N�X�̎���
class SGBBox{
	Vec3f bboxCenter;	///<	BBox�̒��S(���[�J���n)
	Vec3f bboxExtent;	///<	BBox�̑傫��(���[�J���n)
public:
	///	�o�E���f�B���O�{�b�N�X�̐ݒ�
	void SetBBoxCenterExtent(Vec3f c, Vec3f e){
		bboxCenter = c;
		bboxExtent = e;
	}
	///	�o�E���f�B���O�{�b�N�X�̐ݒ�
	void SetBBoxMinMax(Vec3f bmin, Vec3f bmax){
		bboxCenter = (bmin+bmax)*0.5f;
		bboxExtent = (bmax-bmin)*0.5f;
	}
	///	�o�E���f�B���O�{�b�N�X�̒ǉ�
	void AddBBox(Vec3f bmin, Vec3f bmax){
		Vec3f bboxMin = GetBBoxMin();
		Vec3f bboxMax = GetBBoxMax();
		bboxMin.element_min(bmin);
		bboxMax.element_max(bmax);
		SetBBoxMinMax(bboxMin, bboxMax);
	}
	///	���S
	Vec3f GetBBoxCenter(){ return bboxCenter; }
	///	�傫��
	Vec3f GetBBoxExtent(){ return bboxExtent; }
	///	�������[�_
	Vec3f GetBBoxMin(){ return bboxCenter-bboxExtent; }
	///	�傫���[�_
	Vec3f GetBBoxMax(){ return bboxCenter+bboxExtent; }
	///	�^����ꂽ�x�N�g���Ƃ̓��ς��ő�ƍŏ��̓_
	void GetSupport(const Vec3f& dir, float& minS, float& maxS){
		Vec3f ext0( bboxExtent.X(),  bboxExtent.Y(),  bboxExtent.Z());
		Vec3f ext1(-bboxExtent.X(),  bboxExtent.Y(),  bboxExtent.Z());
		Vec3f ext2( bboxExtent.X(), -bboxExtent.Y(),  bboxExtent.Z());
		Vec3f ext3( bboxExtent.X(),  bboxExtent.Y(), -bboxExtent.Z());
		float d = abs(dir*ext0);
		float d1 = abs(dir*ext1);
		if (d < d1) d = d1;
		float d2 = abs(dir*ext2);
		if (d < d2) d = d2;
		float d3 = abs(dir*ext3);
		if (d < d3) d = d3;
		float c = dir * bboxCenter;
		minS = c-d;
		maxS = c+d;
	}
};

class SGFrame;
///	Spr::Frame �̔z��D
class SPR_DLL SGFrames:public std::vector< UTRef<SGFrame> >{
public:
};

/**	�V�[���O���t�̃C���^�t�F�[�X�m�[�h�D
	�V�[���O���t�̑�����󂯕t����m�[�h�D�ʒu�p����ێ�����D
	���̃f�[�^�̃|�C���^��ێ�����D	*/
class SPR_DLL SGFrame: public SGObject, public UTTreeNode<SGFrame, SGFrames>{
public:
	SGOBJECTDEF(SGFrame);
	typedef UTTreeNode<SGFrame, SGFrames> Tree;
protected:
	///	�p��
	Affinef posture;
public:
	///	�o�E���f�B���O�{�b�N�X
	SGBBox bbox;
	///	�t���[���̒��g(�q�t���[���͊܂܂Ȃ�)
	SGObjects contents;

	///@name �V�[���O���t�\���̑���
	//@{
	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NChildObjects();
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ChildObject(size_t i);
	///	�q�I�u�W�F�N�g�̒ǉ�
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	�q�I�u�W�F�N�g�̍폜
	virtual bool DelChildObject(SGObject* o, SGScene* s);

	virtual SGFrame* FindFrame(UTString);

	///	�q�ɂȂ肦��I�u�W�F�N�g�̌^���̔z��
	virtual const UTTypeInfo** ChildCandidates();
	template <class T>
	void FindObject(T*& obj){
		for(unsigned i=0; i<NChildObjects(); ++i){
			obj = DCAST(T, ChildObject(i));
			if (obj) return;
		}
	}
	template <class T> void FindObject(UTRef<T>& obj){
		T* t = &*obj;
		FindObject(t);
		obj = t;
	}


	///	�q�m�[�h�Ȃǂ����ׂď�������D
	void Clear();
	
	///	�֘A�t����ꂽdoc���J��
	virtual void ReleaseDoc(){
		doc = NULL;
		ForEachChild(&SGFrame::ReleaseDoc);
		for(SGObjects::iterator it = contents.begin(); it != contents.end(); ++it){
			(*it)->ReleaseDoc();
		}
	}
	///	�m�[�h�̃_���v�\��
	void Print(std::ostream& os) const;
	//@}
	
	///@name �p��(�ʒu�C����)
	//@{
	const Vec3f& GetPosition(){ return posture.Pos(); }
	Matrix3f GetRotation(){ return posture.Rot(); }
	template<class T> void SetPosition(T p){ posture.Pos() = p; }
	template<class T> void SetRotation(T p){ posture.Rot() = p; }
	///	�e�m�[�h�ɑ΂���p��(�ʒu�Ɖ�]������킷4x4�s��)��Ԃ��D
	const Affinef& GetPosture(){ return posture;}
	/**	�e�m�[�h�ɑ΂���p��(�ʒu�Ɖ�]������킷4x4�s��)��ݒ肷��D
		PHSolid���g���Ă���Ƃ��́CSGFrame::SetPosture()�͎g���Ă͂Ȃ�Ȃ��D
		����CSolid���p�����㏑�����Ă��܂��̂ŁD */
	void SetPosture(const Affinef& a){ posture = a; }
	///	���[���h���W�n�ɑ΂���p��(�ʒu�Ɖ�]������킷4x4�s��)��Ԃ��D
	Affinef GetWorldPosture(){
		Affinef af;
		SGFrame* fr = this;
		while(fr){
			af = fr->posture * af;
			fr = fr->GetParent();
		}
		return af;
	}
	/**	���[���h���W�n�ɑ΂���p��(�ʒu�Ɖ�]������킷4x4�s��)��ݒ肷��D
		PHSolid���g���Ă���Ƃ��́CSGFrame::SetPosture()�͎g���Ă͂Ȃ�Ȃ��D
		����CSolid���p�����㏑�����Ă��܂��̂ŁD */
	void SetWorldPosture(const Affinef& a){
		if (GetParent()){
			posture = GetParent()->GetWorldPosture().inv() * a;
		}else{
			posture = a;
		}
	}
	///	�o�E���f�B���O�{�b�N�X�̌v�Z
	virtual void CalcBBox();
	///	�o�E���f�B���O�{�b�N�X�̗��[�̎擾
	void GetBBoxSupport(const Vec3f& dir, float& minS, float& maxS){
		bbox.GetSupport(GetPosture().Rot().inv()*dir, minS, maxS);
		float c = GetPosture().Pos() * dir;
		minS += c;
		maxS += c;
	}
	//@}
	///	���[�h��C�Q�Ƃ̉������I����Ă���Ăяo���֐��D
	void Loaded(SGScene* scene);

	///	�q�t���[�����܂߂āC���ׂĂ�Contacts��񋓂���
	void EnumContents(SGObjects& objs);
	void EnumContents(SGObjects& objs, SGFrames& frames);
};

}
#endif
