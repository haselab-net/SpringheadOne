// DataIn.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "vcsample.h"
#include "DataIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataIn �_�C�A���O


CDataIn::CDataIn(CWnd* pParent /*=NULL*/)
	: CDialog(CDataIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataIn)
	m_Data = _T("");
	m_Message = _T("");
	//}}AFX_DATA_INIT
}


void CDataIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataIn)
	DDX_Text(pDX, IDC_DATA, m_Data);
	DDX_Text(pDX, IDC_MESSAGE1, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataIn, CDialog)
	//{{AFX_MSG_MAP(CDataIn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataIn ���b�Z�[�W �n���h��

void CDataIn::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData(TRUE);//�ϐ��Ƀ_�C�A���O�l����荞��
	if( !HexCheck(m_Data) )//臒l�̃f�[�^�̃`�F�b�N(HEX�l�łȂ���΂Ȃ�Ȃ�)
	{
		AfxMessageBox("臒l�̒l���s���ł�");
		return;
	}
	sscanf(m_Data,"%x",&DataNum);//m_ThLevel��int�ϐ��Ɏ�荞��
	CDialog::OnOK();
}


//�������HEX(0-9,a-e,A-E)�ȊO�̕������Ȃ����`�F�b�N����֐�
BOOL CDataIn::HexCheck(const char *str)
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
