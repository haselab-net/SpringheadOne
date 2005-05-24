#ifndef D3RENDER_H
#define D3RENDER_H

#include <Graphics/GRRender.h>
#include <WinBasis/WBUtility.h>
#include <WinDx/D3D/WXD3D.h>

namespace Spr {;
/**	�e�N�X�`���t�@�C���̊Ǘ�������N���X�D
	2�d���[�h�����Ȃ��d�g�݁D*/
class SPR_DLL D3TextureManager{
public:
	class D3Render* render;
	D3TextureManager(){ render = NULL; }
	std::map<UTString, WXINTFV(Direct3DTexture)> textures;
	virtual WXINTFV(Direct3DTexture) Get(UTString filename);
};

/**	�O���t�B�b�N�����_����Direct3D�ɂ������D
*/
class SPR_DLL D3Render:public GRRender{
public:
	SGOBJECTDEF(D3Render);
	///	Direct3DDevice
	WXD3DDevice device;
	///	�e�N�X�`���Ǘ�
	D3TextureManager textureManager;
	///	�s��X�^�b�N
	std::vector<Affinef> worldMatrixStack;
	/// �����̐�
	int nLights;

	///	�R���X�g���N�^	
	D3Render();
	///	����
	virtual bool Create(void* arg);
	///	���[�h��̏������CGRMesh�Ȃǂ���D3Mesh�Ȃǂ𐶐�����D
	void Loaded(SGScene* scene);

	///	�����_�����O(���_���܂�)
	virtual void Render(SGScene* s);
	///	�����_�����O(�ċA����)
	virtual void RenderRecurse(SGFrame* n);
	///	�o�b�t�@�N���A
	virtual void ClearBuffer();
	///	�o�b�t�@�̕\��
	void Present();
	///	�o�b�t�@�̕\��
	void Present(HWND hw);
	///	�����_�����O�̊J�n�O�ɌĂԊ֐�
	virtual void BeginScene();
	///	�����_�����O�̏I����ɌĂԊ֐�
	virtual void EndScene();
	///	�^�[�Q�b�g�̃��T�C�Y
	virtual bool Resize(Vec2f size, HWND wnd=NULL);
	///	�����_���̏���
	void Setup(Vec2f screen);
	///	�T�[�t�F�[�X���E�B���h�E or �t���X�N���[���̃T�C�Y�ɍ��킹��D
	bool FitSurface(HWND hDeviceWnd = NULL);
	///
	bool SetViewport(void* arg);
	///
	bool SetViewport(Vec2f sz, Vec2f pos=Vec2f(), float minZ=0.0f, float maxZ=1.0f);
	///
	bool IsFullscreen(){ return device.IsFullscreen(); }
	void Fullscreen();
	void Window();	
	//	DirectX�̃o�[�W�����ɂ��API�̈Ⴂ�̋z��
	///
	HRESULT SetFVF(DWORD fvf);
	//	�`��
	void SetModelMatrix(const Affinef& afw);
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end);
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx);
	virtual void SetMaterial(const GRMaterialData& m);
	virtual void SetTexture(const char* fn);
	virtual void PushLight(const GRLightData& m);
	virtual void PopLight();
	virtual void SetLineWidth(float w);
	virtual void SetDepthTest(bool b);
	virtual void SetDepthFunc(TDepthFunc f);
	virtual bool CanDraw();
	void InitTree(SGFrame* fr, SGScene* scene);

	virtual Vec3f getPointUnderPixel(int x, int y, bool& found);


protected:
};

}	//	Spr
#endif
