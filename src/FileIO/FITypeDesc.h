#ifndef FITYPEDESC_H
#define FITYPEDESC_H

#include "FIIterator.h"
#include <Base/BaseUtility.h>
#include <WinBasis/WBUtility.h>
#include <set>
#include <map>
#include <algorithm>

/**	@page FITypeDesc �h�L�������g�I�u�W�F�N�g�ƌ^�L�q
	C++�̍\���̂���f�[�^�������o���ꍇ�ȂǁC�\���̂�f�[�^�ɂ́C
	�ϐ�����ϐ��̌^�ƌ��������͂Ȃ��D
	�����ŁC�^�L�q�^�I�u�W�F�N�g(FITypeDesc)���g���ċL�q���Ă����D

	�f�[�^�������o���ۂɂ́C�o�C�i���f�[�^����
	�h�L�������g�I�u�W�F�N�g�̃v���p�e�B�����D
	���̍ہC�^�L�q�����ăv���p�e�B����v���p�e�B�^���L������D

	�f�[�^��ǂݏo���ۂɂ́C�^�L�q�����Ȃ���C�o�C�i���f�[�^����
	�\���̂��\�z����D

	������������邽�߂̃C���^�t�F�[�X�Ƃ��āC
	- �ǂݏo���R���e�L�X�g�̍쐬		FIContext* ReadContext();
	- �������݃R���e�L�X�g�̍쐬		FIContext* WriteContext();
	- �R���e�L�X�g�̊J��				void ReleaseContext(FIContext*);
	- ���̃t�B�[���h��ǂݏo���֐�		bool Read(void* ctx, void* ptr, int sz)
	- ���̃t�B�[���h�̃f�[�^����������	void Write(void* ctx, void* ptr, int sz, FIString fn)
	- �t�B�[���h�̃O���[�v�ɓ���		void EnterGroup(FIString fn);
	- �O���[�v���甲����D				void ExitGroup();
	��p�ӂ���D
*/

namespace Spr {;

/**	FIVVector�̊�{�N���X�D
	�x�N�^�ւ̃��[�h�ƃZ�[�u�̃T�|�[�g�ɕK�v�ȉ��z�֐���p��	*/
class FIVVectorBase{
public:
	virtual size_t VItemSize() = 0;
	virtual size_t VSize() = 0;
	virtual void VResize(size_t s) = 0;
	virtual char* VBegin() = 0;
	virtual char* VEnd() = 0;
	virtual void VInsert(int pos, void* v) = 0;
	virtual void VPushBack(void* v) = 0;
	virtual void VPushBack() = 0;
	virtual void VPopBack() = 0;
};
/**	FITypeDesc �������ł���std::vector
	�x�N�^�ւ̃��[�h�ƃZ�[�u�̃T�|�[�g�ɕK�v�ȉ��z�֐������������D
	FITypeDesc �͕��ʂ�vector ���܂ރN���X�̓T�|�[�g���Ȃ��D*/
template <class T>
class FIVVector: public FIVVectorBase, public std::vector<T>{
	virtual size_t VItemSize(){ return sizeof(T); }
	virtual size_t VSize(){ return size(); }
	virtual void VResize(size_t s){ resize(s); }
	virtual char* VBegin(){ return (char*)&*begin(); }
	virtual char* VEnd(){ return (char*) &*end(); }
	virtual void VInsert(int pos, void* v){ insert(begin()+pos, *(T*)v); }
	virtual void VPushBack(void* v){ push_back(*(T*)v); }
	virtual void VPushBack(){ push_back(T()); }
	virtual void VPopBack(){ pop_back(); }
public:
	FIVVector(){}
	FIVVector(const std::vector<T>& v):std::vector<T>(v){}
	FIVVector& operator=(const std::vector<T>& v){ *(std::vector<T>*)this = v; return *this; }
};

class FIDocNodeBase;
class FIDocNodePos;

class FITypeDescDb;
///	�^��\��
class SPR_DLL FITypeDesc:public UTRefCount{
public:
	///	���R�[�h�̃t�B�[���h��\��
	class SPR_DLL Field{
	public:
		Field(): length(1), lengthFieldOffset(0), lengthFieldSize(0), bReference(false), bVector(false){}
		~Field();
		///	�����o��
		UTString name;
		///	�^
		UTRef<FITypeDesc> type;
		///	�z��̗v�f���D
		int length;
		///	�v�f����ʂ̃t�B�[���h����Ƃ�ꍇ�̃t�B�[���h��
		UTString lengthFieldName;
		///	�v�f����ʂ̃t�B�[���h����Ƃ�ꍇ�̃I�t�Z�b�g
		int lengthFieldOffset;
		///	�v�f����ʂ̃t�B�[���h����Ƃ�ꍇ�T�C�Y
		int lengthFieldSize;
		///	vector���ǂ���
		int bVector;
		///	�Q�Ƃ��ǂ���
		bool bReference;
		///	�f�[�^�̃T�C�Y
		size_t Size();
		///
		void Print(std::ostream& os) const;
		///	�m�[�h����C�\���̂̃t�B�[���h��ǂݏo��
		bool Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& ctx);
		///	�m�[�h�ɍ\���̂̃t�B�[���h�������o���D
		void Write(FIDocNodeBase* node, FIIterator& ctx, const char*& from);
	};
	///	�g�ݗ��Č^������킷�ꍇ�Ɏg��
	class SPR_DLL Composit: public std::vector<Field>{
	public:
		///	�f�[�^�̃T�C�Y��Ԃ�
		int Size(){ return Size(""); }
		///	�f�[�^�̃T�C�Y��Ԃ�
		int Size(FIString id);
		///
		void Print(std::ostream& os) const;
		///	�m�[�h����C�\���̂�ǂݏo��
		bool Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& ctx);
		///	�m�[�h�ɍ\���̂������o���D
		void Write(FIDocNodeBase* node, FIIterator& ctx, const char*& from);
	};
protected:
	///	�^��
	UTString typeName;
	///	�f�[�^�̒���
	size_t size;
	///	�g�ݗ��Č^�̒��g�̋L�q�D�P���^�̏ꍇ�́Csize() == 0
	Composit composit;

	friend class FITypeDescDb;
public:
	///	�^��GUID
	WBGuid guid;
	///	�R���X�g���N�^
	FITypeDesc():size(0){}
	///	�R���X�g���N�^
	FITypeDesc(UTString tn, int sz=0): typeName(tn), size(sz){}
	///	
	~FITypeDesc(){}
	///	C++����̃N���X��`���̕����񂩂�C���̌^��\���I�u�W�F�N�g���\�z����D
	bool Init(const char* clsDef, FITypeDescDb& db);
	///
	void Print(std::ostream& os) const;
	///	�m�[�h����C�\���̂�ǂݏo��
	int Read(void* to, FIDocNodeBase* node, FIString id);
	int Read(void* to, FIDocNodeBase* node){ return Read(to, node, ""); }
	///	��������\���̂�ǂݏo��
	bool Read(char*& to, size_t align, FIDocNodeBase* node, FIIterator& ctx);
	///	�m�[�h�ցC�\���̂������o��
	int Write(FIDocNodeBase* node, const void* from);
	///	�����ցC�\���̂������o��
	void Write(FIDocNodeBase* node, FIIterator& ctx, const char*& from, UTString id);
	///	�^��
	UTString GetTypeName() const { return typeName; }
	///	GUID
	const GUID& GetGuid() const { return guid; };
	///	�^�̃T�C�Y
	size_t Size() { return size; }
	///	�g�ݗ��Č^�̗v�f
	Composit& GetComposit(){ return composit; }
};
inline bool operator < (const FITypeDesc& d1, const FITypeDesc& d2){
	return d1.GetTypeName().compare(d2.GetTypeName()) < 0;
}
///	�^�̃f�[�^�x�[�X
class SPR_DLL FITypeDescDb: public UTRefCount{
public:
	///	�R���e�i�̌^
	typedef std::set< UTRef<FITypeDesc>, UTContentsLess< UTRef<FITypeDesc> > > Db;
	struct ProtoDesc{
		FIString fileType;
		UTRef<FITypeDesc> desc;
	};
	typedef std::vector<ProtoDesc> ProtoDescs;
protected:
	FIString prefix;			///<	���O�̂����v���t�B�b�N�X�̕���
	Db db;						///<	FITypeDesc �����Ă����R���e�i
	static ProtoDescs protoDescs;
public:
	///	
	~FITypeDescDb();
	static const ProtoDescs& GetProtoDescs(){ return protoDescs; }
	/**	�^�����f�[�^�x�[�X�ɓo�^�D	*/
	void RegisterDesc(FITypeDesc* n){
		if (prefix.length() && n->typeName.compare(0, prefix.length(), prefix) == 0){
			n->typeName = n->typeName.substr(prefix.length());
		}
		db.insert(n);
	}
	/**	�^�����v���g�^�C�v���X�g�ɓo�^	*/
	void RegisterProto(FITypeDesc* n);
	/**	REG_RECORD�}�N�����g�p����֐��D�^���ƍ\���̂̒�`�����̕�����
		����^���𐶐����C�f�[�^�x�[�X�ɓo�^�D	*/
	void Register(const char* type, const char* def, bool bProto=false){
		UTRef<FITypeDesc> desc = new FITypeDesc(type);
		desc->Init(def, *this);
		RegisterDesc(desc);
		if(bProto) RegisterProto(desc);
	}
	/**	�^����Prefix ��ݒ�D
		�^����Find�Ō�������ۂɁC�����L�[��Prefix�������L�[�ł��^������������D	*/
	void SetPrefix(FIString p);
	/**	�^���𖼑O���猟������D
		@param tn	�^���Dprefix �͏ȗ����Ă悢�D	*/
	FITypeDesc* Find(UTString tn);
	///	DB���̌^���̕\��
	void Print(std::ostream& os) const ;
};

///	FIVVector�̃T�C�Y�w��Dx �ɂ̓t�B�[���h��(�����o�ϐ���)���w�肷��D
#define VSIZE(x)

///	�P���^��o�^����D
#define REG_FIELD(type)	RegisterDesc( new FITypeDesc(#type, sizeof(type)) )
/**	�h�L�������g���烍�[�h�\�ȃN���X�̒�`�D
	�N���X����{�N���X�����ꍇ	*/
#define DEF_RECORDBASE(Type, Base, Def)					\
	struct Type: public Base Def;						\
	static const char* classNameOf##Type = #Type;		\
	static const char* classDefOf##Type = #Def;			\

/**	�h�L�������g���烍�[�h�\�ȃN���X�̒�`�D
	��{�N���X�������ꍇ	*/
#define DEF_RECORD(Type, Def)							\
	struct Type Def;									\
	static const char* classNameOf##Type = #Type;		\
	static const char* classDefOf##Type = #Def;			\

/**	DER_RECORD �Œ�`�����^��o�^����D*/
#define REG_RECORD(Type)	\
	Register(classNameOf##Type, classDefOf##Type)

/**	DER_RECORD �Œ�`�����^���v���g�^�C�v�Ƃ��Ă��o�^����D*/
#define REG_RECORD_PROTO(Type)	\
	Register(classNameOf##Type, classDefOf##Type, true)

/**	@page TypeDesc �^�����N���X
C++�̍\���̐錾�𕶎���Ƃ��Ď󂯎��C�^����͂���D
�g�����F�w�b�_�t�@�C���ŁC��`
DEF_RECORD( Test, {
	int x;
	char y;
}
)

�������āC�N���X�̒�`���s���C
.cpp�t�@�C���̉����̊֐��̒���
	REG_RECORD(Test);
�������ēo�^���܂��D
*/
}
#endif
