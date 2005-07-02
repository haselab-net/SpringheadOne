#include "Physics.h"
#pragma hdrstop

namespace Spr{;

//----------------------------------------------------------------------------
//PHWSolid
PHWSolid::Init(){
	frame = solid->GetFrame();
	//SGFrame::contentsからCDGeometryの派生クラスオブジェクトを探す
	EnumGeometries(frame);
}

PHWSolid::EnumGeometries(SGFrame* f){
	CDMesh* g;
	for(SGObjects::iterator ic = f->contents.begin(); ic != f->contents.end(); ic++){
		g = DCAST(CDMesh, *ic);
		if(g){
			geometries.push_back(new PHWGeometry);
			geometries.back()->Set(f, g);
		}
	}
	for(SGFrames::iterator i = f->Children().begin(); i != f->Children().end(); i++)
		EnumGeometries(*i);
}

//----------------------------------------------------------------------------
//PHWGeometry
void PHWGeometry::Set(SGFrame* f, CDMesh* g){
	frame = f;
	conveces = g->conveces;
}


//----------------------------------------------------------------------------
//	PHWaterContactEngine
//
SGOBJECTIMP(PHWaterContactEngine, SGBehaviorEngine);

PHWaterContactEngine::PHWaterContactEngine(){

}

bool PHWaterContactEngine::AddChildObject(SGObject* o, SGScene* scene){
	if(DCAST(PHSolid, o)){
		solids.push_back(new PHWSolidInfo);
		solids.back()->solid = DCAST(PHSolid, o);
		return true;
	}
	if(DCAST(PHWater, o)){
		water = DCAST(PHWater, o);
		return true;
	}
	return false;
}

void PHWaterContactEngine::Clear(SGScene* s){
	water = 0;
	solids.clear();
}
void PHWaterContactEngine::Loaded(SGScene* scene){
	Init(scene);
}

void PHWaterContactEngine::Init(SGScene* scene){
	//Solidの形状データを吸い出す
	for(PHWSolids::iterator is = solids.begin(); is != solids.end(); is++)
		(*is)->Init();
}

void PHWaterContactEngine::Step(SGScene* s){
	PHWSolid* solid;
	PHWGeometry* geo;
	CDPolyhedron* poly;
	PHWSolids::iterator is;
	PHWGeometries::iterator ig;
	CDConveces::iterator ic;
	CDFaces::iterator iface;
	Vec3d buo, tbuo;	//浮力と浮力によるモーメント
	//全剛体について･･･
	for(is = solids.begin(); is != solids.end(); is++){
		solid = *is;
		//全ジオメトリについて･･･
		for(ig = solid->geometries.begin(); ig != solid->geometries.end(); ig++){
			g = *ig;
			//BBoxレベルでの接触チェック
			//...

			buo.clear();
			tbuo.clear();
			//全凸多面体について･･･
			for(ic = geo->conveces.begin(); ic != geo->conveces.end(); ic++){
				poly = DCAST(CDPolyhedron, *ic);
				if(!poly)continue;
				//ここで凸多面体レベルでのBBox判定した方が速くなる気がする
				//...

				//全面について･･･
				for(iface = poly->faces.begin(); iface != poly->faces.end(); iface++){
					//この面が水面下にあるかどうか調べる
					iface->center;
					p;
					wz = water->LerpHeight(p.X(), p.Y());
					//水面下ならば浮力を計算
					if(wz > p.Z()){
						//このサンプル点が受ける浮力
						b = -iface->normal * (wz - sp->p.Z()) * iface->area;
						buo += b;

						//モーメント
						tbuo += iface->center % b;
					}
				}
			}
		}
	}
}
/*
//ダイナミクスは考慮せずに単純に水面下に沈み込んでいる体積に比例する浮力
void getBuoyancy(ThapticObj *ho, Tpoint3f *buo, Tpoint3f *tbuo) {
    Tpoint3f b, n, r;
    Tpoint3f wve;
    Trealf wz, dz;
    TsamplePoint *sp;
    int i;
    
	//なんか後で必要になりそうなのでコメントアウトで保留
    //for(i=0; i<ho->nhsrc; i++) ho->hsrc[i].a = 0.0;

    sp = ho->sp;
    for(i=0; i<ho->n_sp; i++) {
        wz = lerpWaterHeight(sp->p.X(), sp->p.Y());
		r = sp->p - ho->pos;	//オブジェクト中心からサンプル点へのベクトル
		n = sp->n;			//サンプル点の法線

    	//水面がサンプル点よりも高いならば
        if(wz > sp->p.Z()) {
			//このサンプル点が受ける浮力
			b = -n * (wz - sp->p.Z()) * sp->s;
			buo += b;

			//モーメント
			tbuo += r % b;
            
            //if(ho->nhsrc > 0 && sp->i_hsrc >= 0) ho->hsrc[sp->i_hsrc].a = 1.0;
        }
        sp++;
    }
    
	//スケーリングかな？
	buo *= buo_scl;
    tbuo *= buo_scll;
	
    //for(i=0; i<ho->nhsrc; i++) {
    //    if(ho->hsrc[i].n_sp > 0) ho->hsrc[i].a /= (Trealf)ho->hsrc[i].n_sp;
    //}
}


//----------------------------------------------------------------------------
//	PHWaterContactEngineLoader
//
class PHWaterContactEngineLoader:public FIObjectLoader<PHWaterContactEngine>{
	virtual bool LoadData(FILoadScene* ctx, PHWaterContactEngine* pc){
		ctx->objects.Push(pc);
		return true;
	}
	virtual void Loaded(FILoadScene* ctx){
		ctx->objects.Pop();
	}
};

class PHWaterContactEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterContactEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWaterContactEngine* pc = (PHWaterContactEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("WaterContactEngine", pc);
		ctx->docs.back()->AddChild(doc);
		for(PHSolids::iterator it = pc->solids.begin(); it != pc->solids.end(); ++it){
			doc->AddChild(ctx->CreateDocNode("REF", *it));
		}
		doc->AddChild(ctx->CreateDocNode("REF", water));
	}
};
DEF_REGISTER_BOTH(PHWaterContactEngine);

}
