//---------------------------------------------------------------------------

#ifndef TSHSceneDlgH
#define TSHSceneDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TSceneDlg : public TForm
{
__published:	// IDE 管理のコンポーネント
	TTreeView *TreeView1;
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TSceneDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSceneDlg *SceneDlg;
//---------------------------------------------------------------------------
#endif
