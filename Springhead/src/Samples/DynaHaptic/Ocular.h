// Ocular.h: COcular クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCULAR_H__E5A563D6_26D7_40D0_B245_EDAC8086F8A5__INCLUDED_)
#define AFX_OCULAR_H__E5A563D6_26D7_40D0_B245_EDAC8086F8A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COcular  
{
public:
	COcular();
	virtual ~COcular();
	public:
	float x;
	float y;
	float z;
	float a;
	float b;
	float c;
	float ErrIntX;
	float RelAngX;
	float ErrIntY;
	float RelAngY;
	//void Draw(float GrayLevel);
	void SetPosition(float fx, float fy, float fz, float fa, float fb, float fc);

};

#endif // !defined(AFX_OCULAR_H__E5A563D6_26D7_40D0_B245_EDAC8086F8A5__INCLUDED_)
