#include "DRRealDevicePool.h"
#include "Base/Utilities.h"
#include <iostream>

namespace LVRM {


DRRealDevicePool::DRRealDevicePool(){
}
DRRealDevicePool::~DRRealDevicePool(){
	for(iterator it = begin(); it != end(); ++it){
		delete *it;
	}
	clear();
}
///	“o˜^
void DRRealDevicePool::Register(DRRealDevice* dev){
	push_back(dev);
}
void DRRealDevicePool::Init(DVVirtualDevicePool& vpool){
	for(iterator it = begin(); it != end(); ++it){
		if ((*it)->Init()) (*it)->Register(vpool);
	}
}
void DRRealDevicePool::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DRRealDevicePool>" << std::endl;
	o.width(w+2);
	for(const_iterator it = begin(); it != end(); ++it){
		o << **it;
	}
	o.width(0);
	o << UTPadding(w) << "</DRRealDevicePool>" << std::endl;
	o.width(w);
}

}	//	namespace LVRM
