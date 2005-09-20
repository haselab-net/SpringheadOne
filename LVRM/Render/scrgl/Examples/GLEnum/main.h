//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "scrgl.h"
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *Grid;
    TPanel *Panel1;
    TGroupBox *GroupBox1;
    TMemo *DriverList;
    TSaveDialog *SaveDialog;
    TPanel *TestPanel;
    TSglDriverList *SglDriverList;
    TPanel *Panel2;
    TLabel *Label2;
    TLabel *Label1;
    TLabel *Label3;
    TPanel *Panel3;
    TButton *btnSave;
    TButton *Refresh;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall RefreshClick(TObject *Sender);
    
    void __fastcall btnSaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
    void __fastcall Draw();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
