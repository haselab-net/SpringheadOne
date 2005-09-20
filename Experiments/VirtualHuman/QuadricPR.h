// QuadricPR.h: QuadricPR �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////
#ifndef QUADRICPR_H
#define QUADRICPR_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include <vector>
#include "THuman.h"
#include "quadpr.h"		//�񎟌v��@�ɕK�v�ȃw�b�_�[�i���g�̐錾�R�s�y�ł��n�j�j

class QuadricPR  
{
private:
	//QuadraticPR
	//�ÓI
	//����
	long int input[14];		//�e�ݒ�ϐ��i�K��14�j
	float tol[2];			//���e�덷�i�K���Q�j
	char title[18];			//���̃^�C�g��
	char pfile[14];			//�o�͐�̃t�@�C���̖��O
	int title_len;
	int pfile_len;
	
	//�o�͐�
	float obj[1];			//�ړI�֐��̒l�i�œK���������������́j
	long int iout[2];		//�v�Z�̌��ʂ�\���p�����[�^�i�K���Q�j

	//���I�inew�Ō�Ɋm�ۂ���j
	//�񎟌��z��̂��̂ł�quadpr()���ꎟ���ł����󂯕t���Ă��Ȃ��̂ňꎟ���ɂ��Ă���
	//����
	float* Quad;			//�ړI�֐��̓񎟌W���x�N�g���i�l�~�m�j
	float* cost;			//�ړI�֐��̈ꎟ�W���x�N�g���i�m�j
	float* A;				//��������̍��ӂ̌W���x�N�g���i�l�~�m�j
	float* rhs;				//��������̉E�ӂ̌W���x�N�g���i�l�j
	long int* kt;			//��������̕s�����i�l�j(-1:>=,0:=,1:<=)

	//�o�͐�
	float *solution;		//�œK���i�m�j
	float *rc;				//the reduced cost voctor(N)
	float *dual;			//dual solution vecotr(N)
	float *slk;				//�X���b�N�X�ϐ��̃x�N�g���i�l�j
	
	float *workSpace;		//���[�N�X�y�[�X�i�T�C�Y�́A��������̎��ɓ���������ꍇ�F2*(M+N)^2+12*(M+N)+16
							//							��������̎��ɓ������Ȃ��ꍇ�F2*(M+N)+8*(M+N)+6

	std::vector<float>* jointOpt;

public:
	QuadricPR();
	virtual ~QuadricPR();
	void CalcQuadPR(THuman& human, int numConst, int numVari, bool flagEq);	//�񎟌v���������
	//numConst:����������̐�(M)�AnumVari:�ϐ��̐�(N)

};

#endif // !defined(AFX_QUADRICPR_H__F6084177_9D43_4B10_8028_E44F126CFC0E__INCLUDED_)
