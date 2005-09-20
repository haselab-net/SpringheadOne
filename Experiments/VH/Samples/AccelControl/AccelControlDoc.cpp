// AccelControlDoc.cpp : CAccelControlDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "AccelControl.h"

#include "AccelControlDoc.h"
#include "AccelControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccelControlDoc

IMPLEMENT_DYNCREATE(CAccelControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CAccelControlDoc, CDocument)
	//{{AFX_MSG_MAP(CAccelControlDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccelControlDoc �N���X�̍\�z/����

CAccelControlDoc::CAccelControlDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CAccelControlDoc::~CAccelControlDoc()
{
}

BOOL CAccelControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAccelControlDoc �V���A���C�[�[�V����

void CAccelControlDoc::Serialize(CArchive& ar)
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
// CAccelControlDoc �N���X�̐f�f

#ifdef _DEBUG
void CAccelControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAccelControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAccelControlDoc �R�}���h

BOOL CAccelControlDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CAccelControlView*)pView)->app->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CAccelControlDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CAccelControlView*)pView)->app->Save(lpszPathName);
	}   
	return true;
}
