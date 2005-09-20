//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingForm *SettingForm;
//---------------------------------------------------------------------------
__fastcall TSettingForm::TSettingForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TSettingForm::EditSendWaitTimeExit(TObject *Sender)
{
	if( ((TEdit*)Sender)->Text.ToIntDef(-1) < 0 ){
		Application->MessageBox(
			"適切な数値を入れてください。","なおしてね。",MB_OK );
        ((TEdit*)Sender)->SetFocus();
	}
}
//---------------------------------------------------------------------------

