#include "RDView.h"
namespace LVRM {;

RDView::RDView(){
	invalidated = true;
	for(int i=0; i<sizeof(keyboard)/sizeof(keyboard[0]); ++i){
		keyboard[i] = 0;
	}
	for(int i=0; i<sizeof(specialKey)/sizeof(specialKey[0]); ++i){
		specialKey[i] = 0;
	}
}
RDView::~RDView(){
}

}	//	namespace LVRM;
