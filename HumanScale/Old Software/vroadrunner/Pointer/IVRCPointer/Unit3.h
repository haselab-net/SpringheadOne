//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSettingForm : public TForm
{
__published:	// IDE �Ǘ��̃R���|�[�l���g
	TLabel *Label1;
	TEdit *EditWidth;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *EditHeight;
	TEdit *EditHost;
	TEdit *EditPort;
	TButton *Button1;
	TButton *Button2;
	TEdit *EditSendWaitTime;
	TLabel *Label5;
	void __fastcall EditSendWaitTimeExit(TObject *Sender);
private:	// ���[�U�[�錾
public:		// ���[�U�[�錾
	__fastcall TSettingForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingForm *SettingForm;
//---------------------------------------------------------------------------
#endif
