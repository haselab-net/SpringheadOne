#ifndef GRRENDER_H
#define GRRENDER_H
#include <SceneGraph/SGEngine.h>

namespace Spr{;

class GRCamera;
class SGFrame;
struct GRMaterialData;
struct GRLightData;

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
		TRIANGLES,
	};
	virtual void SetModelMatrix(const Affinef& afw){}
	virtual void SetViewMatrix(const Affinef& afv){}
	virtual void SetProjectionMatrix(const Affinef& afp){}
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){}
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){}
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
};

}
#endif
