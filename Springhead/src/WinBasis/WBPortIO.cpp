#include "WinBasis.h"
#pragma hdrstop
#include "WBPortIO.h"
#include "WBWinDriver.h"

namespace Spr {

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

unsigned char bcb_inp(unsigned short adr){
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
unsigned short bcb_inpw(unsigned short adr){
	unsigned short rv;
	asm{
		xor     eax,eax
		mov     dx, adr
		in      ax, dx
		mov     rv, ax
	}
	return rv;
}
void bcb_outpw(unsigned short adr, unsigned short v){
	asm{
		xor     eax,eax
		mov     dx, adr
		mov     ax, v
		out     dx, ax
	}
}


#endif

}	//	namespace Spr;
