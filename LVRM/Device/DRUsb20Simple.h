// DRUsb20Simple.h: DRUsb20Simple クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRUSB20SIMPLE_H__DF7D202F_F49C_485A_ACBA_27212954662F__INCLUDED_)
#define AFX_DRUSB20SIMPLE_H__DF7D202F_F49C_485A_ACBA_27212954662F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DVCounterBase.h"
#include "DVDaBase.h"
#include "DRRealDevice.h"
#include <WinBasis/WBWin.h>

namespace LVRM {

///	CyverseのUSB2.0(LDR-SPIDAR-AMP)のドライバ
class DRUsb20Simple : public DRRealDevice  
{
	///	仮想デバイス
	class VirtualDeviceDa:public DVDaBase{
	protected:
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceDa(DRUsb20Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	class VirtualDeviceCounter:public DVCounterBase{
	protected:
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceCounter(DRUsb20Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		///	カウンタ値の設定
		virtual void Count(long c){ realDevice->Count(ch, c); }
		///	カウンタ値の読み出し
		virtual long Count(){ return realDevice->Count(ch); }
		///	名前
		virtual const char* Name() const{ return name; }
	};
//----------------------------------------------------------------------------
protected:
	char name[100];
	int channel;
	HANDLE hSpidar;

	int sign[8]; //DA出力用の符号
	long count[8];
	long countOffset[8];
	float voltage[8];

public:
	DRUsb20Simple(int ch);
	virtual ~DRUsb20Simple();

	///	デバイスの名前
	virtual const char* Name() const { return name; }
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual void Register(DVVirtualDevicePool& vpool);
	
	///	電圧出力
	void Voltage(int ch, float v);
	///	カウンタ値の設定
	void Count(int ch, long c);
	///	カウンタ値の読み出し
	long Count(int ch);
	///	状態の更新
	virtual void Update();

	/// リセット
	void Reset();

};

} //namespace LVRM

#endif // !defined(AFX_DRUSB20SIMPLE_H__DF7D202F_F49C_485A_ACBA_27212954662F__INCLUDED_)
