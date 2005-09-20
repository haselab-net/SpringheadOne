// VHCheckDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "VHInteraction.h"
#include "VHCheckDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Spr;

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog �_�C�A���O

VHCheckDialog* vhCheckDialog;

VHCheckDialog::VHCheckDialog(CWnd* pParent /*=NULL*/)
	: CDialog(VHCheckDialog::IDD, pParent)
{
	vhCheckDialog = this;
	bDraw = false;
	bDrawZmp = false;
	bDrawSuArea = false;
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


void VHCheckDialog::Draw(Spr::GRRender* render, Spr::CRHuman* crHuman){
	if (!m_hWnd) return;
	CWnd* viewCWnd = GetDlgItem(IDC_VIEWER);	//�s�N�`���[�iVIEWER)�̎擾
	HWND viewHW = viewCWnd->GetSafeHwnd();		//�E�B���h�n���h�����擾

	if(viewHW){
		//�J�����ݒ�
		Affinef dlgView,		//���̃_�C�A���O�̃J�����ʒu
				mainView;		//���C���_�C�A���O�̃J�����ʒu

		mainView = render->camera->data.view;	//���C���̎��_��ۑ����Ă���

		//dlg�̃J�����p��
		dlgView.yz = -1.0f;
		dlgView.yy = 0.0f;
		dlgView.zy = -1.0f;
		dlgView.zz = 0.0f;
		//dlg�̃J�����̈ʒu
		dlgView.pz = -1.5f;
		//dlgView.px = -crHuman->crBalance.GetTargetZMP().x;
		//dlgView.py = crHuman->crBalance.GetTargetZMP().z;
//		dlgView.px = -crHuman->crBalance.supportArea.coCog.x;
//		dlgView.py = crHuman->crBalance.supportArea.coCog.y;

		render->camera->data.view = dlgView;

		//�r���[�|�[�g�̕ύX
		render->SetViewport(viewHW);

		render->ClearBuffer();			//	�o�b�t�@�N���A
		render->BeginScene();			//	�`��J�n

		//////////		�`��]�[��		//////////
		if(bDrawZmp){
//			crHuman->crBalance.DrawZMP(render);
		}
		if(bDrawSuArea){
//			crHuman->crBalance.supportArea.DrawCollisionVetxs(render);
		}

		//////////////////////////////////////////
		render->EndScene();				//	�`��I��
		render->Present();				//	�\��

		render->camera->data.view = mainView;	//�J�����̎��_�����C���E�B���h�E�ɖ߂�
	}
}

void VHCheckDialog::DrawTest(Spr::GRRender* render, Spr::SGScene* scene){
	if (!m_hWnd) return;
	CWnd* viewCWnd = GetDlgItem(IDC_VIEWER);	//�s�N�`���[�iVIEWER)�̎擾
	HWND viewHW = viewCWnd->GetSafeHwnd();		//�E�B���h�n���h�����擾

	if(viewHW){
		//�r���[�|�[�g�̕ύX
		render->SetViewport(viewHW);

		render->ClearBuffer();			//	�o�b�t�@�N���A
		render->BeginScene();			//	�`��J�n

		//////////		�`��]�[��		//////////
		render->Render(scene);		//	�V�[���̕`��
		render->EndScene();				//	�`��I��
		render->Present();				//	�\��
	}
}

BEGIN_MESSAGE_MAP(VHCheckDialog, CDialog)
	//{{AFX_MSG_MAP(VHCheckDialog)
	ON_BN_CLICKED(IDC_CHECK_ZMP, OnCheckZmp)
	ON_BN_CLICKED(IDC_CHECK_SUPPORT_AREA, OnCheckSupportArea)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog ���b�Z�[�W �n���h��

void VHCheckDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	bDraw = true;
	//CDialog::OnOK();
}

void VHCheckDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	bDraw = false;
	CDialog::OnCancel();
}


void VHCheckDialog::OnCheckZmp() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CButton* chkZmp = (CButton*)GetDlgItem(IDC_CHECK_ZMP);
	if(chkZmp->GetCheck() == 1){
		bDrawZmp = true;
	}
	else{
		bDrawZmp = false;
	}	
}

void VHCheckDialog::OnCheckSupportArea() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CButton* chkSuArea = (CButton*)GetDlgItem(IDC_CHECK_SUPPORT_AREA);
	if(chkSuArea->GetCheck() == 1){
		bDrawSuArea = true;
	}
	else{
		bDrawSuArea = false;
	}		
}
