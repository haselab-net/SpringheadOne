// HISpidarThumb.h: HISpidarThumb クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISPIDARTHUMB_H__0FF14B3A_BB27_47BD_A8DB_B2C9B779ADC5__INCLUDED_)
#define AFX_HISPIDARTHUMB_H__0FF14B3A_BB27_47BD_A8DB_B2C9B779ADC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "HISpidarHand.h"
#include "HISpidarFinger.h"

namespace Spr {

class HISpidarThumb : public HISpidarFinger
{
protected:
	HISpidarMotor* m_pMotor2;
public:
	virtual void InitInstance(HISpidarHand* pHand,HISpidarMotor* pMotor1,HISpidarMotor* pMotor2);
	HISpidarThumb();
	virtual ~HISpidarThumb();

};

}//namespace Spr 
#endif // !defined(AFX_HISPIDARTHUMB_H__0FF14B3A_BB27_47BD_A8DB_B2C9B779ADC5__INCLUDED_)
