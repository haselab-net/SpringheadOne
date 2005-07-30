#ifndef __GRSOUND_H__
#define __GRSOUND_H__

#include <WinDX/Base/WXComBase.h>

#ifndef _WIN64
typedef DWORD DWORD_PTR;
#endif

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
    SAMPLE6 =6,
    SAMPLE7 =7,
    SAMPLE8 =8,
    SAMPLE9 =9,
    SAMPLE10=10,
    SAMPLE11=11,
    SAMPLE12=12,
    SAMPLE13=13,
    SAMPLE14=14,
    SAMPLE15=15,
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
    // this method equals to the constructor
    // return value is instance
	static GRSound* instance(){ return &inst; }

    // called after the instance method
	HRESULT initialize( HWND hwnd );
	HRESULT	play( SOUNDID id );
	HRESULT stop( SOUNDID id );
    HRESULT SetVolume(SOUNDID id, long volume);
    HRESULT GetVolume(SOUNDID id, long *volume);
    HRESULT RetriveVolumes();
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
