#if !defined(AFX_TRGSET_H__09A46B10_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
#define AFX_TRGSET_H__09A46B10_B6F5_11D4_935E_006008B03B0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrgSet.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTrgSet �_�C�A���O

class CTrgSet : public CDialog
{
// �R���X�g���N�V����
public:
	BOOL ShowFlg;
	BOOL HexCheck(const char * str);
	int ThLevelNum;
	CTrgSet(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTrgSet)
	enum { IDD = IDD_TRGSET };
	UINT	m_EndCh;
	CString	m_Message;
	CString	m_ThLevel;
	UINT	m_TrgCh;
	UINT	m_BufSize;
	int		m_UpDown;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTrgSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTrgSet)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TRGSET_H__09A46B10_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
