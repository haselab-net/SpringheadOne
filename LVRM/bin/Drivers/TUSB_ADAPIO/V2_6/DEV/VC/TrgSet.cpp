// TrgSet.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "vcsample.h"
#include "TrgSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrgSet �_�C�A���O


CTrgSet::CTrgSet(CWnd* pParent /*=NULL*/)
	: CDialog(CTrgSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrgSet)
	m_EndCh = 0;
	m_Message = _T("");
	m_ThLevel = _T("");
	m_TrgCh = 0;
	m_BufSize = 0;
	m_UpDown = -1;
	//}}AFX_DATA_INIT
}


void CTrgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrgSet)
	DDX_Text(pDX, IDC_ENDCH, m_EndCh);
	DDV_MinMaxUInt(pDX, m_EndCh, 0, 3);
	DDX_Text(pDX, IDC_MESSAGE2, m_Message);
	DDX_Text(pDX, IDC_THLEVEL, m_ThLevel);
	DDV_MaxChars(pDX, m_ThLevel, 3);
	DDX_Text(pDX, IDC_TRGCH, m_TrgCh);
	DDV_MinMaxUInt(pDX, m_TrgCh, 0, 3);
	DDX_Text(pDX, IDC_BUFSIZE, m_BufSize);
	DDV_MinMaxUInt(pDX, m_BufSize, 1, 512);
	DDX_Radio(pDX, IDC_RADIO1, m_UpDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrgSet, CDialog)
	//{{AFX_MSG_MAP(CTrgSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrgSet ���b�Z�[�W �n���h��

void CTrgSet::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData(TRUE);//�ϐ��Ƀ_�C�A���O�l����荞��
	if( ShowFlg )//臒l�����͍��ڂƂȂ��Ă���ꍇ
	{
		if( !HexCheck(m_ThLevel) )//臒l�̃f�[�^�̃`�F�b�N(HEX�l�łȂ���΂Ȃ�Ȃ�)
		{
			AfxMessageBox("臒l�̒l���s���ł�(1-3FF)");
			return;
		}
		sscanf(m_ThLevel,"%x",&ThLevelNum );//m_ThLevel��int�ϐ��Ɏ�荞��
		if( ThLevelNum <= 0 || ThLevelNum >= 0x3FF )//�͈̓`�F�b�N
		{
			AfxMessageBox("臒l�̒l���s���ł�(1-3FF)");
			return;
		}
	}
	CDialog::OnOK();
}

//�������HEX(0-9,a-e,A-E)�ȊO�̕������Ȃ����`�F�b�N����֐�
BOOL CTrgSet::HexCheck(const char *str)
{
	int len;//������

	len = strlen(str);

	for( int i = 0 ; i < len ; i++ )
	{
		switch( str[i] )
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			continue;
		default:
			return FALSE;//HEX�����ȊO�̕�����������
		}
	}
	return TRUE;
}

BOOL CTrgSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	if( ShowFlg )//臒l�A�g���K�`�����l���A�����オ��A������̕\���A��\���̐ؑ�
	{
		GetDlgItem(IDC_THLEVEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TRGCH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_THLEVEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TRGCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
