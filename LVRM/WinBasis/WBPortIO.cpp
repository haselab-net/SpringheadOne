#include "WBWin.h"
#include "WBPortIO.h"
#include "WBWinDriver.h"

namespace LVRM {

void WBGetPortIO(){
	static bool bFirst = true;
	if (bFirst){
		bFirst = false;
		HANDLE h;
		h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(h == INVALID_HANDLE_VALUE) {
			WBLoadWindowsDriver giveIo;
			giveIo.File("giveio.sys");
			giveIo.Install();
			giveIo.Start();
			h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if(h == INVALID_HANDLE_VALUE) {
				DPF("Couldn't access giveio device\n");
			}
		}
		CloseHandle(h);
	}
}

#ifdef __BORLANDC__

unsigned int bcb_inp(unsigned short adr){
    unsigned char rv;
    asm{
        xor     eax,eax
        mov     dx, adr
        in      al,dx
        mov     rv, al
    }
    return rv;
}
void bcb_outp(unsigned short adr, unsigned char v){
    asm{
        xor     eax,eax
        mov     dx, adr
        mov     al, v
        out     dx, al
    }
}


#endif

}	//	namespace LVRM;
