#include "loadbmp.h"
#include <io.h>
#include <windows.h>
#include "../../common/plAnyDec.h"
#include "../../common/plExcept.h"
#include "../paintLib/plWinBmp.h"
#include <vector>

typedef std::vector<PLWinBmp*> Bmps;
Bmps bmps;

extern "C" PLWinBmp* LoadBmpGet(int hBmp){
	if (0 < hBmp && hBmp <= bmps.size()){
		return bmps[hBmp-1];
	}
	return NULL;
}
extern "C" int __declspec(dllexport) LoadBmpCreate(const char* fn){
	if (_access(fn, 4) == -1) return 0;
	PLAnyPicDecoder decoder;
	PLWinBmp* bmp = new PLWinBmp;
	decoder.MakeBmpFromFile(fn, bmp);
	if (bmp->GetWidth() <= 0) return 0;
	for(Bmps::iterator it = bmps.begin(); it != bmps.end(); ++it){
		if (!*it){
			*it = bmp;
			return it-bmps.begin() + 1;
		}
	}
	bmps.push_back(bmp);
	return bmps.size();
}
extern "C" int __declspec(dllexport) LoadBmpRelease(int h){
	if (h<=0 || h>bmps.size()) return 0;
	delete bmps[h-1];
	bmps[h-1] = NULL;
	return h;
}
extern "C" int __declspec(dllexport) LoadBmpGetWidth(int hBmp){
	PLWinBmp* bmp = LoadBmpGet(hBmp);
	if (bmp) return bmp->GetWidth();
	return 0;
}
extern "C" int __declspec(dllexport) LoadBmpGetHeight(int hBmp){
	PLWinBmp* bmp = LoadBmpGet(hBmp);
	if (bmp) return bmp->GetHeight();
	return 0;
}
extern "C" int __declspec(dllexport) LoadBmpGetBytePerPixel(int hBmp){
	PLWinBmp* bmp = LoadBmpGet(hBmp);
	if (bmp) return bmp->GetBitsPerPixel() / 8;
	return 0;
}
extern "C" int __declspec(dllexport) LoadBmpGetSize(int hBmp){
	return LoadBmpGetWidth(hBmp)*LoadBmpGetHeight(hBmp)*LoadBmpGetBytePerPixel(hBmp);	
}

extern "C" int __declspec(dllexport) LoadBmpGetBmp(int hBmp, char* ptr){
	PLWinBmp* bmp = LoadBmpGet(hBmp);
	if (!bmp) return 0;
	int tx = bmp->GetWidth();
	int ty = bmp->GetHeight();
	int nc = bmp->GetBitsPerPixel() / 8;
	bmp->Lock(true, false);
	for(int i=0; i<ty; ++i){
		memcpy(ptr+tx*nc*i, bmp->GetLineArray()[i], tx*nc);
	}
	bmp->Unlock();
	return tx*nc*ty;
}
