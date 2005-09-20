//---------------------------------------------------------------------------
#ifndef profH
#define profH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TProgressForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TProgressBar *ProgressBar;
    TLabel *Label;
    TButton *btnCancel;
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TProgressForm(TComponent* Owner);
    bool FCancelled;
};
//---------------------------------------------------------------------------
extern PACKAGE TProgressForm *ProgressForm;
//---------------------------------------------------------------------------
#endif
