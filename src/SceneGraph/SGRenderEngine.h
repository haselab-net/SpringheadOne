#ifndef SGRENDERENGINE_H
#define SGRENDERENGINE_H

#include "SGEngine.h"

namespace Spr {;

///	動作エンジンの基本クラス．
class SGRenderEngine:public SGEngine{
	SGOBJECTDEF(SGRenderEngine);
public:
	///	レンダリングを行う
	virtual void Render(SGScene* s){}
};

///	SGEngineの配列
class SGRenderEngines: public SGEngines{
public:
	///@name レンダリング処理
	//@{
	///	(グラフィックス)レンダリング．
	virtual void Render(SGScene* s);
	//@}
};

}
#endif
