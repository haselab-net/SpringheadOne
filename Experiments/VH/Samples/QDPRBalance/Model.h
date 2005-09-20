// Model.h: Model クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//	[solidsの順番]
//(上半身、体躯）
//0:Waist(腰),1:Abdomen(腹),2:Chest(胸),3:Head(頭),
//(上半身、右腕）
//4:soRUArm(上腕),5:soRFArm,(前腕)6:soRHand(手)
//(上半身、左腕）
//7:soLUArm(上腕),8:soLFArm(前腕),9:soLHand(手)
//(下半身、右）
//10:soRThigh(大腿),11:soRLThigh(下腿),12:soRHeel(踵),13:soRToe(爪先)
//(下半身、右）
//14:soLThigh(大腿),15:soLLThigh(下腿),16:soLHeel(踵),17:soLToe(爪先)
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//	[jointsの順番]
//-------	  上半身   --------------
//	(腰）
//0:joWaist1(-X), 1:joWaist2(Z), 2:joWaist3(-Y)
//	(胸）
//3:joChest1(-X), 4:joChest2(Z), 5:joChest3(-Y)
//	(首）
//6:joNeck1(-X), 7:joNeck2(Z), 8:joNeck3(-Y)
//	(右肩)
//9:joRShoulder1(X), 10:joRShoulder2(Z), 11:joRShoulder3(-Y)
//	(右肘)
//12:joRElbow1(X), 13:joRElbow2(-Y)
//	(右手首)
//14:joRWrist1(-Z), 15:joRWrist2(X)
//	(左肩)
//16:joLShoulder1(X), 17:joLShoulder2(-Z), 18:joLShoulder3(Y)
//	(左肘)
//19:joLElbow1(X), 20:joLElbow2(Y)
//	(左手首)
//21:joLWrist1(Z), 22:joLWrist2(X)
//-------	  下半身   --------------
//	(右股関節)
//23:joRHip1(X), 24:joRHip2(Z), 25:joRHip3(-Y)
//	(右膝)
//26:joRKnee(-X)
//	(右足首)
//27:joRAnkle1(X), 28:joRAnkle2(Z), 29:joRAnkle3(-Y)
//	(右土踏まず)
//30:joRToe(-X)
//	(左股関節)
//31:joLHip1(X), 32:joLHip2(-Z), 33:joLHip3(Y)
//	(左膝)
//34:joLKnee(-X)
//	(左足首)
//35:joLAnkle1(X), 36:joLAnkle2(-Z), 37:joLAnkle3(Y)
//	(左土踏まず)
//38:joLToe(-X)
//////////////////////////////////////////////////////////////

#ifndef MODEL_H
#define MODEL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<Creature/CRHuman.h>
#include "Balance.h"

namespace Spr{;
class Model : public CRHuman  
{
public:
	Model();
	virtual ~Model();

	Balance balance;

	void MaintainBalance(SGScene* scene,GRRender* render);	// バランスをとる
	void RegistSupportParts(UTRef<SGScene> scene);
	void ExceptContactParts(CREnumContactVertex* contact);

	void Load(SGScene* scene);
	void LoadDerivedModel(SGScene* scene);
	void SetJointSpring(float dt);
	bool Connect(UTRef<SGScene> scene);
	void SetJointRange();
	void SetInertia();
	
};

}		// end namespace
#endif // !defined(MODEL_H)
