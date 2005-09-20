// IHCDeviceManager.cpp: IHCDeviceManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "IHCDeviceManager.hpp"
#include "IHCDebuger.hpp"
#include "PacketDEF.h"
#include "TMUCtrl.h"
//#include "iodefine.h"
#include <string.h>
#include <cyg/hal/sh_regs.h>
#include <cyg/hal/hal_io.h>

#ifndef WIN32
//#include <machine.h>
#include "builtin.inl"
//#include "IHCPrinter.h"
#include <cyg/libc/stdio/stdio.h>
#endif //WIN32

#define PACKET_PRINT(s) printf("%s\r\n",(s))
//#define USB_PRINTOUT() //OpUSBOutPut()

//#define _DEBUG_PRINT
#ifdef _DEBUG_PRINT
#define DEBUG_PACKET_PRINT(s) printf("%s\r\n",(s))
#else
#define DEBUG_PACKET_PRINT(s) 
#endif //_DEBUG_PRINT

#ifdef USB_IO
#define USB_OUTPUT() OpUSBOutPut()
#else
#define USB_OUTPUT() 
#endif //USB_IO

#define VERSION "2.3.3"
const char CopyRight[] = "Intelligent Haptic Controller [Version "VERSION"]\r\n(C) Copyright 2002-2003 Sato&Koike Lab.\r\n";

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

IHCDeviceManager::IHCDeviceManager()
{
	printf("%s for SPIDAR (SPIDAR4quad)\r\n",CopyRight);
//	printf(" for SPIDAR (SPIDAR4quad)\r\n");
	spidarSize = 0;
	motorSize = 0;
	this->pDebuger = NULL;
}

IHCDeviceManager::~IHCDeviceManager()
{
	printf("----------------------------------------------------------\r\n");
	printf("Device Manager Destructor\r\n");
	printf("Thank you.\r\n\r\n");
}

//////////////////////////////////////////////////////////////////////////////
// PacketAnalyzerBase
//
// ��{�I�Ȗ��߂̃p�P�b�g���
void IHCDeviceManager::PacketAnalyzerBase(unsigned short *usbData)
{
	if((OP_DA_SET & *usbData) != 0){
		DEBUG_PACKET_PRINT("OP_DA_SET");
		OpDASetReceive(usbData+1); //
		USB_OUTPUT();
	}
	else if((OP_ENC_PHS & *usbData) != 0){
		PACKET_PRINT("OP_ENC_PHS");
		OpENCPHSSetReceive(usbData+1); //
		USB_OUTPUT();
	}
	else{
		if((OP_ENC_HOLD & *usbData) != 0){
			//DEBUG_PACKET_PRINT("OP_ENC_HOLD");
			PACKET_PRINT("OP_ENC_HOLD");
			OpENCGetSend(); //
			return;
		}
		else if((OP_AD_HOLD & *usbData) != 0){
			//DEBUG_PACKET_PRINT("OP_AD_HOLD");
			PACKET_PRINT("OP_AD_HOLD");
			OpADGetSend(); //
			return;
		}
		else if((OP_R_FIFO_CLS & *usbData) != 0){
			PACKET_PRINT("OP_R_FIFO_CLS");
			OpRFIFOCls();
			USB_OUTPUT();
		}
		else if((OP_DA_CLS & *usbData) != 0){
			PACKET_PRINT("OP_DA_CLS");
			OpDACls();
			USB_OUTPUT();
		}
		else if((OP_ENC_CLS & *usbData) != 0){
			PACKET_PRINT("OP_ENC_CLS");
			OpENCCls();
			USB_OUTPUT();
		}
		else if((OP_SYSTEM_RESET & *usbData) != 0){
			PACKET_PRINT("OP_SYSTEM_RESET");
			OpSystemReset();
			USB_OUTPUT();
		}
		else{
			PACKET_PRINT("ERROR!!!!!!!!!!!!");
			//USB_OUTPUT();
		}
	}
	//USB_PRINTOUT();
}


//////////////////////////////////////////////////////////////////////////////
// PacketAnalyzerNext
//
// SPIDAR�̐���p���߃p�P�b�g��́i��͗D��x�F��j
void IHCDeviceManager::PacketAnalyzerNext(unsigned short *usbData)
{
	int i;
	if(OP_SPIDAR_EXIT == *usbData){//////////////////////////////////////////
//		TMU.TSTR.BIT.STR1 = 0; // �^�C�}�[�̃X�g�b�v
		TMUStop(0);
		for(i=0;i<spidarSize;i++){
			PACKET_PRINT("OP_SPIDAR_EXIT");
			pSpidar[i]->SetMinForce();
		}
		Update();
	}
	else if(OP_SPIDAR_STOP == *usbData){//////////////////////////////////////////
		if(pSpidar[0]->GetState() == pSpidar[0]->RUN){
//			TMU.TSTR.BIT.STR1 = 0; // �^�C�}�[�̃X�g�b�v
			TMUStop(0);
			for(i=0;i<spidarSize;i++){
				PACKET_PRINT("OP_SPIDAR_STOP");
				pSpidar[i]->SetState(pSpidar[i]->STOP);
			}
		}
	}
	else if(OP_SPIDAR_BEFORE_CALIB == *usbData){//////////////////////////////////
		if(pSpidar[0]->GetState() == pSpidar[0]->MOTOR_INIT 
		|| pSpidar[0]->GetState() == pSpidar[0]->STOP){
//			TMU.TSTR.BIT.STR1 = 0; // �^�C�}�[�̃X�g�b�v
//			TMUStop(0);
			for(i=0;i<spidarSize;i++){
				PACKET_PRINT("OP_SPIDAR_BEFORE_CALIB");
				pSpidar[i]->SetState(pSpidar[i]->BEFORE_CALIB);
				pSpidar[i]->BeforeCalib(); //
			}
		}
	}
	else if(OP_SPIDAR_CALIB == *usbData){/////////////////////////////////////////
		if(pSpidar[0]->GetState() == pSpidar[0]->BEFORE_CALIB){
//			TMU.TSTR.BIT.STR1 = 0; // �^�C�}�[�̃X�g�b�v
			TMUStop(0);
			for(i=0;i<spidarSize;i++){
				PACKET_PRINT("OP_SPIDAR_CALIB");
				pSpidar[i]->SetState(pSpidar[i]->CALIB);
				pSpidar[i]->Calib(); //
			}
		}
	}
	else if(OP_SPIDAR_AFTER_CALIB == *usbData){///////////////////////////////////
		if(pSpidar[0]->GetState() == pSpidar[0]->CALIB){
//			TMU.TSTR.BIT.STR1 = 0; // �^�C�}�[�̃X�g�b�v
			TMUStop(0);
			for(i=0;i<spidarSize;i++){
				PACKET_PRINT("OP_SPIDAR_AFTER_CALIB");
				pSpidar[i]->SetState(pSpidar[i]->AFTER_CALIB);
				pSpidar[i]->AfterCalib(); //
			}
		}
	}
	else if(OP_SPIDAR_START == *usbData){/////////////////////////////////////////
		if(pSpidar[0]->GetState() == pSpidar[0]->AFTER_CALIB || pSpidar[0]->GetState() == pSpidar[0]->STOP){
			if(SetTimerFreq(usbData+1) == 0){
//				TMU.TSTR.BIT.STR1 = 0; // �^�C�}�[�̃X�^�[�g
				TMUStart(0);
			}
			else{
//				TMU.TSTR.BIT.STR1 = 1; // �^�C�}�[�̃X�^�[�g
				TMUStart(0);
			}
			for(i=0;i<spidarSize;i++){
				PACKET_PRINT("OP_SPIDAR_START");
				pSpidar[i]->SetState(pSpidar[i]->RUN);
			}
		}
	}
	else if(OP_SPIDAR_MOTOR_INFO == *usbData){////////////////////////////////////
		if(pSpidar[0]->GetState() == pSpidar[0]->SYSTEM_INIT){
			for(i=0;i<spidarSize;i++){
				PACKET_PRINT("OP_SPIDAR_MOTOR_INFO");
				pSpidar[i]->SetState(pSpidar[i]->MOTOR_INIT);
			}
			ExOpSetMotorInfo(usbData+1);
		}
	}
	else if(OP_USB_OUTPUT == *usbData){
	}
	else if(OP_WORD_ANALYSIS == *usbData){ // PC�擾�f�[�^�t�H�[�}�b�g�ݒ�
		PACKET_PRINT("OP_WORD_ANALYSIS");
		printf(" words test\n\r");
		dataSet.SetAnalysisWords((char*)(usbData+1));
	}
	else{
		PacketAnalyzerBase(usbData); // ��{����
		return;
	}
	USB_OUTPUT();
}


//
// PacketAnalyzerNext
//
// SPIDAR�̃p�P�b�g��́i��͗D��x�F�ō��j
void IHCDeviceManager::PacketAnalyzer(unsigned short *usbData)
{
	if(OP_ALL_DATA_GET == *usbData){
		DEBUG_PACKET_PRINT("OP_ALL_DATA_GET");
		ExOpAllDataSend();				// Send data to PC.
		ExOpAllDataSet(usbData+1);		// process data from PC.
	}
	else if(OP_SPIDAR_UPDATE == *usbData){
		DEBUG_PACKET_PRINT("OP_SPIDAR_UPDATE");
		ExOpAllDataSend();				// Send data to PC.
		ExOpAllDataSet(usbData+1);		// Process data from PC.
	}
	else{
		PacketAnalyzerNext(usbData); // SPIDAR�̃p�P�b�g��� (SPIDAR��Ԑݒ�)
	}
}

//
// Init
//
// Connect Device and SPIDAR
void IHCDeviceManager::Init(IHCSpidar4quad &spidar)
{
	if(spidarSize < MAX_SPIDAR_SIZE){
		this->pSpidar[spidarSize++] = &spidar;
		spidar.pDevMan = this;

		for(int j=0;j<spidar.motorSize;j++){
			if(motorSize < MAX_MOTOR_SIZE){
				spidar.pMotor[j] = &motor[motorSize++];
			}
			else{
				printf("Spidar motor is full!!!\n\r");
			}
		}
		printf("Please Send SPIDAR information\n\r");
	}
	else{
		printf("Spidar system is full!!!\n\r");
	}
}

void IHCDeviceManager::PrintStatus(){
	int i,j;
	for(i=0;i<spidarSize;i++){
		printf("spidar[%d]:\n\r",i);
		for(j=0;j<pSpidar[i]->motorSize;j++){
			printf("    motor[%d]:\n\r",pSpidar[i]->pMotor[j] - &motor[0] );
		}
		printf("\n\r");
	}
	printf("\n\r");
}


//////////////////////////////////////////////////////////////////////////////
// �G���R�[�_�[�@�A�b�v�f�[�g (0.1kHz) �ᑬ
void IHCDeviceManager::EncoderUpdate(){
	OpENCGet();
	for(int i=0;i<8;i++){
		motor[i].calcLength(hexENC[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////
// D/A�@�A�b�v�f�[�g (5kHz) ����
void IHCDeviceManager::DAUpdate(){
	for(int i=0;i<8;i++){
		hexDA[i] = motor[i].calcHexDA();
	}
//	printf("\n\r");
	OpDASet(); // DA
}

//////////////////////////////////////////////////////////////////////////////
// A/D�@�A�b�v�f�[�g (5kHz) ����
void IHCDeviceManager::ADUpdate(){
	OpADGet();
	for(int i=0;i<8;i++){
		motor[i].calcVolt(hexAD[i]);
	}
}


//////////////////////////////////////////////////////////////////////////////
//
void IHCDeviceManager::Update()
{
#if 1
	this->DAUpdate();
	this->EncoderUpdate();
	this->ADUpdate();

#else
	int i = 8;
	IHCMotor* pmotor = motor;
	unsigned short* phexDA = hexDA;
	short* phexAD = hexAD;
	short* phexENC = hexENC;

	for(;i>0;i--){
		*phexDA++ = (int)( (*pmotor).amp  * (*pmotor).hexPerAmp );
		pmotor++;
	}

	OpENCGet();
	OpADGet();
	OpDASet();  // DA

	pmotor = motor;
	for(i=8;i>0;i--){
		(*pmotor).volt = 24.0f - *phexAD++ * (*pmotor).voltPerHex;
		(*pmotor).length = - *phexENC++ * (*pmotor).lengthPerPulse + (*pmotor).offsetLength;
		pmotor++;
	}
#endif
}

//
//
void IHCDeviceManager::OpENCPHSSetReceive(unsigned short* data)
{
	//�f�[�^�̃Z�b�g
	ENC_PHS = data[0]>>8;
	printf("%x\n\r",ENC_PHS);
	OpENCPHSSet();
}
void IHCDeviceManager::OpDASetReceive(unsigned short* data)
{
	int i;
	//�f�[�^�̃Z�b�g
	for(i=0;i<8;i++){
		hexDA[i] = data[i];
		printf("%d ",data[i]);
	}
	printf("\n\r");
	OpDASet();
}
void IHCDeviceManager::OpENCGetSend()
{
	OpENCGet();
	// �f�[�^�̃Z�b�g
	OpSendPC((unsigned short*)hexENC);
}
void IHCDeviceManager::OpADGetSend()
{
	OpADGet();
	// �f�[�^�̃Z�b�g
	OpSendPC((unsigned short*)hexAD);
}


//////////////////////////////////////////////////////////////////////////////
//
// Send data to PC.
void IHCDeviceManager::ExOpAllDataSend()
{
	unsigned short buf[256],*pbuf = buf;
	unsigned short *phexDA = hexDA;
	short *phexAD = hexAD;
	short *phexENC = hexENC;
	int i=8;
	for(;i>0;i--){
		*pbuf++ = (short)*phexENC++;
		*pbuf++ = (short)*phexAD++;
		*pbuf++ = (short)*phexDA++;
	}

	for(i=0;i<spidarSize;i++){
		memcpy(pbuf+6*i,pSpidar[i]->localPos.data,sizeof(float)*3); //���W�f�[�^�̃R�s�[
		memcpy(pSpidar[i]->sendPos.data,pSpidar[i]->localPos.data,sizeof(float)*3);
	}
	OpSendPC((unsigned short*)buf);
}


//////////////////////////////////////////////////////////////////////////////
// ExOpAllDataSet
//
// Update SPIDAR and Device using packet data.
void IHCDeviceManager::ExOpAllDataSet(unsigned short *data)
{
	int i;
#if 0
	TimeMeasureStart();
#endif

	for(i=0;i<spidarSize;i++){
		memcpy(pSpidar[i]->getForce.data,	data+10*i,sizeof(float)*3);
		memcpy(&pSpidar[i]->stiffnessK,		data+10*i+6,sizeof(float)); // data+10*i+6
		memcpy(&pSpidar[i]->dumperB,		data+10*i+8,sizeof(float)); // data+10*i+6+2
	}

	//Update(); ////

#if 0
	TimeMeasureEnd();
#endif

	return;
}


int IHCDeviceManager::SetTimerFreq(unsigned short *sfreq){
/*	
	float Pphi = 33000000.0f; //���ӎ��g��
	float Hz;// �ݒ���g��(Hz);
	memcpy(&Hz,sfreq,sizeof(float));

	printf("freq:%f\n\r",Hz);
	ShFreqHz = Hz;
	if(Hz == 0.0f) return 0;
	// SPIDAR ����p�^�C�}���荞�ݐݒ�
	//INTC.IPRA.BIT._TMU1 = 0x8; //���荞�ݗD�揇��=15(max)
	//TMU1.TCR.BIT.TPSC = 0; // P��/4 �ŃJ�E���g (P��=33MHz, P��/4=8.25MHz)
	//TMU1.TCR.BIT.UNIE = 1; //�A���_�t���[���荞�ݐ���
	TMU1.TCOR = TMU1.TCNT = (int)((Pphi/4.0)/Hz); //�^�C�}�R���X�^���g���W�X�^, �^�C�}�J�E���^
*/
	return 1;
}

#define MOTOR_INFO_SIZE ((3+6+5)*8)
void IHCDeviceManager::ExOpSetMotorInfo(unsigned short *MotorData)
{
	float tempf[MOTOR_INFO_SIZE];
	int i,j;
	if(sizeof(int) != sizeof(float)){
		printf("float�̕ϊ��Ɏ��s���܂����Bint�^�ƁAfloat�^�̃T�C�Y���قȂ�܂��B");
		return;
	}
	memcpy(tempf,MotorData,sizeof(float)*(MOTOR_INFO_SIZE));
	for(int i=0; i<MOTOR_INFO_SIZE; ++i){
		printf("%f", tempf[i]);
		if (i%4 == 0) printf("\n");
	}

	for(i=0,j=0;i<8;i++){
		motor[i].SetPos(tempf[j++],tempf[j++],tempf[j++]);
		//motor[i].pos.X() = tempf[j++];
		//motor[i].pos.Y() = tempf[j++];
		//motor[i].pos.Z() = tempf[j++];

		motor[i].SetAmpInfo(tempf[j++],tempf[j++],tempf[j++],tempf[j++]);
		//motor[i].ampPerNewton = tempf[j++];		// [Amp]/[Newton]
		//motor[i].hexPerAmp = tempf[j++];		// [Hex]/[Amp]
		//motor[i].lengthPerPulse = tempf[j++];	// [Length]/[Pulse]
		//motor[i].voltPerHex = tempf[j++];		// [Volt]/[HEX]

		motor[i].SetMotorInfo(tempf[j++],tempf[j++],tempf[j++],tempf[j++],tempf[j++],tempf[j++],tempf[j++]);
		//motor[i].minForce = tempf[j++];
		//motor[i].maxForce = tempf[j++];
		//motor[i].torqueConst = ;// [mNm/A]
		//motor[i].terminalResist = tempf[j++];// [��]
		//motor[i].terminalInduct = tempf[j++];// [mH]
		//motor[i].speedConst = tempf[j++];// [rpm/V]
		//motor[i].pulleyRadius = tempf[j++];// [mm]
	}

	//�f�[�^�̃Z�b�g
	ENC_PHS = MotorData[MOTOR_INFO_SIZE*2]>>8;
	printf("Encoder PHS[7:0] : %x\n\r",ENC_PHS);
	OpENCPHSSet();

	int val = 1;
	for(i=0;i<8;i++){
		if(ENC_PHS & val){
			motor[i].SetEncPhs(1);//B���@�J�E���g�A�b�v
		}
		else{
			motor[i].SetEncPhs(0);//A���@�J�E���g�A�b�v
		}
		val *= 2;
	}

	// ���[�^���o��
	for(i=0;i<8;i++){
		printf("Motor[%d]:",i);
		motor[i].printInfo();
	}
	printf("\n\r");

	for(i=0;i<spidarSize;i++){
		pSpidar[i]->InitMotor3();
	}
	return;
}

