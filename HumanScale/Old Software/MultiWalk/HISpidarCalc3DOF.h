#ifndef HISPIDARCALC3DOF_H
#define HISPIDARCALC3DOF_H

#include "HISpidarCalcBase.h"
namespace Spr{;

class HISpidarCalc3Dof: public HISpidarCalcBase{

protected:
	TVec3<float> pos;						///<	グリップの位置
	int frequency;							///<	繰り返し回数
	float sigma;							///<	係数行列の対角成分に加える

public:
	HISpidarCalc3Dof();
	virtual ~HISpidarCalc3Dof();

	virtual void Update();					///<	姿勢情報の更新
	virtual void SetForce(TVec3<float>);	///<	提示力の設定（並進力）
	virtual void SetForce(TVec3<float>,TVec3<float>);	///<	提示力の設定（並進・回転力は提示しない）
	TVec3<float> TForce;		///	並進力
	float    smooth;				///	二次形式　張力の２乗項の係数
	float    lambda_t;				///	二次形式　並進項の係数
};

}	//	namespace Spr

#endif
