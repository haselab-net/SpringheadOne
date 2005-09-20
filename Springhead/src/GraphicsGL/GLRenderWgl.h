#ifndef GLRENDERWGL_H
#define GLRENDERWGL_H

#include "GLRender.h"
#include <WinBasis/WBUtility.h>

namespace Spr {;
/**	�O���t�B�b�N�����_����OpenGL(WGL)�ɂ������D*/
class SPR_DLL GLRenderWgl:public GLRender{
public:
	SGOBJECTDEF(GLRenderWgl);
protected:
	///	window handle
	HWND hWnd;
	///	window's device context
	HDC hdc;
	///	gl context
	HGLRC hgl;
public:

	///	�R���X�g���N�^	
	GLRenderWgl();
	///
	virtual ~GLRenderWgl();
	///	Viewport�̐ݒ�
	bool SetViewport(void* arg);
	///	�R���e�L�X�g�̍쐬
	bool Create(void* hWnd);
	///	�R���e�L�X�g�̊J��
	void Release();
	///	�o�b�N�o�b�t�@�̕\��
	virtual void Present();
};

}	//	Spr
#endif
