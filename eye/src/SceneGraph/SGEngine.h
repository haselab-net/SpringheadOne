#ifndef SGENGINE_H
#define SGENGINE_H

#include "SGFrame.h"

namespace Spr {;

///	����G���W���̊�{�N���X�D
class SGEngine:public SGObject{
	SGOBJECTDEF(SGEngine);
public:
	///	�V�[�����N���A����Ƃ��̏���
	virtual void Clear(SGScene* s){}
	///	�v���C�I���e�B	�������قǗD�揇�ʂ�����
	virtual int GetPriority() const { return 0; }
	///	�I�u�W�F�N�g���Z�b�g����
	virtual void Set(SGObject* obj){}
};
inline bool operator < (const SGEngine& e1, const SGEngine& e2){
	return e1.GetPriority() < e2.GetPriority();
}

class FISaveScene;

///	SGEngine�̔z��
class SGEngines: std DOUBLECOLON vector< UTRef<SGEngine> >{
protected:
	SGObjects objs;
	iterator begin2() { return base::begin(); }
	iterator end2() { return base::end(); }
public:
	typedef std::vector< UTRef<SGEngine> > base;
	typedef base::iterator iterator;
	typedef base::const_iterator const_iterator;
	const_iterator begin() const { return base::begin(); }
	const_iterator end() const { return base::end(); }
	size_t size(){ return base::size(); }
	UTRef<SGEngine>& operator [] (int id){ return base::begin()[id]; }
	const UTRef<SGEngine>& operator [] (int id) const { return base::begin()[id]; }
	virtual void Add(SGEngine* e);
	virtual bool Del(SGEngine* e);
	///
	SGEngines();
	///	�Z�b�g���ꂽ�I�u�W�F�N�g����
	SGObjects& GetObjects(){ return objs; }
	///	�I�u�W�F�N�g���Z�b�g����
	virtual void Set(SGObject* obj){
		for(const_iterator it = begin(); it != end(); ++it){
			(*it)->Set(obj);
		}
		objs.push_back(obj);
	}
	///	���[�h��̏���
	virtual void Loaded(SGScene* scene);
	///	�V�[�����N���A����Ƃ��̏���
	virtual void Clear(SGScene* s);
	///	�w�肵���^���p�������G���W���������� (�ŏ��Ɍ����������̂�Ԃ�)
	SGEngine* FindInherit(const UTTypeInfo& tinfo, UTString name) const;
	SGEngine* FindInherit(const UTTypeInfo& tinfo) const;
	///	�w�肵���^�����̔h���N���X�̃G���W���������� (�ŏ��Ɍ����������̂�Ԃ�)
	template <class T> bool Find(T*& t) const{
		t = (T*)FindInherit(*T::GetTypeInfoStatic());
		return t != NULL;
	}
	template <class T> bool Find(UTRef<T>& t) const{
		t = UTRef<T>((T*)FindInherit(*T::GetTypeInfoStatic()));
		return t != NULL;
	}
	///	�w�肵���^�����̔h���N���X�̃G���W���Ŗ��O����v������̂������� (�ŏ��Ɍ����������̂�Ԃ�)
	template <class T> bool Find(T*& t, UTString name) const{
		t = (T*)FindInherit(*T::GetTypeInfoStatic(), name);
		return t != NULL;
	}
	template <class T> bool Find(UTRef<T>& t, UTString name) const{
		t = UTRef<T>((T*)FindInherit(*T::GetTypeInfoStatic(), name));
		return t != NULL;
	}
	void Save(FISaveScene* ctx);
};

}
#endif
