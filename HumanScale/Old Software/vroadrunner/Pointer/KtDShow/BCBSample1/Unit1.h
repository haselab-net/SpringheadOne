//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "KtDShow.h"
#include <Dialogs.hpp>
#include <Menus.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TOpenDialog *OpenDialog1;
	TMainMenu *MainMenu1;
	TMenuItem *OpenO1;
	TMenuItem *Stop1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall OpenO1Click(TObject *Sender);
	void __fastcall Stop1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
private:	// ユーザー宣言
	KtDShowBasic *dshow;
public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
