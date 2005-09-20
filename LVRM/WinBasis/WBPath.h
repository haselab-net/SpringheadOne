#ifndef WINCMP_MISC_CPATH_H
#define WINCMP_MISC_CPATH_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WBWin.h"

namespace LVRM {
class WBPath{
protected:
	std::string path;
	std::string file;
	std::string main;
	std::string ext;
public:
	const char* Path(){ return path.c_str(); }
	void Path(const char* p){
		if (p != path.c_str()) path = p;
	}
	bool Search(const char* file);
	const char* File();
	const char* Main();
	const char* Ext();
};

}	//	namespace LVRM

#endif

