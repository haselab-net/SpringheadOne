#ifndef GLRENDER_H
#define GLRENDER_H

#include <Graphics/GRRender.h>
#include <WinBasis/WBUtility.h>

namespace Spr {;
/**	�e�N�X�`���t�@�C���̊Ǘ�������N���X�D
	2�d���[�h�����Ȃ��d�g�݁D*/
class SPR_DLL GLTextureManager{
public:
	class GLRender* render;
	GLTextureManager(){ render = NULL; }
	int Get(const UTString& filename);
	typedef std::map<const UTString, int> Texs;
	Texs texs;
};


/**	�O���t�B�b�N�����_����Direct3D�ɂ������D
*/
class SPR_DLL GLRender:public GRRender{
public:
	SGOBJECTDEF(GLRender);
protected:
	bool bDrawOpaque;
	///	�e�N�X�`���Ǘ�
	GLTextureManager textureManager;
	/// �����̐�
	int nLights;
	///	Model Matrix
	Affinef afModel;
public:
	///	�R���X�g���N�^	
	GLRender();
	///
	virtual ~GLRender();
	///	�����_�����O(���_���܂�)
	virtual void Render(SGScene* s);
	///	�����_�����O(�ċA����)
	virtual void RenderRecurse(SGFrame* n);
	///	�o�b�t�@�N���A
	virtual void ClearBuffer();

	virtual void ClearColor(float r, float g, float b, float alpha);

	///	�����_�����O�̊J�n�O�ɌĂԊ֐�
	virtual void BeginScene();
	///	�����_�����O�̏I����ɌĂԊ֐�
	virtual void EndScene();
	///	Viewport�Ǝˉe�s���ݒ�
	virtual void Resize(Vec2f screen);

	//	�`��
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
	///	Viewport�̐ݒ�
	bool SetViewport(void* arg);
	///	�R���e�L�X�g�̍쐬
	bool Create(void* hWnd);
	///	�R���e�L�X�g�̊J��
	void Release();
	///	�o�b�N�o�b�t�@�̕\��
	virtual void Present();

protected:
	void Loaded(SGScene* scene);
	void InitTree(SGFrame* fr, SGScene* scene);
};

}	//	Spr
#endif
