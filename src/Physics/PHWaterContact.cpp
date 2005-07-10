#include "Physics.h"
#pragma hdrstop
#include <Physics/PHWater.h>
#include <Physics/PHWaterContact.h>
#include <Collision/CDMesh.h>

namespace Spr{;

//----------------------------------------------------------------------------
//PHWSolid
void PHWSolid::Init(){
	frame = solid->GetFrame();
	//SGFrame::contentsからCDGeometryの派生クラスオブジェクトを探す
	EnumGeometries(frame);
}

void PHWSolid::EnumGeometries(SGFrame* f){
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
	mesh = g;
	conveces.resize(g->conveces.size());
	std::copy(g->conveces.begin(), g->conveces.end(), conveces.begin());
}


//----------------------------------------------------------------------------
//	PHWaterContactEngine
//
SGOBJECTIMP(PHWaterContactEngine, SGBehaviorEngine);

PHWaterContactEngine::PHWaterContactEngine(){

}
void PHWaterContactEngine::LoadState(const SGBehaviorStates& states){
}
void PHWaterContactEngine::SaveState(SGBehaviorStates& states) const{
}

bool PHWaterContactEngine::AddChildObject(SGObject* o, SGScene* scene){
	if(DCAST(PHSolid, o)){
		solids.push_back(new PHWSolid);
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

SGScene* scene;
void PHWaterContactEngine::Step(SGScene* s){
	tris.clear();
	scene = s;
	PHWSolid* solid;
	PHWGeometry* geo;
	CDPolyhedron* poly;
	PHWSolids::iterator is;
	PHWGeometries::iterator ig;
	CDGeometries::iterator ic;
	CDFaces::iterator iface;

	std::vector<int> vtxsUnder;
	std::vector<Vec3f> vtxsOn;
	Vec3f buo, tbuo;	//浮力と浮力によるモーメント
	Affinef	Aw, Awinv,	//water-coord to world-coord transformation
					Ag,			//geomerty-coord to world-coord
					Awg,		//water-coord to geometry-coord
                    As, Asinv,	//solid-coord to world-coord
					Asg;		//solid-coord to geometry-coord
	
	//Aw = water->GetFrame()->GetWorldPosture();
	Aw = water->posture;
	Awinv = Aw.inv();

	//剛体に加わる浮力を計算する
	//全剛体について･･･
	for(is = solids.begin(); is != solids.end(); is++){
		solid = *is;
		As = solid->solid->GetFrame()->GetWorldPosture();
		Asinv = As.inv();

		//全ジオメトリについて･･･
		for(ig = solid->geometries.begin(); ig != solid->geometries.end(); ig++){
			geo = *ig;
			Ag = geo->frame->GetWorldPosture();
			Asg = Asinv * Ag;
			Awg = Awinv * Ag;
			//BBoxレベルでの接触チェック
			//...

			//凸多面体の各面に加わる浮力をジオメトリの中心を基準として積算
			buo.clear();
			tbuo.clear();

			//	頂点の水深を計算
			std::vector<float> depth;
			std::vector<float> height;
			std::vector<Vec3f> vtxs;
			depth.resize(geo->mesh->vertices.size());
			height.resize(geo->mesh->vertices.size());
			vtxs.resize(geo->mesh->vertices.size());
			for(int i=0; i<geo->mesh->tvtxs.size(); ++i){
				vtxs[i] = Awg * geo->mesh->vertices[i];	//	水座標系での頂点
				height[i] = water->LerpHeight(vtxs[i].x, vtxs[i].y);
				depth[i] = height[i] - vtxs[i].z;
				if (depth[i] > 0) vtxsUnder.push_back(i);
			}
			for(ic = geo->conveces.begin(); ic != geo->conveces.end(); ic++){
				poly = DCAST(CDPolyhedron, *ic);
				if(!poly)continue;
				//ここで凸多面体レベルでのBBox判定した方が速くなる気がする
				//...
				//全面について･･･
				for(iface = poly->faces.begin(); iface != poly->faces.end(); iface++){
					int nUnder=0, over, under;
					for(int i=0; i<3; ++i){
						if (depth[iface->vtxs[i]] > 0){
							nUnder++;
							under = i;
						}else{
							over = i;
						}
					}
					if (nUnder == 0) continue;	//	水に漬かっていない
					Vec3f faceVtxs[3];
					float faceDepth[3];
					float faceHeight[3];
					if (nUnder == 1){
						int iUnder = iface->vtxs[under];
						int i1 = iface->vtxs[(under+1)%3];
						int i2 = iface->vtxs[(under+2)%3];
						faceVtxs[0] = vtxs[iUnder];
						float a1 = -depth[i1]/(depth[iUnder]-depth[i1]);
						faceVtxs[1] = a1 * vtxs[iUnder] + (1-a1) * vtxs[i1];
						float a2 = -depth[i2]/(depth[iUnder]-depth[i2]);
						faceVtxs[2] = a2 * vtxs[iUnder] + (1-a2) * vtxs[i2];
						faceDepth[0] = depth[iUnder];
						faceDepth[1] = 0;
						faceDepth[2] = 0;
						faceHeight[0] = height[iUnder];
						faceHeight[1] = a1*height[iUnder] + (1-a1)*height[i1];
						faceHeight[2] = a2*height[iUnder] + (1-a2)*height[i2];
						CalcTriangle(buo, tbuo, faceVtxs, faceDepth, faceHeight, &*iface);
						vtxsOn.push_back(faceVtxs[1]);
						vtxsOn.push_back(faceVtxs[2]);
					}else if (nUnder == 2){
						int iOver = iface->vtxs[over];
						int i1 = iface->vtxs[(over+1)%3];
						int i2 = iface->vtxs[(over+2)%3];
						faceVtxs[0] = vtxs[i1];
						faceVtxs[1] = vtxs[i2];
						float a1 = depth[i1]/(depth[i1]-depth[iOver]);
						float a2 = depth[i2]/(depth[i2]-depth[iOver]);
						faceVtxs[2] = a2*vtxs[iOver] + (1-a2)*vtxs[i2];
						faceDepth[0] = depth[i1];
						faceDepth[1] = depth[i2];
						faceDepth[2] = 0;
						faceHeight[0] = height[i1];
						faceHeight[1] = height[i2];
						faceHeight[2] = a2*height[iOver] + (1-a2)*height[i2];
						CalcTriangle(buo, tbuo, faceVtxs, faceDepth, faceHeight, &*iface);
						vtxsOn.push_back(faceVtxs[2]);
						
						faceVtxs[1] = faceVtxs[2];
						faceVtxs[2] = a1*vtxs[iOver] + (1-a1)*vtxs[i1];
						faceDepth[1] = 0;
						faceDepth[2] = 0;
						faceHeight[1] = faceHeight[2];
						faceHeight[2] = a1*height[iOver] + (1-a1)*height[i1];
						CalcTriangle(buo, tbuo, faceVtxs, faceDepth, faceHeight, &*iface);
						vtxsOn.push_back(faceVtxs[2]);
					}else if (nUnder == 3){
						for(int i=0; i<3; ++i){
							faceVtxs[i] = vtxs[iface->vtxs[i]];
							faceDepth[i] = depth[iface->vtxs[i]];
							faceHeight[i] = height[iface->vtxs[i]];
						}
						CalcTriangle(buo, tbuo, faceVtxs, faceDepth, faceHeight, &*iface);
					}
#if 0
					//この面が水面下にあるかどうか調べる
					center_w = Awg * iface->center;
					//normal_w = Awg * iface->normal;
					wz = water->LerpHeight(center_w.X(), center_w.Y());
					//水面下ならば浮力を計算
					if(wz > center_w.Z()){
						nface++;
						//この段階ではジオメトリフレームで計算していることに注意
						b = -iface->normal * ((wz - center_w.Z()) * iface->area);
						buo += b;

						//モーメント
						tbuo += iface->center % b;
					}
#endif
				}
			}
			//ジオメトリフレームから剛体フレームへ変換してAddForce
			solid->solid->AddForce(Aw.Rot() * buo, Aw.Pos());
			solid->solid->AddTorque(Aw.Rot() * tbuo);
		}
	}
}
void PHWaterContactEngine::CalcTriangle(Vec3f& buo, Vec3f& tbuo, Vec3f* p, float* depth, float* height, CDFace* face){
	assert(depth[0] >=0);
	assert(depth[1] >=0);
	assert(depth[2] >=0);
	
	Vec3f a = p[1] - p[0];
	Vec3f b = p[2] - p[0];
	Vec3f normal = -a^b;
	Vec3f volume = (1.0f/6.0f) * (depth[0] + depth[1] + depth[2]) * normal;
	Vec3f volumeMom = (
				((1.0f/12.0f)*depth[0] + (1.0f/24.0f)*depth[1] + (1.0f/24.0f)*depth[2]) * p[0]
			+	((1.0f/24.0f)*depth[0] + (1.0f/12.0f)*depth[1] + (1.0f/24.0f)*depth[2]) * p[1]
			+	((1.0f/24.0f)*depth[0] + (1.0f/24.0f)*depth[1] + (1.0f/12.0f)*depth[2]) * p[2]
		  ) ^ normal;
	//	波高の補正
	buo += volume * water->density;
	tbuo += volumeMom * water->density;

	tris.push_back(p[0]);
	tris.push_back(p[1]);
	tris.push_back(p[2]);
}
void PHWaterContactEngine::Render(GRRender* render, SGScene* s){	
	//	描画
	if (!render || !render->CanDraw()) return;
	render->SetModelMatrix(water->posture);
	GRMaterialData mat(
		Vec4f(0, 0, 1, 1),
		Vec4f(0, 0, 1, 1),
		Vec4f(0, 0, 1, 1),
		Vec4f(0, 0, 1, 1),
		0.0f);
	render->SetMaterial(mat);
	render->SetDepthTest(false);
	render->DrawDirect(GRRender::TRIANGLES, &*(tris.begin()), &*(tris.end()));
	render->SetDepthTest(true);
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
*/

//----------------------------------------------------------------------------
//	PHWaterContactEngineLoader
//
DEF_RECORD(XWaterContactEngine, {
	GUID Guid(){ return WBGuid("3cec723b-36dc-433e-8ade-06a3e3fd5ee3"); } 
});

class PHWaterContactEngineLoader:public FIObjectLoader<PHWaterContactEngine>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWaterContactEngine* pc){
		ctx->objects.Push(pc);
		return true;
	}
	virtual void Loaded(FILoadScene* ctx){
		ctx->objects.Pop();
	}
	PHWaterContactEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_RECORD_PROTO(XWaterContactEngine);
	}
};

class PHWaterContactEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterContactEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWaterContactEngine* pc = (PHWaterContactEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("WaterContactEngine", pc);
		ctx->docs.back()->AddChild(doc);
		for(PHWSolids::iterator it = pc->solids.begin(); it != pc->solids.end(); ++it){
			doc->AddChild(ctx->CreateDocNode("REF", (*it)->solid));
		}
		doc->AddChild(ctx->CreateDocNode("REF", pc->water));
	}
};
DEF_REGISTER_BOTH(PHWaterContactEngine);

}
