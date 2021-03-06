#ifndef GRRENDER_H
#define GRRENDER_H
#include <SceneGraph/SGEngine.h>
#include <WinBasis/WBCriticalSection.h>

namespace Spr{;

class GRCamera;
class SGFrame;
struct GRMaterialData;
struct GRLightData;

class SPR_DLL GRFont{
public:
	int height;
	int width;
	int weight;
	FIString face;
	DWORD color;
	bool bItalic;
	GRFont(int h=20, const char* f=NULL);
	void SetColor(DWORD c){ color = c; }
	bool operator < (GRFont& f);
};

/**	グラフィックスレンダラーの基本クラス．*/
class SPR_DLL GRRender:public SGEngine{
public:
	SGOBJECTDEFABST(GRRender);
	WBCriticalSection cr;			///<	レンダリングAPIのロック用
	bool bDrawDebug;				///<	デバッグ情報を描画するか？
	UTRef<GRCamera> camera;			///<	使用するカメラ
	SGScene* scene;					///<	描画対象のScene
	UTStack<SGFrame*> frames;		///<	描画用のフレームのスタック
	
	enum TDrawState{
		DRAW_OPAQUE=1,
		DRAW_TRANSPARENT=2,
		DRAW_BOTH=3,
	} drawState;
	GRRender();
	virtual ~GRRender();
	///	生成
	virtual bool Create(void* arg){return false;}
	///	シーングラフのあるフレーム以下を見て，このレンダラで描画できる形式に変換する．
	virtual	void InitTree(SGFrame* fr, SGScene* scene);
	///	Viewportの設定
	virtual bool SetViewport(void* arg){return false;}
	///	レンダラーをクリア
	virtual void Clear(SGScene* s);
	///	フルスクリーンモードか？
	virtual bool IsFullscreen(){return false;}
	///	フルスクリーンモードにする
	virtual void Fullscreen(){}
	///	ウィンドウモードにする
	virtual void Window(){}
	///	@name レンダリングの都度呼ぶ関数
	//@{
	///	バッファのクリア
	virtual void ClearBuffer(){}
	virtual void ClearColor(float r, float g, float b, float alpha){}

	///	レンダリングの開始前に呼ぶ関数
	virtual void BeginScene(){}
	///	シーンのレンダリング(視点の設定を含む)
	virtual void Render(SGScene* s);
	///	フレームのレンダリング(再帰部)
	virtual void RenderRecurse();
	///	レンダリングの終了後に呼ぶ関数
	virtual void EndScene(){}
	///	バックバッファの表示
	virtual void Present(){}
	//@}

	///	@name 描画関数
	//@{
	enum TPrimitiveType {
		POINTS,
		LINES,
		LINESTRIP,
		TRIANGLES,
		TRIANGLESTRIP,
		TRIANGLEFAN
	};
	virtual void MultModelMatrix(const Affinef& afw){}
	virtual void PushModelMatrix(){}
	virtual void PopModelMatrix(){}
	virtual void SetModelMatrix(const Affinef& afw){}
	virtual void SetViewMatrix(const Affinef& afv){}
	virtual void SetProjectionMatrix(const Affinef& afp){}
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){}
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){}
	virtual void DrawText(Vec2f pos, FIString str, const GRFont& font){}
	virtual Vec2f GetTextExtent(FIString str, const GRFont& font){ return Vec2f(); }
	virtual void SetMaterial(const GRMaterialData& m){}
	virtual void SetTexture(const char* fn){}
	virtual void PushLight(const GRLightData& m){}
	virtual void PopLight(){}
	virtual void SetLineWidth(float w){}
	virtual void SetDepthTest(bool b){}
	virtual void SetDepthWrite(bool b){}
	enum TDepthFunc{
		DF_NEVER, DF_LESS, DF_EQUAL, DF_LEQUAL, DF_GREATER, DF_NOTEQUAL, 
		DF_GEQUAL, DF_ALWAYS
	};
	virtual void SetDepthFunc(TDepthFunc f){}
	enum TBlendFunc{
		BF_ZERO = 1,
		BF_ONE = 2,
		BF_SRCCOLOR = 3,
		BF_INVSRCCOLOR = 4,
		BF_SRCALPHA = 5,
		BF_INVSRCALPHA = 6,
		BF_DESTALPHA = 7,
		BF_INVDESTALPHA = 8,
		BF_DESTCOLOR = 9,
		BF_INVDESTCOLOR = 10,
		BF_SRCALPHASAT = 11,
		BF_BOTHSRCALPHA = 12,
		BF_BOTHINVSRCALPHA = 13,
		BF_BLENDFACTOR = 14,
		BF_INVBLENDFACTOR = 15
	};
	virtual void SetAlphaMode(TBlendFunc src, TBlendFunc dest){}
	virtual bool CanDraw(){return true;}
	//@}
	///	カメラの設定
	virtual void Set(SGObject* obj);

	///Get 3d position under pixel x, y. Found is false if the point is on the background, true ortherwise
	virtual Vec3f getPointUnderPixel(int x, int y, bool& found, SGScene* scene){
		found=false; Vec3f res; return res;}


};

}
#endif
