//---------------------------------------------------------------------------

#ifndef SHAffineDlgH
#define SHAffineDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "SHBox.h"
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE �Ǘ��̃R���|�[�l���g
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton3;
        TSpeedButton *SpeedButton4;
        TSpeedButton *SpeedButton5;
        TSpeedButton *SpeedButton6;
        TSpeedButton *SpeedButton7;
        TSHBox *SHBox1;
private:	// ���[�U�[�錾
public:		// ���[�U�[�錾
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
