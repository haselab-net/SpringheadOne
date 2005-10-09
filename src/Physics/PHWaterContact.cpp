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
	//SGFrame::contents����CDGeometry�̔h���N���X�I�u�W�F�N�g��T��
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
#ifdef USE_FRM
	frm = NULL;
#endif
	mesh = NULL;
}
void PHWGeometry::Set(SGFrame* f, CDMesh* g, PHWaterContactEngine* e){
	frame = f;
	mesh = g;
	mesh->CreateTree();
	conveces.resize(g->conveces.size());
	//	frm �̊��蓖��
	std::copy(g->conveces.begin(), g->conveces.end(), conveces.begin());
#ifdef USE_FRM
	frm = NULL;
	for(int i=0; i < e->frms.size(); ++i){
		if (!e->frms[i]->mesh) e->frms[i]->InitMesh();
		if (e->frms[i]->mesh == mesh){
			frm = e->frms[i];
			break;
		}
	}
#endif
}

//----------------------------------------------------------------------------
//	PHWaterContactEngine
//
SGOBJECTIMP(PHWaterContactEngine, SGBehaviorEngine);

PHWaterContactEngine::PHWaterContactEngine(){
	bUseFrm = true;
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
	if(DCAST(SGFrame, o)){
		solids.push_back(new PHWSolid);	//	
		solids.back()->solid = new PHSolid;
		solids.back()->solid->SetMass(FLT_MAX);
        Matrix3d in = Matrix3d::Unit();
		solids.back()->solid->SetInertia(in * 1e200);
		solids.back()->solid->SetFrame((SGFrame*)o);
		return true;
	}
	if(DCAST(PHSolid, o)){
		solids.push_back(new PHWSolid);	//	
		solids.back()->solid = DCAST(PHSolid, o);
		return true;
	}
#ifdef USE_FRM
	if(DCAST(PHWaterResistanceMap, o)){
		PHWaterResistanceMap* frm = (PHWaterResistanceMap*)o;
		frms.push_back(frm);	//	frm�ǉ�
		return true;
	}
#endif
	return false;
}

void PHWaterContactEngine::Clear(SGScene* s){
	water = 0;
	solids.clear();
#ifdef USE_FRM
	frms.clear();
#endif
}
void PHWaterContactEngine::Loaded(SGScene* scene){
	Init(scene);
#ifdef USE_FRM
	for(int i = 0; i < frms.size(); i++)
		frms[i]->Loaded(scene);
#endif
	scene->FindObject(soPaddle, "soPaddle");
}

void PHWaterContactEngine::Init(SGScene* scene){
	//Solid�̌`��f�[�^���z���o��
	for(PHWSolids::iterator is = solids.begin(); is != solids.end(); is++)
		(*is)->Init(this);
}

struct QH2DVertex: public Vec3f{
	QH2DVertex(const Vec3f& v):Vec3f(v){}
	Vec2f GetPos(){ return Vec2f(x,y); }
};

/*	FRM�ɂ�鈳�͕␳�ɂ��Ẵ���
	�E���͂����߂�Ƃ��Ɋe���_�ɑ΂��Ĉ��͂�␳�D
	�E���ɔ��f�����邽�߁C�e���_�Ɉ��͂�������D
	��2���s���D	*/
SGScene* scene;
struct PHWConvexCalc{
	bool bNoBuo;
	PHWaterContactEngine* engine;
	PHWater* water;
	Affinef	Aw, Awinv;	//water-coord to world-coord transformation

	//	Solid�P�ʂ̃f�[�^
	PHWSolid* solid;
	Vec3f solidCenter;	//	���̂̏d�S		(�����W�n)
	Vec3f solidVel;		//	���̂̑��x		(�����W�n)
	Vec3f solidAngVel;	//	���̂̊p���x	(�����W�n)
	Vec3f buo, tbuo;	//���͂ƕ��͂ɂ�郂�[�����g	
	Affinef As, Asinv;	//solid-coord to world-coord
	Affinef Aws;
	Affinef Asg;		//solid-coord to geometry-coord
	
	//	Mesh�P�ʂ̃f�[�^
	Affinef	Ag;			//	geomerty-coord to world-coord
	Affinef Awg;		//	water-coord to geometry-coord
	Affinef Awginv;		//	�����W��Mesh���W
	std::vector<float> depth;		//	���_�̐��[
	std::vector<float> pressure;	//	FRM�ɂ�鈳�͕␳
	std::vector<float> height;		//	���_�ł̔g��
	std::vector<Vec3f> vtxs;		//	�����W�n�ł̒��_�̍��W
#ifdef USE_FRM
	PHWaterResistanceMap* frm;		//	frm
#endif
	
	//	�ʌ`��P�ʂ̃f�[�^
	CDPolyhedron* poly;				//	�Ώۂ̓ʌ`��
	std::vector<Vec3f> vtxsOn;		//	���������ʂ����؂�_(�����W�n)

	//	�h��Ԃ���Ɨp	��cell���W�n (0..mx,0..my)
	std::vector<Vec2f> border;		//	�ʌ`��𐅂ŗ֐؂�ɂ����Ƃ��̗�
	std::vector<Vec2f> line[4];		//	�ւ���E������4�̐܂���ɕ���������
	int iLeft, iRight;				//	�{�[�_�̍��[�̒��_�ƉE�[�̒��_
	float left, right;				//	���[�C�E�[
	int curY;						//	���݂̍s
	float dLeft, dRight;			//	1�s�ł̍��[�̕ω���, �E�[�̕ω���

	void Init(PHWaterContactEngine* e){
		engine = e;
		water = e->water;
	}
	void SetSolid(PHWSolid* s, Affinef posture){
		solid = s;
		As = posture;
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
			//	3�p�`�̒��_���������ɒЂ����Ă��邩������
			int nUnder=0, over, under;
			for(int i=0; i<3; ++i){
				if (depth[iface->vtxs[i]] > 0){
					nUnder++;
					under = i;
				}else{
					over = i;
				}
			}
			if (nUnder == 0) continue;	//	���ɒЂ����Ă��Ȃ�
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
#define NEXTLINE(post, xoff, yoff)											\
	bool NextLine##post(){													\
		curY++;																\
		if (curY>=water->my) return false;									\
		left += dLeft;														\
		right += dRight;													\
		if (iLeft == iRight && curY > border[iLeft].y+yoff) return false;	\
		while (border[iLeft].y+yoff < curY && iLeft!= iRight){				\
			Vec2f last = border[iLeft]+Vec2f(xoff, yoff);					\
			-- iLeft;														\
			Vec2f delta = border[iLeft]+Vec2f(xoff, yoff) - last;			\
			dLeft = delta.x / delta.y;										\
			left = last.x + dLeft * (curY-last.y);							\
		}																	\
		while (border[iRight].y+yoff < curY && iLeft!= iRight){				\
			Vec2f last = border[iRight]+Vec2f(xoff, yoff);					\
			++iRight;														\
			Vec2f delta = border[iRight]+Vec2f(xoff, yoff) - last;			\
			dRight = delta.x / delta.y;										\
			right = last.x + dRight * (curY-last.y);						\
		}																	\
		if (iLeft == iRight && curY > border[iLeft].y+yoff) return false;	\
		return true;														\
	}

	NEXTLINE(X, -0.5f, 0.0f)
	NEXTLINE(Y, 0.0f, -0.5f)
	NEXTLINE(P, 0.0f, 0.0f)
	//	������h��Ԃ��C�A���`�G�C���A�X�̂ӂ��t
	void CalcBorder(){
		border.clear();
		//	���ɋ��E������ݒ�(�ʌ`�󂲂Ƃɏ�������)
		//	���E����钸�_�𐅖ʂɓ��e���C�ʕ�����D
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
		//	�ʕ�̏�[�̒��_��������D
		float top = 1e10f;
		int idTop;
		for(int i=0; i<tmp.size(); ++i){
			if (tmp[i].y < top){
				top = tmp[i].y;
				idTop = i;
			}
		}
		//	border �͐擪�ƍŌ��(y���ŏ���)�������_������D
		border.insert(border.begin(), tmp.begin()+idTop, tmp.end());
		border.insert(border.end(), tmp.begin(), tmp.begin()+idTop);
		border.push_back(border[0]);
		//----------------------------------------------------------------
		//	���E�����̐ݒ�
		//	�ł����ʕ�̓����̃Z���̑��x��ݒ�D�ʕ�̒���h��Ԃ�����
		engine->points.clear();
#if 1
		curY = border[0].y;
		if (curY < -1) curY = -1;
		iLeft = border.size()-1;
		iRight = 0;
		while(NextLineX()){	//	online���܂߂āC�h��Ԃ�
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
		while(NextLineY()){	//	online���܂߂ēh��Ԃ�
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
#endif
		//	������FRM�̕␳��������
		curY = border[0].y;
		if (curY < -1) curY = -1;
		iLeft = border.size()-1;
		iRight = 0;
		while(NextLineP()){	//	online���܂߂ēh��Ԃ�
			int xStart, xEnd;
			xStart = ceil(left);
			xEnd = right;
			if (xStart < 0) xStart = 0;
			if (xEnd > water->mx-1) xEnd = water->mx-1;
			for(int x = xStart; x<=xEnd; ++x){
				AddWaterHeight(x, curY, 1);
			}
		}

		//	border �̎���ɁC�ڂ�����`�悷��D
		//	�����̂��߁C�܂� border ����E������4��������D
		for(int i=0; i<4; ++i) line[i].clear();
		//	��̐�����
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
		//	�E�̐�����
		line[1].push_back(border[i]);
		for(;i<border.size()-1; ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.y<0 || abs(d.x)>d.y) break;
			line[1].push_back(border[i+1]);
		}
		//	���̐�����
		line[2].push_back(border[i]);
		for(;i<border.size()-1; ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.x>0 || -d.x<abs(d.y)) break;
			line[2].push_back(border[i+1]);
		}
		//	���̐�����
		line[3].push_back(border[i]);
		for(;i<border.size()-1; ++i){
			Vec2f d = border[i+1] - border[i];
			if (d.y>0 || abs(d.x)>-d.y) break;
			line[3].push_back(border[i+1]);
		}

		//	�ʕ�̊O���̂ڂ���
		//	online�͏����Ȃ��D
//		float lineWidth = 0.2f/water->dh;	//	���̕�
		const float lineWidth = 2;			//	���̕�
		const float lineWidthInv = 1/lineWidth;
		
		//	���̕`��
		#define DRAWLINE(vtxs, id1, id2, X, Y, XOFF, YOFF, yStart, yEnd, func)\
		for(int i=0; i<vtxs.size()-1; ++i){							\
			Vec2f delta = vtxs[id2] - vtxs[id1];					\
			float k = delta.Y / delta.X;							\
			int ix = ceil(vtxs[id1].X+XOFF);						\
			if (ix < 0) ix = 0;										\
			float y = vtxs[id1].Y+YOFF + k*(ix-(vtxs[id1].X+XOFF));	\
			int ixEnd = vtxs[id2].X+XOFF;							\
			if (ixEnd > water->m##X-1) ixEnd = water->m##X-1;		\
			for(; ix <= ixEnd; ++ix){								\
				int iy = y+yStart+1;								\
				if (iy < 0) iy = 0;									\
				int iyEnd = y+yEnd;									\
				if (iyEnd > water->m##Y-1) iyEnd = water->m##Y-1;	\
				for(; iy <=iyEnd ; ++iy){							\
					func(i##X, i##Y, 1-abs(iy-y)*lineWidthInv);		\
				}													\
				y += k;												\
			}														\
		}															
		//	4����4�p�`�̕`��
		#define DRAWRECT(vtx, XOFF, YOFF, xStart, xEnd, yStart, yEnd, func)	\
		{																	\
			int ix = ceil(vtx.x+XOFF+xStart);								\
			if (ix<0) ix = 0;												\
			int ixEnd = vtx.x+XOFF+xEnd;									\
			if (ixEnd > water->mx-1) ixEnd = water->mx-1;					\
			int iy = ceil(vtx.y+YOFF+yStart);								\
			if (iy<0) iy = 0;												\
			int iyEnd = vtx.y+YOFF+yEnd;									\
			if (iyEnd > water->my-1) iyEnd = water->my-1;					\
			for(; ix <= ixEnd; ++ix){										\
				for(; iy <= iyEnd; ++iy){									\
					float dist = sqrt(Square(ix - (vtx.x+XOFF))				\
										+ Square(iy - (vtx.y+YOFF)));		\
					if (dist > lineWidth) dist = lineWidth;					\
					func(ix, iy, 1-dist*lineWidthInv);						\
				}															\
			}																\
		}
		//	�����̋��E����
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


		//	FRM��g���ɓK�p
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
#ifdef USE_FRM
		if(!frm) return;
		assert(-1e-5<=alpha && alpha<=1+1e-5);
		int cx = (ix + water->bound.x) % water->mx;
		int cy = (iy + water->bound.y) % water->my;

		Vec3f p = Vec3f((ix+0.5f)*water->dh-water->rx, iy*water->dh-water->ry, water->height[cx][cy]) - solidCenter;
		PHWHapticSource* hsrc = frm->FindHsrc(Awginv * p);
		float prs = hsrc->GetPressure();
		water->height[cx][cy] += prs * alpha / (water->density*water->gravity);
#endif
	}

	void SetWaterVelocityU(int ix, int iy, float alpha){
		assert(-1e-5<=alpha && alpha<=1+1e-5);
//		alpha = 1 - 1/(1+exp((alpha-0.5)*4));	//	�V�O���C�h

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
//		alpha = 1 - 1/(1+exp((alpha-0.5)*4));	//	�V�O���C�h

		int cx = (ix + water->bound.x) % water->mx;
		int cy = (iy + water->bound.y) % water->my;

		Vec3f p = Vec3f(ix*water->dh-water->rx, (iy+0.5f)*water->dh-water->ry, water->height[cx][cy]) - solidCenter;
		Vec3f v = (solidVel + (solidAngVel%p)) - Vec3f(water->velocity.x, water->velocity.y, 0);
		water->v[cx][cy] = alpha*v.y + (1-alpha)*water->v[cx][cy];
	}
	void CalcTriangle(Vec3f* p, float* depth, float* height, float* pressure){
		const float B=0.01f;

		assert(depth[0] >=0);
		assert(depth[1] >=0);
		assert(depth[2] >=0);
		
		Vec3f a = p[1] - p[0];
		Vec3f b = p[2] - p[0];
		Vec3f normalS = -a^b;
		float normalS_norm = normalS.norm();
		if (normalS_norm < 1e-6f) return;
		Vec3f normal = normalS / normalS_norm;
		Vec3f vtxVel[3];
		float vel[3];
		float pres[3];
		for(int i=0; i<3; ++i){
			vtxVel[i] = solidVel + (solidAngVel^(p[i]-solidCenter)) - Vec3f(water->velocity.x, water->velocity.y, 0);
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

		//	���͂ɂ��͂��v�Z�D���肳���邽�߁C�S���������D
		const float hVelMul = 0.2f;
		velInt *= hVelMul;
		velIntMom *= hVelMul;
		if (bNoBuo){
			engine->paddleForceK += volume * water->density * water->gravity;
			engine->paddleForceB += (B*velInt) * water->density * water->gravity;
			engine->paddleTorqueK += volumeMom * water->density * water->gravity;
			engine->paddleTorqueB += (B*velIntMom) * water->density * water->gravity;
		}else{
			buo += (volume + B*velInt) * water->density * water->gravity;
			tbuo += (volumeMom + B*velIntMom) * water->density * water->gravity;
		}

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
	if (!bUseFrm){
		render->DrawText(Vec2f(), "FRM Off", GRFont());
	}
	//	�`��
	if (!render->bDrawDebug) return;
	if (!render || !render->CanDraw()) return;
	render->SetModelMatrix(water->GetPosture());
	render->SetDepthTest(false);
	
	D3Render* d3r = DCAST(D3Render, render);
	if (d3r) d3r->cr.Enter();

	//	���E���̕`��
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
	//	���E�����̑��x�̕\��
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
	//	hsrc�̕`��
#ifdef USE_FRM
	render->SetMaterial(GRMaterialData(Vec4f(1,1,0,1),2));
	render->SetLineWidth(3);
	for(int i=0; i<solids.size(); ++i){
		for(int j=0; j<solids[i]->geometries.size(); ++j){
			SGFrame* frame =  solids[i]->geometries[j]->frame;
			PHWaterResistanceMap* frm = solids[i]->geometries[j]->frm;
			if (frm && frame){
				render->SetModelMatrix(frame->GetWorldPosture());
				std::vector<Vec3f> buf;
				for(int i=0; i<frm->hsrc.size(); ++i){
					buf.push_back(frm->hsrc[i].GetPos());
				}
				render->DrawDirect(GRRender::POINTS, &*(buf.begin()), &*(buf.end()));
			}
		}		
	}
#endif

	render->SetDepthTest(true);
	if (d3r) d3r->cr.Leave();
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

	Vec3f waterCenter = water->frame->bbox.GetBBoxCenter();
	Vec3f waterExtent = water->frame->bbox.GetBBoxExtent();
	
	paddleForceK.clear();
	paddleForceB.clear();
	paddleTorqueK.clear();
	paddleTorqueB.clear();
	//���̂ɉ���镂�͂��v�Z����
	//�S���̂ɂ��ĥ��
	for(int i=0; i < solids.size(); i++){
		SGFrame* fr = solids[i]->frame;
		Affinef posture = fr->GetWorldPosture();
		//	�܂�BBox�Ŕ���
		if (!BBoxIntersection(convCalc.Aw, waterCenter, waterExtent,
			posture, fr->bbox.GetBBoxCenter(), fr->bbox.GetBBoxExtent())){
			//	DSTR << "Cull " << fr->GetName() << " in water contact." << std::endl;
			continue;
		}
		if (soPaddle == solids[i]->solid){
			convCalc.bNoBuo = true;
		}else{
			convCalc.bNoBuo = false;
		}
		convCalc.SetSolid(solids[i], posture);

		//�S�W�I���g���ɂ��ĥ��
		for(ig = convCalc.solid->geometries.begin(); ig != convCalc.solid->geometries.end(); ig++){
			geo = *ig;
			SGFrame* fr = geo->frame;
			convCalc.Ag = geo->frame->GetWorldPosture();
			//	�܂�BBox�Ŕ���
			if (!BBoxIntersection(convCalc.Aw, waterCenter, waterExtent,
				convCalc.Ag, fr->bbox.GetBBoxCenter(), fr->bbox.GetBBoxExtent())){
				//	DSTR << "Cull " << fr->GetName() << " in water contact." << std::endl;
				continue;
			}			
			convCalc.Asg = convCalc.Asinv * convCalc.Ag;
			convCalc.Awg = convCalc.Awinv * convCalc.Ag;
			convCalc.Awginv = convCalc.Awg.inv();
			Vec3f meshVel = convCalc.Awginv.Rot() * (convCalc.solidVel - Vec3f(water->velocity.x, water->velocity.y, 0));
#ifdef USE_FRM
			if (bUseFrm && geo->frm) geo->frm->SetVelocity(meshVel, s->GetTimeStep() * s->GetCount() / 10.0);
#endif

			//BBox���x���ł̐ڐG�`�F�b�N
			//...

			//�ʑ��ʑ̂̊e�ʂɉ���镂�͂��W�I���g���̒��S����Ƃ��ĐώZ
			convCalc.buo.clear();
			convCalc.tbuo.clear();

			convCalc.depth.resize(geo->mesh->vertices.size());
			convCalc.pressure.resize(geo->mesh->vertices.size());
			convCalc.height.resize(geo->mesh->vertices.size());
			convCalc.vtxs.resize(geo->mesh->vertices.size());
			for(int i=0; i<geo->mesh->vertices.size(); ++i){
				convCalc.vtxs[i] = convCalc.Awg * geo->mesh->vertices[i];	//	�����W�n�ł̒��_
				convCalc.height[i] = water->LerpHeight(convCalc.vtxs[i].x, convCalc.vtxs[i].y);
				convCalc.depth[i] = convCalc.height[i] - convCalc.vtxs[i].z;
				//	FRM���璸�_�ł̈��͕␳�����߂�D
#ifdef USE_FRM
				if (convCalc.depth[i] > 0 && geo->frm && bUseFrm){
					Vec3f prs, fri;
					convCalc.pressure[i] = geo->frm->vtxHsrcMap[i]->GetPressure();
				}else{
					convCalc.pressure[i] = 0;
				}
#else
				convCalc.pressure[i] = 0;
#endif
			}
#ifdef USE_FRM
			if (bUseFrm) convCalc.frm = geo->frm;
			else convCalc.frm = NULL;
#endif
			for(ic = geo->conveces.begin(); ic != geo->conveces.end(); ic++){
				poly = DCAST(CDPolyhedron, *ic);
				if(!poly)continue;
				//�����œʑ��ʑ̃��x���ł�BBox���肵�����������Ȃ�C������
				//...
				//�S�ʂɂ��ĥ��
				convCalc.Calc(poly);
			}
			//	�����獄�̃t���[���֕ϊ�����AddForce
			convCalc.solid->solid->AddForce((convCalc.Aw.Rot() * convCalc.buo), convCalc.Aw.Pos());
			convCalc.solid->solid->AddTorque((convCalc.Aw.Rot() * convCalc.tbuo));
		}
	}
	//	paddleForce�̍��W�ϊ�(��Όn��)
	paddleTorqueK = convCalc.Aw.Rot() * paddleTorqueK;
	paddleTorqueB = convCalc.Aw.Rot() * paddleTorqueB;

	Vec3f f = convCalc.Aw.Rot() * paddleForceK;
	paddleTorqueK += convCalc.Aw.Pos() ^ f;
	paddleForceK = f;
	
	f = convCalc.Aw.Rot() * paddleForceB;
	paddleTorqueB += convCalc.Aw.Pos() ^ f;
	paddleForceB = f;
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


