//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TPreviewForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TPreviewForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPreviewForm *PreviewForm;
//---------------------------------------------------------------------------
#endif
