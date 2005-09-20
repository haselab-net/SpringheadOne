// VHCheckDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "vhindependence.h"
#include "VHCheckDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Spr;

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog �_�C�A���O


VHCheckDialog::VHCheckDialog(CWnd* pParent /*=NULL*/)
	: CDialog(VHCheckDialog::IDD, pParent)
{
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


void VHCheckDialog::Draw(Spr::GRRender* render, Spr::CRVirtualHuman* crVH){
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

		Vec3f center = Vec3f();
		std::vector<Vec3f> supportVtxs = crVH->supportArea.GetSupportArea();
		for(int i = 0; i < supportVtxs.size(); i++){
			center += supportVtxs[i];
		}
		center /= supportVtxs.size();
		dlgView.px = -center.x;
		dlgView.py = center.y;

		render->camera->data.view = dlgView;

		//�r���[�|�[�g�̕ύX
		render->SetViewport(viewHW);

		render->ClearBuffer();			//	�o�b�t�@�N���A
		render->BeginScene();			//	�`��J�n

		//////////		�`��]�[��		//////////
		if(bDrawZmp){
			crVH->balanceQDPR.DrawZMP(render);
		}
		if(bDrawSuArea){
			crVH->supportObject[0].DrawCollisionVetxs(render);
			crVH->supportObject[1].DrawCollisionVetxs(render);

			// �����ZMP��`��
			for(int i=0; i<2; ++i){
				render->SetModelMatrix(Affinef());
				if (i==0) render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,1,1)));
				else render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,1,1)));
				render->SetLineWidth(10);
				Vec3f v[2];
				if (i==0){
					v[0] = crVH->balanceQDPR.GetAnkleRealZmpR();
					v[1] = crVH->balanceQDPR.GetAnkleGoalZmpR();
				}else{
					v[0] = crVH->balanceQDPR.GetAnkleRealZmpL();
					v[1] = crVH->balanceQDPR.GetAnkleGoalZmpL();
				}
				render->DrawDirect(Spr::GRRender::POINTS, v , v+1);
				render->DrawDirect(Spr::GRRender::LINES, v , v+2);
			}
		}

		//////////////////////////////////////////
		render->EndScene();				//	�`��I��
		render->Present();				//	�\��

		render->camera->data.view = mainView;	//�J�����̎��_�����C���E�B���h�E�ɖ߂�
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
