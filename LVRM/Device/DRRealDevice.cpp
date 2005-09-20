#include "DRRealDevice.h"
#include "Base/utilities.h"
#include <iostream>

namespace LVRM {


void DRRealDevice::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DRDevice name:\"" << Name() << "\"/>" << std::endl;
	o.width(w);
}
}	//	namespace LVRM
