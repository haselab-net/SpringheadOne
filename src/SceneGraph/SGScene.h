#ifndef SGSCENE_H
#define SGSCENE_H

#include "SGFrame.h"
#include "SGBehaviorEngine.h"
#include "SGRenderEngine.h"

namespace Spr {;


class SGScene;

class SGObjectNamesLess{
public:
	bool operator () (SGObject* o1, SGObject* o2) const {
		return strcmp(o1->GetName(), o2->GetName()) < 0;
	}
};
/**	���O�ƃV�[���O���t�̃I�u�W�F�N�g�̑Ή��\
	���O���L�[�ɂ����Z�b�g��p�ӂ��C���O�̈�Ӑ��̕ۏ؂ƃI�u�W�F�N�g��
	��������������D	*/
class SGObjectNames:public std::set<UTRef<SGObject>, SGObjectNamesLess >{
public:
	typedef std::map<UTString, UTString> TNameMap;
	TNameMap nameMap;
	void Print(std::ostream& os) const;
	///	���O����I�u�W�F�N�g���擾
	SGObject* Find(UTString name) const {
		SGObject key;
		key.name = name;
		key.AddRef();
		const_iterator it = find(&key);
		key.DelRef();
		if (it != end()) return *it;
		return NULL;
	}
	std::pair<iterator, iterator> Range(UTString name){
		std::pair<iterator, iterator> rv;
		SGObject key;
		key.name = name;
		key.AddRef();
		rv.first = find(&key);
		key.DelRef();
		if (rv.first == end()){
			rv.second = end();
		}else{
			rv.second = rv.first;
			++rv.second;
			for(; rv.second!=end(); ++rv.second){
				if (rv.second == end()) break;
				if (name.compare(0, name.length(), (*rv.second)->GetName())!=0) break;
			}
		}
		return rv;
	}
	/**	�I�u�W�F�N�g�̒ǉ��C
		���O�̂Ȃ��I�u�W�F�N�g�͒ǉ��ł��Ȃ��D���̏ꍇ false ��Ԃ��D
		�ǉ��ɐ�������� true�D ���łɓo�^����Ă����ꍇ�� false ��Ԃ��D
		���O���Ԃ���ꍇ�́C�ǉ�����I�u�W�F�N�g�̖��O���ύX�����D	*/
	bool Add(SGObject* obj);
	///	�I�u�W�F�N�g�̍폜
	bool Del(SGObject* key){
		iterator it = find(key);
		if (it==end()) return false;
		erase(it);
		return true;
	}
	///	�I�u�W�F�N�g�̍폜
	bool Del(UTString name){
		SGObject key;
		key.name = name;
		key.AddRef();
		iterator it = find(&key);
		key.DelRef();
		if (it==end()) return false;
		erase(it);
		return true;
	}
};
inline std::ostream& operator << (std::ostream& os, const SGObjectNames& ns){
	ns.Print(os); return os;
}

/**	�V�[���O���t�̃g�b�v�m�[�h�D�����E���_�����D
	�����_���ƃV�[���O���t�̊֌W���[�����߁C
	�����_�����Ⴄ�ƃV�[���O���t�͂��̂܂܎g�p�ł��Ȃ��D
	�V�[���O���t�́C���Ƃ��΃����_����OpenGL�Ȃ�CdisplayList
	�������Ă��邵�CD3D�Ȃ�� ID3DXMesh�������Ă���D
	OpenGL�̃V�[���O���t��D3D�ɕϊ����邽�߂ɂ́C��xDocument��
	�Z�[�u���āCD3D�`���Ń��[�h���Ȃ���΂Ȃ�Ȃ��D	*/
class SPR_DLL SGScene:public SGObject{
	SGOBJECTDEF(SGScene);
protected:
	///	���O�ƃI�u�W�F�N�g�̑Ή��\	
	SGObjectNames names;
	///	�r�w�C�r�A�G���W��
	SGBehaviorEngines behaviors;
	///	�����_�����O�G���W��
	SGRenderEngines renderers;
	///	�g�b�v�t���[��
	UTRef<SGFrame> world;
	/// �ϕ��X�e�b�v
	double timeStep;
	/// �ϕ�������
	unsigned int count;
	///	1�X�e�b�v������̑��x�̌������D�S���̂悤�ɓ����D�傫������ƔS������������ɂȂ�D
	double velocityLossPerStep;
public:

	///	�R���X�g���N�^
	SGScene();
	///	�f�X�g���N�^
	~SGScene(){Clear();}
	///	�r�w�C�r�A�G���W���� vector ��Ԃ��D
	SGBehaviorEngines& GetBehaviors(){ return behaviors; }
	const SGBehaviorEngines& GetBehaviors() const { return behaviors; }
	///	�����_�����O�G���W���� vector ��Ԃ��D
	SGEngines& GetRenderers(){
		return renderers;
	}

	///	�g�b�v�t���[����Ԃ��D
	SGFrame* GetWorld(){ return world; }
	///	���O����I�u�W�F�N�g���擾
	SGObject* FindObject(UTString name){ return names.Find(name); }
	///	�^�Ɩ��O����I�u�W�F�N�g���擾
	template <class T> void FindObject(UTRef<T>& t, UTString name){
		T* p;
		FindObject(p, name);
		t = p;
	}
	template <class T> void FindObject(T*& t, UTString name){
		t = DCAST(T, FindObject(name));
	}
	typedef SGObjectNames::iterator SetIt;
	typedef std::pair<SetIt, SetIt> SetRange;
	SetRange RangeObject(UTString n){ return names.Range(n); }
	
	SGObjectNames::TNameMap& GetNameMap(){ return names.nameMap; }

	/// �ϕ��X�e�b�v��Ԃ�
	double GetTimeStep()const{return timeStep;}
	/// �ϕ��X�e�b�v��ݒ肷��
	void SetTimeStep(double dt);
	///	1�X�e�b�v������̌������̎擾
	double GetVelocityLossPerStep(){ return velocityLossPerStep; }
	///	�������̎擾
	double GetVelocityLoss();
	///	�������̐ݒ�
	void SetVelocityLoss(double dt);
	/// �J�E���g����Ԃ�
	unsigned GetCount()const{return count;}
	/// �J�E���g����ݒ肷��
	void SetCount(unsigned c){count = c;}
	///	�V�[���̎�����i�߂� ClearForce(); GenerateForce(); Integrate(); �Ɠ���
	void Step();
	///	�V�[���̎�����i�߂�i�͂̃N���A�j
	void ClearForce();
	///	�V�[���̎�����i�߂�i�͂̐����j
	void GenerateForce();
	///	�V�[���̎�����i�߂�i�͂Ƒ��x��ϕ����āC���x�ƈʒu���X�V�j
	void Integrate();
	///	�����_�����O
	void Render();
	///	���[�h��C�Q�Ƃ̉������I����Ă���Ăяo���֐��D�e�I�u�W�F�N�g��Loaded���Ăяo���D
	void Loaded(SGScene* scene);
	///	�V�[������ɂ���D
	void Clear();
	///	���O�\����C�Q�Ƃ���Ă��Ȃ��I�u�W�F�N�g���폜����D
	void ClearName();
	///	�f�o�b�O�p
	void Print(std::ostream& os) const { names.Print(os); }
	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NChildObjects();
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ChildObject(size_t i);
	///	�q�I�u�W�F�N�g�̒ǉ�
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	�q�I�u�W�F�N�g�̍폜
	virtual bool DelChildObject(SGObject* o, SGScene* s);
	///	�q�ɂȂ肦��I�u�W�F�N�g�̌^���̔z��
	virtual const UTTypeInfo** ChildCandidates();

	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
protected:
//	friend class SGFrame;
	friend class SGObject;
};

}
#endif
