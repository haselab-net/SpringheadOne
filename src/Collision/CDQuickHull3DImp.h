#ifndef CDQUICKHULL3DIMP_H
#define CDQUICKHULL3DIMP_H

#include "CDQuickHull3D.h"
#include <Base/Affine.h>
#include <Base/BaseDebug.h>
#include <vector>
#include <float.h>

namespace Spr{;

template <class TVtx>
void CDQHPlane<TVtx>::Clear(){			///<	�������N���A�D�g���O�ɌĂԁD
	deleted = false;
}
template <class TVtx>
bool CDQHPlane<TVtx>::Visible(TVtx* p){
	//	�ʂ��L���̋����ɂ���ꍇ
	Vec3d GetPos = p->GetPos();
	return GetPos*normal > dist;
}
template <class TVtx>
int CDQHPlane<TVtx>::GetVtxID(TVtx* v){
	int i;
	for(i=0; i<3; ++i) if(v == vtx[i]) break;
	return i;
}
template <class TVtx>
void CDQHPlane<TVtx>::CalcNormal(){
	Vec3d pos0 = vtx[0]->GetPos();
	Vec3d pos1 = vtx[1]->GetPos();
	Vec3d pos2 = vtx[2]->GetPos();
	Vec3d a = pos1 - pos0;
	Vec3d b = pos2 - pos0;
	normal = a ^ b;
	assert(normal.norm());
	normal.unitize();
	dist = pos0 * normal;
}
template <class TVtx>
void CDQHPlane<TVtx>::Reverse(){	
	normal = -normal;
	dist = -dist;
	std::swap(vtx[0], vtx[2]);
}
template <class TVtx>
double CDQHPlane<TVtx>::CalcDist(TVtx* v){
	//	���x���l�����ċ������v�Z
	Vec3d vPos = v->GetPos();
	Vec3d diffMin = vPos - Vec3d(vtx[0]->GetPos());
/*
	Vec3d diff = vPos - Vec3d(vtx[1]->GetPos());
	if (diff.square() < diffMin.square()) diffMin = diff;
	diff = vPos - Vec3d(vtx[2]->GetPos());
	if (diff.square() < diffMin.square()) diffMin = diff;
*/
	return diffMin * normal;
}
template <class TVtx>
void CDQHPlane<TVtx>::Print(std::ostream& os) const {
	os << "Plane:";
	for(int i=0; i<3; ++i){
		os << " " << *vtx[i];
	}
	os << " d:" << dist << " n:" << normal;
	if (deleted) os << " del";
	os << std::endl;
}

template <class TVtx>
std::ostream& operator << (std::ostream& os, const CDQHPlane<TVtx>& pl){
	pl.Print(os);
	return os;
}

template <class TVtx>
unsigned CDQHPlanes<TVtx>::size(){
	return end - begin; 
}
template <class TVtx>
CDQHPlanes<TVtx>::CDQHPlanes(int l):len(l){
	buffer = new CDQHPlane[len];
	Clear();
}
template <class TVtx>
void CDQHPlanes<TVtx>::Clear(){
	begin = buffer;
	end = begin;
	nPlanes = 0;
}
template <class TVtx>
CDQHPlanes<TVtx>::~CDQHPlanes(){
	delete [] buffer;
}
/**	b����e�܂ł̒��_����ʕ�����D�g�p�������_��b����vtxBegin�C
�g�p���Ȃ��������_�́CvtxBegin����e�Ɉړ�����D	
begin����end�͒��_��3�܂ޖʂɂȂ�D�����̖ʂ����ʕ�Ɏg��ꂽ��
�� CDQHPlane::deleted �� false �ɂȂ��Ă���D	*/
template <class TVtx>
void CDQHPlanes<TVtx>::CreateConvexHull(TVtx** b, TVtx** e){
	vtxBeginInput = b;
	vtxEndInput = e;
	vtxBegin = b;
	vtxEnd = e;
	//	�ŏ��̖ʂ����
	CreateFirstConvex();
	HULL_DEBUG_EVAL(
		DSTR << "First:" << begin->vtx[0]->GetPos() << begin->vtx[1]->GetPos() << begin->vtx[2]->GetPos() << std::endl;
		if ((begin->vtx[1]->GetPos() - begin->vtx[0]->GetPos()).norm() < 0.001) {
			DSTR << "same error" << std::endl;
		}
		DSTR << begin->dist << begin->normal << std::endl;
	)
	for(CDQHPlane* cur = begin; cur != end; ++cur){
		if (cur->deleted) continue;
		TreatPlane(cur);
		assert(end < buffer+len);
	}
}
	
template <class TVtx>
void CDQHPlanes<TVtx>::Print(std::ostream& os) const {
	int num=0;
	for(const CDQHPlane* it = begin; it != end; ++it){
		if (!it->deleted) num ++;
	}
	os << num << " planes." << std::endl;
	for(const CDQHPlane* it = begin; it != end; ++it){
		it->Print(os);
	}
}

template <class TVtx>
void CDQHPlanes<TVtx>::CreateFirstConvex(){
	CDQHPlanes& planes = *this;
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
	
	//	2�̒��_�����ӂ����ԉ����_��������
	Vec3d dir = vtxBegin[1]->GetPos() - vtxBegin[0]->GetPos();
	dir.unitize();
	double maxDist = -1;
	TVtxs::iterator third;
	for(it = vtxBegin+2; it != vtxEnd; ++it){
		Vec3d v0 = Vec3d((*it)->GetPos()) - Vec3d(vtxBegin[0]->GetPos());
		Vec3d v1 = Vec3d((*it)->GetPos()) - Vec3d(vtxBegin[1]->GetPos());
		Vec3d v = v0.square() < v1.square() ? v0 : v1;
		double dist2 = v.square() - Square(v*dir);
		if (dist2 > maxDist){
			maxDist = dist2;
			third = it;
		}
	}
	std::swap(*third, vtxBegin[2]);
	planes.end->Clear();
	planes.end->vtx[0] = vtxBegin[2];
	planes.end->vtx[1] = vtxBegin[1];
	planes.end->vtx[2] = vtxBegin[0];
	planes.end->CalcNormal();
	planes.end++;
	
	//	���\������čŏ��̓ʑ��ʑ̂ɂ���D
	*planes.end = *planes.begin;
	planes.end->Reverse();
	planes.begin->neighbor[0] = planes.end;
	planes.begin->neighbor[1] = planes.end;
	planes.begin->neighbor[2] = planes.end;
	planes.end->neighbor[0] = planes.begin;
	planes.end->neighbor[1] = planes.begin;
	planes.end->neighbor[2] = planes.begin;
	planes.end++;
	//	�g�p�������_�𒸓_���X�g����͂����D
	vtxBegin += 3;
	nPlanes = 2;
}

/**	horizon �����D cur ������������ʁCvtx ���V�������_�D
rv ��horizon��ӂɎ���3�p�`��1�Ԃ��D*/
template <class TVtx>
void CDQHPlanes<TVtx>::FindHorizon(TVtx*& rv, CDQHPlane* cur, TVtx* vtx){
	//	cur�̍폜�D�ׂ̖ʂ���̎Q�Ƃ������D
	for(int i=0; i<3; ++i){
		CDQHPlane* next = cur->neighbor[i];
		if (!next) continue;
		for(int j=0; j<3; ++j){
			if (next->neighbor[j] == cur){
				next->neighbor[j] = NULL;
				break;
			}	
		}
	}
	cur->deleted = true;
	nPlanes --;
	//	�ׂ̖ʂɂ��āC���\�𔻒肵�ď���
	bool bRecurse = false;
	TVtx* rvc;
	for(int i=0; i<3; ++i){
		CDQHPlane* next = cur->neighbor[i];
		if (!next) continue;
		if (next->Visible(vtx) && nPlanes>1){	//	������ʂɂ͌���������D
			FindHorizon(rv, next, vtx);
			bRecurse = true;
		}else{
			//	�n��������钸�_�ɂ��̖ʂ�o�^����D
			cur->vtx[i]->horizon = next;
			rvc = cur->vtx[i];
		}
	}
	if (!bRecurse){
		rv = rvc;
	}
}
/**	���_��horizon�̊ԂɃR�[�������D*/
template <class TVtx>
void CDQHPlanes<TVtx>::CreateCone(TVtx* firstVtx, TVtx* top){
	CDQHPlanes& planes = *this; 
	TVtx* curVtx = firstVtx;
	CDQHPlane* curHorizon = firstVtx->horizon;
	HULL_DEBUG_EVAL( std::cout << "Horizon:" << *curVtx; )
	//	�ŏ��̖ʂ𒣂�
	int curVtxID = curHorizon->GetVtxID(curVtx);
	int prevVtxID = (curVtxID+2)%3;
	//	�ʂ̍쐬
	planes.end->Clear();
	planes.end->vtx[0] = curHorizon->vtx[curVtxID];
	planes.end->vtx[1] = curHorizon->vtx[prevVtxID];
	planes.end->vtx[2] = top;
	planes.end->CalcNormal();
	//	curHorizon�Ɛڑ�
	planes.end->neighbor[0] = curHorizon;
	curHorizon->neighbor[prevVtxID] = planes.end;
	CDQHPlane* firstPlane = planes.end;
	//	�ʂ̍쐬����
	planes.end ++;
	nPlanes ++;
	
	//	curHorizon �ׂ̗��3�p�`�������āCcurHorizon���X�V
	curVtx = curHorizon->vtx[prevVtxID];
	curHorizon = curVtx->horizon;

	//	2�Ԗڈȍ~�𒣂�
	while(1){
		HULL_DEBUG_EVAL(std::cout << *curVtx;);
		int curVtxID = curHorizon->GetVtxID(curVtx);
		int prevVtxID = (curVtxID+2)%3;
		//	�ʂ̍쐬
		planes.end->Clear();
		planes.end->vtx[0] = curHorizon->vtx[curVtxID];
		planes.end->vtx[1] = curHorizon->vtx[prevVtxID];
		planes.end->vtx[2] = top;
		planes.end->CalcNormal();
		//	curHorizon�Ɛڑ�
		planes.end->neighbor[0] = curHorizon;
		curHorizon->neighbor[prevVtxID] = planes.end;
		//	1�O�ɍ�����ʂƍ�������ʂ�ڑ�
		planes.end[-1].neighbor[1] = planes.end;
		planes.end->neighbor[2] = planes.end-1;
		//	�ʂ̍쐬����
		planes.end ++;
		if (planes.end - planes.begin > len-1){
			DSTR << "too many planes:" << std::endl;
			for(TVtx** p = vtxBeginInput; p != vtxEndInput; ++p){
				DSTR << (*p)->GetPos() << std::endl;
			}
			DSTR << vtxEndInput - vtxBeginInput << " vertices." << std::endl;

			curHorizon = firstVtx->horizon;
			for(TVtx** p = vtxBeginInput; p != vtxEndInput; ++p){
				if (firstVtx == *p){
					DSTR << p - vtxBeginInput << " ";
				}
			}
			for(int i=0; i<len; ++i){
				curVtxID = curHorizon->GetVtxID(curVtx);
				prevVtxID = (curVtxID+2)%3;
				curVtx = curHorizon->vtx[prevVtxID];
				curHorizon = curVtx->horizon;
				for(TVtx** p = vtxBeginInput; p != vtxEndInput; ++p){
					if (curVtx == *p){
						DSTR << p - vtxBeginInput << " ";
					}
				}
			}
			DSTR << std::endl;
			exit(0);
		}
		nPlanes ++;
		
		//	curHorizon �ׂ̗��3�p�`�������āCcurHorizon���X�V
		curVtx = curHorizon->vtx[prevVtxID];
		if (curVtx == firstVtx) break;
		curHorizon = curVtx->horizon;
	}
	HULL_DEBUG_EVAL( std::cout << std::endl;);
	//	�Ō�̖ʂ�firstPlane��ڑ�
	firstPlane->neighbor[2] = planes.end-1;
	planes.end[-1].neighbor[1] = firstPlane;
}	
/**	��ԉ����̒��_��������D�������炻��𒸓_���X�g����͂���	*/
template <class TVtx>
bool CDQHPlanes<TVtx>::FindFarthest(CDQHPlane* plane){
	TVtx** maxVtx=NULL;
	double maxDist = HULL_EPSILON;
	for(TVtx** it=vtxBegin; it!= vtxEnd; ++it){
		double dist = plane->CalcDist(*it);
		if (dist > maxDist){
			maxDist = dist; 
			maxVtx = it;
		}
	}
	if (maxVtx){
		std::swap(*vtxBegin, *maxVtx);
		vtxBegin++;
#ifdef _DEBUG
		if (	vtxBegin[-1]->GetPos() == plane->vtx[0]->GetPos()
			||	vtxBegin[-1]->GetPos() == plane->vtx[1]->GetPos()
			||	vtxBegin[-1]->GetPos() == plane->vtx[2]->GetPos()){
			DSTR << "Error: same position." << std::endl;
			for(int i=0; i<3; ++i){
				DSTR << "V" << i << ": " << plane->vtx[i]->GetPos() << std::endl;
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
TVtx** CDQHPlanes<TVtx>::DivideByPlaneR(CDQHPlane* plane, TVtx** start, TVtx** end){
	double INNER_DISTANCE = HULL_EPSILON * plane->dist;
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
TVtx** CDQHPlanes<TVtx>::DivideByPlane(CDQHPlane* plane, TVtx** start, TVtx** end){
	double INNER_DISTANCE = HULL_EPSILON * plane->dist;
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
void CDQHPlanes<TVtx>::TreatPlane(CDQHPlane* cur){
	if (!FindFarthest(cur)) return;
	HULL_DEBUG_EVAL(
		DSTR << "Farthest:" << vtxBegin[-1]->GetPos();
		DSTR << " cmp:" << vtxBegin[-1]->GetPos()*cur->normal << " > " << cur->dist << std::endl;
		if (cur->dist < 0){
			DSTR << "faceDist:" << cur->dist << std::endl;
		}
	)
	//	�n�����̒���
	TVtx* hor=NULL;
	FindHorizon(hor, cur, vtxBegin[-1]);
	HULL_DEBUG_EVAL(
		if (!hor){
			DSTR << "No Horizon Error!!" << std::endl;
			assert(hor);
		}
	)
	//	�R�[���̍쐬
	CDQHPlane* coneBegin = end;
	CreateCone(hor, vtxBegin[-1]);
	CDQHPlane* coneEnd = end;
	//	�R�[���ɕ����߂��钸�_��vtxEnd�̌��Ɉړ�
	TVtx** inner = DivideByPlaneR(cur, vtxBegin, vtxEnd);
	for(CDQHPlane* it=coneBegin; it!=coneEnd; ++it){
		if (it->deleted) continue;
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
std::ostream& operator << (std::ostream& os, const CDQHPlanes<TVtx>& pls){
	pls.Print(os);
	return os;
}
template <class TVtx>
inline std::ostream& operator << (std::ostream& os, const TYPENAME CDQHPlanes<TVtx>::TVtxs& f){
	f.Print(os); return os;
}

inline Vec3f TVtx::GetPos() const {
	return dir * dist;
}
inline void TVtx::SetPos(Vec3f p){
	float d = p.norm();
	dir = p / d;
	dist = d;
}
inline void TVtx::Print(std::ostream& os) const {
//		os << Vtx();
		os << id_ << " ";
}
inline std::ostream& operator << (std::ostream& os, const TVtx& f){
	f.Print(os); return os;
}

}

#endif
