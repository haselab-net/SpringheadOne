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
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TOpenDialog *OpenDialog1;
	TMainMenu *MainMenu1;
	TMenuItem *OpenO1;
	TMenuItem *Stop1;
	TMenuItem *CaptureC1;
	TImage *Image1;
	TSavePictureDialog *SavePictureDialog1;
	TMenuItem *SaveF1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall OpenO1Click(TObject *Sender);
	void __fastcall Stop1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CaptureC1Click(TObject *Sender);
	void __fastcall SaveF1Click(TObject *Sender);
private:	// ユーザー宣言
	KtDShowCapture *dshow;
//	Graphics::TBitmap *imagebm;
public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
