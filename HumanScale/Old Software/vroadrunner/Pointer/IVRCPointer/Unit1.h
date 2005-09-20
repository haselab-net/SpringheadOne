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
__published:	// IDE �Ǘ��̃R���|�[�l���g
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
private:	// ���[�U�[�錾
	// �ϐ�
	KtDShowCapture *Dshow;
	bool Calculating; // ���[�U�[�|�C���^���W���v�Z���郂�[�h��
	bool IsMouseMove; // ���[�U�[�|�C���^�ƘA�����ă}�E�X�𓮂������[�h��
	bool IsPreviewWindow; // �v���r���[��ʂ�\�����邩
	bool IsPositionTransform; // ���[�U�[�|�C���^�̍��W���X�N���[���̍��W�ɕϊ����邩
	bool IsCalibrateMatrixMode; // �L���v�`���摜���̃X�N���[���̒��_���w�肷�郂�[�h��
	int PreviewPerFrame; // ���t���[���Ɉ�x�v���r���[��ʂ�\�����邩
	int ScreenApex[4][2]; // �L���v�`���摜���̃X�N���[���̒��_������
	int NumOfScreenApex; // ScreenApex�ɓ����ꂽ���_�̐�
	double OffsetX; // �L���v�`�����W���̃X�N���[�����W�̌��_(�L���v�`�����W)
	double OffsetY;
	double MatrixA; // �L���v�`�����W���X�N���[�����W�ɕϊ����邽�߂̍s��
	double MatrixB;
	double MatrixC;
	double MatrixD;
	int ScreenHeight; // �L���v�`���摜���̃X�N���[���̍���(�X�N���[�����W)
	int ScreenWidth; // �L���v�`���摜���̃X�N���[���̕�(�X�N���[�����W)
	DWORD SendWaitTime; // ��ms�Ɉ�x���W�𑗐M���邩�B

	bool IsCaptured; // ���[�U�[�|�C���^���W���v�Z���ꂽ��(�Y�����W���Ȃ��Ƃ���false)
	double CapX; // �L���v�`���摜���̃��[�U�[�|�C���^�̍��W
	double CapY;
	int ScreenX; // �X�N���[����̃��[�U�[�|�C���^�̍��W
	int ScreenY;
	BYTE RMin; // R��臒l(�ŏ��l)
	BYTE GMax; // G��臒l(�ő�l)
	BYTE BMax; // B��臒l(�ő�l)
	bool XReverse;

	// �֐�
	bool CalculatePoint(); // ���W���v�Z����
	bool CalculatePointWithPreview(); // �v���r���[��ʂ�\�����Ȃ�����W���v�Z
	void MoveMouseToPoint(); // �v�Z���ꂽ���W�Ƀ}�E�X���ړ�
	void WritePointToStatusBar(); // �v�Z���ꂽ���W���X�e�[�^�X�o�[�ɕ\��
	bool CalculateMatrix(); // �N���b�N���ꂽ�_����ϊ��s����v�Z
	void SaveMatrix(); // �ϊ��s��̓��e���t�@�C���ɕۑ�
	void LoadMatrix(); // �ϊ��s��̓��e���t�@�C������ǂݏo��
	void SaveSetting();
	void LoadSetting();

	bool StartCapture(); // �L���v�`���J�n
	void StopCapture(); // �L���v�`����~
	bool CalculateLoop(); // ���W���v�Z���郋�[�v
	bool StopCalculate(); // �v�Z��~
	void SendScreenPoint(); // UDP�ō��W�𑗐M

	void SetRMin(BYTE r_min); // RMin�l��ݒ�
	void SetGMax(BYTE g_max); // GMax�l��ݒ�
	void SetBMax(BYTE b_max); // BMax�l��ݒ�
	void WriteRgbToStatusBar(); // RMin,GMax,BMax���X�e�[�^�X�o�[�ɕ\��
	void SaveRgb();
	void LoadRgb();

public:		// ���[�U�[�錾
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
