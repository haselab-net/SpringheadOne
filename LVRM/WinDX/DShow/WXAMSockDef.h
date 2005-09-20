#ifndef LVRM_DSHOW_WXAMSOCKDEF_H
#define LVRM_DSHOW_WXAMSOCKDEF_H
namespace LVRM {;
enum {
	PID_NEWSEGMENT,
	PID_MEDIASAMPLE,
};
struct WXAMSockPacketNewSegment{
	REFERENCE_TIME tStart;
	REFERENCE_TIME tStop;
	double dRate;
};

}	//	namespace LVRM;

#endif
