// VHIndependenceDoc.cpp : CVHIndependenceDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "VHIndependence.h"

#include "VHIndependenceDoc.h"
#include "VHIndependenceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVHIndependenceDoc

IMPLEMENT_DYNCREATE(CVHIndependenceDoc, CDocument)

BEGIN_MESSAGE_MAP(CVHIndependenceDoc, CDocument)
	//{{AFX_MSG_MAP(CVHIndependenceDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVHIndependenceDoc �N���X�̍\�z/����

CVHIndependenceDoc::CVHIndependenceDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CVHIndependenceDoc::~CVHIndependenceDoc()
{
}

BOOL CVHIndependenceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVHIndependenceDoc �V���A���C�[�[�V����

void CVHIndependenceDoc::Serialize(CArchive& ar)
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
// CVHIndependenceDoc �N���X�̐f�f

#ifdef _DEBUG
void CVHIndependenceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVHIndependenceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVHIndependenceDoc �R�}���h

BOOL CVHIndependenceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVHIndependenceView*)pView)->app.Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CVHIndependenceDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVHIndependenceView*)pView)->app.Save(lpszPathName);
	}   
	return true;
}
