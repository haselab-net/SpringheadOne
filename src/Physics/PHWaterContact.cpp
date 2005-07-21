#include "Physics.h"
#pragma hdrstop
#include <Physics/PHWater.h>
#include <Physics/PHWaterContact.h>
#include <Collision/CDMesh.h>
#include <Collision/CDQuickHull2D.h>
#include <Collision/CDQuickHull2DImp.h>
#include <ImpD3D/D3Render.h>
#include <WinBasis/WBPath.h>

namespace Spr{;

//----------------------------------------------------------------------------
//PHWSolid
void PHWSolid::Init(PHWaterContactEngine* e){
	frame = solid->GetFrame();
	//SGFrame::contentsからCDGeometryの派生クラスオブジェクトを探す
	EnumGeometries(frame, e);
}

void PHWSolid::EnumGeometries(SGFrame* f, PHWaterContactEngine* e){
	CDMesh* g;
	for(SGObjects::iterator ic = f->contents.begin(); ic != f->contents.end(); ic++){
		g = DCAST(CDMesh, *ic);
		if(g){
			geometries.push_back(new PHWGeometry);
			geometries.back()->Set(f, g, e);
		}
	}
	for(SGFrames::iterator i = f->Children().begin(); i != f->Children().end(); i++)
		EnumGeometries(*i, e);
}

//----------------------------------------------------------------------------
//PHWGeometry
PHWGeometry::PHWGeometry(){
	frm = NULL;
	mesh = NULL;
}
void PHWGeometry::Set(SGFrame* f, CDMesh* g, PHWaterContactEngine* e){
	frame = f;
	mesh = g;
	conveces.resize(g->conveces.size());
	//	frm の割り当て
	std::copy(g->conveces.begin(), g->conveces.end(), conveces.begin());
	frm = NULL;
	for(int i=0; i < e->frms.size(); ++i){
		if (!e->frms[i]->mesh) e->frms[i]->InitMesh();
		if (e->frms[i]->mesh == mesh){
			frm = e->frms[i];
			break;
		}
	}
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
	if(DCAST(PHWater, o)){
		water = DCAST(PHWater, o);
		return true;
	}
	if(DCAST(PHSolid, o)){
		solids.push_back(new PHWSolid);	//	
		solids.back()->solid = DCAST(PHSolid, o);
		return true;
	}
	if(DCAST(PHWaterRegistanceMap, o)){
		PHWaterRegistanceMap* frm = (PHWaterRegistanceMap*)o;
		frms.push_back(frm);	//	frm追加
		return true;
	}
	return false;
}

void PHWaterContactEngine::Clear(SGScene* s){
	water = 0;
	solids.clear();
	frms.clear();
}
void PHWaterContactEngine::Loaded(SGScene* scene){
	Init(scene);
	for(int i = 0; i < frms.size(); i++)
		frms[i]->Loaded(scene);
}

void PHWaterContactEngine::Init(SGScene* scene){
	//Solidの形状データを吸い出す
	for(PHWSolids::iterator is = solids.begin(); is != solids.end(); is++)
		(*is)->Init(this);
}

struct QH2DVertex: public Vec3f{
	QH2DVertex(const Vec3f& v):Vec3f(v){}
	Vec2f GetPos(){ return Vec2f(x,y); }
};

/*	FRMによる圧力補正についてのメモ
	・浮力を求めるときに各頂点に対して圧力を補正．
	・水に反映させるため，各頂点に圧力を加える．
	の2つを行う．	*/
SGScene* scene;
struct PHWConvexCalc{
	PHWaterContactEngine* engine;
	PHWater* water;
	Affinef	Aw, Awinv;	//water-coord to world-coord transformation

	//	Solid単位のデータ
	PHWSolid* solid;
	Vec3f solidCenter;	//	剛体の重心		(水座標系)
	Vec3f solidVel;		//	剛体の速度		(水座標系)
	Vec3f solidAngVel;	//	剛体の角速度	(水座標系)
	Vec3f buo, tbuo;	//浮力と浮力によるモーメント	
	Affinef As, Asinv;	//solid-coord to world-coord
	Affinef Aws;
	Affinef Asg;		//solid-coord to geometry-coord
	
	//	Mesh単位のデータ
	Affinef	Ag;			//	geomerty-coord to world-coord
	Affinef Awg;		//	water-coord to geometry-coord
	Affinef Awginv;		//	水座標→Mesh座標
	std::vector<float> depth;		//	頂点の水深
	std::vector<float> pressure;	//	FRMによる圧力補正
	std::vector<float> height;		//	頂点での波高
	std::vector<Vec3f> vtxs;		//	水座標系での頂点の座標
	PHWaterRegistanceMap* frm;		//	frm
	
	//	凸形状単位のデータ
	CDPolyhedron* poly;				//	対象の凸形状
	std::vector<Vec3f> vtxsOn;		//	線分が水面を横切る点(水座標系)

	//	塗りつぶし作業用	水cell座標系 (0..mx,0..my)
	std::vector<Vec2f> border;		//	凸形状を水で輪切りにしたときの輪
	std::vector<Vec2f> line[4];		//	輪を上右下左の4つの折れ線に分けたもの
	int iLeft, iRight;				//	ボーダの左端の頂点と右端の頂点
	float left, right;				//	左端，右端
	int curY;						//	現在の行
	float dLeft, dRight;			//	1行での左端の変化量, 右端の変化量

	void Init(PHWaterContactEngine* e){
		engine = e;
		water = e->water;
	}
	void SetSolid(PHWSolid* s){
		solid = s;
		As = solid->solid->GetFrame()->GetWorldPosture();
		Asinv = As.inv();
		Aws = Awinv * As;
		solidCenter = Aws * solid->solid->GetCenter();
		solidVel = Awinv.Rot() * solid->solid->GetVelocity();
		solidAngVel = Awinv.Rot() * solid->solid->GetAngularVelocity();	
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
			float facePressure[3];
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
				facePressure[0] = pressure[iUnder];
				facePressure[1] = 0;
				facePressure[2] = 0;
				faceHeight[0] = height[iUnder];
				faceHeight[1] = a1*height[iUnder] + (1-a1)*height[i1];
				faceHeight[2] = a2*height[iUnder] + (1-a2)*height[i2];
				CalcTriangle(faceVtxs, faceDepth, faceHeight, facePressure);
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
				facePressure[0] = pressure[i1];
				facePressure[1] = pressure[i2];
				facePressure[2] = 0;
				faceHeight[0] = height[i1];
				faceHeight[1] = height[i2];
				faceHeight[2] = a2*height[iOver] + (1-a2)*height[i2];
				CalcTriangle(faceVtxs, faceDepth, faceHeight, facePressure);
				vtxsOn.push_back(faceVtxs[2]);
				
				faceVtxs[1] = faceVtxs[2];
				faceVtxs[2] = a1*vtxs[iOver] + (1-a1)*vtxs[i1];
				faceDepth[1] = 0;
				faceDepth[2] = 0;
				facePressure[1] = 0;
				facePressure[2] = 0;
				faceHeight[1] = faceHeight[2];
				faceHeight[2] = a1*height[iOver] + (1-a1)*height[i1];
				CalcTriangle(faceVtxs, faceDepth, faceHeight, facePressure);
				vtxsOn.push_back(faceVtxs[2]);
			}else if (nUnder == 3){
				for(int i=0; i<3; ++i){
					faceVtxs[i] = vtxs[iface->vtxs[i]];
					faceDepth[i] = depth[iface->vtxs[i]];
					facePressure[i] = pressure[iface->vtxs[i]];
					faceHeight[i] = height[iface->vtxs[i]];
				}
				CalcTriangle(faceVtxs, faceDepth, faceHeight, facePressure);
			}
		}
	}
	bool NextLineX(){
		curY++;
		left += dLeft;
		right += dRight;
		if (iLeft == iRight && curY > border[iLeft].y) return false;
		while (border[iLeft].y < curY && iLeft!= iRight){
			Vec2f last = border[iLeft]-Vec2f(0.5f, 0);
			-- iLeft;
			Vec2f delta = border[iLeft]-Vec2f(0.5f, 0) - last;
			dLeft = delta.x / delta.y;
			left = last.x + dLeft * (curY-last.y);
		}
		while (border[iRight].y < curY && iLeft!= iRight){
			Vec2f last = border[iRight]-Vec2f(0.5f, 0);
			++iRight;
			Vec2f delta = border[iRight]-Vec2f(0.5f, 0) - last;
			dRight = delta.x / delta.y;
			right = last.x + dRight * (curY-last.y);
		}
		if (iLeft == iRight && curY > border[iLeft].y) return false;
		return true;
	}
	bool NextLineY(){
		curY++;
		left += dLeft;
		right += dRight;
		if (iLeft == iRight && curY > border[iLeft].y-0.5f) return false;
		while (border[iLeft].y-0.5f < curY && iLeft!= iRight){
			Vec2f last = border[iLeft]-Vec2f(0,0.5f);
			-- iLeft;
			Vec2f delta = border[iLeft]-Vec2f(0,0.5f) - last;
			dLeft = delta.x / delta.y;
			left = last.x + dLeft * (curY-last.y);
		}
		while (border[iRight].y-0.5f < curY && iLeft!= iRight){
			Vec2f last = border[iRight]-Vec2f(0,0.5f);
			++iRight;
			Vec2f delta = border[iRight]-Vec2f(0,0.5f) - last;
			dRight = delta.x / delta.y;
			right = last.x + dRight * (curY-last.y);
		}
		if (iLeft == iRight && curY > border[iLeft].y-0.5) return false;
		return true;
	}
	bool NextLine(){
		curY++;
		left += dLeft;
		right += dRight;
		if (iLeft == iRight && curY > border[iLeft].y) return false;
		while (border[iLeft].y < curY && iLeft!= iRight){
			Vec2f last = border[iLeft];
			-- iLeft;
			Vec2f delta = border[iLeft] - last;
			dLeft = delta.x / delta.y;
			left = last.x + dLeft * (curY-last.y);
		}
		while (border[iRight].y < curY && iLeft!= iRight){
			Vec2f last = border[iRight];
			++iRight;
			Vec2f delta = border[iRight] - last;
			dRight = delta.x / delta.y;
			right = last.x + dRight * (curY-last.y);
		}
		if (iLeft == iRight && curY > border[iLeft].y) return false;
		return true;
	}
	//	内部を塗りつぶし，アンチエイリアスのふち付
	void CalcBorder(){
		border.clear();
		//	水に境界条件を設定(凸形状ごとに処理する)
		//	境界を作る頂点を水面に投影し，凸包を作る．
		static CDQHLines<QH2DVertex> lines(100);
		lines.Clear();
		std::vector<QH2DVertex*> qhvtxs;
		qhvtxs.resize(vtxsOn.size());
		for(int i=0; i<vtxsOn.size(); ++i) qhvtxs[i] = (QH2DVertex*)&vtxsOn[i];
		lines.CreateConvexHull(&*qhvtxs.begin(), &*qhvtxs.end());
		float dhInv = 1/water->dh;
		for(; lines.begin!=lines.end && lines.begin->deleted; ++lines.begin);
		if (lines.begin == lines.end) return;
		std::vector<Vec2f> tmp;
		tmp.push_back((lines.begin->vtx[0]->GetPos()+Vec2f(water->rx, water->ry)) * dhInv);
		for(CDQHLine<QH2DVertex>*cur=lines.begin->neighbor[0]; cur!= lines.begin; cur=cur->neighbor[0]){
			tmp.push_back((cur->vtx[0]->GetPos()+Vec2f(water->rx, water->ry)) * dhInv);
		}
		if (tmp.size() < 3) return;
		//	凸包の上端の頂点を見つける．
		float top = 1e10f;
		int idTop;
		for(int i=0; i<tmp.size(); ++i){
			if (tmp[i].y < top){
				top = tmp[i].y;
				idTop = i;
			}
		}
		//	border は先頭と最後に(yが最小の)同じ頂点が入る．
		border.insert(border.begin(), tmp.begin()+idTop, tmp.end());
		border.insert(border.end(), tmp.begin(), tmp.begin()+idTop);
		border.push_back(border[0]);
		//----------------------------------------------------------------
		//	境界条件の設定
		//	できた凸包の内側のセルの速度を設定．凸包の中を塗りつぶし処理
		engine->points.clear();

		curY = border[0].y;
		if (curY < -1) curY = -1;
		iLeft = border.size()-1;
		iRight = 0;
		while(NextLineX()){	//	onlineを含めて，塗りつぶし
			int xStart, xEnd;
			xStart = ceil(left);
			xEnd = right;
			if (xStart < 0) xStart = 0;
			if (xEnd > water->mx-1) xEnd = water->mx-1;
			for(int x = xStart; x<=xEnd; ++x){
				int cx = (x + water->bound.x) % water->mx;
				int cy = (curY + water->bound.y) % water->my;

				Vec3f p = Vec3f((x+0.5f)*water->dh-water->rx, curY*water->dh-water->ry, 0) - solidCenter;
				Vec3f v = solidVel + (solidAngVel^p) - Vec3f(water->velocity.x, water->velocity.y, 0);
				water->u[cx][cy] = v.x;

				engine->points.push_back(Vec3f(x*water->dh-water->rx, curY*water->dh-water->ry, 0));
				engine->points.push_back(engine->points.back() + Vec3f(v.x+water->velocity.x, v.y+water->velocity.y, 0) * 0.1f);
			}
		}
		curY = border[0].y-0.5f;
		if (curY < -1) curY = -1;
		iLeft = border.size()-1;
		iRight = 0;
		while(NextLineY()){	//	onlineを含めて塗りつぶし
			int xStart, xEnd;
			xStart = ceil(left);
			xEnd = right;
			if (xStart < 0) xStart = 0;
			if (xEnd > water->mx-1) xEnd = water->mx-1;
			for(int x = xStart; x<=xEnd; ++x){
				int cx = (x + water->bound.x) % water->mx;
				int cy = (curY + water->bound.y) % water->my;

				Vec3f p = Vec3f(x*water->dh-water->rx, curY*water->dh-water->ry, water->height[cx][cy]) - solidCenter;
				Vec3f v = solidVel + (solidAngVel%p) - Vec3f(water->velocity.x, water->velocity.y, 0);
				water->v[cx][cy] = v.y;
			}
		}
		//	高さにFRMの補正を加える
		curY = border[0].y;
		if (curY < -1) curY = -1;
		iLeft = border.size()-1;
		iRight = 0;
		while(NextLine()){	//	onlineを含めて塗りつぶし
			int xStart, xEnd;
			xStart = ceil(left);
			xEnd = right;
			if (xStart < 0) xStart = 0;
			if (xEnd > water->mx-1) xEnd = water->mx-1;
			for(int x = xStart; x<=xEnd; ++x){
				AddWaterHeight(x, curY, 1);
			}
		}

		//	border の周りに，ぼかしを描画する．
		//	準備のため，まず border を上右下左に4分割する．
		for(int i=0; i<4; ++i) line[i].clear();
		//	上の水平線
		int i=border.size()-1;
		for(; i>=1; --i){
			Vec2f d = border[i] - border[i-1];
			if (d.x<0 || d.x < abs(d.y)) break;
		}
		for(;i<border.size(); ++i){
			line[0].push_back(border[i]);
		}
		for(i=0;i<border.size()-1; ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.x<0 || d.x<abs(d.y)) break;
			line[0].push_back(border[i+1]);
		}
		//	右の垂直線
		line[1].push_back(border[i]);
		for(;i<border.size()-1; ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.y<0 || abs(d.x)>d.y) break;
			line[1].push_back(border[i+1]);
		}
		//	下の水平線
		line[2].push_back(border[i]);
		for(;i<border.size()-1; ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.x>0 || -d.x<abs(d.y)) break;
			line[2].push_back(border[i+1]);
		}
		//	左の垂直線
		line[3].push_back(border[i]);
		for(;i<border.size()-1; ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.y>0 || abs(d.x)>-d.y) break;
			line[3].push_back(border[i+1]);
		}

		//	凸包の外側のぼかし
		//	onlineは書かない．
		const float lineWidth = 2.0f;	//	線の幅
		const float lineWidthInv = 1/lineWidth;
		
		//	線の描画
		#define DRAWLINE(vtxs, id1, id2, X, Y, XOFF, YOFF, yStart, yEnd, func)\
		for(int i=0; i<vtxs.size()-1; ++i){							\
			Vec2f delta = vtxs[id2] - vtxs[id1];					\
			float k = delta.Y / delta.X;							\
			int ix = ceil(vtxs[id1].X+XOFF);						\
			float y = vtxs[id1].Y+YOFF + k*(ix-(vtxs[id1].X+XOFF));	\
			for(; ix < vtxs[id2].X+XOFF; ++ix){						\
				for(int iy = y+yStart+1; iy < y+yEnd; ++iy){		\
					func(i##X, i##Y, 1-abs(iy-y)*lineWidthInv);		\
				}													\
				y += k;												\
			}														\
		}															
		//	4隅の4角形の描画
		#define DRAWRECT(vtx, XOFF, YOFF, xStart, xEnd, yStart, yEnd, func)				\
		for(int ix = ceil(vtx.x+XOFF+xStart); ix < vtx.x+XOFF+xEnd; ++ix){		\
			for(int iy = ceil(vtx.y+YOFF+yStart); iy < vtx.y+YOFF+yEnd; ++iy){	\
				float dist = sqrt(Square(ix - (vtx.x+XOFF))						\
									+ Square(iy - (vtx.y+YOFF)));				\
				if (dist > lineWidth) dist = lineWidth;							\
				func(ix, iy, 1-dist*lineWidthInv);								\
			}																	\
		}
		//	流速の境界条件
		DRAWRECT(line[0].back(), -0.5f, 0, 0, lineWidth, -lineWidth, 0, SetWaterVelocityU);
		DRAWRECT(line[1].back(), -0.5f, 0, 0, lineWidth, 0, lineWidth, SetWaterVelocityU);
		DRAWRECT(line[2].back(), -0.5f, 0, -lineWidth, 0, 0, lineWidth, SetWaterVelocityU);
		DRAWRECT(line[3].back(), -0.5f, 0, -lineWidth, 0, -lineWidth, 0, SetWaterVelocityU);
		
		DRAWRECT(line[0].back(), 0, -0.5f, 0, lineWidth, -lineWidth, 0, SetWaterVelocityV);
		DRAWRECT(line[1].back(), 0, -0.5f, 0, lineWidth, 0, lineWidth, SetWaterVelocityV);
		DRAWRECT(line[2].back(), 0, -0.5f, -lineWidth, 0, 0, lineWidth, SetWaterVelocityV);
		DRAWRECT(line[3].back(), 0, -0.5f, -lineWidth, 0, -lineWidth, 0, SetWaterVelocityV);
		
		DRAWLINE(line[0], i, i+1, x, y, -0.5f, 0, -lineWidth, 0, SetWaterVelocityU);
		DRAWLINE(line[1], i, i+1, y, x, 0, -0.5f,  0, lineWidth, SetWaterVelocityU);
		DRAWLINE(line[2], i+1, i, x, y, -0.5f, 0,  0, lineWidth, SetWaterVelocityU);
		DRAWLINE(line[3], i+1, i, y, x, 0, -0.5f, -lineWidth, 0, SetWaterVelocityU);

		DRAWLINE(line[0], i, i+1, x, y, 0, -0.5f, -lineWidth, 0, SetWaterVelocityV);
		DRAWLINE(line[1], i, i+1, y, x, -0.5f, 0,  0, lineWidth, SetWaterVelocityV);
		DRAWLINE(line[2], i+1, i, x, y, 0, -0.5f,  0, lineWidth, SetWaterVelocityV);
		DRAWLINE(line[3], i+1, i, y, x, -0.5f, 0, -lineWidth, 0, SetWaterVelocityV);


		//	FRMを波高に適用
		DRAWRECT(line[0].back(), 0, 0, 0, lineWidth, -lineWidth, 0, AddWaterHeight);
		DRAWRECT(line[1].back(), 0, 0, 0, lineWidth, 0, lineWidth, AddWaterHeight);
		DRAWRECT(line[2].back(), 0, 0, -lineWidth, 0, 0, lineWidth, AddWaterHeight);
		DRAWRECT(line[3].back(), 0, 0, -lineWidth, 0, -lineWidth, 0, AddWaterHeight);
	
		DRAWLINE(line[0], i, i+1, x, y, 0, 0, -lineWidth, 0, AddWaterHeight);
		DRAWLINE(line[1], i, i+1, y, x, 0, 0,  0, lineWidth, AddWaterHeight);
		DRAWLINE(line[2], i+1, i, x, y, 0, 0,  0, lineWidth, AddWaterHeight);
		DRAWLINE(line[3], i+1, i, y, x, 0, 0, -lineWidth, 0, AddWaterHeight);
	}
	void AddWaterHeight(int ix, int iy, float alpha){
		if(!frm) return;
		assert(-1e-5<=alpha && alpha<=1+1e-5);
		int cx = (ix + water->bound.x) % water->mx;
		int cy = (iy + water->bound.y) % water->my;

		Vec3f p = Vec3f((ix+0.5f)*water->dh-water->rx, iy*water->dh-water->ry, water->height[cx][cy]) - solidCenter;
		PHWHapticSource* hsrc = frm->FindHsrc(Awginv * p);
		float prs = hsrc->GetPressure();
		water->height[cx][cy] += prs*alpha / (water->density*water->gravity);
	}
	void SetWaterVelocityU(int ix, int iy, float alpha){
		assert(-1e-5<=alpha && alpha<=1+1e-5);
//		alpha = 1 - 1/(1+exp((alpha-0.5)*4));	//	シグモイド

		int cx = (ix + water->bound.x) % water->mx;
		int cy = (iy + water->bound.y) % water->my;

		Vec3f p = Vec3f((ix+0.5f)*water->dh-water->rx, iy*water->dh-water->ry, water->height[cx][cy]) - solidCenter;
		Vec3f v = (solidVel + (solidAngVel%p)) - Vec3f(water->velocity.x, water->velocity.y, 0);
		water->u[cx][cy] = alpha*v.x + (1-alpha)*water->u[cx][cy];
		engine->points.push_back(Vec3f((ix+0.5)*water->dh-water->rx, iy*water->dh-water->ry, 0));
		engine->points.push_back(engine->points.back() + Vec3f(v.x+water->velocity.x, v.y+water->velocity.y, 0) * 0.1f * alpha);
	}
	void SetWaterVelocityV(int ix, int iy, float alpha){
		assert(-1e-5<=alpha && alpha<=1+1e-5);
//		alpha = 1 - 1/(1+exp((alpha-0.5)*4));	//	シグモイド

		int cx = (ix + water->bound.x) % water->mx;
		int cy = (iy + water->bound.y) % water->my;

		Vec3f p = Vec3f(ix*water->dh-water->rx, (iy+0.5f)*water->dh-water->ry, water->height[cx][cy]) - solidCenter;
		Vec3f v = (solidVel + (solidAngVel%p)) - Vec3f(water->velocity.x, water->velocity.y, 0);
		water->v[cx][cy] = alpha*v.y + (1-alpha)*water->v[cx][cy];
	}
	void CalcTriangle(Vec3f* p, float* depth, float* height, float* pressure){
		const float B=0.1f;

		assert(depth[0] >=0);
		assert(depth[1] >=0);
		assert(depth[2] >=0);
		
		Vec3f a = p[1] - p[0];
		Vec3f b = p[2] - p[0];
		Vec3f normalS = -a^b;
		Vec3f normal = normalS.unit();

		Vec3f vtxVel[3];
		float vel[3];
		float pres[3];
		for(int i=0; i<3; ++i){
			vtxVel[i] = solidVel + (solidAngVel^(p[i]-solidCenter));
			vel[i] = -vtxVel[i] * normal;
			pres[i] = pressure[i] / (water->density * water->gravity);
		}

		Vec3f volume = (1.0f/6.0f) * (depth[0]+pres[0] + depth[1]+pres[1] + depth[2]+pres[2]) * normalS;
		Vec3f velInt = (1.0f/6.0f) * (vel[0] + vel[1] + vel[2]) * normalS;
		Vec3f volumeMom = (
					((1.0f/12.0f)*(depth[0]+pres[0]) + (1.0f/24.0f)*(depth[1]+pres[1]) + (1.0f/24.0f)*(depth[2]+pres[2])) * p[0]
				+	((1.0f/24.0f)*(depth[0]+pres[0]) + (1.0f/12.0f)*(depth[1]+pres[1]) + (1.0f/24.0f)*(depth[2]+pres[2])) * p[1]
				+	((1.0f/24.0f)*(depth[0]+pres[0]) + (1.0f/24.0f)*(depth[1]+pres[1]) + (1.0f/12.0f)*(depth[2]+pres[2])) * p[2]
			  ) ^ normalS;
		Vec3f velIntMom = (
					((1.0f/12.0f)*vel[0] + (1.0f/24.0f)*vel[1] + (1.0f/24.0f)*vel[2]) * p[0]
				+	((1.0f/24.0f)*vel[0] + (1.0f/12.0f)*vel[1] + (1.0f/24.0f)*vel[2]) * p[1]
				+	((1.0f/24.0f)*vel[0] + (1.0f/24.0f)*vel[1] + (1.0f/12.0f)*vel[2]) * p[2]
			  ) ^ normalS;

/*		ほとんど影響がないので，高速化のため省略
		//	波高の補正	三角形内の水について，波高に衝突による圧力の影響を加える．
		float velH = - (vtxVel[0].z+vtxVel[1].z+vtxVel[2].z)/3 * B * water->dh*water->dh;
		Vec2f vtx[3];
		for(int i=0; i<3; ++i){
			vtx[i].x = (p[i].x+water->rx)*water->dhinv;
			vtx[i].y = (p[i].y+water->ry)*water->dhinv;
		}
		if (vtx[1].y > vtx[2].y){
			std::swap(vtx[1], vtx[2]);
			std::swap(height[1], height[2]);
		}
		if (vtx[0].y > vtx[1].y){
			std::swap(vtx[0], vtx[1]);
			std::swap(height[0], height[1]);
		}
		if (vtx[1].y > vtx[2].y){
			std::swap(vtx[1], vtx[2]);
			std::swap(height[1], height[2]);
		}
		int iy = ceil(vtx[0].y);
		if (iy<0) iy = 0;
		float dLeft = (vtx[1].x-vtx[0].x)/(vtx[1].y-vtx[0].y);
		float left = vtx[0].x + (iy-vtx[0].y)*dLeft;
		float dRight = (vtx[2].x-vtx[0].x)/(vtx[2].y-vtx[0].y);
		float right = vtx[0].x + (iy-vtx[0].y)*dRight;
		
		a.sub_vector(0, Vec2f()) = vtx[1]-vtx[0];
		b.sub_vector(0, Vec2f()) = vtx[2]-vtx[0];
		a.z = height[1]-height[0];
		b.z = height[2]-height[0];
		Vec3f triNormal = (a^b).unit();
		if (triNormal.z <0) triNormal *= -1;
		Vec2f deltaHeight(-triNormal.x/triNormal.z, -triNormal.y/triNormal.z);
		for(; iy < vtx[1].y; ++iy){
			int xStart, xEnd;
			if (left < right){
				xStart = ceil(left); xEnd = right;
			}else{
				xStart = ceil(right); xEnd = left;
			}
			if (xStart < 0) xStart = 0;
			if (xEnd > water->mx) xEnd = water->mx;
			int cy = (iy+water->bound.y)%water->my;

			float curHeight = height[0] + (xStart - vtx[0].x)*deltaHeight.x
				+ (iy - vtx[0].y)*deltaHeight.x;
			for(int ix = xStart; ix<=xEnd; ++ix){
				int cx = (ix+water->bound.x)%water->mx;				
				float heightError = water->height[cx][cy] - curHeight;
				volume += heightError * water->dh * water->dh * normalS;
				volumeMom += (heightError * water->dh * water->dh) * (Vec3f(ix*water->dh - water->rx, iy*water->dh - water->ry, curHeight) ^ normalS);
				water->height[cx][cy] += velH;
				curHeight += deltaHeight.x;
			}
			left += dLeft;
			right += dRight;
		}
		dLeft = (vtx[2].x-vtx[1].x)/(vtx[2].y-vtx[1].y);
		left = vtx[1].x + (iy-vtx[1].y)*dLeft;
		for(; iy < vtx[2].y; ++iy){
			int xStart, xEnd;
			if (left < right){
				xStart = ceil(left); xEnd = right;
			}else{
				xStart = ceil(right); xEnd = left;
			}
			if (xStart < 0) xStart = 0;
			if (xEnd > water->mx) xEnd = water->mx;
			int cy = (iy+water->bound.y)%water->my;
			float curHeight = height[0] + (xStart - vtx[0].x)*deltaHeight.x
				+ (iy - vtx[0].y)*deltaHeight.x;
			for(int ix = xStart; ix<xEnd; ++ix){
				int cx = (ix+water->bound.x)%water->mx;
				float heightError = water->height[cx][cy] - curHeight;
				volume += heightError * water->dh * water->dh * normalS;
				volumeMom += (heightError * water->dh * water->dh) * (Vec3f(ix*water->dh - water->rx, iy*water->dh - water->ry, curHeight) ^ normalS);
				water->height[cx][cy] += velH;
			}
			left += dLeft;
			right += dRight;
		}
*/	
		//	圧力による力を計算．安定させるため，粘性も入れる．
		buo += (volume + B*velInt) * water->density * water->gravity;
		tbuo += (volumeMom + B*velIntMom) * water->density * water->gravity;

		engine->tris.push_back(p[0]);
		engine->tris.push_back(p[1]);
		engine->tris.push_back(p[2]);
	}
};

PHWConvexCalc convCalc;
struct PHWCE_VtxFVF{
	Vec3f pos;
	DWORD color;
};
void PHWaterContactEngine::Render(GRRender* render, SGScene* s){	
	//	描画
	if (!render->bDrawDebug) return;
	if (!render || !render->CanDraw()) return;
	render->SetModelMatrix(water->GetPosture());
	render->SetDepthTest(false);
	
	//	境界線の描画
	GRMaterialData mat(Vec4f(0, 0, 1, 0.5f), 2);
	render->SetMaterial(mat);
	render->DrawDirect(GRRender::TRIANGLES, &*(tris.begin()), &*(tris.end()));

	GRMaterialData mats[4] = {
		GRMaterialData(Vec4f(1,    0.5f, 0.5f, 1), 2),
		GRMaterialData(Vec4f(1,       1, 0.5f, 1), 2),
		GRMaterialData(Vec4f(0.5f,    1,    1, 1), 2),
		GRMaterialData(Vec4f(0.5f, 0.5f,    1, 1), 2)
	};

	float dh = water->dh;
	float rx = water->rx;
	float ry = water->ry;
	for(int ii=0; ii<4; ++ii){
		render->SetMaterial(mats[ii]);
		std::vector<Vec3f> vtxs;
		if (convCalc.line[ii].size() > 1){
			for(int i=0; i<convCalc.line[ii].size()-1; ++i){
				vtxs.push_back(Vec3f((convCalc.line[ii][i].x)*dh-rx, (convCalc.line[ii][i].y)*dh-ry, 0));
				vtxs.push_back(Vec3f((convCalc.line[ii][i+1].x)*dh-rx, (convCalc.line[ii][i+1].y)*dh-ry, 0));
			}
			render->DrawDirect(GRRender::LINES, &*vtxs.begin(), &*vtxs.end());
		}
	}
	//	境界条件の速度の表示
	D3Render* d3r = DCAST(D3Render, render);
	if (d3r){
		d3r->device->SetRenderState(D3DRS_LIGHTING, false);
		d3r->device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
		std::vector<PHWCE_VtxFVF> vtxs;
		for(int i=0; i<points.size(); ++i){
			vtxs.push_back(PHWCE_VtxFVF());
			vtxs.back().pos = points[i];
			vtxs.back().color = D3DCOLOR_RGBA(0xFF,0,0xFF, i%2 ? 0 : 0xFF);
		}
		if (vtxs.size()) d3r->device->DrawPrimitiveUP(D3DPT_LINELIST, vtxs.size()/2, &*(vtxs.begin()), sizeof(PHWCE_VtxFVF));
		d3r->device->SetRenderState(D3DRS_LIGHTING, true);
	}
	//	hsrcの描画
	render->SetMaterial(GRMaterialData(Vec4f(1,1,0,1),2));
	render->SetLineWidth(10);
	for(int i=0; i<solids.size(); ++i){
		for(int j=0; j<solids[i]->geometries.size(); ++j){
			SGFrame* frame =  solids[i]->geometries[j]->frame;
			PHWaterRegistanceMap* frm = solids[i]->geometries[j]->frm;
			if (frm && frame){
				render->SetModelMatrix(frame->GetWorldPosture());
				std::vector<Vec3f> buf;
				for(int i=0; i<frm->hsrc.size(); ++i){
					buf.push_back(frm->hsrc[i].GetPos());
				}
				render->DrawDirect(GRRender::POINTS, buf.begin(), buf.end());
			}
		}		
	}

	render->SetDepthTest(true);
}
void PHWaterContactEngine::Step(SGScene* s){
	tris.clear();
	scene = s;
	PHWGeometry* geo;
	CDPolyhedron* poly;
	PHWGeometries::iterator ig;
	CDGeometries::iterator ic;

	convCalc.Init(this);
	convCalc.Aw = water->GetPosture();
	convCalc.Awinv = convCalc.Aw.inv();

	//剛体に加わる浮力を計算する
	//全剛体について･･･
	for(int i=0; i < solids.size(); i++){
		convCalc.SetSolid(solids[i]);

		//全ジオメトリについて･･･
		for(ig = convCalc.solid->geometries.begin(); ig != convCalc.solid->geometries.end(); ig++){
			geo = *ig;
			convCalc.Ag = geo->frame->GetWorldPosture();
			convCalc.Asg = convCalc.Asinv * convCalc.Ag;
			convCalc.Awg = convCalc.Awinv * convCalc.Ag;
			convCalc.Awginv = convCalc.Awg.inv();
			Vec3f meshVel = convCalc.Awginv.Rot() * (convCalc.solidVel - Vec3f(water->velocity.x, water->velocity.y, 0));
			if (geo->frm) geo->frm->SetVelocity(meshVel);

			//BBoxレベルでの接触チェック
			//...

			//凸多面体の各面に加わる浮力をジオメトリの中心を基準として積算
			convCalc.buo.clear();
			convCalc.tbuo.clear();

			convCalc.depth.resize(geo->mesh->vertices.size());
			convCalc.pressure.resize(geo->mesh->vertices.size());
			convCalc.height.resize(geo->mesh->vertices.size());
			convCalc.vtxs.resize(geo->mesh->vertices.size());
			for(int i=0; i<geo->mesh->vertices.size(); ++i){
				convCalc.vtxs[i] = convCalc.Awg * geo->mesh->vertices[i];	//	水座標系での頂点
				convCalc.height[i] = water->LerpHeight(convCalc.vtxs[i].x, convCalc.vtxs[i].y);
				convCalc.depth[i] = convCalc.height[i] - convCalc.vtxs[i].z;
				//	FRMから頂点での圧力補正を求める．
				if (convCalc.depth[i] > 0 && geo->frm){
					Vec3f prs, fri;
					convCalc.pressure[i] = geo->frm->vtxHsrcMap[i]->GetPressure();
				}else{
					convCalc.pressure[i] = 0;
				}
			}
			convCalc.frm = geo->frm;
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

//----------------------------------------------------------------------------
//PHWaterRegistanceMap

SGOBJECTIMP(PHWaterRegistanceMap, SGObject);

PHWaterRegistanceMap::PHWaterRegistanceMap(){
	dTheta = M_PI / NTHETA;
	dPhi = 2*M_PI / NTHETA;
}

bool PHWaterRegistanceMap::AddChildObject(SGObject* o, SGScene* scene){
	if(DCAST(SGFrame, o)){
		frame = DCAST(SGFrame, o);
		return true;
	}
	return false;
}
void PHWaterRegistanceMap::SetVelocity(Vec3f vel){
	float l = Square(vel.x)+Square(vel.y);
	float th = 0.5f*M_PI - atan2(vel.z, l);
	float phi = atan2(vel.y, vel.x);
	float norm = vel.norm();
	for(int i=0; i<hsrc.size(); ++i){
		hsrc[i].SetVelocity(th, phi, vel, 0.0f);
	}
}
void PHWaterRegistanceMap::Loaded(SGScene* scene){
	//FILE* fp = fopen(filename.c_str(), "rb");
	//if(!fp)return;
    
	//int id;
	//float buo_scl, pres_scl, fric_scl, vel_scl, unit_mass, disp_scl;
	//float wz, wa;
    //fscanf(fp, "%d%f%f%f%f%f%f", &id, &buo_scl, &pres_scl, &fric_scl, &vel_scl, &unit_mass, &disp_scl);
    //fscanf(fp, "%f%f", &wz, &wa);

	//objの位置と姿勢
    /*Vec3f pos0, we0;
	fscanf(fp, "%f%f%f%f%f%f", &pos0.X(), &pos0.Y(), &pos0.Z(), &we0.X(), &we0.Y(), &we0.Z());
	//yaw pitch rollかと思われ
    we0.X() = Rad(we0.X());
    we0.Y() = Rad(we0.Y());
    we0.Z() = Rad(we0.Z());
    float stx, sty, stz, ctx, cty, ctz;
    stx = sin(we0.X()); ctx = cos(we0.X());
    sty = sin(we0.Y()); cty = cos(we0.Y());
    stz = sin(we0.Z()); ctz = cos(we0.Z());
	Affinef aff;
	aff.Rot() = Matrix3f(cty*ctz, -stx*sty*ctz-ctx*stz, -ctx*sty*ctz+stx*stz,
						 cty*stz, -stx*sty*stz+ctx*ctz, -ctx*sty*stz-stx*ctz,
						 sty,	   stx*cty,				 ctx*cty);
	aff.Pos() = pos0;
	posture = aff;*/
    
    //メッシュを細分化してサンプル点を生成する処理
	//float dlen;
    //fscanf(fp, "%f", &dlen);
	/*if(dlen > 0.0) { 
        pobj.mesh = (Tmesh *)malloc(sizeof(Tmesh)*obj[pobj.id_obj].nf);
        for(i=0; i<obj[pobj.id_obj].nf; i++) pobj.mesh[i].dlen = dlen;
        genMesh(&pobj);
    }else {
        // USE OBJECT VERTEX AS SAMPLE POINTS
        pobj.mesh = NULL;
        genPoints();
    }*/
	
	/*float v0;
    filename.resize(256);
	Vec3f d, s;
	fscanf(fp, "%f%s", &v0, &filename[0]);
    fscanf(fp, "%f%f%f%f%f%f", &d.X(), &d.Y(), &d.Z(), &s.X(), &s.Y(), &s.Z());
	*/
	float v0 = 2.0;
    FILE* fm = fopen(filename.c_str(), "rb");
    if(fm != NULL) {
        float dthe, dphi;
		int nhsrc, ntex, ndata, rate, nthe, nphi, sym[3], n;
		fread(&dthe, sizeof(float), 1, fm);	//1.57
        fread(&dphi, sizeof(float), 1, fm);	//0.52
        fread(&nhsrc, sizeof(int), 1, fm);	//12
        fread(&ndata, sizeof(int), 1, fm);	//998
        fread(&rate, sizeof(int), 1, fm);	//999
        fread(&nthe, sizeof(int), 1, fm);	//3
        fread(&nphi, sizeof(int), 1, fm);	//12
        fread(sym, sizeof(int), 3, fm);
        fread(&n, sizeof(int), 1, fm);		//12

		ntex = (nthe - 2) * nphi + 2;		//14
        
		std::vector<int> id; id.resize(n);
        fread(&id[0], sizeof(int), n, fm);
        
		hsrc.resize(nhsrc);
        //frc_set.resize(nhsrc);
        int i;
		float frc_max;
		Vec3f p, normal;
		std::vector<float> xprs(ndata), yprs(ndata), zprs(ndata), xfri(ndata), yfri(ndata), zfri(ndata);
        for(i = 0; i < nhsrc; i++) {
            hsrc[i].dthe = dthe;
            hsrc[i].dphi = dphi;
            hsrc[i].nthe = nthe;
            hsrc[i].nphi = nphi;
            hsrc[i].ntex = ntex;
            hsrc[i].v0 = v0;
            
			//hsrc[i].sym = sym;
            //hsrc[i].vx = hsrc[i].vy = hsrc[i].vz = hsrc[i].v = hsrc[i].tcoord = 0.0;
            
			fread(&hsrc[i].pos, sizeof(Vec3f), 1, fm);
            fread(&hsrc[i].normal, sizeof(Vec3f), 1, fm);
            //p += d;
            //p.X() *= s.X(); p.Y() *= s.Y(); p.Z() *= s.Z();
			//if(nhsrc == 1){normal.X() = 1.0; normal.Y() = 0.0; normal.Z() = 0.0;}
            //hsrc[i].p = hsrc[i].p0 = hsrc[i].p_ori = p;
            //hsrc[i].n = normal;            
			//hsrc[i].prs.clear();

			hsrc[i].ftex.resize(ntex);
			for(int j = 0; j < ntex; j++){
				hsrc[i].ftex[j].ndata = 0;
			}
            for(int j = 0; j < n; j++) {
                int k = id[j];
                hsrc[i].ftex[k].ndata = ndata;
                hsrc[i].ftex[k].rate = rate;
                hsrc[i].ftex[k].v0 = v0;
                fread(&xprs[0], sizeof(float), ndata, fm);
                fread(&yprs[0], sizeof(float), ndata, fm);
                fread(&zprs[0], sizeof(float), ndata, fm);
                fread(&xfri[0], sizeof(float), ndata, fm);
                fread(&yfri[0], sizeof(float), ndata, fm);
                fread(&zfri[0], sizeof(float), ndata, fm);
				hsrc[i].ftex[k].prs.resize(ndata);
                hsrc[i].ftex[k].fri.resize(ndata);
				for(int ii = 0; ii < ndata; ii++){
					hsrc[i].ftex[k].prs[ii].X() = xprs[ii];
					hsrc[i].ftex[k].prs[ii].Y() = yprs[ii];
					hsrc[i].ftex[k].prs[ii].Z() = zprs[ii];
					hsrc[i].ftex[k].fri[ii].X() = xfri[ii];
					hsrc[i].ftex[k].fri[ii].Y() = yfri[ii];
					hsrc[i].ftex[k].fri[ii].Z() = zfri[ii];
				}
                /*hsrc[i].ftex[k].peri_prs.x =
                hsrc[i].ftex[k].peri_prs.y =
                hsrc[i].ftex[k].peri_prs.z = (Trealf)hsrc[i].ftex[k].n/(Trealf)hsrc[i].ftex[k].rate;
                hsrc[i].ftex[k].peri_frdi.x =
                hsrc[i].ftex[k].peri_fri.y =
                hsrc[i].ftex[k].peri_fri.z = (Trealf)hsrc[i].ftex[k].n/(Trealf)hsrc[i].ftex[k].rate;
                hsrc[i].ftex[k].phas_prs.x =
                hsrc[i].ftex[k].phas_prs.y =
                hsrc[i].ftex[k].phas_prs.z = 0.0;
                hsrc[i].ftex[k].phas_fri.x =
                hsrc[i].ftex[k].phas_fri.y =
                hsrc[i].ftex[k].phas_fri.z = 0.0;*/

                frc_max = 0.0;
                for(int m = 0; m < ndata; m++)
					frc_max = std::max(frc_max, std::max(hsrc[i].ftex[k].prs[m].norm(), hsrc[i].ftex[k].fri[m].norm()));
            }
        }
        for(i = 0; i < nhsrc; i++) {
            for(int j = 0; j < n; j++) {
                //if(hsrc[i].ftex[j].rate > 0) {
                    for(int k = 0; k < ndata; k++) {
                        hsrc[i].ftex[id[j]].prs[k] /= frc_max;
                        hsrc[i].ftex[id[j]].fri[k] /= frc_max;
                    }
                //}
            }
        }
    }/* else {
        // SET POINT HAPTIC SOURCE
        int j, n, id_fset;

        fscanf(fp, "%d", &n);
        pobj.nhsrc = n;
        pobj.hsrc = (ThapticSource *)malloc(sizeof(ThapticSource)*n);
        hp = pobj.hsrc;
        for(i=0; i<n; i++) {
            fscanf(fp,"%f%f%f%d", &hp->x0, &hp->y0, &hp->z0, &hp->ntex);
            if(hp->ntex > 0) {
                hp->itex = (int *)malloc(sizeof(int)*hp->ntex);
                for(j=0; j<hp->ntex; j++) fscanf(fp, "%d", &hp->itex[j]);
                hp->fset = NULL;
            } else {
                hp->itex = NULL;
                fscanf(fp, "%d", &id_fset);
                hp->fset = &(frc_set[id_fset]);
            }
            fscanf(fp,"%s%s%s", cx, cy, cz);
            hp->x = hp->x0;
            hp->y = hp->y0;
            hp->z = hp->z0;
            hp->x_ori = hp->x0;
            hp->y_ori = hp->y0;
            hp->z_ori = hp->z0;
            hp->vx = hp->vy = hp->vz =
            hp->v = hp->tcoord = 0.0;
            hp->sym.x = hp->sym.y = hp->sym.z = FALSE;
            if(cx[0] == 'x') hp->sym.x = TRUE;
            if(cy[0] == 'y') hp->sym.y = TRUE;
            if(cz[0] == 'z') hp->sym.z = TRUE;
            hp++;
        }
    }*/
    
    /*pobj.im.x = pobj.im.y = pobj.im.z = -1.0;
    fscanf(fp, "%s", com);
    if(!strcmp("param", com)) {
        fscanf(fp, "%f%f%f%f%f", &pobj.mass, &pobj.im.x, &pobj.im.y, &pobj.im.z, &pobj.loss);
        pobj.ori[0] = 1.0; pobj.ori[1] = 0.0; pobj.ori[2] = 0.0; pobj.ori[3] = 0.0;
        pobj.a.x = pobj.a.y = pobj.a.z = 
        pobj.vel.x = pobj.vel.y = pobj.vel.z = 0.0;
    }*/
	fclose(fm);
	InitFrmMap();
}

void PHWaterRegistanceMap::InitMesh(){
	//-----------------------------------------------------------------------
	//	HSrcと頂点・座標との対応表の作成
	//
	//	フレームからCDMeshを取り出す．
	if (frame){
		SGObjects objs;
		frame->EnumContents(objs);
		for(SGObjects::iterator it = objs.begin(); it != objs.end(); ++it){
			CDMesh* m = DCAST(CDMesh, *it);
			if (m){
				if (mesh) {
					DSTR << "Don't put multiple meshes in the frame for WaterRegistanceMap";
					exit(-1);
				}
				mesh = m;
			}
		}
	}
}
void PHWaterRegistanceMap::InitFrmMap(){
	if (mesh){
		//	vtxHsrcMap を初期化
		vtxHsrcMap.resize(mesh->vertices.size());
		for(int i=0; i<mesh->vertices.size(); ++i){
			float minDist = 1e10f;
			int minId = -1;
			for(int j=0; j<hsrc.size(); ++j){
				Vec3f dist = mesh->vertices[i] - hsrc[j].GetPos();
				float dist_n = dist.norm();
				if (dist_n < minDist){
					minDist = dist_n;
					minId = j;
				}
			}
			vtxHsrcMap[i] = &hsrc[minId];
		}

		//	方向とhsrcのマップの初期化
		dirHsrcMap.resize(NTHETA * NPHI);
		CDPolyhedron* poly = DCAST(CDPolyhedron, mesh->conveces.front());
		assert(poly);
		//	dirHsrcMap を初期化	the, phi を与えると最近傍のhsrcを返す．
		//	theta : 緯度 (0北極 - NTHETA南極)
		//	phi:	経度 (0-NPHIで1周)
		float theta = dTheta*0.5;
		for(int ith = 0; ith < NTHETA; ++ith){
			float phi = dPhi*0.5;
			for(int iphi = 0; iphi < NPHI; ++iphi){
				Vec3f dir;
				float th = M_PI*0.5f-theta;
				dir.z = sin(th);
				dir.x = cos(phi) * cos(th);
				dir.y = sin(phi) * cos(th);
				Vec3f crossPoint;
				for(CDFaces::iterator itf = poly->faces.begin(); itf != poly->faces.end(); ++itf){
					CDFace& face = *itf;
					int i=0;
					for(; i<3; ++i){
						Vec3f ns = poly->base[face.vtxs[i]] ^ poly->base[face.vtxs[(i+1)%3]];
						if (dir*ns < 0) break;
					}
					if (i == 3){	//	dirが3角形の中
						Vec3f n = (poly->base[face.vtxs[2]] - poly->base[face.vtxs[0]]) ^ (poly->base[face.vtxs[1]] - poly->base[face.vtxs[0]]);
						n.unitize();
						float dist = n * poly->base[face.vtxs[0]];
						float mul = dist / (dir*n);
						crossPoint = dir*mul;
						assert(abs(n*(crossPoint - poly->base[face.vtxs[0]])) < 1e-6f);
						break;
					}
				}
				float minDist = 1e10f;
				int minId = -1;
				for(int i=0; i<hsrc.size(); ++i){
					float dist = (crossPoint - hsrc[i].GetPos()).norm();
					if (dist < minDist){
						minDist = dist;
						minId = i;
					}
				}
				dirHsrcMap[ith*NPHI+iphi] = &hsrc[minId];
				phi += dPhi;
			}
			theta += dTheta;
		}

	}else{
		DSTR << "No mesh found in frame " << frame->GetName() << std::endl;
	}
}
//	対応表を使って，pos に一番近いhsrcを求める
PHWHapticSource* PHWaterRegistanceMap::FindHsrc(Vec3f pos){
	float l = Square(pos.x)+Square(pos.y);
	float th = 0.5f*M_PI - atan2(pos.z, l);
	float phi = atan2(pos.y, pos.x);
	if (phi<0) phi += 2*M_PI;
	int ith = th / dTheta;
	int iphi = phi / dPhi;
	return dirHsrcMap[ith*NPHI+iphi];
}

typedef UTString String;
DEF_RECORD(XWaterRegistanceMap, {
	GUID Guid(){ return WBGuid("dbd4feca-a6ac-48f1-87c9-863162e13658"); } 
	String filename;
});

class PHWaterRegistanceMapLoader : public FIObjectLoader<PHWaterRegistanceMap>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWaterRegistanceMap* rm){
		ctx->objects.Push(rm);
		//FRM
		const char* fn = NULL;
		ctx->docs.Top()->GetWholeData(fn);
		WBPath xFilePath;
		xFilePath.Path(ctx->fileName);
		xFilePath.Path(xFilePath.FullPath());
		WBPath imPath;
		imPath.Path(fn);
		FIString oldCwd = imPath.GetCwd();
		imPath.SetCwd(xFilePath.Drive() + xFilePath.Dir());
		rm->filename = imPath.FullPath();
		return true;
	}
	virtual void Loaded(FILoadScene* ctx){
		ctx->objects.Pop();
	}
	PHWaterRegistanceMapLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(String);
		db->REG_RECORD_PROTO(XWaterRegistanceMap);
	}
};

class PHWaterRegistanceMapSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterRegistanceMap"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		/*PHWaterContactEngine* pc = (PHWaterContactEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("WaterContactEngine", pc);
		ctx->docs.back()->AddChild(doc);
		for(PHWSolids::iterator it = pc->solids.begin(); it != pc->solids.end(); ++it){
			doc->AddChild(ctx->CreateDocNode("REF", (*it)->solid));
		}
		doc->AddChild(ctx->CreateDocNode("REF", pc->water));*/
	}
};
DEF_REGISTER_BOTH(PHWaterRegistanceMap);

void PHWHapticSource::SetVelocity(float the, float phi, Vec3f v, float t){
	/*Trealf the, Trealf phi,		//流速の極座標
	Trealf t,					//恐らく未使用
	Tpoint3f *vp,				//流速
	TforceSet *fset,			//FRM
	Tpoint3f *pres,				//返すべき圧力
	Tpoint3f *fric)				//返すべき摩擦*/

	//int ithe, iphi, k00, k01, k10, k11;
    //Tpoint3f prs0, prs1, prs, fri0, fri1, fri, uu, vv, ww;
    //Trealf ft, fp, fthe, fphi;
    //TfluidForceTex *frc;
	
	Vec3f uu, vv, ww;

	vv = -v;
	uu = Vec3f(vv.y, -vv.x, 0.0).unit();
	//uun = uu.norm();
    /*if(a < 0.000000001) {
        if(vv.z > 0.) {
			uu = Vec3d(1.0, 0.0, 0.0);
			ww = Vec3d(0.0, -1.0, 0.0);
        } else {
			uu = Vec3d(1.0, 0.0, 0.0);
			ww = Vec3d(0.0, 1.0, 0.0);
        }
    } else {*/
    //a = sqrt(a);
    //uu.x /= a; uu.y /=a;
	ww = Vec3f( uu.y * vv.z,
			   -uu.x * vv.z,
			    uu.x * vv.y - uu.y * vv.x).unit();
    //}

	//the, phi -> 変位
	//dthe, dphi -> DBの格子幅
	//ithe, iphi -> DBのインデックス
	//fthe, fphi -> 変位を格子幅で正規化したもの
	//ft, fp -> 一番近い格子からの変位[0, 1]
    float	fthe = the / dthe;
    float	fphi = phi / dphi;
    int		ithe = (int)fthe;
    int		iphi = (int)fphi;
    float	ft = fthe - ithe;
    float	fp = fphi - iphi;

	int k00, k01, k10, k11;
    if(ithe > 0) {
        k00 = (ithe - 1) * nphi + (iphi + 1);
		k01 = (iphi + 1 < nphi) ? k00 + 1 : k00 - iphi;
		k10 = std::min(k00 + nphi, ntex - 1);
		k11 = std::min(k10 + 1, ntex - 1);
		//
        //if(k10 >= ntex) k10 = ntex - 1;
        //if(k11 >= ntex) k11 = ntex - 1;
    } else {
        k00 = 0;
        k01 = 0;
        k10 = iphi + 1;
		k11 = (k10 < nphi) ? k10 + 1 : 1;
    }

    Vec3f prs00, prs01, prs10, prs11, fri00, fri01, fri10, fri11;
	Vec3f prs0, prs1, fri0, fri1;
    Vec3f prs, fri, pres, fric;
    //frc = fset->frc;
	
	int idx;
	if(ftex[k00].ndata > 0) {
		idx = (int)(t*ftex[k00].rate) % ftex[k00].ndata;
		prs00 = ftex[k00].prs[idx];
		fri00 = ftex[k00].fri[idx];
    }
    if(ftex[k01].ndata > 0) {
		idx = (int)(t*ftex[k01].rate) % ftex[k01].ndata;
		prs01 = ftex[k01].prs[idx];
		fri01 = ftex[k01].fri[idx];
    }
    if(ftex[k10].ndata > 0) {
		idx = (int)(t*ftex[k10].rate) % ftex[k10].ndata;
		prs10 = ftex[k10].prs[idx];
        fri10 = ftex[k10].fri[idx];
    }
    if(ftex[k11].ndata > 0) {
		idx = (int)(t*ftex[k11].rate) % ftex[k11].ndata;
		prs11 = ftex[k11].prs[idx];
        fri11 = ftex[k11].fri[idx];
    }

	Matrix3f mat;
	mat.col(0) = uu;
	mat.col(1) = vv;
	mat.col(2) = ww;
	prs0 = prs00 * (1.0 - ft) + prs10 * ft;
	prs1 = prs01 * (1.0 - ft) + prs11 * ft;
	prs  = prs0 * (1.0 - fp) + prs1 * fp;
    pres = - mat * prs;
    
	fri0 = fri00 * (1.0 - ft) + fri10 * ft;
    fri1 = fri01 * (1.0 - ft) + fri11 * ft;
	fri = fri0 * (1.0 - fp) + fri1 * fp;
	fric = mat * fri;
	
	pressure = prs * normal;
}

float PHWHapticSource::GetPressure(){
	return pressure * 100;
}

}

