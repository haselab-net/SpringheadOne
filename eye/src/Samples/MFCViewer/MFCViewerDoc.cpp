// MFCViewerDoc.cpp : CMFCViewerDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "MFCViewer.h"

#include "MFCViewerDoc.h"
#include "MFCViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerDoc

IMPLEMENT_DYNCREATE(CMFCViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CMFCViewerDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerDoc �N���X�̍\�z/����

CMFCViewerDoc::CMFCViewerDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CMFCViewerDoc::~CMFCViewerDoc()
{
}

BOOL CMFCViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMFCViewerDoc �V���A���C�[�[�V����

void CMFCViewerDoc::Serialize(CArchive& ar)
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
// CMFCViewerDoc �N���X�̐f�f

#ifdef _DEBUG
void CMFCViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerDoc �R�}���h

BOOL CMFCViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CMFCViewerView*)pView)->app.Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CMFCViewerDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CMFCViewerView*)pView)->app.Save(lpszPathName);
	}   
	return true;
}
