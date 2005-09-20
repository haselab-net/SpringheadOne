// RSPACE.h: RSPACE �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPACE_H__764D0CDA_F546_4215_9247_D430CE001023__INCLUDED_)
#define AFX_RSPACE_H__764D0CDA_F546_4215_9247_D430CE001023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//���{�b�g�ƂƂ��Ɉړ������Ԃ��L�q����N���X
//2005.03.31
class RSPACE  
{

public:
	//��Ԃ̐��@	
	enum {
		SPACE_LENGTH = 100,
	    SPACE_WIDTH = 100,
	    SPACE_HIGH = 100	
	};
	//���������̋�Ԏ���
    static int ary_RSPACE[SPACE_LENGTH][SPACE_WIDTH][SPACE_HIGH];

	typedef int DataType;
	typedef struct{
		DataType data[SPACE_LENGTH];
	}sqlist;
	
	//��Ԃ̒��Ƀ��{�b�g�̑��Έʒu
	int DISTANCE_AFTER_ROBOTX;
	static const int DISTANCE_AFTER_ROBOTY;
	static const int DISTANCE_AFTER_ROBOTZ;

	//���{�b�g�̈ړ���
	int i_moveX,
	    i_moveY,
	    i_moveZ;
	

public:
	RSPACE();
	virtual ~RSPACE();

	//get�@���{�b�g�̈ړ���
	void get_RMovement(int i_x,int i_y,int i_z);


};

#endif // !defined(AFX_RSPACE_H__764D0CDA_F546_4215_9247_D430CE001023__INCLUDED_)
