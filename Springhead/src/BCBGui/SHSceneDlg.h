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
__published:	// IDE �Ǘ��̃R���|�[�l���g
	TTreeView *TreeView1;
private:	// ���[�U�[�錾
public:		// ���[�U�[�錾
	__fastcall TSceneDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSceneDlg *SceneDlg;
//---------------------------------------------------------------------------
#endif
