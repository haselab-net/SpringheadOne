// PHMagneticEngine.cpp: PHMagneticEngine クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "PHMagneticEngine.h"
#include <Base/BaseDebug.h>

namespace Spr{;
//派生関係を示すためのマクロ
SGOBJECTIMP(PHMagneticEngine, SGBehaviorEngine);

//////////////////////////////////////////////////////////////////////
//磁力の物理エンジン
//////////////////////////////////////////////////////////////////////
PHMagneticEngine::PHMagneticEngine()
{

}

PHMagneticEngine::~PHMagneticEngine()
{

}

void PHMagneticEngine::Step(SGScene *s)
{
	ApplyForce();
}

bool PHMagneticEngine::AddChildObject(SGObject *o, SGScene *s)
{
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	return false;
}

void PHMagneticEngine::ApplyForce()
{
	PHSolids::iterator it;
	PHSolid* solid;
	PHSolid* poin;
	it = solids.begin();
	poin = *it;
	it++;
	for(it = it; it != solids.end(); it++){
		solid = *it;
		Vec3d soPosition = solid->GetCenterPosition();
		Vec3d distance = poin->GetCenterPosition()-soPosition;
		double dis=distance*distance;
		dis=sqrt(dis);
		dis=dis*dis*dis;
		if(10000>dis){
		//ここに力の計算を挿入
		Vec3d power;
		if(dis<0.01){
			dis=0.01;
		}else {
			power=(30.0/dis)*distance;
		}
		DSTR << dis << "\n";
		solid->AddForce(power);
		poin->AddForce(-power);
		}
	}
}
///////////////////////////////////////////////
//磁力エンジンのローダーとセイバー

class PHMagneticEngineLoader:public FIObjectLoader<PHMagneticEngine>{
public:
	PHMagneticEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Vector);
		db->REG_RECORD_PROTO(XMagneticEngine);
	}
	virtual bool LoadData(FILoadScene* ctx, PHMagneticEngine* magnet){
		ctx->docs.Top()->GetWholeData(magnet->mag);
		return true;
	}
};

class PHMagneticEngineSaver:public FIObjectSaver<PHMagneticEngine>{
	virtual UTString GetType() const{ return "PHMagneticEngine"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, PHMagneticEngine* magnet){
			doc->SetWholeData(magnet->mag);
	}
};
DEF_REGISTER_BOTH(PHMagneticEngine);	//ローダー,セイバーの登録

}	//	namespace Spr


