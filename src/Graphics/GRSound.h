#ifndef __GRSOUND_H__
#define __GRSOUND_H__

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
//#include <afxext.h>         // MFC �̊g������
//#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
//#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g

#include <sstream>
#include <mmsystem.h>
#include <dsound.h>

// ID���蓖�Ă͌v��I�Ɂi��
// �Ǘ����ʓ|�ɂȂ�����Aini�t�@�C���őΉ����܂�
enum SOUNDID{
	SAMPLE0 =0,
	SAMPLE1 =1,
	SAMPLE2 =2,
	SAMPLE3 =3,
	SAMPLE4 =4,
	SAMPLE5 =5,
};

#define SOUNDBUFFER_MAX	100 //100�ȉ��ł�

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

class GRSound
{
protected:
    IDirectSound8* pDS;
	LPDIRECTSOUNDBUFFER      pDsbPrimary;
	LPDIRECTSOUNDBUFFER8     pDsb[SOUNDBUFFER_MAX];
	LPDIRECTSOUND3DLISTENER	 pListener;

private:
	GRSound::GRSound();
public: // �Վ��[�u(�H) ���Ƃ��Ƃ�private�������̂�����
	GRSound::~GRSound();

public:
	static GRSound* instance(){ return &inst; }
	HRESULT initialize( HWND hwnd );
	HRESULT	play( SOUNDID id );
	HRESULT stop( SOUNDID id );
private:
	HRESULT createSoundBuffer( LPDIRECTSOUNDBUFFER8* ppDsb8 );
	HRESULT getListener( LPDIRECTSOUND3DLISTENER* ppListener );
	HRESULT loadWaveFile( SOUNDID id ,TCHAR* strFileName );

	HRESULT freeDirectSound();

	void	convertCoord(LPD3DVECTOR* pos);
	VOID    Set3DParameters( FLOAT fDopplerFactor, FLOAT fRolloffFactor, FLOAT fMinDistance,   FLOAT fMaxDistance );
	VOID    SetObjectProperties( D3DVECTOR* pvPosition, D3DVECTOR* pvVelocity );
	FLOAT   ConvertLinearSliderPosToLogScale( LONG lSliderPos );
	LONG    ConvertLogScaleToLinearSliderPosTo( FLOAT fValue );

	static GRSound inst;
};

#endif //__GRSOUND_H__