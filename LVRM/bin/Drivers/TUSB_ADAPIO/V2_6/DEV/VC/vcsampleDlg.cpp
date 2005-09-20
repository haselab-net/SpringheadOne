// vcsampleDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "vcsample.h"
#include "vcsampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "tuadapio.h"//DLL�p�w�b�_�t�@�C���̃C���N���[�h

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
// CVcsampleDlg �_�C�A���O

CVcsampleDlg::CVcsampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVcsampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVcsampleDlg)
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVcsampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVcsampleDlg)
	DDX_Control(pDX, IDC_DEVOPEN, m_DevOpen);
	DDX_Control(pDX, IDC_DEVCLOSE, m_DevClose);
	DDX_Control(pDX, IDC_GETSTATUS, m_GetStatus);
	DDX_Control(pDX, IDC_GETDATAS, m_GetDatas);
	DDX_Control(pDX, IDC_ANALOGTRG, m_AnalogTrg);
	DDX_Control(pDX, IDC_DIGITALTRG, m_DigitalTrg);
	DDX_Control(pDX, IDC_SINGLESAMPLE, m_SingleSample);
	DDX_Control(pDX, IDC_DACOUT2, m_DacOut2);
	DDX_Control(pDX, IDC_DACOUT1, m_DacOut1);
	DDX_Control(pDX, IDC_CLOCKSTOP, m_ClockStop);
	DDX_Control(pDX, IDC_CLOCKOUT, m_ClockOut);
	DDX_Control(pDX, IDC_WORDWRITE, m_WordWrite);
	DDX_Control(pDX, IDC_WORDREAD, m_WordRead);
	DDX_Control(pDX, IDC_LOWWRITE, m_LowWrite);
	DDX_Control(pDX, IDC_HIGHWRITE, m_HighWrite);
	DDX_Control(pDX, IDC_HIGHREAD, m_HighRead);
	DDX_Control(pDX, IDC_LOWREAD, m_LowRead);
	DDX_Control(pDX, IDC_HIGHDIR, m_HighDir);
	DDX_Control(pDX, IDC_LOWDIR, m_LowDir);
	DDX_Control(pDX, IDC_IDNUM, m_IdNum);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVcsampleDlg, CDialog)
	//{{AFX_MSG_MAP(CVcsampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOWDIR, OnLowdir)
	ON_BN_CLICKED(IDC_DEVOPEN, OnDevopen)
	ON_BN_CLICKED(IDC_DEVCLOSE, OnDevclose)
	ON_BN_CLICKED(IDC_HIGHDIR, OnHighdir)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LOWWRITE, OnLowwrite)
	ON_BN_CLICKED(IDC_HIGHWRITE, OnHighwrite)
	ON_BN_CLICKED(IDC_LOWREAD, OnLowread)
	ON_BN_CLICKED(IDC_HIGHREAD, OnHighread)
	ON_BN_CLICKED(IDC_WORDWRITE, OnWordwrite)
	ON_BN_CLICKED(IDC_WORDREAD, OnWordread)
	ON_BN_CLICKED(IDC_CLOCKOUT, OnClockout)
	ON_BN_CLICKED(IDC_CLOCKSTOP, OnClockstop)
	ON_BN_CLICKED(IDC_DACOUT1, OnDacout1)
	ON_BN_CLICKED(IDC_DACOUT2, OnDacout2)
	ON_BN_CLICKED(IDC_SINGLESAMPLE, OnSinglesample)
	ON_BN_CLICKED(IDC_DIGITALTRG, OnDigitaltrg)
	ON_BN_CLICKED(IDC_ANALOGTRG, OnAnalogtrg)
	ON_BN_CLICKED(IDC_GETSTATUS, OnGetstatus)
	ON_BN_CLICKED(IDC_GETDATAS, OnGetdatas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVcsampleDlg ���b�Z�[�W �n���h��

BOOL CVcsampleDlg::OnInitDialog()
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
	m_IdNum.SetCurSel(0);
	Button_Disable();
	SampleDatNum = 0 ;
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CVcsampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVcsampleDlg::OnPaint() 
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
HCURSOR CVcsampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//�A�v���P�[�V�����I�����̏���
void CVcsampleDlg::OnClose() 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	if( m_DevClose.IsWindowEnabled() )
		Adapio_Device_Close(m_IdNum.GetCurSel() );//�f�o�C�X�N���[�Y
	CDialog::OnClose();
}

//***�f�o�C�X�̃I�[�v��***
void CVcsampleDlg::OnDevopen() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( Adapio_Device_Open(m_IdNum.GetCurSel()) == 0 )
	{
		Button_Enable();//�e�@�\�{�^����L���ɂ���
	}
	else
	{
		AfxMessageBox("�I�[�v���ł��܂���ł���");//�I�[�v���Ɏ��s�����ꍇ
	}
}
//***�f�o�C�X�̃N���[�Y***
void CVcsampleDlg::OnDevclose() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	Adapio_Device_Close(m_IdNum.GetCurSel());//�f�o�C�X�N���[�Y
	Button_Disable();//�{�^���̗L�������؂�ւ�
}

//***�I�[�v����ID�I���{�^���݂̂𖳌��ɂ���B***
void CVcsampleDlg::Button_Enable()
{
	m_DevOpen.EnableWindow(FALSE);//�I�[�v���{�^������
	m_DevClose.EnableWindow(TRUE);//�N���[�Y�{�^���L��
	m_IdNum.EnableWindow(FALSE);//ID�I���{�^������
    //�ȉ��̃{�^�����ׂėL��
	m_LowDir.EnableWindow(TRUE);
    m_LowDir.EnableWindow(TRUE);
    m_HighDir.EnableWindow(TRUE);
    m_LowRead.EnableWindow(TRUE);
    m_HighRead.EnableWindow(TRUE);
    m_LowWrite.EnableWindow(TRUE);
    m_HighWrite.EnableWindow(TRUE);
    m_WordRead.EnableWindow(TRUE);
    m_WordWrite.EnableWindow(TRUE);
    m_ClockOut.EnableWindow(TRUE);
    m_ClockStop.EnableWindow(TRUE);
    m_DacOut1.EnableWindow(TRUE);
    m_DacOut2.EnableWindow(TRUE);
    m_SingleSample.EnableWindow(TRUE);
    m_DigitalTrg.EnableWindow(TRUE);
    m_AnalogTrg.EnableWindow(TRUE);
    m_GetStatus.EnableWindow(TRUE);
    m_GetDatas.EnableWindow(TRUE);
}

//***�I�[�v����ID�I���{�^���ȊO�̊e�{�^���@�\�𖳌��ɂ���***
void CVcsampleDlg::Button_Disable()
{
	m_DevOpen.EnableWindow(TRUE);//�I�[�v���{�^���L��
	m_DevClose.EnableWindow(FALSE);//�N���[�Y�{�^������
	m_IdNum.EnableWindow(TRUE);//ID�I���{�^���L��
    //�ȉ��̃{�^�����ׂĖ���
	m_LowDir.EnableWindow(FALSE);
    m_LowDir.EnableWindow(FALSE);
    m_HighDir.EnableWindow(FALSE);
    m_LowRead.EnableWindow(FALSE);
    m_HighRead.EnableWindow(FALSE);
    m_LowWrite.EnableWindow(FALSE);
    m_HighWrite.EnableWindow(FALSE);
    m_WordRead.EnableWindow(FALSE);
    m_WordWrite.EnableWindow(FALSE);
    m_ClockOut.EnableWindow(FALSE);
    m_ClockStop.EnableWindow(FALSE);
    m_DacOut1.EnableWindow(FALSE);
    m_DacOut2.EnableWindow(FALSE);
    m_SingleSample.EnableWindow(FALSE);
    m_DigitalTrg.EnableWindow(FALSE);
    m_AnalogTrg.EnableWindow(FALSE);
    m_GetStatus.EnableWindow(FALSE);
    m_GetDatas.EnableWindow(FALSE);
}
//*****���ʃo�C�g�̓��o�͕����ݒ�*******************
void CVcsampleDlg::OnLowdir() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	DATAIN.m_Data = "0";//�_�C�A���O�̃f�[�^�̏����l��0
	DATAIN.m_Message = "���ʃo�C�g�̓��o�͒l����͂��Ă�������(HEX)";
	if( DATAIN.DoModal() == IDOK )//���[�_���_�C�A���O�\��
	{
		//OK�ŕԂ��ė������̏���
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//�f�[�^�͈̔͂ɓ����Ă��邩
		{
			if( Adapio_Pio_LowDirection(m_IdNum.GetCurSel(),DATAIN.DataNum) != 0 )//DLL�֐��̎��s
			{
				AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
				OnDevclose();//�f�o�C�X�̃N���[�Y
			}
		}
		else
		{
			AfxMessageBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B");
		}
	}
}

//*****��ʃo�C�g�̓��o�͕����ݒ�*******************
void CVcsampleDlg::OnHighdir() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	DATAIN.m_Data = "0";//�_�C�A���O�̃f�[�^�̏����l��0
	DATAIN.m_Message = "��ʃo�C�g�̓��o�͒l����͂��Ă�������(HEX)";
	if( DATAIN.DoModal() == IDOK )//���[�_���_�C�A���O�\��
	{
		//OK�ŕԂ��ė������̏���
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//�f�[�^�͈̔͂ɓ����Ă��邩
		{
			if( Adapio_Pio_HighDirection(m_IdNum.GetCurSel(),DATAIN.DataNum) != 0 )//DLL�֐��̎��s
			{
				AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
				OnDevclose();//�f�o�C�X�̃N���[�Y
			}
		}
		else
		{
			AfxMessageBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B");
		}
	}
}

//***���ʃo�C�g�̏o�̓f�[�^��������**********
void CVcsampleDlg::OnLowwrite() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	DATAIN.m_Data = "0";//�_�C�A���O�̃f�[�^�̏����l��0
	DATAIN.m_Message = "���ʃo�C�g�̏o�͒l����͂��Ă�������(HEX)";
	if( DATAIN.DoModal() == IDOK )//���[�_���_�C�A���O�\��
	{
		//OK�ŕԂ��Ă����Ƃ��̏���
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//�f�[�^�͈̔͂ɂ��邩
		{
			if( Adapio_Pio_LowByteWrite(m_IdNum.GetCurSel(),DATAIN.DataNum ) != 0 )//DLL�֐��̎��s
			{
				AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
				OnDevclose();//�f�o�C�X�̃N���[�Y
			}
		}
	}
}

//***��ʃo�C�g�̏o�̓f�[�^��������**********
void CVcsampleDlg::OnHighwrite() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	DATAIN.m_Data = "0";//�_�C�A���O�̃f�[�^�̏����l��0
	DATAIN.m_Message = "��ʃo�C�g�̏o�͒l����͂��Ă�������(HEX)";
	if( DATAIN.DoModal() == IDOK )//���[�_���_�C�A���O�\��
	{
		//OK�ŕԂ��Ă����Ƃ��̏���
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//�f�[�^�͈̔͂ɂ��邩
		{
			if( Adapio_Pio_HighByteWrite(m_IdNum.GetCurSel(),DATAIN.DataNum ) != 0 )//DLL�֐��̎��s
			{
				AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
				OnDevclose();//�f�o�C�X�̃N���[�Y
			}
		}
	}	
}

//***���ʃo�C�g�̓��̓f�[�^�̎擾*********
void CVcsampleDlg::OnLowread() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned char dat;//�f�[�^��荞�ݗp�o�b�t�@
	CString str;
	if( Adapio_Pio_LowByteRead(m_IdNum.GetCurSel(),&dat) != 0 )//DLL�֐��̎��s
	{
		AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
		OnDevclose();//�f�o�C�X�̃N���[�Y
	}
	else
	{
		str.Format("���ʃo�C�g���͒l�� %x(HEX) �ł�",dat);
		AfxMessageBox(str);
	}
}

//***��ʃo�C�g�̓��̓f�[�^�̎擾*********
void CVcsampleDlg::OnHighread() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned char dat;//�f�[�^��荞�ݗp�o�b�t�@
	CString str;
	if( Adapio_Pio_HighByteRead(m_IdNum.GetCurSel(),&dat) != 0 )//DLL�֐��̎��s
	{
		AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
		OnDevclose();//�f�o�C�X�̃N���[�Y
	}
	else
	{
		str.Format("��ʃo�C�g���͒l�� %x(HEX) �ł�",dat);
		AfxMessageBox(str);
	}
}

//***���[�h�f�[�^��������****************************
void CVcsampleDlg::OnWordwrite() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	DATAIN.m_Data = "0";//�_�C�A���O�f�[�^�̏����l��0
	DATAIN.m_Message = "�o�͒l����͂��Ă�������(HEX)";
	if( DATAIN.DoModal() == IDOK )//���[�_���_�C�A���O�\��
	{
		//OK�ŕԂ��Ă����Ƃ��̏���
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xffff )//�f�[�^�͈̔͂ɂ��邩
		{
			if( Adapio_Pio_WordWrite(m_IdNum.GetCurSel(),DATAIN.DataNum) != 0 )//DLL�֐��̎��s
			{
				AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
				OnDevclose();//�f�o�C�X�̃N���[�Y
			}
		}
		else
		{
			AfxMessageBox("�f�[�^�͈̔͂�0�`FFFF(HEX)�ł��B");
		}
	}
}

//***���̓��[�h�f�[�^�̎擾*******************
void CVcsampleDlg::OnWordread() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int dat;//�f�[�^��荞�ݗp�o�b�t�@
	CString str;
	if( Adapio_Pio_WordRead(m_IdNum.GetCurSel(),&dat) != 0 )//DLL�֐��̎��s
	{
		AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
		OnDevclose();//�f�o�C�X�̃N���[�Y
	}
	else
	{
		str.Format("���[�h���͒l�� %x(HEX) �ł�",dat);
	}
}

//***�N���b�N�̏o��*********************************
void CVcsampleDlg::OnClockout() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	DATAIN.m_Data = "0";//�_�C�A���O�̃f�[�^�̏����l��0
	DATAIN.m_Message = "�N���b�N�̎�������͂��Ă�������(HEX)";
	if( DATAIN.DoModal() == IDOK )//���[�_���_�C�A���O�\��
	{
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xffff )//�f�[�^�͈̔͂ɂ��邩
		{
			if( Adapio_Pio_Clock_Out(m_IdNum.GetCurSel(),DATAIN.DataNum ) != 0 )//DLL�֐��̎��s
			{
				AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
				OnDevclose();//�f�o�C�X�̃N���[�Y
			}
		}
		else
		{
			AfxMessageBox("�f�[�^�͈̔͂�0�`FFFF(HEX)�ł��B");
		}
	}
}

//***�N���b�N�o�͂̒�~*******************************
void CVcsampleDlg::OnClockstop() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( Adapio_Pio_Clock_Stop(m_IdNum.GetCurSel() ) != 0 )//DLL�֐��̎��s
	{
		AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
		OnDevclose();//�f�o�C�X�̃N���[�Y
	}
}

//***DAC 1ch �̏o�̓f�[�^�̐ݒ�***************************
void CVcsampleDlg::OnDacout1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	DATAIN.m_Data = "0";//�_�C�A���O�̃f�[�^�̏����l��0
	DATAIN.m_Message = "DAC(ch1)�ւ̏o�͒l����͂��Ă�������(HEX)";
	if( DATAIN.DoModal() == IDOK )//���[�_���_�C�A���O�\��
	{
		//OK�ŕԂ��Ă����Ƃ��̏���
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//�f�[�^�͈̔͂ɂ��邩
			{
			if( Adapio_Dac_Out(m_IdNum.GetCurSel(),0,DATAIN.DataNum) != 0 )//DLL�֐��̎��s
			{
				AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
				OnDevclose();//�f�o�C�X�̃N���[�Y
			}
		}
		else
		{
			AfxMessageBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B");
		}
	}
}

//***DAC 2ch �̏o�̓f�[�^�̐ݒ�***************************
void CVcsampleDlg::OnDacout2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	DATAIN.m_Data = "0";//�_�C�A���O�̃f�[�^�̏����l��0
	DATAIN.m_Message = "DAC(ch2)�ւ̏o�͒l����͂��Ă�������(HEX)";
	if( DATAIN.DoModal() == IDOK )//���[�_���_�C�A���O�\��
	{
		//OK�ŕԂ��Ă����Ƃ��̏���
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//�f�[�^�͈̔͂ɂ��邩
			{
			if( Adapio_Dac_Out(m_IdNum.GetCurSel(),1,DATAIN.DataNum) != 0 )//DLL�֐��̎��s
			{
				AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
				OnDevclose();//�f�o�C�X�̃N���[�Y
			}
		}
		else
		{
			AfxMessageBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B");
		}
	}
}

//***��ʃo�C�g�̓��̓f�[�^�̎擾************************
void CVcsampleDlg::OnSinglesample() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	short dat[6];//�f�[�^�z��
	int ch;//�`�����l��
	CString str,tmp;
	for(ch = 0 ; ch < 6 ; ch++ )
	{
		if( Adapio_Adc_SingleSample(m_IdNum.GetCurSel(),ch,&dat[ch]) )
		{
			AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
			OnDevclose();//�f�o�C�X�̃N���[�Y
			return;
		}
	}
	tmp.Format("ch1�̃f�[�^ %d \r\n",dat[0]);str = tmp;
	tmp.Format("ch2�̃f�[�^ %d \r\n",dat[1]);str += tmp;
	tmp.Format("ch3�̃f�[�^ %d \r\n",dat[2]);str += tmp;
	tmp.Format("ch4�̃f�[�^ %d \r\n",dat[3]);str += tmp;
	tmp.Format("ch5�̃f�[�^ %d \r\n",dat[4]);str += tmp;
	tmp.Format("ch6�̃f�[�^ %d \r\n",dat[5]);str += tmp;
	AfxMessageBox(str);
}

//***�f�B�W�^���g���K����̐ݒ�ƊJ�n*******************************
void CVcsampleDlg::OnDigitaltrg() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	TRGSET.m_Message = "�g���K������ݒ肵�Ă�������";
	//�_�C�A���O�ɕ\�����鏉���l��ݒ肷��
	TRGSET.m_EndCh = 0;
	TRGSET.m_BufSize = 1;
	TRGSET.ShowFlg = FALSE;//���͂Ɏg�p���Ȃ����ڂ͔�\���ɂ���
	if( TRGSET.DoModal() == IDOK )//���͗p�_�C�A���O�̌Ăяo��
	{
		//OK�{�^���������ꂽ�Ƃ��̏���
		if( Adapio_Adc_DigitalTrg(m_IdNum.GetCurSel(),TRGSET.m_EndCh,TRGSET.m_BufSize) != 0 )//DLL�֐��̎��s
		{
			AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
			OnDevclose();//�f�o�C�X�̃N���[�Y
		}
		else
		{
			SampleDatNum = TRGSET.m_BufSize;
		}
	}
}

//***�A�i���O�g���K����̐ݒ�ƊJ�n*********************
void CVcsampleDlg::OnAnalogtrg() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	TRGSET.m_Message = "�g���K������ݒ肵�Ă�������";
	//�_�C�A���O�ɕ\�����鏉���l��ݒ肷��
	TRGSET.m_EndCh = 0;
	TRGSET.m_BufSize = 1;
	TRGSET.m_ThLevel = "1";
	TRGSET.m_TrgCh = 0;
	TRGSET.m_UpDown = 0;
	TRGSET.ShowFlg = TRUE;//���ׂĂ̍��ڂ�\������
	if( TRGSET.DoModal() == IDOK )//���͗p�_�C�A���O�̌Ăяo��
	{
		//OK�{�^���������ꂽ�ꍇ�̏���
		if( Adapio_Adc_AnalogTrg(m_IdNum.GetCurSel(),TRGSET.m_EndCh,
			TRGSET.m_BufSize,TRGSET.ThLevelNum,TRGSET.m_TrgCh,
			TRGSET.m_UpDown) != 0 )//DLL�֐��̎��s
		{
			AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
			OnDevclose();//�f�o�C�X�̃N���[�Y
		}
		else
			SampleDatNum = TRGSET.m_BufSize;
	}
}

//***AD��荞�݃X�e�[�^�X�̓ǂݍ���******************************
void CVcsampleDlg::OnGetstatus() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	short Running;
	short SampleNum;
	CString str;

	if( Adapio_Adc_GetStatus(m_IdNum.GetCurSel(),&Running,&SampleNum) != 0 )//DLL�֐��̎��s
	{
		AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
		OnDevclose();//�f�o�C�X�̃N���[�Y
	}
	else
	{
		if( Running == 1 )
		{
			str.Format("���쒆   ��荞�ݍς݃f�[�^��(%d)",SampleNum);
		}
		else
		{
			str.Format("��荞�݊���   ��荞�ݍς݃f�[�^��(%d)",SampleNum);
		}
		AfxMessageBox(str);
	}
}

//***�T���v�����O�ς݃f�[�^�̓ǂݍ���*******************************
void CVcsampleDlg::OnGetdatas() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	short dat[512];
	CString str;

	if( Adapio_Adc_GetDatas(m_IdNum.GetCurSel(),dat,SampleDatNum) != 0 )//DLL�֐��̎��s
	{
		AfxMessageBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B");//�������݂Ɏ��s
		OnDevclose();//�f�o�C�X�̃N���[�Y
	}
	else
	{
		CFile SaveFile;
		if( SaveFile.Open("Sample.dat",CFile::modeCreate | CFile::modeWrite,NULL) )//�ۑ��t�@�C���̃I�[�v��
		{
			for(int i = 0 ; i < SampleDatNum ; i++ )
			{
				str.Format("%d,\r\n",dat[i]);
				SaveFile.Write((const char *) str,str.GetLength());
			}
			SaveFile.Close();
			AfxMessageBox("��荞�݃f�[�^��sample.dat�ɕۑ�����܂���");
		}
		else
		{
			AfxMessageBox("�t�@�C���̃I�[�v���Ɏ��s���܂���");
		}
	}
}
