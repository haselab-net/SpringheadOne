#include "Physics.h"
#pragma hdrstop
#include <Physics/PHWater.h>
#include <Physics/PHWaterContact.h>
#include <Collision/CDMesh.h>
#include <Collision/CDQuickHull2D.h>
#include <Collision/CDQuickHull2DImp.h>

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

struct QH2DVertex: public Vec3f{
	QH2DVertex(const Vec3f& v):Vec3f(v){}
	Vec2f GetPos(){ return Vec2f(x,y); }
};

SGScene* scene;
struct PHWConvexCalc{
	PHWaterContactEngine* engine;
	PHWater* water;
	Affinef	Aw, Awinv;	//water-coord to world-coord transformation

	//	Solid単位のデータ
	PHWSolid* solid;
	Vec3f buo, tbuo;	//浮力と浮力によるモーメント	
	Affinef As, Asinv;	//solid-coord to world-coord
	Affinef Aws;
	Affinef Asg;		//solid-coord to geometry-coord
	
	//	Mesh単位のデータ
	Affinef	Ag;			//geomerty-coord to world-coord
	Affinef Awg;		//water-coord to geometry-coord
	std::vector<float> depth;		//	頂点の水深
	std::vector<float> height;		//	頂点での波高
	std::vector<Vec3f> vtxs;		//	水座標系での頂点の座標
	
	//	凸形状単位のデータ
	CDPolyhedron* poly;				//	対象の凸形状
	std::vector<Vec3f> vtxsOn;		//	線分が水面を横切る点(水座標系)

	//	塗りつぶし作業用	水cell座標系 (0..mx,0..my)
	std::vector<Vec2f> border;		//	凸形状を水で輪切りにしたときの輪
	int iLeft, iRight;				//	ボーダの左端の頂点と右端の頂点
	float left, right;				//	左端，右端
	int curY;						//	現在の行
	float dLeft, dRight;			//	1行での左端の変化量, 右端の変化量

	void Init(PHWaterContactEngine* e){
		engine = e;
		water = e->water;
	}
	void Calc(CDPolyhedron* p){
		poly = p;
		CalcPressure();
		CalcBorder();
	}
	void CalcPressure(){
		vtxsOn.clear();
		for(CDFaces::iterator iface = poly->faces.begin(); iface != poly->faces.end(); iface++){
			//	3角形の頂点がいくつ水に漬かっているか数える
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
				CalcTriangle(faceVtxs, faceDepth, faceHeight);
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
				CalcTriangle(faceVtxs, faceDepth, faceHeight);
				vtxsOn.push_back(faceVtxs[2]);
				
				faceVtxs[1] = faceVtxs[2];
				faceVtxs[2] = a1*vtxs[iOver] + (1-a1)*vtxs[i1];
				faceDepth[1] = 0;
				faceDepth[2] = 0;
				faceHeight[1] = faceHeight[2];
				faceHeight[2] = a1*height[iOver] + (1-a1)*height[i1];
				CalcTriangle(faceVtxs, faceDepth, faceHeight);
				vtxsOn.push_back(faceVtxs[2]);
			}else if (nUnder == 3){
				for(int i=0; i<3; ++i){
					faceVtxs[i] = vtxs[iface->vtxs[i]];
					faceDepth[i] = depth[iface->vtxs[i]];
					faceHeight[i] = height[iface->vtxs[i]];
				}
				CalcTriangle(faceVtxs, faceDepth, faceHeight);
			}
		}
	}
	bool NextLine(){
		curY++;
		left += dLeft;
		right += dRight;
		if (iLeft == iRight && curY > border[iLeft].y) return false;
		while (border[iLeft].y < curY && iLeft!= iRight){
			Vec2f last = border[iLeft];
			iLeft = (iLeft-1+border.size()) % border.size();
			Vec2f delta = border[iLeft] - last;
			dLeft = delta.x / delta.y;
			left = last.x + dLeft * (curY-last.y);
		}
		while (border[iRight].y < curY && iLeft!= iRight){
			Vec2f last = border[iRight];
			iRight = (iRight+1) % border.size();
			Vec2f delta = border[iRight] - last;
			dRight = delta.x / delta.y;
			right = last.x + dRight * (curY-last.y);
		}
		if (iLeft == iRight && curY > border[iLeft].y) return false;
		return true;
	}
	void CalcBorder(){
		Vec3f center = (Aws * solid->solid->GetCenter() - Vec3f(-water->dx,-water->dy,0)) * water->dhinv;
		Vec3f vel = Aws.Rot() * solid->solid->GetVelocity() * water->dhinv;
		Vec3f aVel = Aws.Rot() * solid->solid->GetAngularVelocity() * water->dhinv;
		//	水に境界条件を設定(凸形状ごとに処理する)
		//	境界を作る頂点を水面に投影し，凸包を作る．
		border.clear();
		CDQHLines<QH2DVertex> lines(100);
		std::vector<QH2DVertex*> qhvtxs;
		qhvtxs.resize(vtxsOn.size());
		for(int i=0; i<vtxsOn.size(); ++i) qhvtxs[i] = (QH2DVertex*)&vtxsOn[i];
		lines.CreateConvexHull(qhvtxs.begin(), qhvtxs.end());
		float dhInv = 1/engine->water->dh;
		CDQHLine<QH2DVertex>* start=NULL;
		for(CDQHLine<QH2DVertex>* cur = lines.begin; cur != lines.end; ++cur){
			if (!cur->deleted){
				start = cur;
				break;
			}
		}
		if (!start) return;
		border.push_back((start->vtx[0]->GetPos()+Vec2f(water->dx, water->dy)) * dhInv);
		for(CDQHLine<QH2DVertex>*cur=start->neighbor[0]; cur!= start; cur=cur->neighbor[0]){
			border.push_back((cur->vtx[0]->GetPos()+Vec2f(water->dx, water->dy)) * dhInv);
		}
		if (border.size() < 3) return;
		
		//	境界条件
		//	できた凸包の内側のセルの速度を設定．ブレゼンハムで塗りつぶし
		float top = 1e10f;
		for(int i=0; i<border.size(); ++i){
			if (border[i].y < top){
				top = ceil(border[i].y-1);
				iLeft = i;
			}
		}
		if (top < -1) top = -1;
		curY = top;
		border.insert(border.begin()+iLeft, border[iLeft]);
		iRight = iLeft+1;
		engine->points.clear();
		while(NextLine()){
			int xStart, xEnd;
			xStart = ceil(left);
			xEnd = right;
			if (xStart < 0) xStart = 0;
			if (xEnd > water->mx) xEnd = water->mx;
			for(int x = xStart; x<xEnd; ++x){
				int cx = (x + engine->water->bound.x) % engine->water->mx;
				int cy = (curY + engine->water->bound.y) % engine->water->my;

				Vec3f p = Vec3f(x, curY, engine->water->height[cx][cy]*water->dhinv) - center;
				Vec3f v = (vel + (aVel%p)) * water->dh - Vec3f(water->velocity.x, water->velocity.y, 0);
				engine->water->u[cx][cy] = v.x * 0.1;
				engine->water->v[cx][cy] = v.y * 0.1;
				engine->points.push_back(Vec3f(x*water->dh-water->dx, curY*water->dh-water->dy, 0));
				engine->points.push_back(engine->points.back() + Vec3f(v.x, v.y, 0));
			}
		}
	}
	void CalcTriangle(Vec3f* p, float* depth, float* height){
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
		buo += volume * engine->water->density;
		tbuo += volumeMom * engine->water->density;

		engine->tris.push_back(p[0]);
		engine->tris.push_back(p[1]);
		engine->tris.push_back(p[2]);
	}
};

PHWConvexCalc convCalc;
void PHWaterContactEngine::Render(GRRender* render, SGScene* s){	
	//	描画
	if (!render || !render->CanDraw()) return;
	render->SetModelMatrix(water->GetPosture());
	render->SetDepthTest(false);
	GRMaterialData mat(Vec4f(0, 0, 1, 1), 2);
	render->SetMaterial(mat);
	render->DrawDirect(GRRender::TRIANGLES, &*(tris.begin()), &*(tris.end()));

	GRMaterialData mat2(Vec4f(1, 1, 0, 1), 2);
	render->SetMaterial(mat2);
	std::vector<Vec3f> vtxs;
	float dh = water->dh;
	float dx = water->dx;
	float dy = water->dy;
	for(int i=0; i<convCalc.border.size(); ++i){
		vtxs.push_back(Vec3f(convCalc.border[i].x*dh-dx, convCalc.border[i].y*dh-dy, 0));
		vtxs.push_back(Vec3f(convCalc.border[(i+1)%convCalc.border.size()].x*dh-dx, convCalc.border[(i+1)%convCalc.border.size()].y*dh-dy, 0));
	}
	render->DrawDirect(GRRender::LINES, vtxs.begin(), vtxs.end());

	GRMaterialData mat3(Vec4f(1, 0, 1, 1), 2);
	render->SetMaterial(mat3);
	render->DrawDirect(GRRender::LINES, points.begin(), points.end());
	render->SetDepthTest(true);
}
void PHWaterContactEngine::Step(SGScene* s){
	tris.clear();
	scene = s;
	PHWGeometry* geo;
	CDPolyhedron* poly;
	PHWSolids::iterator is;
	PHWGeometries::iterator ig;
	CDGeometries::iterator ic;

	convCalc.Init(this);
	convCalc.Aw = water->GetPosture();
	convCalc.Awinv = convCalc.Aw.inv();

	//剛体に加わる浮力を計算する
	//全剛体について･･･
	for(is = solids.begin(); is != solids.end(); is++){
		convCalc.solid = *is;
		convCalc.As = convCalc.solid->solid->GetFrame()->GetWorldPosture();
		convCalc.Asinv = convCalc.As.inv();
		convCalc.Aws = convCalc.Awinv * convCalc.As;

		//全ジオメトリについて･･･
		for(ig = convCalc.solid->geometries.begin(); ig != convCalc.solid->geometries.end(); ig++){
			geo = *ig;
			convCalc.Ag = geo->frame->GetWorldPosture();
			convCalc.Asg = convCalc.Asinv * convCalc.Ag;
			convCalc.Awg = convCalc.Awinv * convCalc.Ag;
			//BBoxレベルでの接触チェック
			//...

			//凸多面体の各面に加わる浮力をジオメトリの中心を基準として積算
			convCalc.buo.clear();
			convCalc.tbuo.clear();

			convCalc.depth.resize(geo->mesh->vertices.size());
			convCalc.height.resize(geo->mesh->vertices.size());
			convCalc.vtxs.resize(geo->mesh->vertices.size());
			for(int i=0; i<geo->mesh->tvtxs.size(); ++i){
				convCalc.vtxs[i] = convCalc.Awg * geo->mesh->vertices[i];	//	水座標系での頂点
				convCalc.height[i] = water->LerpHeight(convCalc.vtxs[i].x, convCalc.vtxs[i].y);
				convCalc.depth[i] = convCalc.height[i] - convCalc.vtxs[i].z;
			}
			for(ic = geo->conveces.begin(); ic != geo->conveces.end(); ic++){
				poly = DCAST(CDPolyhedron, *ic);
				if(!poly)continue;
				//ここで凸多面体レベルでのBBox判定した方が速くなる気がする
				//...
				//全面について･･･
				convCalc.Calc(poly);
			}
			//	水から剛体フレームへ変換してAddForce
			convCalc.solid->solid->AddForce(convCalc.Aw.Rot() * convCalc.buo, convCalc.Aw.Pos());
			convCalc.solid->solid->AddTorque(convCalc.Aw.Rot() * convCalc.tbuo);
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
