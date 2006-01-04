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

/**	シーングラフ全体	*/
class PACKAGE TSHScene:public TSHComponent{
private:
	bool showTree;
	void __fastcall SetShowTree(bool);
	virtual Spr::SGScene* __fastcall GetImp();
	virtual void __fastcall WriteState(TWriter* Writer);
	virtual void __fastcall Loaded(void);

protected:
	///	Springheadのアプリケーションフレームワーク
	Spr::FWAppD3D app;
	Spr::D3Render* __fastcall GetRenderer();
	void __fastcall SetRenderer(Spr::D3Render* r);
	TSHSceneGraphDlg* __fastcall GetDlg();

	//	ファイル名
    AnsiString fileName;
	void __fastcall SetFileName(AnsiString fn);
	AnsiString __fastcall GetFileName();

	//	表示Box
	std::vector<TSHBox*> boxes;
	TSHBox* __fastcall GetBoxes(int idx);
	int __fastcall GetBoxCount();
	friend class TSHBox;

	//	編集ダイアログ
	TSHSceneGraphDlg* dlg;
	friend class TSHSceneGraphDlg;
public:
	///
	__property Spr::SGScene* Imp = {read=GetImp};
	///
	__fastcall TSHScene(TComponent* Owner);
	///
	virtual __fastcall ~TSHScene();
	///	変更を伝える（Boxを再描画する）
	void __fastcall NotifyChange(TObject* o=NULL);
	///	オブジェクト名が変わった事を伝える
	void NotifyChangeName(Spr::SGObject* o);
	///	ファイルを読み出す
	bool Load(Spr::UTString filename);
	///	ファイルを読み直し、シーンをリセットする
	void Reset();
	///	ファイルを読み出し，frの下にロードする
	bool Import(Spr::SGObject* obj, Spr::UTString filename);
	///	obj をファイルに書き出す
	bool Export(Spr::SGObject* obj, Spr::UTString filename);
	///	レンダリングを行う
	void __fastcall Render(TSHBox* box);
	///	名前からSGObjectを検索
	Spr::SGObject* FindObject(const Spr::UTString& key);
	void Step(){ app.Step(); }

	///	レンダラー
	__property Spr::D3Render* Renderer = {read=GetRenderer, write=SetRenderer};
	///	シーングラフ編集ダイアログ
	__property TSHSceneGraphDlg* Dlg={read=GetDlg};
	///	シーングラフのWorldノード
	DEFINE_PROPERTYR(Spr::UTRef<Spr::SGFrame>, World, Imp->GetWorld());
	///	SGObjectに対応するTSHComponentの派生クラスを作成
	TSHComponent* CreateComponent(TComponent* Owner, Spr::SGObject* o){
		return (TSHComponent*)CreateObjectOrClass(Owner, o, false);
	}
	///	SGObjectに対応するTSHComponentの派生クラスのMetaClassを返す．
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

	///	SGObjectの型に対応する TMetaClass か，コンポーネントを返す．
	void* CreateObjectOrClass(TComponent* Owner, Spr::SGObject* o, bool bClass);
};
//---------------------------------------------------------------------------
#endif
