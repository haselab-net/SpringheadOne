#if !defined(AFX_HISCONFIG_H__5D92FEB6_3366_465C_B6FC_FDDFEE6FB266__INCLUDED_)
#define AFX_HISCONFIG_H__5D92FEB6_3366_465C_B6FC_FDDFEE6FB266__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HisConfig.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CHisConfig �_�C�A���O

class CHisConfig : public CDialog
{
// �R���X�g���N�V����
public:
	CHisConfig(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CHisConfig)
	enum { IDD = IDD_HIS_CONFIG };
	CString	hisList;
	float	forceScale;
	float	posScale;
	float	timeStep;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CHisConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CHisConfig)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_HISCONFIG_H__5D92FEB6_3366_465C_B6FC_FDDFEE6FB266__INCLUDED_)
