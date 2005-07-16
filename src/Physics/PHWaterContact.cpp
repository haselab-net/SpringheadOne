#include "Physics.h"
#pragma hdrstop
#include <Physics/PHWater.h>
#include <Physics/PHWaterContact.h>
#include <Collision/CDMesh.h>
#include <Collision/CDQuickHull2D.h>
#include <Collision/CDQuickHull2DImp.h>
#include <ImpD3D/D3Render.h>

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
	Vec3f solidCenter;	//	剛体の重心		(水座標系)
	Vec3f solidVel;		//	剛体の速度		(水座標系)
	Vec3f solidAngVel;	//	剛体の角速度	(水座標系)
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

//				engine->points.push_back(Vec3f(x*water->dh-water->rx, curY*water->dh-water->ry, 0));
//				engine->points.push_back(engine->points.back() + Vec3f(v.x+water->velocity.x, v.y+water->velocity.y, 0) * 0.1f);
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
		for(i=0;i<border.size(); ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.x<0 || d.x<abs(d.y)) break;
			line[0].push_back(border[i+1]);
		}
		//	右の垂直線
		line[1].push_back(border[i]);
		for(;i<border.size(); ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.y<0 || abs(d.x)>d.y) break;
			line[1].push_back(border[i+1]);
		}
		//	下の水平線
		line[2].push_back(border[i]);
		for(;i<border.size(); ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.x>0 || -d.x<abs(d.y)) break;
			line[2].push_back(border[i+1]);
		}
		//	左の垂直線
		line[3].push_back(border[i]);
		for(;i<border.size(); ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.y>0 || abs(d.x)>-d.y) break;
			line[3].push_back(border[i+1]);
		}

		//	凸包の外側のぼかし
		//	onlineは書かない．
		const float lineWidth = 3.0f;	//	線の幅
		const float lineWidthInv = 1/lineWidth;
		
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

#define DRAWRECT(vtx, XOFF, YOFF, xStart, xEnd, yStart, yEnd, func)				\
		for(int ix = ceil(vtx.x+XOFF+xStart); ix < vtx.x+XOFF+xEnd; ++ix){		\
			for(int iy = ceil(vtx.y+YOFF+yStart); iy < vtx.y+YOFF+yEnd; ++iy){	\
				float dist = sqrt(Square(ix - (vtx.x+XOFF))						\
									+ Square(iy - (vtx.y+YOFF)));				\
				if (dist > lineWidth) dist = lineWidth;							\
				func(ix, iy, 1-dist*lineWidthInv);								\
			}																	\
		}

		DRAWRECT(line[0].back(), -0.5f, 0, 0, lineWidth, -lineWidth, 0, SetWaterVelocityU);
		DRAWRECT(line[1].back(), -0.5f, 0, 0, lineWidth, 0, lineWidth, SetWaterVelocityU);
		DRAWRECT(line[2].back(), -0.5f, 0, -lineWidth, 0, 0, lineWidth, SetWaterVelocityU);
		DRAWRECT(line[3].back(), -0.5f, 0, -lineWidth, 0, -lineWidth, 0, SetWaterVelocityU);
		
		DRAWRECT(line[0].back(), 0, -0.5f, 0, lineWidth, -lineWidth, 0, SetWaterVelocityV);
		DRAWRECT(line[1].back(), 0, -0.5f, 0, lineWidth, 0, lineWidth, SetWaterVelocityV);
		DRAWRECT(line[2].back(), 0, -0.5f, -lineWidth, 0, 0, lineWidth, SetWaterVelocityV);
		DRAWRECT(line[3].back(), 0, -0.5f, -lineWidth, 0, -lineWidth, 0, SetWaterVelocityV);
		
		DRAWLINE(line[0], i, i+1, x, y, -0.5f, 0, -lineWidth, 0, SetWaterVelocityU);
		DRAWLINE(line[1], i, i+1, y, x, 0, -0.5f,  0, lineWidth, SetWaterVelocityU)
		DRAWLINE(line[2], i+1, i, x, y, -0.5f, 0,  0, lineWidth, SetWaterVelocityU)
		DRAWLINE(line[3], i+1, i, y, x, 0, -0.5f, -lineWidth, 0, SetWaterVelocityU)

		DRAWLINE(line[0], i, i+1, x, y, 0, -0.5f, -lineWidth, 0, SetWaterVelocityV)
		DRAWLINE(line[1], i, i+1, y, x, -0.5f, 0,  0, lineWidth, SetWaterVelocityV)
		DRAWLINE(line[2], i+1, i, x, y, 0, -0.5f,  0, lineWidth, SetWaterVelocityV)
		DRAWLINE(line[3], i+1, i, y, x, -0.5f, 0, -lineWidth, 0, SetWaterVelocityV)

#if 0
		//	凸包の外側にアンチエイリアス処理
		//	onlineは書かない．
		const float lineWidth = 2.0f;	//	線の幅
		const float lineWidthInv = 1/lineWidth;

		//	u
		TVec2<int> water_m(water->mx, water->my);
		for(int i=0; i<border.size()-1; ++i){
			Vec2f delta = border[i+1] - border[i];
			int X, Y;
			float offsetX, offsetY;
			if (abs(delta.x) < abs(delta.y)){
				X=1; Y=0;
				offsetX = 0;
				offsetY = -0.5f;
			}else{
				X=0; Y=1;
				offsetX = -0.5f;
				offsetY = 0;
			}
			float DY = delta[Y] / delta[X];
			if (delta[X] > 0){
				int ix = ceil(border[i][X]+offsetX);
				float y = border[i][Y]+offsetY + DY*(ix-(border[i][X]+offsetX));
				for(; ix < border[i+1][X]+offsetX; ++ix){
					if (Y){
						for(int iy = y; iy > y-lineWidth; --iy){
							SetWaterVelocityU(ix, iy, (iy-(y-lineWidth))*lineWidthInv);
						}
					}else{
						for(int iy = y + 1; iy < y+lineWidth; ++iy){
							SetWaterVelocityU(iy, ix, ((y+lineWidth)-iy)*lineWidthInv);
						}
					}
				}
			}else{
				int ix = border[i][X]+offsetX;
				float y = border[i][Y]+offsetY + DY*(ix-(border[i][X]+offsetX));
				for(; ix > ceil(border[i+1][X]+offsetX); --ix){
					for(int iy = y + 1; iy < y+lineWidth; ++iy){						
//						if(Y) SetWaterVelocityU(ix, iy, (y+lineWidth-iy)*lineWidthInv);
//						else SetWaterVelocityU(iy, ix, (y+lineWidth-iy)*lineWidthInv);
					}
				}
			}
		}
#endif
#if 0
		//	できた凸包の外側にアンチエイリアス処理．グラデーションしながら線分を描画
		const float lineWidth = 2.0f;	//	線の幅
		const float lineWidthInv = 1/lineWidth;
		TVec2<int> water_m(water->mx, water->my);
		for(int i=0; i<border.size()-1; ++i){
			Vec2f delta = border[i+1] - border[i];
			Vec2f dir = delta.unit();
			Vec2f dAlpha;	
			dAlpha.x = -dir.y*lineWidthInv;	//	dir.x に1進むと dAlpha濃くなる
			dAlpha.y = dir.x*lineWidthInv;	//	dir.y に1進むと dAlpha濃くなる
			Vec2f alphaLen;					//	alphaLen進むと線から抜ける
			alphaLen.x = -1/dAlpha.x;
			alphaLen.y = -1/dAlpha.y;

			int X=0, Y=1;
			if (abs(delta.x) < abs(delta.y)){
				X=1; Y=0;
			}
			Vec2f vtx[2];
			if (delta[X] < 0){
				delta *= -1;
				vtx[0] = border[i+1];
				vtx[1] = border[i];
			}else{
				vtx[0] = border[i];
				vtx[1] = border[i+1];
			}
			float k = delta[Y] / delta[X];

#if 1
			//--------------------------------------------------
			//	u
			//	最初の四角
			int ix = ceil(vtx[0][X]-0.5f -lineWidth);
			float alphaX = 1 - (vtx[0][X]-0.5f-ix)*lineWidthInv;
			int xEnd = vtx[0][X]-0.5f;
			if (xEnd > water_m[X]-1) xEnd = water_m[X]-1;
			float yStart = vtx[0][Y] + alphaLen[Y];
			float yEnd = vtx[0][Y];
			float yLine = yEnd;
			if (yStart > yEnd){
				std::swap(yStart, yEnd);
			}
			for(; ix<=xEnd; ++ix){
				int iy=ceil(yStart);
				if (iy<0) iy = 0;
				int iyEnd = yEnd;
				if (iyEnd > water_m[Y]-1) iyEnd = water_m[Y]-1;
				float alphaY = 1 + (iy-vtx[0][Y])*dAlpha[Y];
				for(; iy<=iyEnd; ++iy){
					if (Y) SetWaterVelocityU(ix, iy, alphaX*alphaY);
					else SetWaterVelocityV(iy, ix, alphaX*alphaY);
					alphaY += dAlpha[Y];
				}
				alphaX += lineWidthInv;
			}
			//	直線部分
			xEnd = vtx[1][X]-0.5f;
			for(; ix<=xEnd; ++ix){
				int iy=ceil(yStart);
				if (iy<0) iy = 0;
				int iyEnd = yEnd;
				if (iyEnd > water_m[Y]-1) iyEnd = water_m[Y]-1;
				float alpha = 1 + (iy-yLine)*dAlpha[Y];
				for(; iy<=iyEnd; ++iy){
					if (Y){
						SetWaterVelocityU(ix, iy, alpha);
					}else{
						SetWaterVelocityV(iy, ix, alpha);
					}
					alpha += dAlpha[Y];
				}
				yLine += k;
				yStart += k;
				yEnd += k;
			}
			//	最後の四角
			alphaX = 1 - (ix-(vtx[1][X]-0.5f))*lineWidthInv;
			xEnd = vtx[1][X]-0.5f + lineWidth;
			if (xEnd > water_m[X]-1) xEnd = water_m[X]-1;
			for(; ix<=xEnd; ++ix){
				int iy=ceil(yStart);
				if (iy<0) iy = 0;
				int iyEnd = yEnd;
				if (iyEnd > water_m[Y]-1) iyEnd = water_m[Y]-1;
				float alphaY = 1 + (iy-yLine)*dAlpha[Y];
				for(; iy<=iyEnd; ++iy){
					if (Y) SetWaterVelocityU(ix, iy, alphaX*alphaY);
					else SetWaterVelocityV(iy, ix, alphaX*alphaY);
					alphaY += dAlpha[Y];
				}
				alphaX -= lineWidthInv;
			}

			//--------------------------------------------------
			//	v
			//	最初の四角
			ix = ceil(vtx[0][X]-lineWidth);
			alphaX = 1 - (vtx[0][X]-ix)*lineWidthInv;
			xEnd = vtx[0][X];
			if (xEnd > water_m[X]-1) xEnd = water_m[X]-1;
			yStart = vtx[0][Y]-0.5f + alphaLen[Y];
			yEnd = vtx[0][Y]-0.5f;
			yLine = yEnd;
			if (yStart > yEnd){
				std::swap(yStart, yEnd);
			}
			for(; ix<=xEnd; ++ix){
				int iy=ceil(yStart);
				if (iy<0) iy = 0;
				int iyEnd = yEnd;
				if (iyEnd > water_m[Y]-1) iyEnd = water_m[Y]-1;
				float alphaY = 1 + (iy-(vtx[0][Y]-0.5f))*dAlpha[Y];
				for(; iy<=iyEnd; ++iy){
					if (Y) SetWaterVelocityV(ix, iy, alphaX*alphaY);
					else SetWaterVelocityU(iy, ix, alphaX*alphaY);
					alphaY += dAlpha[Y];
				}
				alphaX += lineWidthInv;
			}
			//	直線部分
			xEnd = vtx[1][X];
			for(; ix<=xEnd; ++ix){
				int iy=ceil(yStart);
				if (iy<0) iy = 0;
				int iyEnd = yEnd;
				if (iyEnd > water_m[Y]-1) iyEnd = water_m[Y]-1;
				float alpha = 1 + (iy-yLine)*dAlpha[Y];
				for(; iy<=iyEnd; ++iy){
					if (Y){
						SetWaterVelocityV(ix, iy, alpha);
					}else{
						SetWaterVelocityU(iy, ix, alpha);
					}
					alpha += dAlpha[Y];
				}
				yLine += k;
				yStart += k;
				yEnd += k;
			}
			//	最後の四角
			alphaX = 1 - (ix-vtx[1][X])*lineWidthInv;
			xEnd = vtx[1][X] + lineWidth;
			if (xEnd > water_m[X]-1) xEnd = water_m[X]-1;
			for(; ix<=xEnd; ++ix){
				int iy=ceil(yStart);
				if (iy<0) iy = 0;
				int iyEnd = yEnd;
				if (iyEnd > water_m[Y]-1) iyEnd = water_m[Y]-1;
				float alphaY = 1 + (iy-yLine)*dAlpha[Y];
				for(; iy<=iyEnd; ++iy){
					if (Y) SetWaterVelocityV(ix, iy, alphaX*alphaY);
					else SetWaterVelocityU(iy, ix, alphaX*alphaY);
					alphaY += dAlpha[Y];
				}
				alphaX -= lineWidthInv;
			}
#endif
			//	波高の境界条件
#if 0
			int to= alphaLen[Y] < 0 ? 1 : -1;
			float y = vtx[0][Y];
			if (to > 0) y += 1;
			for(int ix = ceil(vtx[0][X]); ix < vtx[1][X]; ++ix){
				int iy = y;
				y += k;
				int iy2 = iy + to;
				if (Y){
					int cx = (ix + water->bound.x)%water->mx;
					int cy = (iy + water->bound.y)%water->my;
					int cy2 = (iy2 + water->bound.y)%water->my;
					water->height[cx][cy2] = water->height[cx][cy];
					engine->points.push_back(Vec3f(ix*water->dh-water->rx, iy*water->dh-water->ry, 0));
					engine->points.push_back(Vec3f(ix*water->dh-water->rx, iy2*water->dh-water->ry, 0));
				}else{
					int cy = (ix + water->bound.y)%water->my;
					int cx = (iy + water->bound.x)%water->mx;
					int cx2 = (iy2 + water->bound.x)%water->mx;
					water->height[cx2][cy] = water->height[cx][cy];
					engine->points.push_back(Vec3f(iy*water->dh-water->ry, ix*water->dh-water->rx, 0));
					engine->points.push_back(Vec3f(iy2*water->dh-water->ry, ix*water->dh-water->rx, 0));
				}
			}
#endif
#if 0
			int from= alphaLen[Y] < 0 ? -4 : 4;
			float y = vtx[0][Y];
			if (from < 0) y += 1;
			for(int ix = ceil(vtx[0][X]); ix < vtx[1][X]; ++ix){
				int iy = y;
				y += k;
				int iy2 = iy + from;
				if (Y){
					int cx = (ix + water->bound.x)%water->mx;
					int cy = (iy + water->bound.y)%water->my;
					int cy2 = (iy2 + water->bound.y)%water->my;
					water->height[cx][cy] = water->height[cx][cy2];
					engine->points.push_back(Vec3f(ix*water->dh-water->rx, iy*water->dh-water->ry, 0));
					engine->points.push_back(Vec3f(ix*water->dh-water->rx, iy2*water->dh-water->ry, 0));
				}else{
					int cy = (ix + water->bound.y)%water->my;
					int cx = (iy + water->bound.x)%water->mx;
					int cx2 = (iy2 + water->bound.x)%water->mx;
					water->height[cx][cy] = water->height[cx2][cy];
					engine->points.push_back(Vec3f(iy*water->dh-water->ry, ix*water->dh-water->rx, 0));
					engine->points.push_back(Vec3f(iy2*water->dh-water->ry, ix*water->dh-water->rx, 0));
				}
			}
#endif
		}
#endif
	}
	void SetWaterVelocityU(int ix, int iy, float alpha){
		assert(0<=alpha && alpha<=1);
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
		assert(0<=alpha && alpha<=1);
//		alpha = 1 - 1/(1+exp((alpha-0.5)*4));	//	シグモイド

		int cx = (ix + water->bound.x) % water->mx;
		int cy = (iy + water->bound.y) % water->my;

		Vec3f p = Vec3f(ix*water->dh-water->rx, (iy+0.5f)*water->dh-water->ry, water->height[cx][cy]) - solidCenter;
		Vec3f v = (solidVel + (solidAngVel%p)) - Vec3f(water->velocity.x, water->velocity.y, 0);
		water->v[cx][cy] = alpha*v.y + (1-alpha)*water->v[cx][cy];
	}
	void CalcTriangle(Vec3f* p, float* depth, float* height){
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
		for(int i=0; i<3; ++i){
			vtxVel[i] = solidVel + (solidAngVel^(p[i]-solidCenter));
			vel[i] = -vtxVel[i] * normal;
		}
		Vec3f volume = (1.0f/6.0f) * (depth[0] + depth[1] + depth[2]) * normalS;
		Vec3f velInt = (1.0f/6.0f) * (vel[0] + vel[1] + vel[2]) * normalS;
		Vec3f volumeMom = (
					((1.0f/12.0f)*depth[0] + (1.0f/24.0f)*depth[1] + (1.0f/24.0f)*depth[2]) * p[0]
				+	((1.0f/24.0f)*depth[0] + (1.0f/12.0f)*depth[1] + (1.0f/24.0f)*depth[2]) * p[1]
				+	((1.0f/24.0f)*depth[0] + (1.0f/24.0f)*depth[1] + (1.0f/12.0f)*depth[2]) * p[2]
			  ) ^ normalS;
		Vec3f velIntMom = (
					((1.0f/12.0f)*vel[0] + (1.0f/24.0f)*vel[1] + (1.0f/24.0f)*vel[2]) * p[0]
				+	((1.0f/24.0f)*vel[0] + (1.0f/12.0f)*vel[1] + (1.0f/24.0f)*vel[2]) * p[1]
				+	((1.0f/24.0f)*vel[0] + (1.0f/24.0f)*vel[1] + (1.0f/12.0f)*vel[2]) * p[2]
			  ) ^ normalS;

		//	波高の補正	三角形内の水について，波高に衝突による圧力の影響を加える．
/*		ついでに圧力の波高による補正をやっているがバグがあって，発散する．
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
		for(int i=0; i<convCalc.line[ii].size()-1; ++i){
			vtxs.push_back(Vec3f((convCalc.line[ii][i].x)*dh-rx, (convCalc.line[ii][i].y)*dh-ry, 0));
			vtxs.push_back(Vec3f((convCalc.line[ii][i+1].x)*dh-rx, (convCalc.line[ii][i+1].y)*dh-ry, 0));
		}
		render->DrawDirect(GRRender::LINES, &*vtxs.begin(), &*vtxs.end());
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
		if (vtxs.size()) d3r->device->DrawPrimitiveUP(D3DPT_LINELIST, vtxs.size()/2, vtxs.begin(), sizeof(PHWCE_VtxFVF));
		d3r->device->SetRenderState(D3DRS_LIGHTING, true);
	}
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
		convCalc.SetSolid(*is);

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

