
#define 	WM_USBDIOEVENT  				(WM_USER+0x1000)
#define		USBDIO_INTERRUPT_START			0x01
#define		USBDIO_INTERRUPT_END			0x02
#define		USBDIO_INTERRUPT_ERROR			0x03
#define		USBDIO_INTERRUPT_UNITTIMEOUT	0x04
#define		USBDIO_INTERRUPT_ABORT			0x05

#define		USBDIO_READ_START   			0x10
#define		USBDIO_READ_END					0x11
#define		USBDIO_READ_ERROR   			0x12
#define		USBDIO_READ_STOP   			0x13

#define		USBDIO_WRITE_START   			0x20
#define		USBDIO_WRITE_END				0x21
#define		USBDIO_WRITE_ERROR   			0x22
#define		USBDIO_WRITE_STOP   			0x23

#define		USBDIO_GETFIFO_ERROR   			0x30
#define		USBDIO_FIFO_OVERFLOW   			0x31

#define		USBDIO_HANDSHAKE_START   		0x40
#define		USBDIO_HANDSHAKE_END			0x41
#define		USBDIO_HANDSHAKE_ERROR   		0x42
#define		USBDIO_HANDSHAKE_STOP   		0x43

#define		USBDIO_CLOCK_START   			0x50
#define		USBDIO_CLOCK_END				0x51
#define		USBDIO_CLOCK_ERROR   			0x52
// }

#define		PORT3		0x8		// PIO31-PIO24
#define		PORT2		0x4		// PIO23-PIO16
#define		PORT1		0x2		// PIO15-PIO 8
#define		PORT0		0x1		// PIO 7-PIO 0

#define		PORT_IN		0	// ì¸óÕï˚å¸
#define		PORT_OUT	1	// èoóÕï˚å¸

#define		DIOUNIT_TO_EXTERNAL		1	// Ãß≤ŸëóêM”∞ƒﬁ
#define		EXTERNAL_TO_DIOUNIT		0	// Ãß≤ŸéÛêM”∞ƒﬁ

#define		TXWIDTH_32PORT		0x04
#define		TXWIDTH_16PORT		0x02
#define		TXWIDTH_8PORT		0x01

/*
 *	DLLä÷êîÇÃ≤›Œﬂ∞ƒêÈåæ
 */
#define DllImport	__declspec( dllimport )
#pragma comment(lib, "UsbDio32")

#ifdef __cplusplus
extern "C" {
#endif

DllImport INT APIENTRY dioEnumUnit( HWND hWnd, PUSHORT pUnitId, USHORT MaxUnit );
DllImport INT APIENTRY dioOpenUnit( HWND hWnd, USHORT UnitId );
DllImport VOID APIENTRY dioCloseUnit( USHORT UnitId );
DllImport INT APIENTRY dioSetEventMode( HWND hwnd, USHORT UnitId, USHORT EventMode );
DllImport INT APIENTRY dioStartInterrupt( HWND hWnd, USHORT UnitId );
DllImport INT APIENTRY dioStopInterrupt( USHORT UnitId );
DllImport INT APIENTRY dioSetDirection( HWND hwnd, USHORT UnitId, USHORT PortDef, USHORT Direction );
DllImport INT APIENTRY dioOutPort( HWND hwnd, USHORT UnitId, USHORT PortId, UCHAR Value );
DllImport INT APIENTRY dioInPort( HWND hwnd, USHORT UnitId, USHORT PortId, PUCHAR pValToRead );
DllImport INT APIENTRY dioDWOutPort( HWND hwnd, USHORT UnitId, DWORD dwOutVal );
DllImport INT APIENTRY dioDWInPort( HWND hwnd, USHORT UnitId, PDWORD pdwInVal );
DllImport INT APIENTRY dioWOutPort( HWND hwnd, USHORT UnitId, USHORT PortId, USHORT wOutVal );
DllImport INT APIENTRY dioWInPort( HWND hwnd, USHORT UnitId, USHORT PortId, PUSHORT pwInVal );
DllImport INT APIENTRY dioStartWrite( HWND hWnd, USHORT UnitId, LPVOID pBufToWrite, ULONG nBytesToWrite );
DllImport INT APIENTRY dioStopWrite( USHORT UnitId );
DllImport INT APIENTRY dioStartRead( HWND hWnd, USHORT UnitId, LPVOID pBufToRead, ULONG nBytesToRead );
DllImport INT APIENTRY dioStopRead( USHORT UnitId );
DllImport INT APIENTRY dioGetNumInFifo( HWND hwnd, USHORT UnitId, PULONG pByteInFifo );
DllImport INT APIENTRY dioSetHandShakeMode( HWND hwnd, USHORT UnitId, USHORT TxDirection, USHORT TxWidth );
DllImport INT APIENTRY dioStartHandShake( HWND hwnd, USHORT UnitId, ULONG nBytes );
DllImport INT APIENTRY dioStopHandShake( USHORT UnitId );
DllImport INT APIENTRY dioSetClockMode( HWND hwnd, USHORT UnitId, USHORT TxDirection, USHORT TxWidth );
DllImport INT APIENTRY dioSetClock( HWND hwnd, USHORT UnitId, USHORT ClockTimer, USHORT TimeUnit );
DllImport INT APIENTRY dioStartClock( HWND hwnd, USHORT UnitId );
DllImport INT APIENTRY dioStopClock( HWND hwnd, USHORT UnitId );

#ifdef __cplusplus
}
#endif

