#ifndef Spr_DX_CUNKNOWN_H
#define Spr_DX_CUNKNOWN_H
#if _MSC_VER >= 1000
# pragma once
#endif // _MSC_VER >= 1000

#undef  __RPCNDR_H_VERSION__
#define __RPCNDR_H_VERSION__        ( 475 )

/**	�g�p����DirectX�̃o�[�W����(Dx8�ȍ~�C7�ȑO��WinDXOld���Ή��\��)
	������������������CWinDXLib.bat �����������Ă��������D
	�܂��CSpringheadGUI.bpg �̃��C�u�������ύX���Ă��������D*/
#ifndef SPR_DXVER
# define SPR_DXVER 9
#endif

#include <WinBasis/WBWin.h>
#include <Base/BaseUtility.h>

#ifdef __BORLANDC__
#define sqrtf sqrt
#endif

namespace Spr {

template <class T>
void RELEASE(T& x){
	if (x){
		x->Release();
		x = NULL;
	}
}

/**	IUnknown �̃��b�p
	
*/
class SPR_DLL WXUnknown{
protected:
	///	IUnknown�ւ̃|�C���^
	IUnknown* intf;

public:
	///	�R�s�[�R���X�g���N�^�Cu ��NULL�łȂ���ΎQ�ƃJ�E���^�𑝂₷(IUnknown::AddRef���Ă�)
	WXUnknown(const WXUnknown& u);
	///	�\�z�CpObj ��NULL�łȂ���ΎQ�ƃJ�E���^�𑝂₷(IUnknown::AddRef���Ă�)
	WXUnknown(IUnknown* i = NULL);

	///	�j���C�Q�Ƃ����炷(IUnknown::Release���Ă�)
	virtual ~WXUnknown();

	///	����CRelease()��CpObject=o.pObject�Ƃ��C�Q�Ƃ𑝂₷�D
	WXUnknown& operator = (const WXUnknown& o);

	///	COM�C���^�[�t�F�C�X��GUID
	virtual const IID& InterfaceID() const;

	///	COM�N���X��GUID
	virtual const CLSID& ClassID() const;
	///	IUnknown �ւ̎Q�Ƃ����炵�CpObject=NULL�Ƃ���D
	virtual void Release();
};

#define WXINTF(OBJ) WXInterface<I##OBJ, &IID_I##OBJ>
#if SPR_DXVER==8
# define WXINTFV(OBJ) WXInterface<I##OBJ##8, &IID_I##OBJ##8>
#elif SPR_DXVER==9
# define WXINTFV(OBJ) WXInterface<I##OBJ##9, &IID_I##OBJ##9>
#endif

/**	COM�C���^�t�F�[�X�̃��b�p�D
*/
template <class IF, const IID* iid>
class WXInterface: public WXUnknown{
public:
	///
	typedef IF intf_type;
	///
	typedef WXInterface<intf_type, iid> base_type;
	///
	WXInterface(IF* i = NULL): WXUnknown(i){}
	///
	WXInterface(IUnknown* i):WXUnknown(NULL){
		if (i) i->QueryInterface(iid, &Intf());
	}
	///
 	IF*& Intf(){ return (IF*&)intf; }
	///
 	IF* Intf() const { return (IF*)intf; }
	///
	IF* operator ->() const { return Intf(); }
	///
	operator IF*() const { return Intf(); }
	///
	WXInterface<intf_type, iid>& operator =(intf_type* t){
		if (Intf()) Intf()->Release();
		Intf() = t;
		if (Intf()) Intf()->AddRef();
		return *this;
	}
	///	COM�C���^�[�t�F�C�X��GUID
	virtual const IID& InterfaceID() const{ return *iid; }
	///
	const IID& InterfaceIDStatic(){ return *iid; }
};


#define WXSINGLETON(OBJ) WXSingletonInterface<I##OBJ, &IID_I##OBJ>
#if SPR_DXVER==8
#define WXSINGLETONV(OBJ) WXSingletonInterface<I##OBJ##8, &IID_I##OBJ##8>
#elif SPR_DXVER==9
#define WXSINGLETONV(OBJ) WXSingletonInterface<I##OBJ##9, &IID_I##OBJ##9>
#endif
/**	�V���O���g���p���b�p�[�D
	�v���O�����S�̂Ŋm���ɂЂƂ������̂����������D
	���C�����O�ł��m���Ɏg�p�ł���D
	�v���O�����I�����ɊJ�������D
*/
template <class IF, const IID* iid>
class WXSingletonInterface:public WXInterface<IF, iid>{
public:
	///	�R���X�g���N�^
	void Prepare(){
		if (Intf()) return;
		WXInterface<IF,iid>& singleton = Singleton< WXInterface<IF,iid> >();
		if (!singleton.Intf()){
			singleton.Intf() = (IF*)Create();
		}
		Intf() = singleton.Intf();
		if (Intf()) Intf()->AddRef();
	}
	/**	�C���^�t�F�[�X�𐶐����ĕԂ��֐��D
		�v���O�����S�̂�1�x�����Ă΂��\��������D
	*/
	virtual IUnknown* Create()=0;
};

class SPR_DLL WXComInitImp: public UTRefCount{
public:
	WXComInitImp();
	~WXComInitImp();
	static WXComInitImp* comInitImp;
};
class SPR_DLL WXComInit:public UTRef<WXComInitImp>{
public:
	WXComInit();
};

}	//	namespace Spr
#endif