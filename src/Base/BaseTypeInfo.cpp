#include "Base.h"
#ifdef _WIN32
#pragma hdrstop
#endif

namespace Spr{;
bool UTTypeInfo::Inherit(const UTTypeInfo* key) const {
	const UTTypeInfo* info = this;
	while (info){
#ifdef __BORLANDC__
		if(strcmp(info->ClassName(),key->ClassName())==0) return true;
#else
		if(info == key) return true;
#endif
		info = info->base;
	}
	return false;
}
bool UTTypeInfo::Inherit(const char* key) const {
	const UTTypeInfo* info = this;
	while (info){
		if(strcmp(info->ClassName(),key)==0) return true;
		info = info->base;
	}
	return false;
}
char* UTAcastError(const char* str){
	DSTR << "ACAST: " << str << std::endl;
	assert(0);
	return NULL;
}

}
