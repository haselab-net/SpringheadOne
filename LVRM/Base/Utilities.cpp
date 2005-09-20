#include "utilities.h"
#include <iostream>
namespace LVRM {
using namespace std;

#ifdef _DEBUG
int UTRefCount::nObject;
UTRefCount::UTRefCount(){
	refCount = 0;
	nObject++;
}
UTRefCount::UTRefCount(const UTRefCount&){
	refCount = 0;
	nObject++;
}
UTRefCount::~UTRefCount(){
	assert(refCount==0); 
	nObject--;
}
#endif

std::ostream& operator << (std::ostream& os, UTPadding p){
	if (p.len == -1){
		p.len = os.width();
	}
	for(int i=0; i<p.len; i++) os << ' ';
	return os;
}
std::istream& operator >> (std::istream& is, const UTEatWhite& e){
	while(is.good()){
		char ch = is.peek();
		if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') break;
		is.get();
	}
	return is;
}

}	//	namespace LVRM
