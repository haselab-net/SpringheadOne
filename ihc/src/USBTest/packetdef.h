#ifndef _PACKET_DEF_H_
#define _PACKET_DEF_H_


#ifdef WIN32
// 低水準命令セット
#define OP_SYSTEM_RESET				0x0001
#define OP_R_FIFO_CLS				0x0002
#define OP_DA_CLS					0x0004
#define OP_DA_SET					0x0008
#define OP_ENC_HOLD					0x0010
#define OP_AD_HOLD					0x0020
#define OP_ENC_CLS					0x0040
#define OP_ENC_PHS					0x0080
// 高水準命令セット
#define OP_ALL_DATA_GET				0x0200
#define OP_SPIDAR_MOTOR_INFO		0x0400
#define OP_SPIDAR_BEFORE_CALIB		0x0600
#define OP_SPIDAR_CALIB				0x0800
#define OP_SPIDAR_AFTER_CALIB		0x0900
#define OP_SPIDAR_START				0x0a00
#define OP_SPIDAR_UPDATE			0x0b00
#define OP_SPIDAR_STOP				0x0c00
#define OP_SPIDAR_EXIT				0x0e00
#define OP_USB_OUTPUT				0x1000
#define OP_WORD_ANALYSIS			0x1200

#else //WIN32 //SH4のとき
// 低水準命令セット
#define OP_SYSTEM_RESET				0x0100
#define OP_R_FIFO_CLS				0x0200
#define OP_DA_CLS					0x0400
#define OP_DA_SET					0x0800
#define OP_ENC_HOLD					0x1000
#define OP_AD_HOLD					0x2000
#define OP_ENC_CLS					0x4000
#define OP_ENC_PHS					0x8000
// 高水準命令セット
#define OP_ALL_DATA_GET				0x0002 //0x0200
#define OP_SPIDAR_MOTOR_INFO		0x0004 //0x0400
#define OP_SPIDAR_BEFORE_CALIB		0x0006 //0x0600
#define OP_SPIDAR_CALIB				0x0008 //0x0800
#define OP_SPIDAR_AFTER_CALIB		0x0009 //0x0900
#define OP_SPIDAR_START				0x000a //0x0a00
#define OP_SPIDAR_UPDATE			0x000b //0x0b00
#define OP_SPIDAR_STOP				0x000c //0x0c00
#define OP_SPIDAR_EXIT				0x000e //0x0e00
#define OP_USB_OUTPUT				0x0010 //0x1000
#define OP_WORD_ANALYSIS			0x0012 //0x1200
#endif //WIN32

#endif //_PACKET_DEF_H_

