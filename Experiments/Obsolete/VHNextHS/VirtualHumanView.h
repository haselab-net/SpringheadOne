// VirtualHumanView.h : CVirtualHumanView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "THuman.h"
#include "TDiffFile.h"
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <ImpD3D/D3Render.h>
#include <Collision/CDFramePair.h>
#include <Physics/PHForceField.h>
#include <Physics/PHSolid.h>
#include <Physics/PHContactEngine.h>
#include <Graphics/GRCamera.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinBasis/WBMMTimer.h>
#include <HIS/HIMouse.h>

class CVirtualHumanView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CVirtualHumanView();
	DECLARE_DYNCREATE(CVirtualHumanView)
// �A�g���r���[�g
public:
	bool bMinForce;						//	�ŏ����͂ɂ��邩�ǂ����̃t���O
	float time;							//	���ݎ���
	Spr::UTRef<Spr::GRRender> render;	//	�����_���[
	Spr::UTRef<Spr::SGScene> scene;		//	�V�[���O���t
	std::string loadFile;				//	OnInitialUpdate�Ń��[�h���ׂ��t�@�C����
	std::string loadFileName;			//	�����[�h�p�ɋL�����Ă����t�@�C����
	UINT simTimerID;					//	�V�~�����[�V�����p��WM_TIMER�̃^�C�}�[ID
	UINT drawTimerID;					//	�`��p��WM_TIMER�̃^�C�}�[ID
	Spr::WBMMTimer timer;				//	�}���`���f�B�A�^�C�}�[
	Spr::HIMouse mouse;					//	�}�E�X�f�o�C�X
	Spr::UTRef<Spr::PHSolid> pointer;	//	�͊o�|�C���^
	Spr::GRCameraData initialCamera;	//	�t�@�C�����[�h���̃J�����f�[�^
	Spr::UTRef<Spr::PHContactEngine> contactEngine;	//	�Փ˔���G���W��

	THuman human;						//	Virtual Human
	TDiffFile diff;						//	Diff �t�@�C��
	enum TAppState{						//	�A�v���P�[�V�����̏��
		INVALID,						//	����
		MAKE_ANGLE,						//	DIFF�t�@�C���̒��_��񂩂�p�x�t�@�C���𐶐�
		MAKE_FORCE,						//	�����͌v�Z
		SET_TORQUE,						//�@�g���N�f�[�^�t�@�C���̏����Z�b�g����
		NON_GRAVITY						//	���Œ�̃g���N����
	} appState;
	void Load(std::string filename);
	void Save(const char* filename);
	void UpdateMouse();
	void Step();

	CVirtualHumanDoc* GetDocument();
// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CVirtualHumanView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CVirtualHumanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CVirtualHumanView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // VirtualHumanView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CVirtualHumanDoc* CVirtualHumanView::GetDocument()
   { return (CVirtualHumanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
