// DynaClockTestDoc.cpp : CDynaClockTestDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "DynaClockTest.h"

#include "DynaClockTestDoc.h"
#include "DynaClockTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestDoc

IMPLEMENT_DYNCREATE(CDynaClockTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynaClockTestDoc, CDocument)
	//{{AFX_MSG_MAP(CDynaClockTestDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestDoc �N���X�̍\�z/����

CDynaClockTestDoc::CDynaClockTestDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CDynaClockTestDoc::~CDynaClockTestDoc()
{
}

BOOL CDynaClockTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestDoc �V���A���C�[�[�V����

void CDynaClockTestDoc::Serialize(CArchive& ar)
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
// CDynaClockTestDoc �N���X�̐f�f

#ifdef _DEBUG
void CDynaClockTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynaClockTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestDoc �R�}���h

BOOL CDynaClockTestDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaClockTestView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CDynaClockTestDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaClockTestView*)pView)->Save(lpszPathName);
	}   
	return true;
}
