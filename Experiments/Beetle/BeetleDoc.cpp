// BeetleDoc.cpp : CBeetleDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "Beetle.h"

#include "BeetleDoc.h"
#include "BeetleView.h"


/////////////////////////////////////////////////////////////////////////////
// CBeetleDoc

IMPLEMENT_DYNCREATE(CBeetleDoc, CDocument)

BEGIN_MESSAGE_MAP(CBeetleDoc, CDocument)
	//{{AFX_MSG_MAP(CBeetleDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBeetleDoc �N���X�̍\�z/����

CBeetleDoc::CBeetleDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CBeetleDoc::~CBeetleDoc()
{
}

BOOL CBeetleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBeetleDoc �V���A���C�[�[�V����

void CBeetleDoc::Serialize(CArchive& ar)
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
// CBeetleDoc �N���X�̐f�f

#ifdef _DEBUG
void CBeetleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBeetleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBeetleDoc �R�}���h

BOOL CBeetleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CBeetleView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CBeetleDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CBeetleView*)pView)->Save(lpszPathName);
	}   
	return true;
}
