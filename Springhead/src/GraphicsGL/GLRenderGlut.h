#ifndef GLRENDERGLUT_H
#define GLRENDERGLUT_H

#include "GLRender.h"

namespace Spr {;
/**	グラフィックレンダラのOpenGL(WGL)による実装．*/
class SPR_DLL GLRenderGlut:public GLRender{
public:
	SGOBJECTDEF(GLRenderGlut);
public:

	///	コンストラクタ	
	GLRenderGlut();
	///
	virtual ~GLRenderGlut();
	///	Viewportの設定
	bool SetViewport(void* arg);
	///	コンテキストの作成
	bool Create(void* arg);
	///	コンテキストの開放
	void Release();
	///	バックバッファの表示
	virtual void Present();
};

}	//	Spr
#endif
