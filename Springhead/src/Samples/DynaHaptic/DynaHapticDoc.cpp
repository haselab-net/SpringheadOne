// DynaHapticDoc.cpp : CDynaHapticDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "DynaHaptic.h"

#include "DynaHapticDoc.h"
#include "DynaHapticView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticDoc

IMPLEMENT_DYNCREATE(CDynaHapticDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynaHapticDoc, CDocument)
	//{{AFX_MSG_MAP(CDynaHapticDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticDoc �N���X�̍\�z/����

CDynaHapticDoc::CDynaHapticDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CDynaHapticDoc::~CDynaHapticDoc()
{
}

BOOL CDynaHapticDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDynaHapticDoc �V���A���C�[�[�V����

void CDynaHapticDoc::Serialize(CArchive& ar)
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
// CDynaHapticDoc �N���X�̐f�f

#ifdef _DEBUG
void CDynaHapticDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynaHapticDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticDoc �R�}���h

BOOL CDynaHapticDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaHapticView*)pView)->app->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CDynaHapticDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaHapticView*)pView)->app->Save(lpszPathName);
	}   
	return true;
}
