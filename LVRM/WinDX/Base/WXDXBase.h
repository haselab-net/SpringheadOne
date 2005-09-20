#ifndef LVRM_DX_CUNKNOWN_H
#define LVRM_DX_CUNKNOWN_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#undef  __RPCNDR_H_VERSION__
#define __RPCNDR_H_VERSION__        ( 475 )

#include "WinBasis/WBWin.h"
#include <Base/utilities.h>

namespace LVRM {

template <class T>
void RELEASE(T& x){
	if (x){
		x->Release();
		x = NULL;
	}
}

///	IUnknown �̃��b�p
class WXDXBase {
protected:
	///	IUnknown�ւ̃|�C���^
	IUnknown* intf;

public:
	///	�R�s�[�R���X�g���N�^�Cu ��NULL�łȂ���ΎQ�ƃJ�E���^�𑝂₷(IUnknown::AddRef���Ă�)
	WXDXBase(const WXDXBase& u);
	///	�\�z�CpObj ��NULL�łȂ���ΎQ�ƃJ�E���^�𑝂₷(IUnknown::AddRef���Ă�)
	WXDXBase(IUnknown* i = NULL);

	///	�j���C�Q�Ƃ����炷(IUnknown::Release���Ă�)
	virtual ~WXDXBase();

	///	����CRelease()��CpObject=o.pObject�Ƃ��C�Q�Ƃ𑝂₷�D
	WXDXBase& operator = (const WXDXBase& o);

	///	COM�C���^�[�t�F�C�X��GUID
	virtual const IID& InterfaceID() const;

	///	COM�N���X��GUID
	virtual const CLSID& ClassID() const;
	///	IUnknown �ւ̎Q�Ƃ����炵�CpObject=NULL�Ƃ���D
	virtual void Release();
};
template <class IF, const IID* iid>
class CDXInterface: public WXDXBase{
public:
	///
	typedef IF intf_type;
	///
	typedef CDXInterface<intf_type, iid> base_type;
	///
	CDXInterface(IF* i = NULL): WXDXBase(i){}
	///
 	IF*& Intf(){ return (IF*&)intf; }
	///
 	IF* Intf() const { return (IF*)intf; }
	///
	IF* operator ->() const { return Intf(); }
	///
	operator IF*() const { return Intf(); }
	///	COM�C���^�[�t�F�C�X��GUID
	virtual const IID& InterfaceID() const{ return *iid; }
};
#define CDXINTF(OBJ) CDXInterface<I##OBJ, &IID_I##OBJ>

class CDXComInitImp: public UTRefCount{
public:
	CDXComInitImp();
	~CDXComInitImp();
	static CDXComInitImp* comInitImp;
};
class CDXComInit:public UTRef<CDXComInitImp>{
public:
	CDXComInit();
};

}	//	namespace LVRM
#endif