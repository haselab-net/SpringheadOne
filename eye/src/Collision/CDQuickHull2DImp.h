#ifndef CDQUICKHULL2DIMP_H
#define CDQUICKHULL2DIMP_H

#include "CDQuickHull2D.h"
#include <Base/Affine.h>
#include <Base/BaseDebug.h>
#include <vector>
#include <float.h>

namespace Spr{;

template <class TVtx>
void CDQHLine<TVtx>::Clear(){			///<	�������N���A�D�g���O�ɌĂԁD
	deleted = false;
}
template <class TVtx>
bool CDQHLine<TVtx>::Visible(TVtx* p){
	Vec2d pos = p->GetPos();
	return pos*normal > dist;
}
template <class TVtx>
int CDQHLine<TVtx>::GetVtxID(TVtx* v){
	int i;
	for(i=0; i<2; ++i) if(v == vtx[i]) break;
	return i;
}
template <class TVtx>
void CDQHLine<TVtx>::CalcNormal(){
	Vec2d pos0 = vtx[0]->GetPos();
	Vec2d pos1 = vtx[1]->GetPos();
	Vec2d a = pos1 - pos0;
	normal = Vec2d(-a.y, a.x);
	assert(normal.norm());
	normal.unitize();
	dist = pos0 * normal;
}
template <class TVtx>
double CDQHLine<TVtx>::CalcDist(TVtx* v){
	Vec2d vPos = v->GetPos();
	Vec2d diffMin = vPos - Vec2d(vtx[0]->GetPos());
	return diffMin * normal;
}
template <class TVtx>
void CDQHLine<TVtx>::Print(std::ostream& os) const {
	os << "Plane:";
	for(int i=0; i<2; ++i){
		os << " " << *vtx[i];
	}
	os << " d:" << dist << " n:" << normal;
	if (deleted) os << " del";
	os << std::endl;
}

template <class TVtx>
std::ostream& operator << (std::ostream& os, const CDQHLine<TVtx>& pl){
	pl.Print(os);
	return os;
}

template <class TVtx>
unsigned CDQHLines<TVtx>::size(){
	return end - begin; 
}
template <class TVtx>
CDQHLines<TVtx>::CDQHLines(int l):len(l), epsilon(1e-6), infinite(1e8){
	buffer = new CDQHLine[len];
	Clear();
}
template <class TVtx>
void CDQHLines<TVtx>::Clear(){
	begin = buffer;
	end = begin;
	nLines = 0;
}
template <class TVtx>
void CDQHLine<TVtx>::Reverse(){	
	normal = -normal;
	dist = -dist;
	std::swap(vtx[0], vtx[1]);
}
template <class TVtx>
CDQHLines<TVtx>::~CDQHLines(){
	delete [] buffer;
}
/**	b����e�܂ł̒��_����ʕ�����D�g�p�������_��b����vtxBegin�C
�g�p���Ȃ��������_�́CvtxBegin����e�Ɉړ�����D	
begin����end�͒��_��3�܂ޖʂɂȂ�D�����̖ʂ����ʕ�Ɏg��ꂽ��
�� CDQHLine::deleted �� false �ɂȂ��Ă���D	*/
template <class TVtx>
void CDQHLines<TVtx>::CreateConvexHull(TVtx** b, TVtx** e){
	if (e-b < 2){
		if (e-b == 1){
			end->Clear();
			end->vtx[0] = b[0];
			end->vtx[1] = b[0];
			end->neighbor[0] = end->neighbor[1] = end;
			end++;
		}
		return;
	}
	vtxBeginInput = b;
	vtxEndInput = e;
	vtxBegin = b;
	vtxEnd = e;
	//	�ŏ��̖ʂ����
	CreateFirstConvex();
	HULL_DEBUG_EVAL(
		DSTR << "First:" << begin->vtx[0]->GetPos() << begin->vtx[1]->GetPos() << std::endl;
		if ((begin->vtx[1]->GetPos() - begin->vtx[0]->GetPos()).norm() < 0.001) {
			DSTR << "same error" << std::endl;
		}
		DSTR << begin->dist << begin->normal << std::endl;
	)
	for(CDQHLine* cur = begin; cur != end; ++cur){
		if (cur->deleted) continue;
		TreatPlane(cur);
		assert(end < buffer+len);
	}
}
	
template <class TVtx>
void CDQHLines<TVtx>::Print(std::ostream& os) const {
	int num=0;
	for(const CDQHLine* it = begin; it != end; ++it){
		if (!it->deleted) num ++;
	}
	os << num << " lines." << std::endl;
	for(const CDQHLine* it = begin; it != end; ++it){
		it->Print(os);
	}
}

template <class TVtx>
void CDQHLines<TVtx>::CreateFirstConvex(){
	CDQHLines& lines = *this;
	double xMin, xMax;
	TVtxs::iterator it, xMinVtx, xMaxVtx;
	xMin = xMax = (*vtxBegin)->GetPos().X();
	xMinVtx = xMaxVtx = vtxBegin;
	//	�ő�ƍŏ���������
	for(it = vtxBegin+1; it != vtxEnd; ++it){
		double x = (*it)->GetPos().X();
		if (x < xMin){
			xMin = x;
			xMinVtx = it;
		}
		if (x > xMax){
			xMax = x;
			xMaxVtx = it;
		}
	}
	//	�ő���ŏ��C�ŏ����ŏ�����2�Ԗڂɒu��
	std::swap(*xMaxVtx, vtxBegin[0]);		//	�擪�ƍő�����ւ�
	if (xMinVtx == vtxBegin){				//	�擪���ŏ���������
		std::swap(*xMaxVtx, vtxBegin[1]);	//	�ő傾�����ꏊ=�擪�������Ă���ꏊ���ŏ�
	}else{
		std::swap(*xMinVtx, vtxBegin[1]);	//	�ŏ���擪����2�ԖڂƓ���ւ�
	}

	//	�ŏ��̕ӂ����
	lines.end->Clear();
	lines.end->vtx[0] = vtxBegin[1];
	lines.end->vtx[1] = vtxBegin[0];
	lines.end->CalcNormal();
	lines.end++;
	
	//	���\������čŏ��̓ʑ��ʑ̂ɂ���D
	*lines.end = *lines.begin;
	lines.end->Reverse();
	lines.begin->neighbor[0] = lines.end;
	lines.begin->neighbor[1] = lines.end;
	lines.end->neighbor[0] = lines.begin;
	lines.end->neighbor[1] = lines.begin;
	lines.end++;
	//	�g�p�������_�𒸓_���X�g����͂����D
	vtxBegin += 2;
	nLines = 2;
}

/**	��cur�ƁC���̖ʂ����ԉ������_ top ���󂯎��C
	cur�Ƃ��̎��͂̕ӂ��폜���C�ʕ��top���܂߂�D
	end[-1], end[-2]���V���ɍ��ꂽ�ӂɂȂ�D	*/
template <class TVtx>
void CDQHLines<TVtx>::CreateCone(CDQHLine* cur, TVtx* top){
	cur->deleted = true;							//	cur�͍폜
	nLines --;
	//	�ׂ̕ӂ����Ă����āC���_����݂���ӂ͍폜����D
	CDQHLine* horizon[2];
	for(int i=0; i<2; ++i){
		horizon[i] = cur->neighbor[i];
		while(horizon[i]->Visible(top) && nLines>1){
			horizon[i]->deleted = true;
			nLines --;
			horizon[i] = horizon[i]->neighbor[i];
		}
	}
	//	�폜����Ȃ������ӂƒ��_�̊Ԃɕӂ����D
	//	horizon[0]���̕�
	end->Clear();
	horizon[0]->neighbor[1] = end;
	end->vtx[0] = horizon[0]->vtx[1];
	end->vtx[1] = top;
	end->neighbor[0] = horizon[0];
	end->neighbor[1] = end+1;
	end->CalcNormal();
	end++;
	nLines ++;
	//	horizon[1]���̕�
	end->Clear();
	horizon[1]->neighbor[0] = end;
	end->vtx[0] = top;
	end->vtx[1] = horizon[1]->vtx[0];
	end->neighbor[0] = end-1;
	end->neighbor[1] = horizon[1];
	end->CalcNormal();
	end++;
	nLines ++;
}	
/**	��ԉ����̒��_��������D�������炻��𒸓_���X�g����͂���	*/
template <class TVtx>
bool CDQHLines<TVtx>::FindFarthest(CDQHLine* line){
	TVtx** maxVtx=NULL;
	double maxDist = epsilon;
	for(TVtx** it=vtxBegin; it!= vtxEnd; ++it){
		double dist = line->CalcDist(*it);
		if (dist > maxDist){
			maxDist = dist; 
			maxVtx = it;
		}
	}
	if (maxVtx){
		std::swap(*vtxBegin, *maxVtx);
		vtxBegin++;
#ifdef _DEBUG
		if (	vtxBegin[-1]->GetPos() == line->vtx[0]->GetPos()
			||	vtxBegin[-1]->GetPos() == line->vtx[1]->GetPos()){
			DSTR << "Error: same position." << std::endl;
			for(int i=0; i<2; ++i){
				DSTR << "V" << i << ": " << line->vtx[i]->GetPos() << std::endl;
			}
			DSTR << "P : " << vtxBegin[-1]->GetPos() << std::endl;
			assert(0);
		}
#endif
		return true;
	}
	return false;
}
/*	�O�� ���� �̏��ɕ��ׂ�D
�O���̏I��聁�����̎n�܂肪 inner	*/
template <class TVtx>
TVtx** CDQHLines<TVtx>::DivideByPlaneR(CDQHLine* plane, TVtx** start, TVtx** end){
	double INNER_DISTANCE = epsilon * plane->dist;
	while(start != end){
		double d = -plane->CalcDist(*start);
		if (d <= INNER_DISTANCE){	//	�����̏ꍇ�͌��Ɉړ�
			-- end;
			std::swap(*end, *start);
		}else{
			++ start;
		}
	}
	return start;
}
template <class TVtx>
TVtx** CDQHLines<TVtx>::DivideByPlane(CDQHLine* plane, TVtx** start, TVtx** end){
	double INNER_DISTANCE = epsilon * plane->dist;
	while(start != end){
		double d = plane->CalcDist(*start);
		if (d <= INNER_DISTANCE){	//	�����̏ꍇ�͌��Ɉړ�
			-- end;
			std::swap(*end, *start);
		}else{
			++ start;
		}
	}
	return start;
}
/**	��̖ʂɑ΂��鏈�����s���D��ԉ����̒��_�������C
�n�����𒲂ׁC�R�[�������C�����̒��_���͂����D*/
template <class TVtx>
void CDQHLines<TVtx>::TreatPlane(CDQHLine* cur){
	//	��ԉ����̒��_�̒T��
	if (!FindFarthest(cur)) return;
	HULL_DEBUG_EVAL(
		DSTR << "Farthest:" << vtxBegin[-1]->GetPos();
		DSTR << " cmp:" << vtxBegin[-1]->GetPos()*cur->normal << " > " << cur->dist << std::endl;
		if (cur->dist < 0){
			DSTR << "faceDist:" << cur->dist << std::endl;
		}
	)
	//	�V�������_�œʕ�����D
	CreateCone(cur, vtxBegin[-1]);
	HULL_DEBUG_EVAL(
		if (!hor){
			DSTR << "No Horizon Error!!" << std::endl;
			assert(hor);
		}
	)
	//	���ڂ�����(cur)�ƐV���ȕ�(end-2,end-1)�ɂ���ĕ����߂��钸�_��vtxEnd�̌��Ɉړ�
	TVtx** inner = DivideByPlaneR(cur, vtxBegin, vtxEnd);
	for(CDQHLine* it=end-2; it!=end; ++it){
		HULL_DEBUG_EVAL(
			std::cout << "Inner:";
			for(TVtx** v = inner; v != vtxEnd; ++v){
				std::cout << **v;
			}
			std::cout << std::endl;
		);
		inner = DivideByPlane(it, inner, vtxEnd);
	}
	HULL_DEBUG_EVAL(
		std::cout << "InnerFinal:";
		for(TVtx** v = inner; v != vtxEnd; ++v){
			std::cout << **v;
		}
		std::cout << std::endl;
	);
	vtxEnd = inner;
}

template <class TVtx>
std::ostream& operator << (std::ostream& os, const CDQHLines<TVtx>& pls){
	pls.Print(os);
	return os;
}
template <class CDQHVtx2DSample>
inline std::ostream& operator << (std::ostream& os, const TYPENAME CDQHLines<CDQHVtx2DSample>::TVtxs& f){
	f.Print(os); return os;
}

inline void CDQHVtx2DSample::Print(std::ostream& os) const {
//		os << Vtx();
		os << id_ << " ";
}
inline std::ostream& operator << (std::ostream& os, const CDQHVtx2DSample& f){
	f.Print(os); return os;
}

}

#endif
