// vcsampleDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "vcsample.h"
#include "vcsampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "tuadapio.h"//DLL用ヘッダファイルのインクルード

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
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
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVcsampleDlg ダイアログ

CVcsampleDlg::CVcsampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVcsampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVcsampleDlg)
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
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
// CVcsampleDlg メッセージ ハンドラ

BOOL CVcsampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
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

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	m_IdNum.SetCurSel(0);
	Button_Disable();
	SampleDatNum = 0 ;
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
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

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CVcsampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CVcsampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//アプリケーション終了時の処理
void CVcsampleDlg::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if( m_DevClose.IsWindowEnabled() )
		Adapio_Device_Close(m_IdNum.GetCurSel() );//デバイスクローズ
	CDialog::OnClose();
}

//***デバイスのオープン***
void CVcsampleDlg::OnDevopen() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( Adapio_Device_Open(m_IdNum.GetCurSel()) == 0 )
	{
		Button_Enable();//各機能ボタンを有効にする
	}
	else
	{
		AfxMessageBox("オープンできませんでした");//オープンに失敗した場合
	}
}
//***デバイスのクローズ***
void CVcsampleDlg::OnDevclose() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	Adapio_Device_Close(m_IdNum.GetCurSel());//デバイスクローズ
	Button_Disable();//ボタンの有効無効切り替え
}

//***オープンとID選択ボタンのみを無効にする。***
void CVcsampleDlg::Button_Enable()
{
	m_DevOpen.EnableWindow(FALSE);//オープンボタン無効
	m_DevClose.EnableWindow(TRUE);//クローズボタン有効
	m_IdNum.EnableWindow(FALSE);//ID選択ボタン無効
    //以下のボタンすべて有効
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

//***オープンとID選択ボタン以外の各ボタン機能を無効にする***
void CVcsampleDlg::Button_Disable()
{
	m_DevOpen.EnableWindow(TRUE);//オープンボタン有効
	m_DevClose.EnableWindow(FALSE);//クローズボタン無効
	m_IdNum.EnableWindow(TRUE);//ID選択ボタン有効
    //以下のボタンすべて無効
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
//*****下位バイトの入出力方向設定*******************
void CVcsampleDlg::OnLowdir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	DATAIN.m_Data = "0";//ダイアログのデータの初期値を0
	DATAIN.m_Message = "下位バイトの入出力値を入力してください(HEX)";
	if( DATAIN.DoModal() == IDOK )//モーダルダイアログ表示
	{
		//OKで返って来た時の処理
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//データの範囲に入っているか
		{
			if( Adapio_Pio_LowDirection(m_IdNum.GetCurSel(),DATAIN.DataNum) != 0 )//DLL関数の実行
			{
				AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
				OnDevclose();//デバイスのクローズ
			}
		}
		else
		{
			AfxMessageBox("データの範囲は0～FF(HEX)です。");
		}
	}
}

//*****上位バイトの入出力方向設定*******************
void CVcsampleDlg::OnHighdir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	DATAIN.m_Data = "0";//ダイアログのデータの初期値を0
	DATAIN.m_Message = "上位バイトの入出力値を入力してください(HEX)";
	if( DATAIN.DoModal() == IDOK )//モーダルダイアログ表示
	{
		//OKで返って来た時の処理
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//データの範囲に入っているか
		{
			if( Adapio_Pio_HighDirection(m_IdNum.GetCurSel(),DATAIN.DataNum) != 0 )//DLL関数の実行
			{
				AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
				OnDevclose();//デバイスのクローズ
			}
		}
		else
		{
			AfxMessageBox("データの範囲は0～FF(HEX)です。");
		}
	}
}

//***下位バイトの出力データ書きこみ**********
void CVcsampleDlg::OnLowwrite() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	DATAIN.m_Data = "0";//ダイアログのデータの初期値を0
	DATAIN.m_Message = "下位バイトの出力値を入力してください(HEX)";
	if( DATAIN.DoModal() == IDOK )//モーダルダイアログ表示
	{
		//OKで返ってきたときの処理
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//データの範囲にあるか
		{
			if( Adapio_Pio_LowByteWrite(m_IdNum.GetCurSel(),DATAIN.DataNum ) != 0 )//DLL関数の実行
			{
				AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
				OnDevclose();//デバイスのクローズ
			}
		}
	}
}

//***上位バイトの出力データ書きこみ**********
void CVcsampleDlg::OnHighwrite() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	DATAIN.m_Data = "0";//ダイアログのデータの初期値を0
	DATAIN.m_Message = "上位バイトの出力値を入力してください(HEX)";
	if( DATAIN.DoModal() == IDOK )//モーダルダイアログ表示
	{
		//OKで返ってきたときの処理
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//データの範囲にあるか
		{
			if( Adapio_Pio_HighByteWrite(m_IdNum.GetCurSel(),DATAIN.DataNum ) != 0 )//DLL関数の実行
			{
				AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
				OnDevclose();//デバイスのクローズ
			}
		}
	}	
}

//***下位バイトの入力データの取得*********
void CVcsampleDlg::OnLowread() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned char dat;//データ取り込み用バッファ
	CString str;
	if( Adapio_Pio_LowByteRead(m_IdNum.GetCurSel(),&dat) != 0 )//DLL関数の実行
	{
		AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
		OnDevclose();//デバイスのクローズ
	}
	else
	{
		str.Format("下位バイト入力値は %x(HEX) です",dat);
		AfxMessageBox(str);
	}
}

//***上位バイトの入力データの取得*********
void CVcsampleDlg::OnHighread() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned char dat;//データ取り込み用バッファ
	CString str;
	if( Adapio_Pio_HighByteRead(m_IdNum.GetCurSel(),&dat) != 0 )//DLL関数の実行
	{
		AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
		OnDevclose();//デバイスのクローズ
	}
	else
	{
		str.Format("上位バイト入力値は %x(HEX) です",dat);
		AfxMessageBox(str);
	}
}

//***ワードデータ書きこみ****************************
void CVcsampleDlg::OnWordwrite() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	DATAIN.m_Data = "0";//ダイアログデータの初期値を0
	DATAIN.m_Message = "出力値を入力してください(HEX)";
	if( DATAIN.DoModal() == IDOK )//モーダルダイアログ表示
	{
		//OKで返ってきたときの処理
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xffff )//データの範囲にあるか
		{
			if( Adapio_Pio_WordWrite(m_IdNum.GetCurSel(),DATAIN.DataNum) != 0 )//DLL関数の実行
			{
				AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
				OnDevclose();//デバイスのクローズ
			}
		}
		else
		{
			AfxMessageBox("データの範囲は0～FFFF(HEX)です。");
		}
	}
}

//***入力ワードデータの取得*******************
void CVcsampleDlg::OnWordread() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int dat;//データ取り込み用バッファ
	CString str;
	if( Adapio_Pio_WordRead(m_IdNum.GetCurSel(),&dat) != 0 )//DLL関数の実行
	{
		AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
		OnDevclose();//デバイスのクローズ
	}
	else
	{
		str.Format("ワード入力値は %x(HEX) です",dat);
	}
}

//***クロックの出力*********************************
void CVcsampleDlg::OnClockout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	DATAIN.m_Data = "0";//ダイアログのデータの初期値を0
	DATAIN.m_Message = "クロックの周期を入力してください(HEX)";
	if( DATAIN.DoModal() == IDOK )//モーダルダイアログ表示
	{
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xffff )//データの範囲にあるか
		{
			if( Adapio_Pio_Clock_Out(m_IdNum.GetCurSel(),DATAIN.DataNum ) != 0 )//DLL関数の実行
			{
				AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
				OnDevclose();//デバイスのクローズ
			}
		}
		else
		{
			AfxMessageBox("データの範囲は0～FFFF(HEX)です。");
		}
	}
}

//***クロック出力の停止*******************************
void CVcsampleDlg::OnClockstop() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( Adapio_Pio_Clock_Stop(m_IdNum.GetCurSel() ) != 0 )//DLL関数の実行
	{
		AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
		OnDevclose();//デバイスのクローズ
	}
}

//***DAC 1ch の出力データの設定***************************
void CVcsampleDlg::OnDacout1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	DATAIN.m_Data = "0";//ダイアログのデータの初期値を0
	DATAIN.m_Message = "DAC(ch1)への出力値を入力してください(HEX)";
	if( DATAIN.DoModal() == IDOK )//モーダルダイアログ表示
	{
		//OKで返ってきたときの処理
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//データの範囲にあるか
			{
			if( Adapio_Dac_Out(m_IdNum.GetCurSel(),0,DATAIN.DataNum) != 0 )//DLL関数の実行
			{
				AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
				OnDevclose();//デバイスのクローズ
			}
		}
		else
		{
			AfxMessageBox("データの範囲は0～FF(HEX)です。");
		}
	}
}

//***DAC 2ch の出力データの設定***************************
void CVcsampleDlg::OnDacout2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	DATAIN.m_Data = "0";//ダイアログのデータの初期値を0
	DATAIN.m_Message = "DAC(ch2)への出力値を入力してください(HEX)";
	if( DATAIN.DoModal() == IDOK )//モーダルダイアログ表示
	{
		//OKで返ってきたときの処理
		if( DATAIN.DataNum >= 0 && DATAIN.DataNum <= 0xff )//データの範囲にあるか
			{
			if( Adapio_Dac_Out(m_IdNum.GetCurSel(),1,DATAIN.DataNum) != 0 )//DLL関数の実行
			{
				AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
				OnDevclose();//デバイスのクローズ
			}
		}
		else
		{
			AfxMessageBox("データの範囲は0～FF(HEX)です。");
		}
	}
}

//***上位バイトの入力データの取得************************
void CVcsampleDlg::OnSinglesample() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	short dat[6];//データ配列
	int ch;//チャンネル
	CString str,tmp;
	for(ch = 0 ; ch < 6 ; ch++ )
	{
		if( Adapio_Adc_SingleSample(m_IdNum.GetCurSel(),ch,&dat[ch]) )
		{
			AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
			OnDevclose();//デバイスのクローズ
			return;
		}
	}
	tmp.Format("ch1のデータ %d \r\n",dat[0]);str = tmp;
	tmp.Format("ch2のデータ %d \r\n",dat[1]);str += tmp;
	tmp.Format("ch3のデータ %d \r\n",dat[2]);str += tmp;
	tmp.Format("ch4のデータ %d \r\n",dat[3]);str += tmp;
	tmp.Format("ch5のデータ %d \r\n",dat[4]);str += tmp;
	tmp.Format("ch6のデータ %d \r\n",dat[5]);str += tmp;
	AfxMessageBox(str);
}

//***ディジタルトリガ動作の設定と開始*******************************
void CVcsampleDlg::OnDigitaltrg() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	TRGSET.m_Message = "トリガ条件を設定してください";
	//ダイアログに表示する初期値を設定する
	TRGSET.m_EndCh = 0;
	TRGSET.m_BufSize = 1;
	TRGSET.ShowFlg = FALSE;//入力に使用しない項目は非表示にする
	if( TRGSET.DoModal() == IDOK )//入力用ダイアログの呼び出し
	{
		//OKボタンが押されたときの処理
		if( Adapio_Adc_DigitalTrg(m_IdNum.GetCurSel(),TRGSET.m_EndCh,TRGSET.m_BufSize) != 0 )//DLL関数の実行
		{
			AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
			OnDevclose();//デバイスのクローズ
		}
		else
		{
			SampleDatNum = TRGSET.m_BufSize;
		}
	}
}

//***アナログトリガ動作の設定と開始*********************
void CVcsampleDlg::OnAnalogtrg() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	TRGSET.m_Message = "トリガ条件を設定してください";
	//ダイアログに表示する初期値を設定する
	TRGSET.m_EndCh = 0;
	TRGSET.m_BufSize = 1;
	TRGSET.m_ThLevel = "1";
	TRGSET.m_TrgCh = 0;
	TRGSET.m_UpDown = 0;
	TRGSET.ShowFlg = TRUE;//すべての項目を表示する
	if( TRGSET.DoModal() == IDOK )//入力用ダイアログの呼び出し
	{
		//OKボタンが押された場合の処理
		if( Adapio_Adc_AnalogTrg(m_IdNum.GetCurSel(),TRGSET.m_EndCh,
			TRGSET.m_BufSize,TRGSET.ThLevelNum,TRGSET.m_TrgCh,
			TRGSET.m_UpDown) != 0 )//DLL関数の実行
		{
			AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
			OnDevclose();//デバイスのクローズ
		}
		else
			SampleDatNum = TRGSET.m_BufSize;
	}
}

//***AD取り込みステータスの読み込み******************************
void CVcsampleDlg::OnGetstatus() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	short Running;
	short SampleNum;
	CString str;

	if( Adapio_Adc_GetStatus(m_IdNum.GetCurSel(),&Running,&SampleNum) != 0 )//DLL関数の実行
	{
		AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
		OnDevclose();//デバイスのクローズ
	}
	else
	{
		if( Running == 1 )
		{
			str.Format("動作中   取り込み済みデータ数(%d)",SampleNum);
		}
		else
		{
			str.Format("取り込み完了   取り込み済みデータ数(%d)",SampleNum);
		}
		AfxMessageBox(str);
	}
}

//***サンプリング済みデータの読み込み*******************************
void CVcsampleDlg::OnGetdatas() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	short dat[512];
	CString str;

	if( Adapio_Adc_GetDatas(m_IdNum.GetCurSel(),dat,SampleDatNum) != 0 )//DLL関数の実行
	{
		AfxMessageBox("DLL関数の実行に失敗しました。デバイスをクローズします。");//書きこみに失敗
		OnDevclose();//デバイスのクローズ
	}
	else
	{
		CFile SaveFile;
		if( SaveFile.Open("Sample.dat",CFile::modeCreate | CFile::modeWrite,NULL) )//保存ファイルのオープン
		{
			for(int i = 0 ; i < SampleDatNum ; i++ )
			{
				str.Format("%d,\r\n",dat[i]);
				SaveFile.Write((const char *) str,str.GetLength());
			}
			SaveFile.Close();
			AfxMessageBox("取り込みデータはsample.datに保存されました");
		}
		else
		{
			AfxMessageBox("ファイルのオープンに失敗しました");
		}
	}
}
