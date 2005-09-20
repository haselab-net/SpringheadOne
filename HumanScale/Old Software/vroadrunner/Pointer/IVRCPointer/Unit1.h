//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "KtDShow.h"
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <Sockets.hpp>

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TMainMenu *MainMenu1;
	TMenuItem *O1;
	TMenuItem *OpenO1;
	TMenuItem *StopS1;
	TMenuItem *CaptureC1;
	TMenuItem *N1;
	TMenuItem *ExitE1;
	TMenuItem *S1;
	TMenuItem *U1;
	TMenuItem *D1;
	TMenuItem *N2;
	TStatusBar *StatusBar1;
	TActionList *ActionList1;
	TAction *Start;
	TAction *Stop;
	TAction *Capture;
	TAction *RMinUp;
	TAction *RMinDown;
	TAction *CameraProperty;
	TAction *OutputProperty;
	TMenuItem *C1;
	TMenuItem *V1;
	TMenuItem *N3;
	TAction *ToggleMoveMouse;
	TMenuItem *M1;
	TMenuItem *N4;
	TAction *ToggleShowPreview;
	TMenuItem *P1;
	TAction *LoadTransformMatrix;
	TAction *TogglePositionTransform;
	TAction *CalibrateMatrix;
	TMenuItem *CalibrateMatrix1;
	TMenuItem *P2;
	TMenuItem *N5;
	TUdpSocket *UdpSocket1;
	TAction *ShowSettingForm;
	TMenuItem *S2;
	TAction *GMaxUp;
	TAction *GMaxDown;
	TAction *BMaxUp;
	TAction *BMaxDown;
	TMenuItem *GMaxJ1;
	TMenuItem *GMaxC1;
	TMenuItem *BMaxM1;
	TMenuItem *BMaxV1;
	TMenuItem *RGBR1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall StartExecute(TObject *Sender);
	void __fastcall StopExecute(TObject *Sender);
	void __fastcall CaptureExecute(TObject *Sender);
	void __fastcall RMinUpExecute(TObject *Sender);
	void __fastcall RMinDownExecute(TObject *Sender);
	void __fastcall ExitE1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CameraPropertyExecute(TObject *Sender);
	void __fastcall OutputPropertyExecute(TObject *Sender);
	void __fastcall ToggleMoveMouseExecute(TObject *Sender);
	void __fastcall ToggleShowPreviewExecute(TObject *Sender);
	void __fastcall LoadTransformMatrixExecute(TObject *Sender);
	void __fastcall TogglePositionTransformExecute(TObject *Sender);
	void __fastcall CalibrateMatrixExecute(TObject *Sender);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall ShowSettingFormExecute(TObject *Sender);
	void __fastcall GMaxUpExecute(TObject *Sender);
	void __fastcall GMaxDownExecute(TObject *Sender);
	void __fastcall BMaxUpExecute(TObject *Sender);
	void __fastcall BMaxDownExecute(TObject *Sender);
private:	// ユーザー宣言
	// 変数
	KtDShowCapture *Dshow;
	bool Calculating; // レーザーポインタ座標を計算するモードか
	bool IsMouseMove; // レーザーポインタと連動してマウスを動かすモードか
	bool IsPreviewWindow; // プレビュー画面を表示するか
	bool IsPositionTransform; // レーザーポインタの座標をスクリーンの座標に変換するか
	bool IsCalibrateMatrixMode; // キャプチャ画像中のスクリーンの頂点を指定するモードか
	int PreviewPerFrame; // 何フレームに一度プレビュー画面を表示するか
	int ScreenApex[4][2]; // キャプチャ画像中のスクリーンの頂点を入れる
	int NumOfScreenApex; // ScreenApexに入れられた頂点の数
	double OffsetX; // キャプチャ座標中のスクリーン座標の原点(キャプチャ座標)
	double OffsetY;
	double MatrixA; // キャプチャ座標をスクリーン座標に変換するための行列
	double MatrixB;
	double MatrixC;
	double MatrixD;
	int ScreenHeight; // キャプチャ画像中のスクリーンの高さ(スクリーン座標)
	int ScreenWidth; // キャプチャ画像中のスクリーンの幅(スクリーン座標)
	DWORD SendWaitTime; // 何msに一度座標を送信するか。

	bool IsCaptured; // レーザーポインタ座標が計算されたか(該当座標がないときはfalse)
	double CapX; // キャプチャ画像中のレーザーポインタの座標
	double CapY;
	int ScreenX; // スクリーン上のレーザーポインタの座標
	int ScreenY;
	BYTE RMin; // Rの閾値(最小値)
	BYTE GMax; // Gの閾値(最大値)
	BYTE BMax; // Bの閾値(最大値)
	bool XReverse;

	// 関数
	bool CalculatePoint(); // 座標を計算する
	bool CalculatePointWithPreview(); // プレビュー画面を表示しながら座標を計算
	void MoveMouseToPoint(); // 計算された座標にマウスを移動
	void WritePointToStatusBar(); // 計算された座標をステータスバーに表示
	bool CalculateMatrix(); // クリックされた点から変換行列を計算
	void SaveMatrix(); // 変換行列の内容をファイルに保存
	void LoadMatrix(); // 変換行列の内容をファイルから読み出し
	void SaveSetting();
	void LoadSetting();

	bool StartCapture(); // キャプチャ開始
	void StopCapture(); // キャプチャ停止
	bool CalculateLoop(); // 座標を計算するループ
	bool StopCalculate(); // 計算停止
	void SendScreenPoint(); // UDPで座標を送信

	void SetRMin(BYTE r_min); // RMin値を設定
	void SetGMax(BYTE g_max); // GMax値を設定
	void SetBMax(BYTE b_max); // BMax値を設定
	void WriteRgbToStatusBar(); // RMin,GMax,BMaxをステータスバーに表示
	void SaveRgb();
	void LoadRgb();

public:		// ユーザー宣言
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
