//---------------------------------------------------------------------------
#ifndef SPH_CBVectorH
#define SPH_CBVectorH
//---------------------------------------------------------------------------
#include <Base/Affine.h>
//---------------------------------------------------------------------------
///	�R�����x�N�g��
class SHVec3f:public Spr::Vec3f{
public:
#undef TYPENAME
#define TYPENAME
	DEF_TVECTOR_BASIC_MEMBER(SHVec3f);
	SHVec3f(float x, float y, float z): Spr::Vec3f(x, y, z){}
	float __fastcall GetX() const { return Spr::Vec3f::X(); }
	float __fastcall GetY() const { return Spr::Vec3f::Y(); }
	float __fastcall GetZ() const { return Spr::Vec3f::Z(); }
	void __fastcall SetX(float f){ Spr::Vec3f::X()=f; }
	void __fastcall SetY(float f){ Spr::Vec3f::Y()=f; }
	void __fastcall SetZ(float f){ Spr::Vec3f::Z()=f; }

	__property float X= {read=GetX, write=SetX};
	__property float Y= {read=GetY, write=SetY};
	__property float Z= {read=GetZ, write=SetZ};
	SHVec3f* GetThis(){ return this; }
/*
	SHVec3f& operator = (const SHVec3f& v){
        (Spr::Vec3f&)*this = v;
		return *this;
	}*/
};

//	C++�R�[�h������SHVec3f�̃C���^�t�F�[�X�D���̂͂Ȃ��D
class TSHVec3f{
	//	SHVec3f�̎擾�Ɛݒ�
	typedef SHVec3f __fastcall (__closure *TGetter)();
	typedef void __fastcall (__closure *TSetter)(SHVec3f);
	TGetter getter;
	TSetter setter;

	//�X�V�ʒm�C�x���g�n���h��
	Classes::TNotifyEvent FOnChange;

	//�X�V��ʒm
	void __fastcall NotifyChange(TObject* o=NULL){if (FOnChange) FOnChange(o);}

	//�v�f�̎擾/�ݒ�
	float GetX() const{return getter().X;}
	float GetY() const{return getter().Y;}
	float GetZ() const{return getter().Z;}
	void SetX(float f){ SHVec3f v=getter(); v.X=f; setter(v); NotifyChange(); }
	void SetY(float f){ SHVec3f v=getter(); v.Y=f; setter(v); NotifyChange(); }
	void SetZ(float f){ SHVec3f v=getter(); v.Z=f; setter(v); NotifyChange(); }
public:
	//	�R���X�g���N�^
	TSHVec3f(TGetter g, TSetter s):FOnChange(NULL), getter(g), setter(s){}
	//	���
	TSHVec3f& operator=(const TSHVec3f& src){ return *this= SHVec3f(src); }
	TSHVec3f& operator=(const SHVec3f& src){ setter(src); NotifyChange(); return *this; }
	//	SHVec3f�ɕϊ�
	operator SHVec3f ()const{ return getter(); }
	TSHVec3f* operator ->(){ return this; }
	const TSHVec3f* operator ->() const { return this; }

	//�X�V�ʒm�C�x���g
	__property Classes::TNotifyEvent OnChange= {read=FOnChange, write=FOnChange};

	//�v�f
	__property float X= {read=GetX, write=SetX};
	__property float Y= {read=GetY, write=SetY};
	__property float Z= {read=GetZ, write=SetZ};
};

//---------------------------------------------------------------------------
//	�I�u�W�F�N�g�C���X�y�N�^�\���p��SHVec3�̃C���^�t�F�[�X
class TSHVec3fDummy: public TPersistent
{
	TSHVec3f& tvec;

	//�v�f�̎擾/�ݒ�
	float __fastcall GetX() const { return tvec.X; }
	float __fastcall GetY() const { return tvec.Y; }
	float __fastcall GetZ() const { return tvec.Z; }
	void __fastcall SetX(float f){ tvec.X=f; }
	void __fastcall SetY(float f){ tvec.Y=f; }
	void __fastcall SetZ(float f){ tvec.Z=f; }

public:
	//	�w���TSHVec3�̃I�u�W�F�N�g�C���X�y�N�^�\���p�C���^�t�F�[�X�̍쐬
	__fastcall TSHVec3fDummy(TSHVec3f& v): tvec(v){}
	virtual __fastcall ~TSHVec3fDummy(){}
	virtual void __fastcall Assign(TPersistent* src){
		if (src->InheritsFrom(__classid(TSHVec3fDummy))){
			tvec= ((TSHVec3fDummy*)src)->tvec;
		}
	}
__published:
	//�v�f
	__property float X= {read=GetX, write=SetX};
	__property float Y= {read=GetY, write=SetY};
	__property float Z= {read=GetZ, write=SetZ};
};

//---------------------------------------------------------------------------

#endif
