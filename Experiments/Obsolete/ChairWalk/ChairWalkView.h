// ChairWalkView.h : CChairWalkView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ChairWalkVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_ChairWalkVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ImpD3D/SGD3D.h>
#include <Collision/CDDetector.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinDX/DInput/WXDInput.h> 

using namespace Spr;

#if _WIN32_WINNT >= 0x501
class WBRawInputDevice:public RAWINPUT{
public:
	UTString name;
};
inline bool operator < (WBRawInputDevice& d1, WBRawInputDevice& d2){
	if (d1.header.dwType < d2.header.dwType) return true;
	if (d1.name.compare(d2.name) < 0) return true;
	return false;
}

class WBRawInput{
public:
	class WBRawInputDevices: public std::vector<WBRawInputDevice>{
	public:
	};
	WBRawInputDevices devices;
	struct HandleLess{
		bool operator() (WBRawInputDevice* d1, WBRawInputDevice* d2) const{
			return (DWORD)d1->header.hDevice < (DWORD)d2->header.hDevice;
		}
	};
	typedef std::set<WBRawInputDevice*, HandleLess> Index;
	Index index;

	void Enum();
	void Register(HWND hWnd);
	int Get(LPARAM lParam);
	WBRawInputDevice* Device(HANDLE hD);
};

#endif

class CChairWalkView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CChairWalkView();
	DECLARE_DYNCREATE(CChairWalkView)

// �A�g���r���[�g
public:
	bool bFullscreen;
	CChairWalkDoc* GetDocument();
	Spr::WXDInput dinput;
	Spr::UTRef<Spr::D3Render> render;
	Spr::UTRef<Spr::SGScene> scene;
	Spr::Affinef afBody, afHead;
	UINT timerID;
	float dt;
	/// ���݂̃J����
	Spr::UTRef<Spr::SGCamera> camera;

	//	RawInput
	WBRawInput rawInput;
	//	�}�E�X�̒l
	Vec3f angleMouse;		//	�w������̊p�x
	Vec3f floorMouse[2];	//	���̃}�E�X
	Vec3f rotMouse;			//	���Ȃ̉�]�p
	//	�֎q�̏��
	Affinef afLower;
	Affinef afUpper;

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CChairWalkView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL
	LRESULT AFX_MSG_CALL OnInput(WPARAM w, LPARAM l);
// �C���v�������e�[�V����
public:
	void Step();
	virtual ~CChairWalkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Load(const char* filename);
	void Save(const char* filename);

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CChairWalkView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ChairWalkView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CChairWalkDoc* CChairWalkView::GetDocument()
   { return (CChairWalkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ChairWalkVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
