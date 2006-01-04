#ifndef TSHSceneH
#define TSHSceneH
//---------------------------------------------------------------------------
#include "SHComponent.h"
#include <Framework/FWAppD3D.h>
//---------------------------------------------------------------------------
namespace Spr{
	class D3Render;
}

class __declspec(delphiclass) TSHSceneGraphDlg;
class __declspec(delphiclass) TSHCamera;
class __declspec(delphiclass) TSHBox;

/**	�V�[���O���t�S��	*/
class PACKAGE TSHScene:public TSHComponent{
private:
	bool showTree;
	void __fastcall SetShowTree(bool);
	virtual Spr::SGScene* __fastcall GetImp();
	virtual void __fastcall WriteState(TWriter* Writer);
	virtual void __fastcall Loaded(void);

protected:
	///	Springhead�̃A�v���P�[�V�����t���[�����[�N
	Spr::FWAppD3D app;
	Spr::D3Render* __fastcall GetRenderer();
	void __fastcall SetRenderer(Spr::D3Render* r);
	TSHSceneGraphDlg* __fastcall GetDlg();

	//	�t�@�C����
    AnsiString fileName;
	void __fastcall SetFileName(AnsiString fn);
	AnsiString __fastcall GetFileName();

	//	�\��Box
	std::vector<TSHBox*> boxes;
	TSHBox* __fastcall GetBoxes(int idx);
	int __fastcall GetBoxCount();
	friend class TSHBox;

	//	�ҏW�_�C�A���O
	TSHSceneGraphDlg* dlg;
	friend class TSHSceneGraphDlg;
public:
	///
	__property Spr::SGScene* Imp = {read=GetImp};
	///
	__fastcall TSHScene(TComponent* Owner);
	///
	virtual __fastcall ~TSHScene();
	///	�ύX��`����iBox���ĕ`�悷��j
	void __fastcall NotifyChange(TObject* o=NULL);
	///	�I�u�W�F�N�g�����ς��������`����
	void NotifyChangeName(Spr::SGObject* o);
	///	�t�@�C����ǂݏo��
	bool Load(Spr::UTString filename);
	///	�t�@�C����ǂݒ����A�V�[�������Z�b�g����
	void Reset();
	///	�t�@�C����ǂݏo���Cfr�̉��Ƀ��[�h����
	bool Import(Spr::SGObject* obj, Spr::UTString filename);
	///	obj ���t�@�C���ɏ����o��
	bool Export(Spr::SGObject* obj, Spr::UTString filename);
	///	�����_�����O���s��
	void __fastcall Render(TSHBox* box);
	///	���O����SGObject������
	Spr::SGObject* FindObject(const Spr::UTString& key);
	void Step(){ app.Step(); }

	///	�����_���[
	__property Spr::D3Render* Renderer = {read=GetRenderer, write=SetRenderer};
	///	�V�[���O���t�ҏW�_�C�A���O
	__property TSHSceneGraphDlg* Dlg={read=GetDlg};
	///	�V�[���O���t��World�m�[�h
	DEFINE_PROPERTYR(Spr::UTRef<Spr::SGFrame>, World, Imp->GetWorld());
	///	SGObject�ɑΉ�����TSHComponent�̔h���N���X���쐬
	TSHComponent* CreateComponent(TComponent* Owner, Spr::SGObject* o){
		return (TSHComponent*)CreateObjectOrClass(Owner, o, false);
	}
	///	SGObject�ɑΉ�����TSHComponent�̔h���N���X��MetaClass��Ԃ��D
	TMetaClass* GetMetaClass(Spr::SGObject* o){
		return (TMetaClass*)CreateObjectOrClass(NULL, o, true);
	}
	__property TSHBox* Boxes[int index]={read=GetBoxes};
	__property int BoxCount={read=GetBoxCount};

__published:
	__property AnsiString FileName={ read=GetFileName, write=SetFileName};
	__property bool ShowTree={read=showTree, write=SetShowTree};

private:
    Spr::SGFrame* GetBBoxFrame();
    Spr::SGFrame* GetAxisFrame();
	Spr::SGFrame* GetWorkFrame(const char* fn);

	///	SGObject�̌^�ɑΉ����� TMetaClass ���C�R���|�[�l���g��Ԃ��D
	void* CreateObjectOrClass(TComponent* Owner, Spr::SGObject* o, bool bClass);
};
//---------------------------------------------------------------------------
#endif
