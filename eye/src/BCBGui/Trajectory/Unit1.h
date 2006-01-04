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
#include "SHPathGraph.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
    TSHBox *SHBox1;
    TSHScene *SHScene1;
    TButton *btStep;
	TButton *btRun;
    TTimer *Timer1;
	TSHPathGraph *SHPathGraph1;
	TButton *btLoad;
	TButton *btSave;
	TSHPathGraph *SHPathGraph2;
	TSHPathGraph *SHPathGraph3;
	TSHPathGraph *SHPathGraph4;
	TSHPathGraph *SHPathGraph5;
	TSHPathGraph *SHPathGraph6;
	TSHPathGraph *SHPathGraph7;
	TSHPathGraph *SHPathGraph8;
	TSHPathGraph *SHPathGraph9;
	TSHPathGraph *SHPathGraph10;
	TSHPathGraph *SHPathGraph11;
	TSHPathGraph *SHPathGraph12;
	TSHBox *SHBox2;
    void __fastcall btStepClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btRunClick(TObject *Sender);
	void __fastcall btLoadClick(TObject *Sender);
	void __fastcall btSaveClick(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	Spr::UTRef<Spr::PHControlPath> path;
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
