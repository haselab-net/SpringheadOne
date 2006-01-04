//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SHBox.h"
#include "SHComponent.h"
#include "SHScene.h"
#include <ExtCtrls.hpp>
#include "SHFrame.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TSHBox *SHBox1;
	TSHScene* Scene;
	TTimer *Timer1;
	TButton *btLeft;
	TButton *btStop;
	TButton *btRight;
	TSHFrame *Penguin;
	TButton *Button1;
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall btLeftClick(TObject *Sender);
	void __fastcall btStopClick(TObject *Sender);
	void __fastcall btRightClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 