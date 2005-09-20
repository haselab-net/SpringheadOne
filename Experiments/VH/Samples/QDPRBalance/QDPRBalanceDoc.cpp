// QDPRBalanceDoc.cpp : CQDPRBalanceDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "QDPRBalance.h"

#include "QDPRBalanceDoc.h"
#include "QDPRBalanceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc

IMPLEMENT_DYNCREATE(CQDPRBalanceDoc, CDocument)

BEGIN_MESSAGE_MAP(CQDPRBalanceDoc, CDocument)
	//{{AFX_MSG_MAP(CQDPRBalanceDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc �N���X�̍\�z/����

CQDPRBalanceDoc::CQDPRBalanceDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CQDPRBalanceDoc::~CQDPRBalanceDoc()
{
}

BOOL CQDPRBalanceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc �V���A���C�[�[�V����

void CQDPRBalanceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc �N���X�̐f�f

#ifdef _DEBUG
void CQDPRBalanceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CQDPRBalanceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc �R�}���h

BOOL CQDPRBalanceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CQDPRBalanceView*)pView)->app.Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CQDPRBalanceDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CQDPRBalanceView*)pView)->app.Save(lpszPathName);
	}   
	return true;
}
