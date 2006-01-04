#ifndef Spr_DSHOW_AMBASECLASSES_H
#define Spr_DSHOW_AMBASECLASSES_H

#ifdef _MSC_VER
#if _MSC_VER>=1100
 #define AM_NOVTABLE __declspec(novtable)
#else
 #define AM_NOVTABLE
#endif
#else
 #define AM_NOVTABLE
 #undef _X86_
#endif
#ifndef InterlockedExchangePointer
	#define InterlockedExchangePointer(Target, Value) \
   (PVOID)InterlockedExchange((PLONG)(Target), (LONG)(Value))
#endif

#include <amstream.h>
#include <uuids.h>
#include <strmif.h>
#include <amvideo.h>
#include <control.h>

#if DIRECTDRAW_VERSION >= 0x700
#include <videoacc.h>
#if 0	//	DirectX 7.0 ÇÕÇ±ÇøÇÁ
#include <../samples/Multimedia/DirectShow/BaseClasses/reftime.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/wxdebug.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/wxutil.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/combase.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/wxlist.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/mtype.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/amfilter.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/ctlutil.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/renbase.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/source.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/amextra.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/dllsetup.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/transfrm.h>
#include <../samples/Multimedia/DirectShow/BaseClasses/transip.h>
#else	//	DirectX 8.0 à»ç~ÇÕëΩï™Ç±ÇøÇÁ
#include <../samples/C++/DirectShow/BaseClasses/reftime.h>
#include <../samples/C++/DirectShow/BaseClasses/wxdebug.h>
#include <../samples/C++/DirectShow/BaseClasses/wxutil.h>
#include <../samples/C++/DirectShow/BaseClasses/combase.h>
#include <../samples/C++/DirectShow/BaseClasses/wxlist.h>
#include <../samples/C++/DirectShow/BaseClasses/mtype.h>
#include <../samples/C++/DirectShow/BaseClasses/amfilter.h>
#include <../samples/C++/DirectShow/BaseClasses/ctlutil.h>
#include <../samples/C++/DirectShow/BaseClasses/renbase.h>
#include <../samples/C++/DirectShow/BaseClasses/source.h>
#include <../samples/C++/DirectShow/BaseClasses/amextra.h>
#include <../samples/C++/DirectShow/BaseClasses/dllsetup.h>
#include <../samples/C++/DirectShow/BaseClasses/transfrm.h>
#include <../samples/C++/DirectShow/BaseClasses/transip.h>
#endif

#endif

#endif
