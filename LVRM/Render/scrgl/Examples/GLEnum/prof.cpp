//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "prof.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProgressForm *ProgressForm;
//---------------------------------------------------------------------------
__fastcall TProgressForm::TProgressForm(TComponent* Owner)
    : TForm(Owner)
{ FCancelled=false;
}
//---------------------------------------------------------------------------
void __fastcall TProgressForm::btnCancelClick(TObject *Sender)
{ FCancelled=true;
}
//---------------------------------------------------------------------------
void __fastcall TProgressForm::FormCreate(TObject *Sender)
{ Top=Screen->Height-Height-100;
  Left=Screen->Width-Width-100;
}
//---------------------------------------------------------------------------

