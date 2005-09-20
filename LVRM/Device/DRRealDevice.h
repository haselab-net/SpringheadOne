#ifndef VR_DRREALDEVICEBASE_H
#define VR_DRREALDEVICEBASE_H

#include "DVVirtualDevicePool.h"

namespace LVRM {

///	実デバイスの基本クラス．ISAカード1枚などに相当する.
class DRRealDevice{
public:
	DRRealDevice(){}
	virtual ~DRRealDevice(){}
	///	デバイスの名前
	virtual const char* Name() const=0;
	///	初期化
	virtual bool Init()=0;
	///	仮想デバイスの登録
	virtual void Register(DVVirtualDevicePool& vpool)=0;
	///	状態の更新
	virtual void Update(){}
	///	ダンプ出力
	virtual void Print(std::ostream& o) const;
};
inline std::ostream& operator << (std::ostream& o, const DRRealDevice& d){
	d.Print(o);
	return o;
}
}	//	namespace LVRM

#endif
