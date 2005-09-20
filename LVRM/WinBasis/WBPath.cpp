#include "WBPath.h"

namespace LVRM {
bool WBPath::Search(const char* file){
	char* fn=NULL;
	char buf[1024];
	if (!SearchPath(NULL, file, NULL, sizeof(buf), buf, &fn)) return false;
	path = buf;
	return true;
}
const char* WBPath::File(){
	char buf[1024];
	char buf2[1024];
	_splitpath(path.c_str(), NULL, NULL, buf, buf2);
	file = buf;
	file += buf2;
	return file.c_str();
}
const char* WBPath::Main(){
	char buf[1024];
	_splitpath(path.c_str(), NULL, NULL, buf, NULL);
	main = buf;
	return main.c_str();
}
const char* WBPath::Ext(){
	char buf[1024];
	_splitpath(path.c_str(), NULL, NULL, NULL, buf);
	ext = buf;
	return ext.c_str();
}

}	//	namespace LVRM
