#ifndef GLRENDERWGL_H
#define GLRENDERWGL_H

#include "GLRender.h"
#include <WinBasis/WBUtility.h>

namespace Spr {;
/**	グラフィックレンダラのOpenGL(WGL)による実装．*/
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

	///	コンストラクタ	
	GLRenderWgl();
	///
	virtual ~GLRenderWgl();
	///	Viewportの設定
	bool SetViewport(void* arg);
	///	コンテキストの作成
	bool Create(void* hWnd);
	///	コンテキストの開放
	void Release();
	///	バックバッファの表示
	virtual void Present();
};

}	//	Spr
#endif
