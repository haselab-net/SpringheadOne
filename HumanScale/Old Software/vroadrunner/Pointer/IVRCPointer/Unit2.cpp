//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPreviewForm *PreviewForm;
//---------------------------------------------------------------------------
__fastcall TPreviewForm::TPreviewForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TPreviewForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Action = caHide;
	MainForm->ToggleShowPreview->Execute();
}
//---------------------------------------------------------------------------


