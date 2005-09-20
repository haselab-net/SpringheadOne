// RSPACE.h: RSPACE クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPACE_H__764D0CDA_F546_4215_9247_D430CE001023__INCLUDED_)
#define AFX_RSPACE_H__764D0CDA_F546_4215_9247_D430CE001023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//ロボットとともに移動する空間を記述するクラス
//2005.03.31
class RSPACE  
{

public:
	//空間の寸法	
	enum {
		SPACE_LENGTH = 100,
	    SPACE_WIDTH = 100,
	    SPACE_HIGH = 100	
	};
	//メモリ中の空間実体
    static int ary_RSPACE[SPACE_LENGTH][SPACE_WIDTH][SPACE_HIGH];

	typedef int DataType;
	typedef struct{
		DataType data[SPACE_LENGTH];
	}sqlist;
	
	//空間の中にロボットの相対位置
	int DISTANCE_AFTER_ROBOTX;
	static const int DISTANCE_AFTER_ROBOTY;
	static const int DISTANCE_AFTER_ROBOTZ;

	//ロボットの移動量
	int i_moveX,
	    i_moveY,
	    i_moveZ;
	

public:
	RSPACE();
	virtual ~RSPACE();

	//get　ロボットの移動量
	void get_RMovement(int i_x,int i_y,int i_z);


};

#endif // !defined(AFX_RSPACE_H__764D0CDA_F546_4215_9247_D430CE001023__INCLUDED_)
