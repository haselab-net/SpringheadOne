// SLCSampleDlg.cpp : インプリメンテーション ファイル
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

UINT WM_SLC_ON_DATAGET = ::RegisterWindowMessage("WM_SLC_ON_DATAGET");//▼SLC API ﾒｯｾｰｼﾞID
UINT WM_SLC_ON_DATAGET_DEB = ::RegisterWindowMessage("WM_SLC_ON_DATAGET_DEB");//▼SLC API ﾒｯｾｰｼﾞID

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
// CSLCSampleDlg ダイアログ

CSLCSampleDlg::CSLCSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSLCSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSLCSampleDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hSLC_device = NULL;
}

void CSLCSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSLCSampleDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
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
	ON_REGISTERED_MESSAGE(WM_SLC_ON_DATAGET,OnSlcDataGet)	//SLCｲﾍﾞﾝﾄﾊﾝﾄﾞﾗ
	ON_REGISTERED_MESSAGE(WM_SLC_ON_DATAGET_DEB,OnSlcDataGetDeb)	//SLCｲﾍﾞﾝﾄﾊﾝﾄﾞﾗ
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSLCSampleDlg メッセージ ハンドラ

BOOL CSLCSampleDlg::OnInitDialog()
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
	{	//▼SLC API ハンドルオープン
		if((m_hSLC_device = OpenSLC("SLC-C01-00006",m_hWnd,WM_SLC_ON_DATAGET))==NULL)
		{CString str;
			str.Format("SLCハンドルのオープンに失敗しました！\r\n（ErrorID:%d）",
				GetSLCErrorCode());
			AfxMessageBox(str);
			return FALSE;
		}
		//▼SLC API デバッグ情報送信設定
		SetSLCDebugItem(m_hSLC_device,m_hWnd,WM_SLC_ON_DATAGET_DEB);
	}
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
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

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CSLCSampleDlg::OnPaint() 
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
HCURSOR CSLCSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//========================================================
//■概要
//	SLC APIからのメッセージを受け取るハンドラ関数
//□機能説明
//	SLC APIがカメラから受け取った情報から三次元座標を取得した
//	段階でこのハンドラが呼ばれる。
//	三次元座標値（その他）群とその要素数を受け取る。
//□パラメタ説明
//	WPARAM wParam	SLCOBJECTへのポインタ
//	LPARAM lParam	SLCOBJECTの要素数
//□戻り値
//	TRUEのみ
//□備考
//	
//========================================================
BOOL CSLCSampleDlg::OnSlcDataGet(WPARAM wParam,LPARAM lParam)
{
	//▼SLC API ドライバーから情報を取得するイベントハンドラ
int nSlcObjSize = (int)lParam;
SLCOBJECT sSlcObj[SLCOBJECT_MARKERMAX];
	memcpy(sSlcObj,(SLCOBJECT*)wParam,sizeof(SLCOBJECT)*nSlcObjSize);

	//マーカ個数を表示
	{CString str;
		str.Format("%d",nSlcObjSize);
	CWnd* lpWnd = GetDlgItem(IDC_STATIC_MKSIZE);
		lpWnd->SetWindowText(str);
	}

	//マーカ座標値を表示
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
//■概要
//	SLC APIからのメッセージを受け取るハンドラ関数
//□機能説明
//	SLC APIがカメラから受け取った段階でこのハンドラが呼ばれる。
//	デバッグ用（２次元情報等）
//□パラメタ説明
//	WPARAM wParam	SLCDEV_2DIMOBJへのポインタ（NULLならエラー発生）
//	LPARAM lParam	USBエラーのコード
//□戻り値
//	TRUEのみ
//□備考
//	
//========================================================
BOOL CSLCSampleDlg::OnSlcDataGetDeb(WPARAM wParam,LPARAM lParam)
{
	if(!wParam)
		return FALSE;

SLCDEV_2DIMOBJ* obj = (SLCDEV_2DIMOBJ*)wParam;

	{	//左カメラ
	CString out_str="",str;
		out_str.Format("%d\r\n",obj->_nLeftSize);	//個数
		
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
	{	//右カメラ
	CString out_str="",str;
		out_str.Format("%d\r\n",obj->_nRightSize);	//個数

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
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	//▼SLC API ハンドルクローズ
	if(m_hSLC_device)
		CloseSLC(m_hSLC_device);
}	

void CSLCSampleDlg::OnButtonCamcfg() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//▼SLC API  カメラサブ設定ダイアログ
	DlgSLCCamSubCfgLabeling(m_hSLC_device);
}

void CSLCSampleDlg::OnButtonVidcfg() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//▼SLC API  ビデオ出力設定ダイアログ
	DlgSLCCamSubCfgVideoOut(m_hSLC_device);
}
