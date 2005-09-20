// IHCDeviceManager.h: IHCDeviceManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IHCDEVICEMANAGER_H__71A67AB8_8A30_4A97_B264_458946672B6F__INCLUDED_)
#define AFX_IHCDEVICEMANAGER_H__71A67AB8_8A30_4A97_B264_458946672B6F__INCLUDED_

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/


#include "IHCDeviceManagerBase.hpp"
#include "IHCSpidar4quad.hpp"
#include "IHCMotor.hpp"

#include "SH4DataSet.hpp"

#define MAX_MOTOR_SIZE 8
#define MAX_SPIDAR_SIZE 2

class IHCDebuger;

class IHCDeviceManager : public IHCDeviceManagerBase
{
protected:
	//Vec3f v_sendforce[MAX_SPIDAR_SIZE];
	int spidarSize;
	int motorSize;
	IHCMotor motor[MAX_MOTOR_SIZE];
	IHCSpidar4quad* pSpidar[MAX_SPIDAR_SIZE];
	IHCDebuger* pDebuger;
	CSH4DataSet dataSet;

public:
	float ShFreqHz;

public:
	void PrintStatus();
	void ADUpdate();
	void DAUpdate();
	void EncoderUpdate();
	void Update();
	void Init(IHCSpidar4quad& spidar);
	void PacketAnalyzer(unsigned short* usbData);
	void PacketAnalyzerNext(unsigned short* usbData);
	void PacketAnalyzerBase(unsigned short* usbData);
	IHCDeviceManager();
	~IHCDeviceManager();
	int SetTimerFreq(unsigned short *sfreq);

public:
	void ExOpAllDataSet(unsigned short *data);
	void ExOpSetMotorInfo(unsigned short* MotorData);
	void ExOpAllDataSend();

	void OpENCPHSSetReceive(unsigned short*);
	void OpDASetReceive(unsigned short*);
	void OpENCGetSend();
	void OpADGetSend();

};

#endif // !defined(AFX_IHCDEVICEMANAGER_H__71A67AB8_8A30_4A97_B264_458946672B6F__INCLUDED_)
