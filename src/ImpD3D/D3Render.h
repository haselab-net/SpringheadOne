#ifndef D3RENDER_H
#define D3RENDER_H

#include <Graphics/GRRender.h>
#include <WinBasis/WBUtility.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinDx/D3D/WXD3D.h>


struct TLVertex
{
    float    x, y, z, rhw;
    D3DCOLOR diffuse;    
};

const DWORD VertexFVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

namespace Spr {;
/**	テクスチャファイルの管理をするクラス．
	2重ロードをしない仕組み．*/
class SPR_DLL D3TextureManager{
public:
	class D3Render* render;
	D3TextureManager(){ render = NULL; }
	std::map<UTString, WXINTFV(Direct3DTexture)> textures;
	virtual WXINTFV(Direct3DTexture) Get(UTString filename);
};

/**	グラフィックレンダラのDirect3Dによる実装．
*/
class SPR_DLL D3Render:public GRRender{
public:
	SGOBJECTDEF(D3Render);
	///	Direct3DDevice
	WXD3DDevice device;
	///	テクスチャ管理
	D3TextureManager textureManager;
	///	行列スタック
	UTStack<Affinef> modelMatrixStack;
	/// 光源の数
	int nLights;

	///	コンストラクタ	
	D3Render();
	///	生成
	virtual bool Create(void* arg);
	///	ロード後の初期化，GRMeshなどからD3Meshなどを生成する．
	void Loaded(SGScene* scene);

	///	バッファクリア
	virtual void ClearBuffer();

	virtual void ClearColor(float r, float g, float b, float alpha);

	///	バッファの表示
	void Present();
	///	バッファの表示
	void Present(HWND hw);
	///	レンダリングの開始前に呼ぶ関数
	virtual void BeginScene();
	///	レンダリングの終了後に呼ぶ関数
	virtual void EndScene();
	///	ターゲットのリサイズ
	virtual bool Resize(Vec2f size, HWND wnd=NULL);
	///	レンダラの準備
	void Setup(Vec2f screen);
	///	サーフェースをウィンドウ or フルスクリーンのサイズに合わせる．
	bool FitSurface(HWND hDeviceWnd = NULL);
	///
	bool SetViewport(void* arg);
	///
	bool SetViewport(Vec2f sz, Vec2f pos=Vec2f(), float minZ=0.0f, float maxZ=1.0f);
	///
	bool IsFullscreen(){ return device.IsFullscreen(); }
	void Fullscreen();
	void Window();	
	//	DirectXのバージョンによるAPIの違いの吸収
	///
	HRESULT SetFVF(DWORD fvf);
	//	描画
	virtual void MultModelMatrix(const Affinef& afw);
	virtual void PushModelMatrix();
	virtual void PopModelMatrix();
	virtual void SetModelMatrix(const Affinef& afw);
	virtual void SetViewMatrix(const Affinef& afv);
	virtual void SetProjectionMatrix(const Affinef& afp);
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end);
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx);
	virtual void DrawText(Vec2f pos, FIString str, const GRFont& font);
	virtual Vec2f GetTextExtent(FIString str, const GRFont& font);
	virtual void SetMaterial(const GRMaterialData& m);
	virtual void SetTexture(const char* fn);
	virtual void PushLight(const GRLightData& m);
	virtual void PopLight();
	virtual void SetLineWidth(float w);
	virtual void SetDepthTest(bool b);
	virtual void SetDepthWrite(bool b);
	virtual void SetDepthFunc(TDepthFunc f);
	virtual void SetAlphaMode(TBlendFunc src, TBlendFunc dest);
	virtual bool CanDraw();
	void InitTree(SGFrame* fr, SGScene* scene);
	void Render(SGScene* s);

	virtual Vec3f getPointUnderPixel(int x, int y, bool& found, SGScene* scene);


protected:
};

}	//	Spr
#endif
