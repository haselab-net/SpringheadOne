#ifndef SPR_GRAPHICS_H
#define SPR_GRAPHICS_H
/**	@page Graphics Graphicsライブラリ
	グラフィックスレンダラと表示されるオブジェクトの基本クラス．
	一部のクラス(CDMesh，CDSphere) は，表示オブジェクトの基本クラスではなく，
	単に情報を保持している．
	一部のエンジン(D3Render, GLRender, CDCollisionEngine)が，この情報を参照し，
	それぞれの作業に適した形式(D3Mesh, CDMesh) を生成する．	*/

namespace Spr{}
#include <Base/Base.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include "GRVisual.h"
#include "GRCamera.h"
#include "GRMaterial.h"
#include "GRRender.h"
#include "GRMesh.h"

#endif
