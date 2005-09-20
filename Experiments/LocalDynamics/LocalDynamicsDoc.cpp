// LocalDynamicsDoc.cpp : CLocalDynamicsDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "LocalDynamics.h"

#include "LocalDynamicsDoc.h"
#include "LocalDynamicsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsDoc

IMPLEMENT_DYNCREATE(CLocalDynamicsDoc, CDocument)

BEGIN_MESSAGE_MAP(CLocalDynamicsDoc, CDocument)
	//{{AFX_MSG_MAP(CLocalDynamicsDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsDoc �N���X�̍\�z/����

CLocalDynamicsDoc::CLocalDynamicsDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CLocalDynamicsDoc::~CLocalDynamicsDoc()
{
}

BOOL CLocalDynamicsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsDoc �V���A���C�[�[�V����

void CLocalDynamicsDoc::Serialize(CArchive& ar)
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
// CLocalDynamicsDoc �N���X�̐f�f

#ifdef _DEBUG
void CLocalDynamicsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLocalDynamicsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsDoc �R�}���h

BOOL CLocalDynamicsDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CLocalDynamicsView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CLocalDynamicsDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CLocalDynamicsView*)pView)->Save(lpszPathName);
	}   
	return true;
}
