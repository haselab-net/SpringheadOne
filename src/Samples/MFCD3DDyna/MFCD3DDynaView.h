// VirtualHumanView.h : CMFCD3DDynaView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinBasis/WBMMTimer.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <Physics/PHSolid.h>
#include <Physics/PHContactEngine.h>
#include <HIS/HIMouse.h>
#include <Framework/FWPointer.h>

///	View
class CMFCD3DDynaView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CMFCD3DDynaView();
	DECLARE_DYNCREATE(CMFCD3DDynaView)
// �A�g���r���[�g
public:
	float dt;							///<	�X�V����
	Spr::UTRef<Spr::GRRender> render;	///<	�����_���[
	Spr::UTRef<Spr::SGScene> scene;		///<	�V�[���O���t
	std::string loadFile;				///<	OnInitialUpdate�Ń��[�h���ׂ��t�@�C����
	std::string loadFileName;			///<	�����[�h�p�ɋL�����Ă����t�@�C����
	Spr::GRCameraData initialCamera;	///<	�t�@�C�����[�h���̃J�����f�[�^
	Spr::UTRef<Spr::PHContactEngine> contactEngine;
										///<	�ڐG�G���W���D�ڐG�͂̕`��Ɏg�p
	Spr::WBMMTimer timer;				///<	�}���`���f�B�A�^�C�}�[
	UINT simTimerID;					///<	�V�~�����[�V�����p��WM_TIMER�̃^�C�}�[ID
	UINT drawTimerID;					///<	�`��p��WM_TIMER�̃^�C�}�[ID
	volatile bool bUpdate;				///<	�X�V�t���O

	Spr::UTRef<Spr::HIMouse> mouse;		///<	�}�E�X�f�o�C�X
	Spr::FWPointer6D pointer;			///<	�͊o�|�C���^
	bool bMinForce;						///<	�ŏ����͂ɂ��邩�ǂ����̃t���O

	void Import(std::string filename);	///<	�f�[�^�t�@�C���̃C���|�[�g
	void Load(std::string filename);	///<	�f�[�^�t�@�C���̃��[�h
	void Save(std::string filename);	///<	�f�[�^�t�@�C���̃Z�[�u
	void Step();						///<	�V�~�����[�V�����̃X�e�b�v
	void UpdateMouse();					///<	�}�E�X�f�o�C�X�̍X�V
	void ThrowSphere(float r);			///<	�{�[���𓊂���

	CMFCD3DDynaDoc* GetDocument();
// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMFCD3DDynaView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMFCD3DDynaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMFCD3DDynaView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRenderD3d();
	afx_msg void OnRenderGl();
	afx_msg void OnFileInsert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // VirtualHumanView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CMFCD3DDynaDoc* CMFCD3DDynaView::GetDocument()
   { return (CMFCD3DDynaDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
