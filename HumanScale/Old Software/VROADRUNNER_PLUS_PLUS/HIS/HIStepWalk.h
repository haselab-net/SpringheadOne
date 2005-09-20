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
#define BUFFER 256

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
			offsetMin = 2.0f;//�����l�i�K���j
			offsetMax = 0.1f;//�����l�i�K���j
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
	float inner, resultOfInner[3], memoryOfInner;

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
	void SetDirection(Vec2f vector){ direction = vector; }
	int GetStep(){ return step; }
	void Print(std::ostream& os) const;
	float GetSensorVoltage(int n){ return sensor[n].Get(); }
	///�L�����u���C�V�����p
	void beforeCalib();
	void PSenRange();
	void PSenMax();
	void PSenMin();
	void ValueOutput();
	void SetCenterOfGravity(Vec2f& centerG);
	Vec2f GetCenterOfGravity(){ return centerOfGravity[0]; };
	void PSenCal();
	bool pSenCal; 
	bool printForCenterG;
	bool rotateSwitch;
	void SetThreshold(float t) { threshold = t; }
	void CloseSIGGRAPHFile();
	int GetNSENSOR(){ return NSENSOR; }

	void DataOutput(float x, float y, float z);

	std::ofstream *dataFileSIGGRAPH;
	char* folderName;
	char folderNameBack[20];
	int fileNameTime;
	char fileTime[10];


protected:

	bool SideStep();				///�T�C�h�X�e�b�v�̌��o
	Vec2f walkingDirection;			///���̓Z���T�݂̂��猟�o���ꂽ���݂̑����ݕ���
	Vec2f gravityDiff;				///���݂̏d�S�́i���ς��猩���j�ʒu	
	int sideStep;					///�T�C�h�X�e�b�v��
	bool frontCheck;				///���ʂ������Ă��邩�̃`�F�b�N
	float frontGap;					///�`�F�b�N�̍ې��ʂ���ǂꂭ�炢�̃Y���𖳎����邩

	float dwLastTime;               ///<    StepWalk���O��Ăяo���ꂽ���̃J�E���g
	float dwTime;                   ///<    StepWalk������Ăяo���ꂽ���̃J�E���g
	int step;						///<	����
	float walkVel;					///<	���s���x
	int backStep;                   ///<    ���ɐi��
	bool BackStep(float value);     ///<    ���ɐi�ނ��ǂ������f����
	unsigned long dwInterval;		///<	�����݊Ԋu
	unsigned long dwStepTime;		///<	�����݂����o�����u�Ԃ̎���
	float pitch;					///<	dwInterval�̋t��
	float pitchParam;               ///<    pitch�̈ړ����������p�ϐ�
	bool flag_SIGGRAPH;             ///<    SIGGRAPH�f�[�^���悤�t���O 
	float threshold;                ///<    �ړ������臒l
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

	FILE* calibValue;
	FILE* calibOutput;
	char filestr[BUFFER];
};

}

#endif
