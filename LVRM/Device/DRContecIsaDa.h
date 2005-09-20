#ifndef VR_DRCONTECISADA_H
#define VR_DRCONTECISADA_H

#include "DRRealDevice.h"
#include "DVDaBase.h"

namespace LVRM {

///	ContecのISA D/Aカード用のドライバ.
class DRContecIsaDa:public DRRealDevice{
public:
	///	仮想デバイス
	class VirtualDevice:public DVDaBase{
	protected:
		int ch;
		DRContecIsaDa* realDevice;
		char name[100];
	public:
		VirtualDevice(DRContecIsaDa* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual const char* Name() const{ return name; }
	};
protected:
	char name[100];
	///	D/AのIOアドレス
	int address;
public:
	/**	コンストラクタ
		@param daA		D/AのI/Oアドレス
	*/
	DRContecIsaDa(int address);
	///	デバイスの名前
	virtual const char* Name() const { return name; }
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual void Register(DVVirtualDevicePool& vpool);

	///	電圧出力
	void Voltage(int ch, float v);
};


}	//	namespace LVRM

#endif