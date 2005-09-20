// IHCDeviceManagerBase.h: IHCDeviceManagerBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IHCDEVICEMANAGERBASE_H__FF341B5B_3980_4F85_9B75_AF34F2A1B9C7__INCLUDED_)
#define AFX_IHCDEVICEMANAGERBASE_H__FF341B5B_3980_4F85_9B75_AF34F2A1B9C7__INCLUDED_

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/

class IHCDeviceManagerBase  
{
protected:
	unsigned short hexDA[8];
	short hexAD[8];
	short hexENC[8];
	volatile unsigned char ENC_PHS;

public:
	void OpSystemReset();
	void OpDACls();
	void OpRFIFOCls();
	void OpENCCls();
	void OpENCPHSSet();
	void OpENCGet();
	void OpADGet();
	void OpDASet();

public:
	void OpUSBOutPut();
	void OpSendPC(unsigned short* sendData);
	IHCDeviceManagerBase();
	~IHCDeviceManagerBase();

	void TimeMeasureStart(void);
	void TimeMeasureEnd(void);
};

#endif // !defined(AFX_IHCDEVICEMANAGERBASE_H__FF341B5B_3980_4F85_9B75_AF34F2A1B9C7__INCLUDED_)
