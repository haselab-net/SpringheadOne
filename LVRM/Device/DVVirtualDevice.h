#ifndef VR_DVDEVICEBASE_H
#define VR_DVDEVICEBASE_H

#include "Base/Debug.h"
#include "Base/utilities.h"
#include <iosfwd>

namespace LVRM {

//----------------------------------------------------------------------------
//	DVVirtualDevice

class DRRealDevice;

///	仮想デバイスの基本クラス．DA,カウンタなどの1チャンネルに相当する.
class DVVirtualDevice:public UTRefCount{
	bool used;
	virtual bool GetUsed(){return used;}
	virtual void SetUsed(bool b){used = b;}
	friend class DVVirtualDevicePool;
public:
	DVVirtualDevice(){ used = false; }
	~DVVirtualDevice(){}
	///	デバイスの名前
	virtual const char* Name() const=0;
	///	デバイスの種類
	virtual const char* Type() const=0;
	///	実デバイスへのポインタ
	virtual DRRealDevice* RealDevice()=0;
	///	ダンプ出力
	virtual void Print(std::ostream& o) const;
	///
	virtual void Update(){}
};
inline std::ostream& operator << (std::ostream& o, const DVVirtualDevice& d){
	d.Print(o);
	return o;
}
}	//	namespace LVRM

#endif
