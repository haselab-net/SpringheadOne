#ifndef GLRENDER_H
#define GLRENDER_H

#include <Graphics/GRRender.h>
#include <WinBasis/WBUtility.h>

namespace Spr {;
/**	テクスチャファイルの管理をするクラス．
	2重ロードをしない仕組み．*/
class SPR_DLL GLTextureManager{
public:
	class GLRender* render;
	GLTextureManager(){ render = NULL; }
	int Get(const UTString& filename);
	typedef std::map<const UTString, int> Texs;
	Texs texs;
};


/**	グラフィックレンダラのDirect3Dによる実装．
*/
class SPR_DLL GLRender:public GRRender{
public:
	SGOBJECTDEF(GLRender);
protected:
	bool bDrawOpaque;
	///	テクスチャ管理
	GLTextureManager textureManager;
	/// 光源の数
	int nLights;
	///	Model Matrix
	Affinef afModel;
public:
	///	コンストラクタ	
	GLRender();
	///
	virtual ~GLRender();
	///	レンダリング(視点を含む)
	virtual void Render(SGScene* s);
	///	レンダリング(再帰部分)
	virtual void RenderRecurse(SGFrame* n);
	///	バッファクリア
	virtual void ClearBuffer();

	virtual void ClearColor(float r, float g, float b, float alpha);

	///	レンダリングの開始前に呼ぶ関数
	virtual void BeginScene();
	///	レンダリングの終了後に呼ぶ関数
	virtual void EndScene();
	///	Viewportと射影行列を設定
	virtual void Resize(Vec2f screen);

	//	描画
	virtual void SetModelMatrix(const Affinef& afm);
	virtual void SetViewMatrix(const Affinef& afv);
	virtual void SetProjectionMatrix(const Affinef& afp);
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end);
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx);
	virtual void DrawText(Vec2f pos, FIString str, const GRFont& font);
	virtual void SetMaterial(const GRMaterialData& m);
	virtual void SetTexture(const char* fn);
	virtual void PushLight(const GRLightData& m);
	virtual void PopLight();
	virtual void SetLineWidth(float w);
	virtual void SetDepthTest(bool b);
	virtual void SetDepthFunc(TDepthFunc f);
	virtual bool CanDraw();

	virtual Vec3f getPointUnderPixel(int x, int y, bool& found, SGScene* scene);
	
protected:
	///	window handle
	HWND hWnd;
	///	window's device context
	HDC hdc;
	///	gl context
	HGLRC hgl;

public:
	///	Viewportの設定
	bool SetViewport(void* arg);
	///	コンテキストの作成
	bool Create(void* hWnd);
	///	コンテキストの開放
	void Release();
	///	バックバッファの表示
	virtual void Present();

protected:
	void Loaded(SGScene* scene);
	void InitTree(SGFrame* fr, SGScene* scene);
};

}	//	Spr
#endif
