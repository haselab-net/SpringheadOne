#ifndef FIDOCSCENE_H
#define FIDOCSCENE_H
#include "FIDocNode.h"
#include "FIFileDoc.h"
#include "FITypeDesc.h"
#include <SceneGraph/SGScene.h>

/**	@file FIDocScene.h �h�L�������g����V�[���O���t�����[�h����d�g��	*/
namespace Spr{;

/**	����^�̃h�L�������g�m�[�h�̃��[�_�[	*/
class SPR_DLL FIBaseLoader:public UTRefCount{
public:
	///	���[�_���S������^��
	virtual UTString GetNodeType() const =0;
	///	���[�h�֐��D�q�m�[�h�����[�h�����O�ɌĂ΂��D
	virtual void Load(class FILoadScene* context)=0;
	///	���[�h�㏈���֐��D�q�m�[�h�����[�h���ꂽ��Ă΂��D
	virtual void Loaded(class FILoadScene* context){}
};
template <class T, bool bAdd=true, bool bCont=true> class FIObjectLoader: public FIBaseLoader{
public:
	typedef T TLoaded;
	void Create(class FILoadScene* context, UTRef<TLoaded>& t){
		t = new TLoaded;
		t->SetName(context->docs.Top()->GetName().c_str(), context->scene);
	}
	///	���[�_���S������m�[�h�̌^���DC++�N���X�̌^������ړ���2�������Ȃ��������́D
	virtual UTString GetNodeType() const{
		return TLoaded::GetTypeInfoStatic()->ClassName()+2;
	}
	///	���[�h�֐��D�q�m�[�h�����[�h�����O�ɌĂ΂��D
	virtual void Load(class FILoadScene* context){
		UTRef<TLoaded> t=NULL;
		Create(context, t);
		t->doc = context->docs.Top();
		t->SetName(t->doc->GetName().c_str(), context->scene);
		if (bAdd) context->AddObject(t);
		if (bCont) context->AddContainer(t);
		if (!LoadData(context, t)){
			if (bAdd){
				context->CancelAddObject();
			}
		}
	}
	///	���[�h�̎���
	virtual bool LoadData(class FILoadScene* context, TLoaded* t)=0;
};

template <class P=SGFrame> class FIOtherLoader: public FIBaseLoader{
public:
	///	���[�h�֐��D�q�m�[�h�����[�h�����O�ɌĂ΂��D
	virtual void Load(class FILoadScene* context){
		P* p=NULL;
		context->objects.Find(p);
		Load(context, p);
	}
	virtual void Load(class FILoadScene* context, P* p)=0;
	///	���[�h�㏈���֐��D�q�m�[�h�����[�h���ꂽ��Ă΂��D
};

inline bool operator < (const FIBaseLoader& t1, const FIBaseLoader& t2){
	return t1.GetNodeType().compare(t2.GetNodeType()) < 0;
}
///	�e�m�[�h�̃��[�_�[��Set
class SPR_DLL FILoders:public std::multiset<FIBaseLoader*, UTContentsLess<FIBaseLoader*> >{
public:
	class FITypeLoaderKey:public FIBaseLoader{
		UTString* key;
	public:
		FITypeLoaderKey(UTString& k):key(&k){}
		virtual UTString GetNodeType() const{ return *key; }
		virtual void Load(FILoadScene* context){}
	};
	iterator LowerBound(UTString s){
		return lower_bound(&FITypeLoaderKey(s));
	}
	iterator UpperBound(UTString s){
		return upper_bound(&FITypeLoaderKey(s));
	}
	std::pair<iterator, iterator> EqualRange(UTString s){
		return equal_range(&FITypeLoaderKey(s));
	}
	void Erase(UTString s){
		std::pair<iterator, iterator> r = EqualRange(s);
		erase(r.first, r.second);
	}
};

class SPR_DLL FIReferenceBase:public UTRefCount{
protected:
	SGObject* Find(SGScene* s);
	///	���t�@�����X��̖��O
	UTString name;
public:
	virtual ~FIReferenceBase(){}
	///	�Q�Ƃ̉���
	virtual void SolveReference(SGScene* s)=0;
};
class SPR_DLL FIReferences:public std::vector< UTRef<FIReferenceBase> >{
};
/**	���t�@�����X�ێ��N���X	*/
template <class T>
class SPR_DLL FIReference:public FIReferenceBase{
	///	���t�@�����X���|�C���^�̃A�h���X�D����������������D
	UTRef<T>* reference;
	///	
public:
	FIReference(UTRef<T>& t, UTString n): reference(&t) { name=n; }
	///	�Q�Ƃ̉���
	virtual void SolveReference(SGScene* s){
		T* t = (T*)Find(s);
		*reference = t;
//		reference->ForceOverWrite(t);
//		t->AddRef();
	}
};
template <class CT>
class SPR_DLL FIReferenceVector:public FIReferenceBase{
	///	���t�@�����X���|�C���^�̃A�h���X�D����������������D
	CT* container;
	typedef TYPENAME CT::value_type T;
	int pos;
public:
	FIReferenceVector(CT& c, int p, UTString n): container(&c), pos(p) { name=n; }
	///	�Q�Ƃ̉���
	virtual void SolveReference(SGScene* s){
		T t = T(UTRef<SGObject>(Find(s)));
		(*container)[pos] = t;
	}
};
template <class T>
FIReference<T>* createFIReference(UTRef<T>& t, UTString n){
	return new FIReference<T>(t, n);
}
template <class CT>
FIReferenceVector<CT>* createFIReference(CT& ct, int p, UTString n){
	return new FIReferenceVector<CT>(ct, p, n);
}

class SPR_DLL FILink: public SGObject{
public:
	SGOBJECTDEF(FILink);
	UTString linkName;
};
class SGObject;
class SPR_DLL FIObjectAdder{
public:
	///	�ǉ���̃I�u�W�F�N�g
	SGObjects containers;
	///	�ǉ�����I�u�W�F�N�g
	SGObjects objects;
	///	�Q�Ƃ�����
	void Link(SGScene* scene);
	void Add(SGObject* obj);
	void Add(UTString name);
};
class SPR_DLL FIObjectAdders:public UTStack<FIObjectAdder>{
public:
};

/**	�h�L�������g�m�[�h�̃c���[��ǂ�ŃV�[���O���t�����D
	�R���e�L�X�g���ێ�����D
	�h�L�������g�m�[�h�ɂ́C�^������^���ƂɃ��[�_���p�ӂł���D
	�^���Ƃ̃��[�_�[��FIBaseLoader��h�������č��D
*/
class SPR_DLL FILoadScene:public UTRefCount{
	///	�^���Ƃ̃��[�_�[
	FILoders loaders;
	/**	�I�u�W�F�N�g�̒ǉ����X�g�i�Q�Ɖ�����ɒǉ�����j
		�ǉ����ׂ��R���e�i���r���ő����邱�Ƃ�����̂ŁC
		�܂�adderStack�ɓ���Ă����C�����������Ɉڂ��D*/
	FIObjectAdders adders;
	/**	�I�u�W�F�N�g�̒ǉ����X�g�̃X�^�b�N�D�m�[�h���Ƃ�Push,Pop����D
		�ЂƂ̃m�[�h�������̃��[�_�����ꍇ�C���[�h���ɃR���e�i�������邱�Ƃ�����D
		�����ŁC���[�h���̒ǉ����X�g�ƁC���[�h��̃R���e�i���ЂƂ܂������ɓ���Ă����D
		Pop����Ƃ��ɁC�ǉ����X�g��adders �Ɉڂ��D*/
	FIObjectAdders adderStack;
	///	�q�m�[�h�̂��߂̃R���e�i���X�g
	SGObjects containers;
public:
	///	@name �e�m�[�h�̃��[�_���g�������o
	//@{
	///	���Ƃ̃t�@�C����
	UTString fileName;
	///	���[�h���̃h�L�������g�̃X�^�b�N
	FIDocNodes docs;
	///	���[�h��V�[��
	UTRef<SGScene> scene;
	///	���[�h���Ɏg���I�u�W�F�N�g�̃X�^�b�N
	SGObjects objects;
	///	���[�h�����I�u�W�F�N�g�����݂̃R���e�i�ɒǉ�
	void AddObject(SGObject* obj);
	///	���[�h�������t�@�����X�����݂̃R���e�i�ɒǉ�
	void AddObject(UTString name);
	///	�Ō�̒ǉ����L�����Z��
	void CancelAddObject();
	///	�R���e�i��ǉ�
	void AddContainer(SGObject* cont){
		containers.Push(cont);
	}
	///	�R���e�i�X�^�b�N��Push�D���������true��Ԃ��D
	bool PushContainer();
	///	�R���e�i�X�^�b�N��Pop
	void PopContainer();
	//@}

	//@name	�O������Ăяo���֐��D
	//@{
	///	�h�L�������g�I�u�W�F�N�g����V�[���O���t���쐬
	virtual void Load(SGScene* s, SGObject* obj, FIFileDoc* fileDoc);
	virtual void Load(SGScene* s, FIFileDoc* fileDoc);
	virtual void Load(SGScene* s, SGObject* obj, FIDocNodes& docs);
	virtual void Load(SGScene* s, FIDocNodes& docs);
	///	���[�_�[��o�^
	void Register(FIBaseLoader* l){ loaders.insert(l); }
	///	���[�_�[���폜
	void Erase(UTString k){ loaders.Erase(k); }
	///	�h�L�������g�I�u�W�F�N�g����V�[���O���t���쐬
	virtual void LoadRecursive();
	//@}


protected:
	///	�Q�Ƃ̉����D
	void SolveReferences();
	///	�I�u�W�F�N�g�̒ǉ��D
	void AddObjects();
};

///	����^(SGFrame/Visual/..) ��ۑ�����Z�[�o�[
class FIBaseSaver:public UTRefCount{
public:
	virtual UTString GetType() const =0;
	virtual UTString GetNodeType() const { return GetType().substr(2); }
	virtual void Save(class FISaveScene* ctx, SGObject* target){}
};
inline bool operator < (const FIBaseSaver& t1, const FIBaseSaver& t2){
	return t1.GetType().compare(t2.GetType()) < 0;
}
///	SGObject�̂��߂̃Z�[�o�[
template <class T, bool bAdd=true, bool bCont=true> class FIObjectSaver: public FIBaseSaver{
public:
	typedef T TSaved;
	///	�^��
	virtual UTString GetType() const {
		return TSaved::GetTypeInfoStatic()->ClassName();
	}
	///	�f�[�^�̃Z�[�u�D�I�[�o�[���C�h���Ă��������D
	virtual void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, TSaved* target)=0;
	///	�Z�[�u
	virtual void Save(class FISaveScene* ctx, SGObject* target){
		TSaved* t = (TSaved*) target;
		UTRef<FIDocNodeBase> doc = t->doc;
		if (!doc) doc = ctx->CreateDocNode(GetNodeType(), t);
        doc->SetName(target->GetName());
		doc->ClearChildren();
		SaveData(ctx, doc, t);
		ctx->AddSavedObject(target);
		if (bAdd) ctx->docs.Top()->AddChild(doc);
		if (bCont){
			ctx->docs.Push(doc);
			for(unsigned i=0; i<t->NChildObjects(); ++i){
				if(ctx->IsSavedObject(t->ChildObject(i))){
					ctx->SaveReference(t->ChildObject(i));
				}else{
					ctx->SaveRecursive(t->ChildObject(i));
				}
			}
			for(unsigned i=0; i<t->NReferenceObjects(); ++i){
				ctx->SaveReference(t->ReferenceObject(i));
			}
			ctx->docs.Pop();
		}
	}
};

///	����^(SGFrame/Visual/..) ��ۑ�����Z�[�o�[��Set
class FISavers:public std::set<FIBaseSaver*, UTContentsLess<FIBaseSaver*> >{
public:
	class FITypeSaverKey:public FIBaseSaver{
		UTString* key;
	public:
		FITypeSaverKey(UTString& k):key(&k){}
		virtual UTString GetType() const{ return *key; }
		virtual void Save(FISaveScene* context, SGObject* target){}
	};
	iterator Find(UTString s){
		return find(&FITypeSaverKey(s));
	}
};
/**	�V�[���O���t���h�L�������g�ɃZ�[�u����Z�[�o�[
*/
class SPR_DLL FISaveScene:public UTRefCount{
protected:
	///	�^���Ƃ̃Z�[�o�[
	FISavers savers;
	///	�h�L�������g�m�[�h�̐���
	virtual UTRef<FIDocNodeBase> CreateDocNodeImp(UTString type)=0;
public:
	///	�Z�[�u����V�[��
	UTRef<SGScene> scene;
	///	�h�L�������g�m�[�h�̃X�^�b�N
	FIDocNodes docs;
	///	�Z�[�u�ς݃I�u�W�F�N�g�̋L�^
	std::set<SGObject*> savedObjects;
	///	�h�L�������g�m�[�h�̍쐬
	virtual UTRef<FIDocNodeBase> CreateDocNode(UTString type, SGObject* o=NULL){
		UTRef<FIDocNodeBase> doc = CreateDocNodeImp(type);
		if (o){
			doc->SetName(o->GetName());
			if (type.compare("REF")!=0) o->doc = doc;
		}
		return doc;
	}
	///	�V�[���O���t�̓���̃m�[�h�ȉ����Z�[�u
	virtual void Save(FIDocNodeBase* doc, SGScene* s, SGObject* obj=NULL);
	///	Save�̍ċA��
	virtual void SaveRecursive(SGObject* obj);
	///	�Q�Ƃ��Z�[�u
	virtual void SaveReference(SGObject* obj);
	
	///	����̌^�̃Z�[�o�[��o�^
	void Register(FIBaseSaver* s){ savers.insert(s); }
	///	�Z�[�u�ς݃I�u�W�F�N�g�̓o�^
	void AddSavedObject(SGObject * o){
		if (!savedObjects.insert(o).second){
			DSTR << "Fail to insert " << o->GetTypeInfo()->ClassName() << ":" << o->GetName() << std::endl;
		}
	}
	///	�Z�[�u�ς݃I�u�W�F�N�g���ǂ�������
	bool IsSavedObject(SGObject * o){
		return savedObjects.find(o) != savedObjects.end();
	}
};

#define DEF_REGISTER_LOADER(cls)				\
void RegisterLoaderFor##cls(FILoadScene* l){	\
	static cls##Loader load_##cls;				\
	l->Register(&load_##cls);					\
}

#define DEF_REGISTER_SAVER(cls)				\
void RegisterSaverFor##cls(FISaveScene* s){	\
	static cls##Saver save_##cls;			\
	s->Register(&save_##cls);				\
}

#define DEF_REGISTER_BOTH(cls)	DEF_REGISTER_SAVER(cls)	DEF_REGISTER_LOADER(cls)

#define REGISTER_LOADER(l, cls)									\
	extern void RegisterLoaderFor##cls(FILoadScene* ld);		\
	RegisterLoaderFor##cls(l);									\

#define REGISTER_SAVER(s, cls)									\
	extern void RegisterSaverFor##cls(FISaveScene* sv);			\
	RegisterSaverFor##cls(s);									\

}
#endif
