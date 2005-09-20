#include "HISpidarCalc3Dof.h"
namespace Spr{;

class HISpidarCalc6Dof : public HISpidarCalc3Dof {

public:
	HISpidarCalc6Dof();
	virtual ~HISpidarCalc6Dof();

protected:
	Quaternionf ori;
public:
	void Update();								///<	姿勢情報の更新
	void SetForce(TVec3<float>);				///<	提示力の設定（並進のみ）
	void SetForce(TVec3<float>,TVec3<float>);	///<	提示力の設定（並進・回転力）
	TVec3<float> RForce;		///回転力
	float lambda_r;		///二次形式　回転項の係数
	float* InputForTest(float,float,float,float*,float); //テスト用入力関数
};

}	//	namespace Spr
