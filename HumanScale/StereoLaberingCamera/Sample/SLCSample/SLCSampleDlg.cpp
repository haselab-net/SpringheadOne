// SLCSampleDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "SLCSample.h"
#include "SLCSampleDlg.h"
#include "CySLC.h"
#include "SLCdevelop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT WM_SLC_ON_DATAGET = ::RegisterWindowMessage("WM_SLC_ON_DATAGET");//��SLC API ү����ID
UINT WM_SLC_ON_DATAGET_DEB = ::RegisterWindowMessage("WM_SLC_ON_DATAGET_DEB");//��SLC API ү����ID

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSLCSampleDlg �_�C�A���O

CSLCSampleDlg::CSLCSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSLCSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSLCSampleDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hSLC_device = NULL;
}

void CSLCSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSLCSampleDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSLCSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CSLCSampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CAMCFG, OnButtonCamcfg)
	ON_BN_CLICKED(IDC_BUTTON_VIDCFG, OnButtonVidcfg)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_SLC_ON_DATAGET,OnSlcDataGet)	//SLC����������
	ON_REGISTERED_MESSAGE(WM_SLC_ON_DATAGET_DEB,OnSlcDataGetDeb)	//SLC����������
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSLCSampleDlg ���b�Z�[�W �n���h��

BOOL CSLCSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	{	//��SLC API �n���h���I�[�v��
		if((m_hSLC_device = OpenSLC("SLC-C01-00006",m_hWnd,WM_SLC_ON_DATAGET))==NULL)
		{CString str;
			str.Format("SLC�n���h���̃I�[�v���Ɏ��s���܂����I\r\n�iErrorID:%d�j",
				GetSLCErrorCode());
			AfxMessageBox(str);
			return FALSE;
		}
		//��SLC API �f�o�b�O��񑗐M�ݒ�
		SetSLCDebugItem(m_hSLC_device,m_hWnd,WM_SLC_ON_DATAGET_DEB);
	}
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CSLCSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CSLCSampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CSLCSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//========================================================
//���T�v
//	SLC API����̃��b�Z�[�W���󂯎��n���h���֐�
//���@�\����
//	SLC API���J��������󂯎������񂩂�O�������W���擾����
//	�i�K�ł��̃n���h�����Ă΂��B
//	�O�������W�l�i���̑��j�Q�Ƃ��̗v�f�����󂯎��B
//���p�����^����
//	WPARAM wParam	SLCOBJECT�ւ̃|�C���^
//	LPARAM lParam	SLCOBJECT�̗v�f��
//���߂�l
//	TRUE�̂�
//�����l
//	
//========================================================
BOOL CSLCSampleDlg::OnSlcDataGet(WPARAM wParam,LPARAM lParam)
{
	//��SLC API �h���C�o�[��������擾����C�x���g�n���h��
int nSlcObjSize = (int)lParam;
SLCOBJECT sSlcObj[SLCOBJECT_MARKERMAX];
	memcpy(sSlcObj,(SLCOBJECT*)wParam,sizeof(SLCOBJECT)*nSlcObjSize);

	//�}�[�J����\��
	{CString str;
		str.Format("%d",nSlcObjSize);
	CWnd* lpWnd = GetDlgItem(IDC_STATIC_MKSIZE);
		lpWnd->SetWindowText(str);
	}

	//�}�[�J���W�l��\��
	{CString strLines="";
		for(int nMk=0;nMk<nSlcObjSize;nMk++)
		{CString str;
			str.Format("%2d  %7.2f %7.2f %7.2f\r\n",
				nMk,
				sSlcObj[nMk].dX,
				sSlcObj[nMk].dY,
				sSlcObj[nMk].dZ
				);
			strLines = strLines + str;
		}
	CWnd* lpWnd = GetDlgItem(IDC_STATIC_XYZ);
		lpWnd->SetWindowText(strLines);
	}
	return TRUE;
}

//========================================================
//���T�v
//	SLC API����̃��b�Z�[�W���󂯎��n���h���֐�
//���@�\����
//	SLC API���J��������󂯎�����i�K�ł��̃n���h�����Ă΂��B
//	�f�o�b�O�p�i�Q������񓙁j
//���p�����^����
//	WPARAM wParam	SLCDEV_2DIMOBJ�ւ̃|�C���^�iNULL�Ȃ�G���[�����j
//	LPARAM lParam	USB�G���[�̃R�[�h
//���߂�l
//	TRUE�̂�
//�����l
//	
//========================================================
BOOL CSLCSampleDlg::OnSlcDataGetDeb(WPARAM wParam,LPARAM lParam)
{
	if(!wParam)
		return FALSE;

SLCDEV_2DIMOBJ* obj = (SLCDEV_2DIMOBJ*)wParam;

	{	//���J����
	CString out_str="",str;
		out_str.Format("%d\r\n",obj->_nLeftSize);	//��
		
		for(int i=0;i<obj->_nLeftSize;i++)
		{	//str.Format("%d : %6.2f %6.2f  %d\r\n",
			str.Format("%d\t%6.2f\t%6.2f\t%d\r\n",
				i,obj->_sLeft[i]._dX,obj->_sLeft[i]._dY,obj->_sLeft[i]._nAreaSize);
			out_str = out_str+str;
		}
	CWnd* lpWnd = GetDlgItem(IDC_STATIC_XY_LCAM);
		if(lpWnd)
			lpWnd->SetWindowText(out_str);
	}
	{	//�E�J����
	CString out_str="",str;
		out_str.Format("%d\r\n",obj->_nRightSize);	//��

		for(int i=0;i<obj->_nRightSize;i++)
		{	str.Format("%d\t%6.2f\t%6.2f\t%d\r\n",
				i,obj->_sRight[i]._dX,obj->_sRight[i]._dY,obj->_sRight[i]._nAreaSize);
			out_str = out_str+str;
		}
	CWnd* lpWnd = GetDlgItem(IDC_STATIC_XY_RCAM);
		if(lpWnd)
			lpWnd->SetWindowText(out_str);
	}

	return TRUE;
}

void CSLCSampleDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	//��SLC API �n���h���N���[�Y
	if(m_hSLC_device)
		CloseSLC(m_hSLC_device);
}	

void CSLCSampleDlg::OnButtonCamcfg() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	//��SLC API  �J�����T�u�ݒ�_�C�A���O
	DlgSLCCamSubCfgLabeling(m_hSLC_device);
}

void CSLCSampleDlg::OnButtonVidcfg() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	//��SLC API  �r�f�I�o�͐ݒ�_�C�A���O
	DlgSLCCamSubCfgVideoOut(m_hSLC_device);
}
