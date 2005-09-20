// DynaHapticView.h : CDynaHapticView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <FrameWork/FWAppD3D.h>
#include <FrameWork/FWPointer.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <WinBasis/WBMMTimer.h>
#include "HisConfig.h"

//add by hh 20060820
#include "target.h"
#include "Binocular.h"
//end

class CDynaHapticView : public CView {
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CDynaHapticView();
	DECLARE_DYNCREATE(CDynaHapticView)

//For test
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	
	float m_fI;
	
	float m_fAbsPosX;
	float m_fAbsPosY;
	float m_fAbsPosZ;
	CBinocular m_Binocular;
	//CTarget m_Target;
	float m_Log[2][900][3];
	BOOL m_Saved;
	float m_Deg0;
	float m_Deg1;
	float m_Deg2;
	float m_Deg3;
	float m_LookAtX;
	float m_LookAtY;
	float m_LookAtZ;
	float m_LookAtRot;
	BOOL m_ShutOne;

	float m_LOE; //���዗���̔���

	//end test


// �A�g���r���[�g
public:
	CHisConfig dlConfig;
	Spr::UTRef<Spr::FWAppD3D> app;
	int drawTimerId;

	CDynaHapticDoc* GetDocument();

// �I�y���[�V����
public:
	void InitHis(std::string hisList, float ps, float fs);
	void ReadRegistry();
	void WriteRegistry();
//	void Step();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDynaHapticView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CDynaHapticView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	// hh
	Spr::Vec3f pos_target1;	
	//end

	void ThrowSphere(float r);
	//{{AFX_MSG(CDynaHapticView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnD3d();
	afx_msg void OnGL();
	afx_msg void OnHisConfig();
	afx_msg BOOL OnDemo1();
	afx_msg BOOL OnDemo2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
};

#ifndef _DEBUG  // DynaHapticView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CDynaHapticDoc* CDynaHapticView::GetDocument()
   { return (CDynaHapticDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)