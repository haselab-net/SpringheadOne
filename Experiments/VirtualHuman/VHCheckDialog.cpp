// VHCheckDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "VirtualHuman.h"
#include "VHCheckDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern Spr::GRMaterialData Mat(Spr::Vec4f c);

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog �_�C�A���O


VHCheckDialog::VHCheckDialog(CWnd* pParent /*=NULL*/)
	: CDialog(VHCheckDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(VHCheckDialog)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void VHCheckDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(VHCheckDialog)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}

void VHCheckDialog::DrawDlg(Spr::GRRender* render, TBalance& balance,Spr::PHHuman* phHuman){

	if (!m_hWnd) return;
	CWnd* viewCWnd = GetDlgItem(IDC_VIEWER);	//�s�N�`���[�iVIEWER)�̎擾
	HWND viewHW = viewCWnd->GetSafeHwnd();		//�E�B���h�n���h�����擾

	if(viewHW){

		//�J�����ݒ�
		Spr::Affinef dlgView,		//���̃_�C�A���O�̃J�����ʒu
					mainView;		//���C���_�C�A���O�̃J�����ʒu
					
		mainView = render->camera->data.view;	//���C���̎��_��ۑ����Ă���

		//dlg�̃J�����ʒu�̐ݒ�
		dlgView.yz = -1.0f;
		dlgView.yy = 0.0f;
		dlgView.zy = -1.0f;
		dlgView.zz = 0.0f;
		dlgView.pz = -1.0f;
		render->camera->data.view = dlgView;

		//�r���[�|�[�g�̕ύX
		render->SetViewport(viewHW);

		render->ClearBuffer();			//	�o�b�t�@�N���A
		render->BeginScene();			//	�`��J�n

		//DrawFootArea(render,balance);
		DrawCollisionArea(balance.collisionArea);
		DrawZmp(render,balance,phHuman);

		render->EndScene();				//	�`��I��
		render->Present();				//	�\��

		render->camera->data.view = mainView;	//�J�����̎��_�����C���E�B���h�E�ɖ߂�
	}

}

void VHCheckDialog::DrawFootArea(Spr::GRRender* render, TBalance& balance){
		Spr::Vec3f* supportPoint = balance.GetFootPoint();
		Spr::Vec3f supportArea[9];

		//����`�悷�邽�߁A����₷�B
		for(int i = 0; i < 8; ++i){
			supportArea[i] = supportPoint[i];
		}
		supportArea[8] = supportArea[0];

		//���C���E�B���h���W�����̃_�C�A���O���W�ɕϊ����邽��
		Spr::Vec3f center;
		for(int i = 0; i < 8; ++i){
			center += supportArea[i];
		}
		center = center/8;
		for(int i = 0; i < 9; ++i){
			supportArea[i] -= center;
			SetNonDepth(&supportArea[i]);
		}

		//����̈�̕`��
		render->SetLineWidth(6);
		render->SetMaterial(Mat(Spr::Vec4f(1,0,1,1)));
		for(int i=0; i<8; ++i){
			render->DrawDirect(Spr::GRRender::POINTS, &supportArea[i] , &supportArea[i]+1);
			render->DrawDirect(Spr::GRRender::LINES, &supportArea[i], &supportArea[i+2]);
		}

		//ZMP�̕`��
		render->SetMaterial(Mat(Spr::Vec4f(1,0,0,1)));
		render->SetLineWidth(3);
		Spr::Vec3f v = balance.GetZmp();
		v -= center;
		SetNonDepth(&v);
		render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);

		//���_�̕`��
		Spr::Vec3f zero;
		render->DrawDirect(Spr::GRRender::POINTS, &zero , &zero+1);

}

void VHCheckDialog::DrawCollisionArea(CollisionArea& collisionArea){
	collisionArea.DrawCollisionVetxs();
}

void VHCheckDialog::DrawZmp(Spr::GRRender* render, TBalance& balance, Spr::PHHuman* phHuman){
	balance.DrawZmp(phHuman,render);
}

BEGIN_MESSAGE_MAP(VHCheckDialog, CDialog)
	//{{AFX_MSG_MAP(VHCheckDialog)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog ���b�Z�[�W �n���h��
