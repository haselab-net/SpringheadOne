// DynaLocalHapDoc.cpp : CDynaLocalHapDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "DynaLocalHap.h"

#include "DynaLocalHapDoc.h"
#include "DynaLocalHapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapDoc

IMPLEMENT_DYNCREATE(CDynaLocalHapDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynaLocalHapDoc, CDocument)
	//{{AFX_MSG_MAP(CDynaLocalHapDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapDoc �N���X�̍\�z/����

CDynaLocalHapDoc::CDynaLocalHapDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CDynaLocalHapDoc::~CDynaLocalHapDoc()
{
}

BOOL CDynaLocalHapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapDoc �V���A���C�[�[�V����

void CDynaLocalHapDoc::Serialize(CArchive& ar)
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
// CDynaLocalHapDoc �N���X�̐f�f

#ifdef _DEBUG
void CDynaLocalHapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynaLocalHapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapDoc �R�}���h

BOOL CDynaLocalHapDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaLocalHapView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CDynaLocalHapDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaLocalHapView*)pView)->Save(lpszPathName);
	}   
	return true;
}
