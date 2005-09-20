#ifndef HID_STEPWALK_H
#define HID_STEPWALK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Device/DVDeviceManager.h>
#include <Device/DVAdBase.h>
#include <Base/Affine.h>

namespace LVRM{;
using namespace std;

#define SENSOR4 //�Z���T�[��4�̂Ƃ�
//#define SIDESTEP //to use sidestep and backstep, please define it.
//#define ACTION   //to use jump and squat actions, please define it.
//#define SLC_USE //���x�����O�J�������g���Ƃ�
#define BUFFER 256 //���o�̓t�@�C���̃o�b�t�@�T�C�Y

class HIStepWalk{
	//	�f�[�^���ő�l�ƍŏ��l�Ő��K������N���X�D
	class Sensor{
	public:
		float offsetMin;//�L�����u���C�V�����p�ɐV���ɍ����
		float offsetMax;// 2002/6/18
//		float offset; //��L�̑O�܂ō]������̃v���O�����Ŏg���Ă�������
		float scale;
		float rawCur;
		float cur;
	public:
		Vec2f pos;	//	�Z���T�̈ʒu(���[�g���P��)
		DVAdBase* ad;
		Sensor(){
//			offset = 2.5f;
			offsetMin = 3.0f;//�����l�i�K���j
			offsetMax = 0.5f;//�����l�i�K���j
			scale = 2.0f;//�����l�i�K��?�j
			rawCur = 0;
			cur = 0;
			ad = NULL;
		}
		void CalibMin(){
			if (ad) rawCur = ad->Voltage();
			if (rawCur < offsetMin) offsetMin = rawCur;
		}
		void CalibMax(){
			if (ad) rawCur = ad->Voltage();
			if (rawCur > offsetMax) offsetMax = rawCur;
		}
		void SetMin(){
			if (ad) rawCur = ad->Voltage();
			offsetMin = rawCur;
		}
		void SetMax(){
			if (ad) rawCur = ad->Voltage();
			offsetMax = rawCur;
		}
		///	���K����̒l��Ԃ�(0����1).
		float Get(){
			if (ad) rawCur = ad->Voltage();
			cur = (rawCur - offsetMin) / scale;
//			if (cur > 1) cur = 1; //1���傫���Ȃ��Ă������񂶂�Ȃ����H
//			if (cur < 0) cur = 0; //0��菬�����Ȃ��Ă������񂶂�Ȃ����H
//			std::cout << "cur = " << cur << std::endl;
			return cur;
		}
		/*�]�����񂪍��������2002/6/18��L�ɕύX
		void Calib(){
			if (ad) rawCur = ad->Voltage();
			if (rawCur < offset) offset = rawCur;
		}
		///	���K����̒l��Ԃ�(0����1).
		float Get(){
			if (ad) rawCur = ad->Voltage();
			cur = (rawCur - offset) / scale;
			if (cur > 1) cur = 1;
			if (cur < 0) cur = 0;
			return cur;
		}
		*/
	};

public:
	Vec2f cogAve, cog, direction;
	float inner, inner2, resultOfInner[3], memoryOfInner;

	HIStepWalk();
	virtual ~HIStepWalk();
	///	������
	bool Init(DVDeviceManager& devMan);
	///	�{�́D��������20ms���ƂɌĂяo���D	
	void Step(float);
	float GetVelocity(){ return walkVel; }
	float GetPitch(){ return pitch; }
	float GetPitchParam(){ return pitchParam; }
	void  SetPitchParam(float p){ pitchParam = p; }
	Vec2f GetDirection(){ return direction; }
	int GetNumSensor() { return NSENSOR; }
	void SetDirection(Vec2f vector){ direction = vector; }
	int GetStep(){ return step; }
	void Print(std::ostream& os) const;
	float GetSensorVoltage(int n){ return sensor[n].Get(); }
	float GetInner() { return inner; }
	void SetInner(float a) {inner = a;}
	bool GetIsWalking() { return isWalking; }
	///�L�����u���C�V�����p
	void ValueOutput();
	void beforeCalib();
	void PSenRange();
	void PSenMax();
	void PSenMin();
	void PSenSetMax();
	void PSenSetMin();
	void SetCenterOfGravity(Vec2f& centerG);
	Vec2f GetCenterOfGravity(){ return centerOfGravity[0]; };
	void PSenCal();
	bool pSenCal; 
	bool printForCenterG;
	bool rotateSwitch;
	void SetThreshold(float t) { threshold = t; }
	void CloseSIGGRAPHFile();

	void DataOutput(float x, float y, float z);

	std::ofstream *dataFileSIGGRAPH;
	char* folderName;
	char folderNameBack[20];
	int fileNameTime;
	char fileTime[10];

	float jumpPower;
	void SetJump(float& jump);

	void SetAlpha(float a) { alpha = a; }
	void SetBeta(float b) { beta = b; }

	/// �����������Z�b�g(�������̓��x�����O�J�����ɂĎ擾)
	void SetEyeAngle(float setEyeAngle) {eyeAngle = setEyeAngle;}

	void SetStatusOfUser(int n) {statusOfUser = n;}
	int GetStatusOfUser() {return statusOfUser;}
	float GetSideStepDirection();

protected:

	bool SideStep();				///�T�C�h�X�e�b�v�̌��o
	Vec2f walkingDirection;			///���̓Z���T�݂̂��猟�o���ꂽ���݂̑����ݕ���
	Vec2f gravityDiff;				///���݂̏d�S�́i���ς��猩���j�ʒu	
	int sideStep;					///�T�C�h�X�e�b�v��
	bool frontCheck;				///���ʂ������Ă��邩�̃`�F�b�N
	float frontGap;					///�`�F�b�N�̍ې��ʂ���ǂꂭ�炢�̃Y���𖳎����邩
	float offset[4];                ///�^�[���e�[�u�����X���Ă���Ƃ����̓Z���T����Ƃ�l��ω����������iVM�����X���Ă邩������Ȃ�����j

	float dwLastTime;               ///<    StepWalk���O��Ăяo���ꂽ���̃J�E���g
	float dwTime;                   ///<    StepWalk������Ăяo���ꂽ���̃J�E���g
	float dt;
	int step;						///<	����
	int backStep;                   ///<    ���ɐi��
	bool BackStep(float value);     ///<    ���ɐi�ނ��ǂ������f����
	float walkVel;					///<	���s���x	
	unsigned long dwInterval;		///<	�����݊Ԋu
	unsigned long dwStepTime;		///<	�����݂����o�����u�Ԃ̎���
	float pitch;					///<	dwInterval�̋t��
	float pitchParam;               ///<    pitch�̈ړ����������p�ϐ�
	bool flag_SIGGRAPH;             ///<    SIGGRAPH�f�[�^���悤�t���O 
	float threshold;                ///<    �ړ������臒l
	float alpha, beta;              ///<    �d�S�ړ��̐ϕ�����
	bool isWalking;                 ///<    �����Ă邩�ǂ����̔���t���O

	int FootSide();                 ///<    �ǂ���̑��ő����݂����Ă���̂�������.
	Vec2f nowDCenterG;              ///<    �֐�FootSide()�p.CheckDirection�ł��g��
	Vec2f lastDCenterG;             ///<    �֐�FootSide()�p.����pCheckDirection�ł��g��
	float nowDCenterAngle;          ///<    �֐�FootSide()�p.CheckDirection�ł��g��
	float lastDCenterAngle;         ///<    �֐�FootSide()�p.CheckDirection�ł��g��
	int footSide;                   ///<    �֐�FootSide()�p.1���E��,-1������,0���s���̂Ƃ�
	bool footSideFlag;              ///<    �֐�FootSide()�p.
	Vec2f vecFooToFoot;             ///<    �֐�FootSide()�p.
	Vec2f firstVec[2];              ///<    �֐�FootSide()�p.
	float centerGNorm;              ///<    �֐�FootSide()�p.
	float thresholdFootHigh;        ///<    �֐�FootSide()�p.
	float thresholdFootLow;         ///<    �֐�FootSide()�p.
	int testFootSide[3];

	Vec2f CheckDirection();         ///<	���[�U�̌��������΂łȂ������ׂ�D�Ԃ�l�͒��ׂ���̌���
	bool checkSideDirection;
	int checkSide;
	int checkNum;
	float checkDir[11];
	Vec2f footToFoot;
	Vec2f userDir;
	int badNum;

	float SumSensor();              ///<	�Z���T�[�̒l�̘a
	float sumSensors[6];            ///<	�Z���T�[�̒l�̘a �����Ƃ���5�Ƃ�
	float sumSensorAve;             ///<	�Z���T�[�̒l�̘a�̕��� <- 1.0�ɋ߂��l�ɂ���.(offset[4]���g����.��ق�
	float sumSensorDef;
	typedef struct
	{
		float value;                ///<	�Z���T�[�̒l�̘a
		float dCenterG;             ///<	�׏d���S�̕ω���
		float time;                 ///<	value��ʂ�������
	} flagSumSensor;
	flagSumSensor sumSensorMax[2];
	bool sumSensorMaxFlag;
	flagSumSensor sumSensorBig;
	flagSumSensor sumSensorMin[2];
	bool sumSensorMinFlag;
	flagSumSensor sumSensorSmall;
	float sumTime;
	void LogChangeMax();
	void LogChangeMin();

	typedef struct
	{
		float value;                ///<	�׏d���S�̕ω���
		float sumSensorValue[2];       ///<	�������̃Z���T�̘a�̒l
		Vec2f cog;                  ///<	�������̉׏d���S�̈ʒu
		float time;
	} dCenterGMaxLog;
	void LogChangeDCenterGMax();
	dCenterGMaxLog dCenterGMax[2];
	bool dCenterGMaxFlag;

	int statusOfUser;               ///<	���[�U�̏�ԁ@0�F���g�p�@1�F�����@2�F�O�i�@3�F����@4�F��ށ@5�F�E�T�C�h�X�e�b�v�@6�F���T�C�h�X�e�b�v�@7�F�W�����v�@8�F���L�@9�F���L����߂�
	bool moving;                    ///<    ���[�U���ړ����悤�Ƃ��Ă��邩�̔���D
	int StatusOfUser();             ///<	���[�U�̏�Ԃ𒲂ׂ�.statusOfUser��Ԃ�l�Ƃ���.
	int notUsing;                   ///<	notUsing/1000�b���[�U���������ĂȂ���Ԃł����sumSensorBig,sumSensorSmall���������� �l�͍��̂Ƃ���K���i5000�j
	int sumMaxMinSetTime;           ///<	sumMaxMinSetTime/1000�b�̊Ԃ���sumSensorMax,sumSensorMin�̒l��ێ�. �l�͍��̂Ƃ���K���i2000�j
	typedef struct
	{
		Vec2f sideStepVec;
		float time;
	} sideStepLog;
	sideStepLog sideStepV;
	bool syagamiFlag;
	bool sideStepFlag;
	float sideStepStartTime;
	float dSumSensors;
	float dLastSumSensors;


	///	�Z���T�[��, �O�C���C�E��3�̃Z���T�[
#ifndef SENSOR4
	enum {NSENSOR = 3};
#else
	enum {NSENSOR = 4};
#endif
	///	�O�C���C�E�̃Z���T�[
	Sensor sensor[NSENSOR];

	///	�����̐�
	enum {CENTER_OF_GRAVITY_LEN = 6};
	///	�d�S�̍��W(�������܂�)
	Vec2f centerOfGravity[CENTER_OF_GRAVITY_LEN];
	///	�d�S�̈ړ����x
	float dCenterOfGravity;

	/// ��������(���x�����O�J�����ɂĎ擾)
	float eyeAngle;

	//  �L�����u���[�V�������ʕۑ��p�t�@�C��
	FILE* calibValue;
	FILE* calibOutput;
	char filestr[BUFFER];
};

}

#endif
