// Turn.h: Turn クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_TURN_H__826E27D9_4D7A_4604_BE53_6077E837409E__INCLUDED_)
#define AFX_TURN_H__826E27D9_4D7A_4604_BE53_6077E837409E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define V_CENTER 0x80

class Tokin;
class Turn{
public:
	void ResetAngle();
	double dangle;
	void GetAngle();
	Turn();
	virtual ~Turn();
protected:
	Tokin* tokin;
	void Uninit();
	void Init();
private:
	double now_angle , start_angle;
	float v_turn;
	unsigned int dwTime;
};

#endif // !defined(AFX_TURN_H__826E27D9_4D7A_4604_BE53_6077E837409E__INCLUDED_)
