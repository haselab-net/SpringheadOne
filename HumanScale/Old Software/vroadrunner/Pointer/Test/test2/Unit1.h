//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <dshow.h>
#include <Dialogs.hpp>
#include <Menus.hpp>

#ifndef KT_RELEASE
#define KT_RELEASE(x) if((x)){ (x)->Release(); (x)=NULL; }
#endif // KT_RELEASE
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TOpenDialog *OpenDialog1;
	TMainMenu *MainMenu1;
	TMenuItem *OpenO1;
	TMenuItem *StopS1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall OpenO1Click(TObject *Sender);
	void __fastcall StopS1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// ユーザー宣言
	IGraphBuilder *pGraph;
	IMediaControl *pMediaControl;
	IVideoWindow *pVidWin;
	IBasicVideo *pBasicVideo;
	bool IsAudioOnly();
public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
	void PlayStart();
	void PlayStop();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
