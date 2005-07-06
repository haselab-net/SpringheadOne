#ifndef GRRENDER_H
#define GRRENDER_H
#include <SceneGraph/SGEngine.h>

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

/**	�O���t�B�b�N�X�����_���[�̊�{�N���X�D*/
class SPR_DLL GRRender:public SGEngine{
public:
	SGOBJECTDEFABST(GRRender);
	UTRef<GRCamera> camera;
	enum TDrawState{
		DRAW_OPAQUE=1,
		DRAW_TRANSPARENT=2,
		DRAW_BOTH=3,
	} drawState;
	GRRender();
	virtual ~GRRender();
	///	����
	virtual bool Create(void* arg){return false;}
	///	�V�[���O���t�̂���t���[���ȉ������āC���̃����_���ŕ`��ł���`���ɕϊ�����D
	virtual	void InitTree(SGFrame* fr, SGScene* scene);
	///	Viewport�̐ݒ�
	virtual bool SetViewport(void* arg){return false;}
	///	�����_���[���N���A
	virtual void Clear(SGScene* s);
	///	�t���X�N���[�����[�h���H
	virtual bool IsFullscreen(){return false;}
	///	�t���X�N���[�����[�h�ɂ���
	virtual void Fullscreen(){}
	///	�E�B���h�E���[�h�ɂ���
	virtual void Window(){}
	///	@name �����_�����O�̓s�x�ĂԊ֐�
	//@{
	///	�o�b�t�@�̃N���A
	virtual void ClearBuffer(){}
	virtual void ClearColor(float r, float g, float b, float alpha){}

	///	�����_�����O�̊J�n�O�ɌĂԊ֐�
	virtual void BeginScene(){}
	///	�V�[���̃����_�����O(���_�̐ݒ���܂�)
	virtual void Render(SGScene* s) = 0;
	///	�t���[���̃����_�����O(�ċA��)
	virtual void RenderRecurse(SGFrame* n);
	///	�����_�����O�̏I����ɌĂԊ֐�
	virtual void EndScene(){}
	///	�o�b�N�o�b�t�@�̕\��
	virtual void Present(){}
	//@}

	///	@name �`��֐�
	//@{
	enum TPrimitiveType {
		POINTS,
		LINES,
		LINESTRIP,
		TRIANGLES,
		TRIANGLESTRIP,
		TRIANGLEFAN
	};
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
	enum TDepthFunc{
		DF_NEVER, DF_LESS, DF_EQUAL, DF_LEQUAL, DF_GREATER, DF_NOTEQUAL, DF_GEQUAL, DF_ALWAYS
	};
	virtual void SetDepthFunc(TDepthFunc f){}
	virtual bool CanDraw(){return true;}
	//@}
	///	�J�����̐ݒ�
	virtual void Set(SGObject* obj);

	///Get 3d position under pixel x, y. Found is false if the point is on the background, true ortherwise
	virtual Vec3f getPointUnderPixel(int x, int y, bool& found, SGScene* scene){
		found=false; Vec3f res; return res;}


};

}
#endif
