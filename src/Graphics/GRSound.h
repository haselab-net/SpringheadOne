#ifndef __GRSOUND_H__
#define __GRSOUND_H__

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
//#include <afxext.h>         // MFC の拡張部分
//#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
//#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート

#include <sstream>
#include <mmsystem.h>
#include <dsound.h>

// ID割り当ては計画的に（ぉ
// 管理が面倒になったら、iniファイルで対応します
enum SOUNDID{
	SAMPLE0 =0,
	SAMPLE1 =1,
	SAMPLE2 =2,
	SAMPLE3 =3,
	SAMPLE4 =4,
	SAMPLE5 =5,
};

#define SOUNDBUFFER_MAX	100 //100以下でね

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
public: // 臨時措置(？) もともとはprivateだったのだけど
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