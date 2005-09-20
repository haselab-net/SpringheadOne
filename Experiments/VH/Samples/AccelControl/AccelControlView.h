// AccelControlView.h : CAccelControlView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AccelControlVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_AccelControlVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <FrameWork/FWAppD3D.h>
#include <FrameWork/FWPointer.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <WinBasis/WBMMTimer.h>
#include "HisConfig.h"


class CAccelControlView : public CView {
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CAccelControlView();
	DECLARE_DYNCREATE(CAccelControlView)
// �A�g���r���[�g
public:
	CHisConfig dlConfig;
	Spr::UTRef<Spr::FWAppD3D> app;
	int drawTimerId;

	CAccelControlDoc* GetDocument();

// �I�y���[�V����
public:
	void InitHis(std::string hisList, float ps, float fs);
	void ReadRegistry();
	void WriteRegistry();
	void Step();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAccelControlView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CAccelControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	void ThrowSphere(float r);
	//{{AFX_MSG(CAccelControlView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnD3d();
	afx_msg void OnGL();
	afx_msg void OnHisConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AccelControlView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CAccelControlDoc* CAccelControlView::GetDocument()
   { return (CAccelControlDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_AccelControlVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
