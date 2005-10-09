#ifndef BASETYPEINFO_H
#define BASETYPEINFO_H
/**	���s���^���e���v���[�g���C�u����
	�R���p�C����rtti���g�킸�}�N���ƃe���v���[�g�Ŏ������Ă���D
*/
namespace Spr{;

///	���s���^���
class UTTypeInfo {
public:
	const char* className;
	UTTypeInfo** base;

	UTTypeInfo(const char* cn, UTTypeInfo** b): className(cn), base(b){}
	virtual bool Inherit(const UTTypeInfo* key) const ;
	virtual bool Inherit(const char* str) const ;
	virtual const char* ClassName() const = 0;
	virtual void* CreateInstance() const = 0;
};

///	���s���^���N���X�̎���
template <class T>
class UTTypeInfoImp: public UTTypeInfo{
public:
	UTTypeInfoImp(const char* cn, UTTypeInfo** b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return new T; }
	virtual const char* ClassName() const { return className; }
};

///	���s���^���N���X�̎����D���ۃN���X��
template <class T>
class UTTypeInfoImpAbst: public UTTypeInfo{
public:
	UTTypeInfoImpAbst(const char* cn, UTTypeInfo** b): UTTypeInfo(cn, b){}
	virtual void* CreateInstance() const { return 0; }
	virtual const char* ClassName() const { return className; }
};

///	���s���^�������N���X�̊�{�N���X
class UTTypeInfoBase{
public:
	virtual const UTTypeInfo* GetTypeInfo() const =0;
};

//----------------------------------------------------------------------
//	�N���X�̐錾(�w�b�_)�ɏ�������
///	���s���^�������N���X�����ׂ������o�̐錾��
#define DEF_UTTYPEINFODEF(cls)							\
public:													\
	static UTTypeInfoImp<cls> typeInfo;					\
	virtual const UTTypeInfo* GetTypeInfo() const {		\
		return &typeInfo;								\
	}													\
	static const UTTypeInfo* GetTypeInfoStatic(){		\
		return &typeInfo;								\
	}													\

///	���s���^�������N���X�����ׂ������o�̐錾���D���ۃN���X��
#define DEF_UTTYPEINFOABSTDEF(cls)						\
public:													\
	static UTTypeInfoImpAbst<cls> typeInfo;				\
	virtual const UTTypeInfo* GetTypeInfo() const {		\
		return &typeInfo;								\
	}													\
	static const UTTypeInfo* GetTypeInfoStatic(){		\
		return &typeInfo;								\
	}													\


///	���s���^�������N���X�����ׂ������o�̎����D
#define DEF_UTTYPEINFO(cls)									\
	UTTypeInfo* cls##_BASE[] = {NULL};						\
	UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, cls##_BASE);

///	���s���^�������N���X�����ׂ������o�̎����D1�̃N���X�p��������ꍇ
#define DEF_UTTYPEINFO1(cls, base1)							\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, NULL};	\
	UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, cls##_BASE);

///	���s���^�������N���X�����ׂ������o�̎����D2�̃N���X�p��������ꍇ
#define DEF_UTTYPEINFO2(cls, base1, base2)									\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, &base2::typeInfo, NULL};	\
	UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, cls##_BASE);

///	���s���^�������N���X�����ׂ������o�̎����D3�̃N���X�p��������ꍇ
#define DEF_UTTYPEINFO3(cls, base1, base2, base3)												\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, &base2::typeInfo, &base3::typeInfo, NULL};	\
	UTTypeInfoImp<cls> cls::typeInfo = UTTypeInfoImp<cls>(#cls, cls##_BASE);

///	���s���^�������N���X�����ׂ������o�̎����D���ۃN���X��
#define DEF_UTTYPEINFOABST(cls)								\
	UTTypeInfo* cls##_BASE[] = {NULL};						\
	UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, cls##_BASE);

///	���s���^�������N���X�����ׂ������o�̎����D���ۃN���X�ŁD1�̃N���X���p������ꍇ
#define DEF_UTTYPEINFOABST1(cls, base)						\
	UTTypeInfo* cls##_BASE[] = {&base::typeInfo,NULL};		\
	UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, cls##_BASE);

///	���s���^�������N���X�����ׂ������o�̎����D���ۃN���X�ŁD2�̃N���X���p������ꍇ
#define DEF_UTTYPEINFOABST2(cls, base1, base2)									\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, &base2::typeInfo, NULL};		\
	UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, cls##_BASE);

///	���s���^�������N���X�����ׂ������o�̎����D���ۃN���X�ŁD2�̃N���X���p������ꍇ
#define DEF_UTTYPEINFOABST3(cls, base)															\
	UTTypeInfo* cls##_BASE[] = {&base1::typeInfo, &base2::typeInfo, &base3::typeInfo, NULL};	\
	UTTypeInfoImpAbst<cls> cls::typeInfo = UTTypeInfoImpAbst<cls>(#cls, cls##_BASE);

#define GETCLASSNAME(p)		(p->GetTypeInfo()->className)
#define GETCLASSNAMES(T)	(T::GetTypeInfoStatic()->className)

///	�_�C�i�~�b�N�L���X�g
#define DCAST(T, p)	UTDcastImp<T>(p)
template <class T, class P> T* UTDcastImp(P p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	return NULL;
}

///	�L���X�g�Ɏ��s�����assert����_�C�i�~�b�N�L���X�g
#define ACAST(T, p)	UTAcastImp<T>(p)
template <class T, class P> T* UTAcastImp(P p){
	if (p && p->GetTypeInfo()->Inherit(T::GetTypeInfoStatic())) return (T*)&*(p);
	UTAcastError((p)->GetTypeInfo()->className);
	return NULL;
}
///	�_�C�i�~�b�N�L���X�g�D�^���Ƀ|�C���^�^���g���o�[�W����
#define DCASTP(TP, p)	UTDcastpImp<TP>(p)
template <class TP, class P> TP UTDcastpImp(P p){
	TP tp=NULL;
	if (p && p->GetTypeInfo()->Inherit(tp->GetTypeInfoStatic())) return (TP)&*(p);
	return NULL;
}
///	�L���X�g�Ɏ��s�����assert����_�C�i�~�b�N�L���X�g�D�^���Ƀ|�C���^�^���g���o�[�W����
#define ACASTP(TP, p)	UTAcastpImp<TP>(p)
template <class TP, class P> TP UTAcastpImp(P p){
	TP tp=NULL;
	if (p && p->GetTypeInfo()->Inherit(tp->GetTypeInfoStatic())) return (TP)&*(p);
	UTAcastError((p)->GetTypeInfo()->className);
	return NULL;
}
///	�L���X�g�Ɏ��s�����ꍇ��assert�֐�
char* UTAcastError(const char* );

}
#endif
