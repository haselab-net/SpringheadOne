// TrgSet.cpp : インプリメンテーション ファイル
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
// CTrgSet ダイアログ


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
// CTrgSet メッセージ ハンドラ

void CTrgSet::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);//変数にダイアログ値を取り込む
	if( ShowFlg )//閾値が入力項目となっている場合
	{
		if( !HexCheck(m_ThLevel) )//閾値のデータのチェック(HEX値でなければならない)
		{
			AfxMessageBox("閾値の値が不正です(1-3FF)");
			return;
		}
		sscanf(m_ThLevel,"%x",&ThLevelNum );//m_ThLevelをint変数に取り込む
		if( ThLevelNum <= 0 || ThLevelNum >= 0x3FF )//範囲チェック
		{
			AfxMessageBox("閾値の値が不正です(1-3FF)");
			return;
		}
	}
	CDialog::OnOK();
}

//文字列にHEX(0-9,a-e,A-E)以外の文字がないかチェックする関数
BOOL CTrgSet::HexCheck(const char *str)
{
	int len;//文字列長

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
			return FALSE;//HEX文字以外の文字があった
		}
	}
	return TRUE;
}

BOOL CTrgSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	if( ShowFlg )//閾値、トリガチャンネル、立ち上がり、立下りの表示、非表示の切替
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
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
