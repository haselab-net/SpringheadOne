#ifndef GLRENDERGLUT_H
#define GLRENDERGLUT_H

#include "GLRender.h"

namespace Spr {;
/**	�O���t�B�b�N�����_����OpenGL(WGL)�ɂ������D*/
class SPR_DLL GLRenderGlut:public GLRender{
public:
	SGOBJECTDEF(GLRenderGlut);
public:

	///	�R���X�g���N�^	
	GLRenderGlut();
	///
	virtual ~GLRenderGlut();
	///	Viewport�̐ݒ�
	bool SetViewport(void* arg);
	///	�R���e�L�X�g�̍쐬
	bool Create(void* arg);
	///	�R���e�L�X�g�̊J��
	void Release();
	///	�o�b�N�o�b�t�@�̕\��
	virtual void Present();
};

}	//	Spr
#endif
