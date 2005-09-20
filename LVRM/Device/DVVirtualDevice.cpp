#include "DVVirtualDevice.h"
#include "Base/utilities.h"
#include <iostream>

namespace LVRM {


void DVVirtualDevice::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DVDevice type:\"" << Type() << "\" name:\"" << Name() << "\"/>" << std::endl;
	o.width(w);
}
}	//	namespace LVRM
