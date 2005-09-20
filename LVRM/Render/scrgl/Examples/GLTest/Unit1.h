//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "scrgl.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TTimer *Timer1;
	TSglPanel *SglPanel2;
	TSglPanel *SglPanel1;
    void __fastcall SglPanel1GLInit(TObject *Sender);
    void __fastcall SglPanel1GLPaint(TObject *Sender);
    void __fastcall SglPanel2GLInit(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall SglPanel2GLPaint(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
