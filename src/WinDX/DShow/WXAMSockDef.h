#ifndef Spr_DSHOW_WXAMSOCKDEF_H
#define Spr_DSHOW_WXAMSOCKDEF_H
namespace Spr {;
enum {
	PID_NEWSEGMENT,
	PID_MEDIASAMPLE,
};
struct WXAMSockPacketNewSegment{
	REFERENCE_TIME tStart;
	REFERENCE_TIME tStop;
	double dRate;
};

}	//	namespace Spr;

#endif
