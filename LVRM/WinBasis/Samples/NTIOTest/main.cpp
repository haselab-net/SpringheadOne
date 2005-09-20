#include <WinCmp/Misc/CPortIO.h>

void main(){
	GetIO();
	_outp(0x300, 0);
	int rv = _inp(0x300);
}
