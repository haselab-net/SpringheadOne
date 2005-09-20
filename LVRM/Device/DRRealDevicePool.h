#ifndef VR_DRREALDEVICEPOOL_H
#define VR_DRREALDEVICEPOOL_H
#include "DRRealDevice.h"

namespace LVRM {

/**	実デバイスのプール. 
	ユーザはISAカードやPCカードなどの実デバイスをここに登録する．
	デバイスは初期化時に，仮想デバイス(DVVirtualDevices)に登録される．	*/
class DRRealDevicePool: std::vector<DRRealDevice*>{
public:
	DRRealDevicePool();
	~DRRealDevicePool();
	///	登録
	void Register(DRRealDevice* dev);
	///	初期化
	void Init(DVVirtualDevicePool& vpool);
	///	ダンプ出力
	void Print(std::ostream& o) const;
};
inline std::ostream& operator << (std::ostream& o, const DRRealDevicePool& d){
	d.Print(o);
	return o;
}
}	//	namespace LVRM

#endif
