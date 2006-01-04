#ifndef GR_VISUAL_H
#define GR_VISUAL_H

#include <Base/BaseUtility.h>
#include <Base/BaseTypeInfo.h>
#include <Base/Affine.h>
#include <FileIO/FIDocNode.h>
#include <WinBasis/WBWin.h>
#include <SceneGraph/SGObject.h>


namespace Spr {;

class SGFrame;
class GRRender;
/**	レンダリングできるものの基本クラス．
	MeshやLightが派生する．
*/
class SPR_DLL GRVisual:public SGObject{
public:
	SGOBJECTDEFABST(GRVisual);
	
	virtual ~GRVisual(){}
	///
	virtual void Render(SGFrame* n, GRRender* r)=0;
	///
	virtual void Rendered(SGFrame* n, GRRender* r){}
};

}
#endif GR_VISUAL_H
