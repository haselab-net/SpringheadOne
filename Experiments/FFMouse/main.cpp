#include <WinDX/DDraw/WXDDraw.h>
#include <conio.h>
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
#include <Device/DVDeviceManager.h>
using namespace Spr;
using namespace std;
void main(){
	DVDeviceManager devMan;		//	デバイスドライバを入れておく入れ物
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0アンプ(ID 0)を登録
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	USB2.0アンプ(ID 0)を登録
	devMan.Init();
	DVDaBase* da = NULL;
	devMan.VPool().Rent(da);
	
	WXDDraws ddraws;
	if (ddraws[0].Create()){
		while(!kbhit()){
			POINT pt;
			GetCursorPos(&pt);
			if (ddraws[0].surfPrimary.Lock()){
				BYTE* line = ddraws[0].surfPrimary.Line(pt.y);
				DWORD bpp = ddraws[0].surfPrimary.BytePerPixel();
				BYTE* pixel = line +  bpp * pt.x;
				int value=0;
				if (bpp == 2){
					WORD p = *(WORD*) pixel;
					value = (p & 0x1F)*2 + ((p>>5) & 0x3F) + ((p>>11) & 0x1F)*2;
					value *= 4;
				}else if (bpp == 4){
					DWORD p = *(DWORD*) pixel;
					value = (p & 0xFF) + ((p>>8) & 0xFF) + ((p>>16) & 0xFF);
				}else{
					cout << "This pixel format is not supported." << endl;
				}
				cout << bpp << " : " << 255*3-value << endl;
				ddraws[0].surfPrimary.Unlock();
				float v = (float)value / (float)(255*3);
				v = 1-v;
				if (v>1) v = 1;
				if (v<0) v = 0;
				//	0.5オームなので，1V = 2A 最大0.5Aにしたい．
				if (da) da->Voltage(0.25f*v);
			}else{
				if (da) da->Voltage(0);
			}
		}
		getch();
	}else{
		cout << "Fail to create direct draw." << endl;
	}
}
