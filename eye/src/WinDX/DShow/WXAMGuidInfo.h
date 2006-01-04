#ifndef WXAMGUIDINFO_H
#define WXAMGUIDINFO_H

#include "WinDX/Base/WXError.h"
#include "WXAMPin.h"
#include <uuids.h>

namespace Spr{

// Media types
struct WXAMGuidInfo{
    const GUID *pGuid;
    char name[64];
};

const WXAMGuidInfo majortypes[] = {
    &MEDIATYPE_AnalogAudio      ,"Analog audio", 
    &MEDIATYPE_AnalogVideo      ,"Analog video",
    &MEDIATYPE_Audio            ,"Audio",
    &MEDIATYPE_AUXLine21Data    ,"Line 21 data (CC)",
    &MEDIATYPE_File             ,"File (CC)",
    &MEDIATYPE_Interleaved      ,"Interleaved (DV)",
    &MEDIATYPE_LMRT             ,"LMRT (Obsolete)",
    &MEDIATYPE_Midi             ,"MIDI",
    &MEDIATYPE_MPEG2_PES        ,"MPEG2 (DVD)",
    &MEDIATYPE_ScriptCommand    ,"ScriptCommand (CC)",
    &MEDIATYPE_Stream           ,"Byte stream (no time stamps)",
    &MEDIATYPE_Text             ,"Text",
    &MEDIATYPE_Timecode         ,"Timecode data",
    &MEDIATYPE_URL_STREAM       ,"URL_STREAM (Obsolete)",
    &MEDIATYPE_Video            ,"Video",
};

// Media subtypes
const WXAMGuidInfo audiosubtypes[] = {
    &MEDIASUBTYPE_PCM           ,"PCM audio", 
    &MEDIASUBTYPE_MPEG1Packet   ,"MPEG1 Audio Packet", 
    &MEDIASUBTYPE_MPEG1Payload  ,"MPEG1 Audio Payload",
};

const WXAMGuidInfo line21subtypes[] = {
    &MEDIASUBTYPE_Line21_BytePair       ,"BytePairs",
    &MEDIASUBTYPE_Line21_GOPPacket      ,"DVD GOP Packet",
    &MEDIASUBTYPE_Line21_VBIRawData     ,"VBI Raw Data",
};

const WXAMGuidInfo mpeg2subtypes[] = {
    &MEDIASUBTYPE_DVD_SUBPICTURE        ,"DVD Subpicture",
    &MEDIASUBTYPE_DVD_LPCM_AUDIO        ,"DVD Audio (LPCM)",
    &MEDIASUBTYPE_DOLBY_AC3             ,"Dolby AC3",
    &MEDIASUBTYPE_MPEG2_AUDIO           ,"MPEG-2 Audio",
    &MEDIASUBTYPE_MPEG2_TRANSPORT       ,"MPEG-2 Transport Stream",
    &MEDIASUBTYPE_MPEG2_PROGRAM         ,"MPEG-2 Program Stream",
};

const WXAMGuidInfo streamsubtypes[] = {
    &MEDIASUBTYPE_AIFF              ,"AIFF",
    &MEDIASUBTYPE_Asf               ,"ASF",
    &MEDIASUBTYPE_Avi               ,"AVI",
    &MEDIASUBTYPE_AU                ,"AU",
    &MEDIASUBTYPE_DssAudio          ,"DSS Audio",
    &MEDIASUBTYPE_DssVideo          ,"DSS Video",
    &MEDIASUBTYPE_MPEG1Audio        ,"MPEG1 Audio",
//    &MEDIASUBTYPE_MPEG1System       ,"MPEG1 System",
//    &MEDIASUBTYPE_MPEG1SystemStream ,"MPEG1 System Stream",
    &MEDIASUBTYPE_MPEG1Video        ,"MPEG1 Video",
    &MEDIASUBTYPE_MPEG1VideoCD      ,"MPEG1 VideoCD",
    &MEDIASUBTYPE_WAVE              ,"Wave",
};

const WXAMGuidInfo videosubtypes[] = {
    &MEDIASUBTYPE_YVU9              ,"YVU9",
    &MEDIASUBTYPE_Y411              ,"YUV 411",
    &MEDIASUBTYPE_Y41P              ,"Y41P",
    &MEDIASUBTYPE_YUY2              ,"YUY2",
    &MEDIASUBTYPE_YVYU              ,"YVYU",
    &MEDIASUBTYPE_UYVY              ,"UYVY",
    &MEDIASUBTYPE_Y211              ,"YUV 211",
    &MEDIASUBTYPE_CLJR              ,"Cirrus YUV 411",
    &MEDIASUBTYPE_IF09              ,"Indeo YVU9",
    &MEDIASUBTYPE_CPLA              ,"Cinepak UYVY",
    &MEDIASUBTYPE_MJPG              ,"Motion JPEG",
    &MEDIASUBTYPE_TVMJ              ,"TrueVision MJPG",
    &MEDIASUBTYPE_WAKE              ,"MJPG (Wake)",
    &MEDIASUBTYPE_CFCC              ,"MJPG (CFCC)",
    &MEDIASUBTYPE_IJPG              ,"Intergraph JPEG",
    &MEDIASUBTYPE_Plum              ,"Plum MJPG",
    &MEDIASUBTYPE_RGB1              ,"RGB1 (Palettized)",
    &MEDIASUBTYPE_RGB4              ,"RGB4 (Palettized)",
    &MEDIASUBTYPE_RGB8              ,"RGB8 (Palettized)",
    &MEDIASUBTYPE_RGB565            ,"RGB565",
    &MEDIASUBTYPE_RGB555            ,"RGB555",
    &MEDIASUBTYPE_RGB24             ,"RGB24",
    &MEDIASUBTYPE_RGB32             ,"RGB32",
    &MEDIASUBTYPE_ARGB32            ,"ARGB32",
    &MEDIASUBTYPE_Overlay           ,"Overlay video (from HW)",
    &MEDIASUBTYPE_QTMovie           ,"Apple QuickTime",
    &MEDIASUBTYPE_QTRpza            ,"QuickTime RPZA",
    &MEDIASUBTYPE_QTSmc             ,"QuickTime SMC",
    &MEDIASUBTYPE_QTRle             ,"QuickTime RLE",
    &MEDIASUBTYPE_QTJpeg            ,"QuickTime JPEG",
    &MEDIASUBTYPE_dvsd              ,"Standard DV",
    &MEDIASUBTYPE_dvhd              ,"High Definition DV",
    &MEDIASUBTYPE_dvsl              ,"Long Play DV",
    &MEDIASUBTYPE_MPEG1Packet       ,"MPEG1 Video Packet",
    &MEDIASUBTYPE_MPEG1Payload      ,"MPEG1 Video Payload",
//    &MEDIASUBTYPE_VideoPort         ,"Video Port (DVD)",
    &MEDIASUBTYPE_VPVideo           ,"Video port video",
    &MEDIASUBTYPE_VPVBI             ,"Video port VBI",
};

const WXAMGuidInfo analogvideosubtypes[] = {
    &MEDIASUBTYPE_AnalogVideo_NTSC_M   ,"(M) NTSC",
    &MEDIASUBTYPE_AnalogVideo_PAL_B    ,"(B) PAL",
    &MEDIASUBTYPE_AnalogVideo_PAL_D    ,"(D) PAL",
    &MEDIASUBTYPE_AnalogVideo_PAL_G    ,"(G) PAL",
    &MEDIASUBTYPE_AnalogVideo_PAL_H    ,"(H) PAL",
    &MEDIASUBTYPE_AnalogVideo_PAL_I    ,"(I) PAL",
    &MEDIASUBTYPE_AnalogVideo_PAL_M    ,"(M) PAL",
    &MEDIASUBTYPE_AnalogVideo_PAL_N    ,"(N) PAL",
    &MEDIASUBTYPE_AnalogVideo_SECAM_B  ,"(B) SECAM",
    &MEDIASUBTYPE_AnalogVideo_SECAM_D  ,"(D) SECAM",
    &MEDIASUBTYPE_AnalogVideo_SECAM_G  ,"(G) SECAM",
    &MEDIASUBTYPE_AnalogVideo_SECAM_H  ,"(H) SECAM",
    &MEDIASUBTYPE_AnalogVideo_SECAM_K  ,"(K) SECAM",
    &MEDIASUBTYPE_AnalogVideo_SECAM_K1 ,"(K1) SECAM",
    &MEDIASUBTYPE_AnalogVideo_SECAM_L  ,"(L) SECAM",
};

const char* GetTextForMajorType(const GUID& guid);
const char* GetTextForSubType(const GUID& guidMajor, const GUID& guidSub);

///	DirectDraw の 返り値をチェックし、エラーがあればエラーメッセージを表示する。
class WXAMError:public WXDXError{
	public:
	virtual bool Check(DWORD n, const char* msg, const char* fn=NULL, int ln=0);
	static bool AMErrorToString(char*& err, char*& desc, HRESULT error);
	static WXAMError& Error();
};
}
#endif
