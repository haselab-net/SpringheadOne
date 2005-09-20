#ifndef HULL_H
#define HULL_H

#include <Base/Affine.h>
#include <vector>
#include <float.h>

#define HULL_EPSILON 1e-6

//#define HULL_DEBUG		//	�f�o�b�O�o��
#ifdef HULL_DEBUG
 #define HULL_DEBUG_EVAL(x) x
#else
 #define HULL_DEBUG_EVAL(x)
#endif
namespace LVRM{;


///	������
template <class TVtx>
class TPlane{
public:
	Vec3d normal;			///<	�ʂ̖@��
	float dist;				///<	�ʂ̌��_����̋���
	
	TVtx* vtx[3];			///<	�ʂ��\�����钸�_
	TPlane* neighbor[3];	///<	�ׂ̖� vtx[0]-vtx[1] �ׂ̗� neighbor[0]
	bool deleted;			///<	�폜���ꂽ�ʂ�true
	void Clear(){			///<	�������N���A�D�g���O�ɌĂԁD
		deleted = false;
	}
	///	���_ v ����\���������邩�ǂ���
	bool Visible(const Vec3d& v){
		return normal * (v - vtx[0]->Vtx()) > 0;
	}
	///	v�̒��_�ԍ���Ԃ�(0..2��Ԃ�)�D������Ȃ����3��Ԃ��D
	int GetVtxID(TVtx* v){
		int i;
		for(i=0; i<3; ++i) if(v == vtx[i]) break;
		return i;
	}
	///	�@���x�N�g���Ƌ������v�Z����D
	void CalcNormal(){
		normal = (vtx[2]->Vtx() - vtx[0]->Vtx()) ^ (vtx[1]->Vtx() - vtx[0]->Vtx());
		normal.unitize();
		dist = normal * vtx[0]->Vtx();
	}
	///	�\�����Ђ�����Ԃ��D
	void Reverse(){	
		normal = -normal;
		dist = -dist;
		std::swap(vtx[0], vtx[2]);
	}
	///	�f�o�b�O�p�\��
	void Print(std::ostream& os) const {
		os << "Plane:";
		for(int i=0; i<3; ++i){
			os << " " << *vtx[i];
		}
		os << " n:" << normal;
		if (deleted) os << "del";
		os << std::endl;
	}
};
template <class TVtx>
std::ostream& operator << (std::ostream& os, const TPlane<TVtx>& pl){
	pl.Print(os);
	return os;
}

///	�ʂ̃o�b�t�@
template <class TVtx>
class TPlanes{
public:
	///	���_��Vector
	class TVtxs: public std::vector<TVtx*>{
	public:
		void Print(std::ostream& os) const {
			for(TVtxs::const_iterator it = begin(); it != end(); ++it){
				(*it)->Print(os);
			}
		}
	};

	typedef TPlane<TVtx> TPlane;

	TPlane* buffer;		///<	�o�b�t�@�ւ̃|�C���^ new ����D
	int len;			///<	�o�b�t�@�̒���
	TPlane* begin;		///<	�ŏ��̖�
	TPlane* end;		///<	�Ō�̖ʂ̎�
	TVtx** vtxBegin;	///<	�c���Ă��钸�_�̐擪
	TVtx** vtxEnd;		///<	�c���Ă��钸�_�̍Ō�̎�
	TPlanes(int l):len(l){
		buffer = new TPlane[len];
		begin = buffer;
		end = begin;
	}
	~TPlanes(){ delete buffer; }
	void CreateConvexHull(TVtx** b, TVtx** e){
		vtxBegin = b;
		vtxEnd = e;
		//	�ŏ��̖ʂ����
		CreateFirstConvex();
		HULL_DEBUG_EVAL(std::cout << "First:" << e[-3]->id_ << " " << e[-2]->id_ << " " << e[-1]->id_ << std::endl;)
		for(TPlane* cur = begin; cur != end; ++cur){
			if (cur->deleted) continue;
			TreatPlane(cur);
			assert(end < buffer+len);
		}
	}
	
	void Print(std::ostream& os) const {
		int num=0;
		for(const TPlane* it = begin; it != end; ++it){
			if (!it->deleted) num ++;
		}
		os << num << " planes." << std::endl;
		for(const TPlane* it = begin; it != end; ++it){
			it->Print(os);
		}
	}

private:
	void CreateFirstConvex(){
		TPlanes& planes = *this;
		float xMin, xMax;
		TVtxs::iterator it, xMinVtx, xMaxVtx;
		xMin = xMax = (*vtxBegin)->Vtx().X();
		xMinVtx = xMaxVtx = vtxBegin;
		//	�ő�ƍŏ���������
		for(it = vtxBegin+1; it != vtxEnd; ++it){
			float x = (*it)->Vtx().X();
			if (x < xMin){
				xMin = x;
				xMinVtx = it;
			}
			if (x > xMax){
				xMax = x;
				xMaxVtx = it;
			}
		}
		//	�ő���Ō�C�ŏ����Ōォ��2�Ԗڂɒu��
		std::swap(*xMaxVtx, vtxEnd[-1]);		//	�Ō�ƍő�����ւ�
		if (xMinVtx == vtxEnd-1){				//	�Ōオ�ŏ���������
			std::swap(*xMaxVtx, vtxEnd[-2]);	//	�ő傾�����ꏊ=�Ōオ�����Ă���ꏊ���ŏ�
		}else{
			std::swap(*xMinVtx, vtxEnd[-2]);	//	�ŏ����Ōォ��2�ԖڂƓ���ւ�
		}
		
		//	2�̒��_�����ӂ����ԉ����_��������
		Vec3d dir = vtxEnd[-2]->Vtx() - vtxEnd[-1]->Vtx();
		dir.unitize();
		Vec3d pos = vtxEnd[-1]->Vtx();
		float maxDist = -1;
		TVtxs::iterator third;
		for(it = vtxBegin; it != vtxEnd-2; ++it){
			Vec3d v = (*it)->Vtx() - pos;
			float dist2 = v.square() - Square(v*dir);
			assert(dist2>=0);
			if (dist2 > maxDist){
				maxDist = dist2;
				third = it;
			}
		}
		std::swap(*third, vtxEnd[-3]);
		planes.end->Clear();
		planes.end->vtx[0] = vtxEnd[-3];
		planes.end->vtx[1] = vtxEnd[-2];
		planes.end->vtx[2] = vtxEnd[-1];
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
		vtxEnd -= 3;
	}

	/**	horizon �����D cur ������������ʁCvtx ���V�������_�D
	rv ��horizon��ӂɎ���3�p�`��1�Ԃ��D*/
	void FindHorizon(TVtx*& rv, TPlane* cur, TVtx* vtx){
		//	cur�̍폜�D�ׂ̖ʂ���̎Q�Ƃ������D
		for(int i=0; i<3; ++i){
			TPlane* next = cur->neighbor[i];
			if (!next) continue;
			for(int j=0; j<3; ++j){
				if (next->neighbor[j] == cur){
					next->neighbor[j] = NULL;
				break;
				}
			}
		}
		cur->deleted = true;
		//	�ׂ̖ʂɂ��āC���\�𔻒肵�ď���
		for(int i=0; i<3; ++i){
			TPlane* next = cur->neighbor[i];
			if (!next) continue;
			if (next->Visible(vtx->Vtx())){	//	������ʂɂ͌���������D
				FindHorizon(rv, next, vtx);
			}else{
				//	�n��������钸�_�ɂ��̖ʂ�o�^����D
				cur->vtx[i]->horizon = next;
				rv = cur->vtx[i];
			}
		}
	}
	/**	���_��horizon�̊ԂɃR�[�������D*/
	void CreateCone(TVtx* firstVtx, TVtx* top){
		TPlanes& planes = *this; 
		TVtx* curVtx = firstVtx;
		TPlane* curHorizon = firstVtx->horizon;
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
		TPlane* firstPlane = planes.end;
		//	�ʂ̍쐬����
		planes.end ++;
	
		//	curHorizon �ׂ̗��3�p�`�������āCcurHorizon���X�V
		curVtx = curHorizon->vtx[prevVtxID];
		curHorizon = curVtx->horizon;
	 	
		//	2�Ԗڈȍ~�𒣂�
		while(1){
			HULL_DEBUG_EVAL(std::cout << *curVtx;)
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
	
			//	curHorizon �ׂ̗��3�p�`�������āCcurHorizon���X�V
			curVtx = curHorizon->vtx[prevVtxID];
			if (curVtx == firstVtx) break;
			curHorizon = curVtx->horizon;
		}
		HULL_DEBUG_EVAL( std::cout << std::endl;)
		//	�Ō�̖ʂ�firstPlane��ڑ�
		firstPlane->neighbor[2] = planes.end-1;
		planes.end[-1].neighbor[1] = firstPlane;
	}	
	/**	��ԉ����̒��_��������D�������炻��𒸓_���X�g����͂���	*/
	bool FindFarthest(const Vec3d& normal, float maxDist){
		TVtx** maxVtx=NULL;
		for(TVtx** it=vtxBegin; it!= vtxEnd; ++it){
			float dist = (*it)->Vtx() * normal;
			if (dist > maxDist + HULL_EPSILON){
				maxDist = dist; 
				maxVtx = it;
			}
		}
		if (maxVtx){
			std::swap(*vtxBegin, *maxVtx);
			vtxBegin++;
			return true;
		}
		return false;
	}
	/*	�O�� ���� �̏��ɕ��ׂ�D
		�O���̏I��聁�����̎n�܂肪 inner	*/
	TVtx** DivideByPlane(const Vec3d& normal, float dist, TVtx** start, TVtx** end){
		while(start != end){
			float d = (*start)->Vtx() * normal;
			if (d < dist){	//	�����̏ꍇ�͌��Ɉړ�
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
	void TreatPlane(TPlane* cur){
		if (!FindFarthest(cur->normal, cur->dist)) return;
		HULL_DEBUG_EVAL(std::cout << "Farthest:" << *vtxBegin[-1] << std::endl;)
		//	�n�����̒���
		TVtx* hor=NULL;
		FindHorizon(hor, cur, vtxBegin[-1]);
		//	�R�[���̍쐬
		TPlane* coneBegin = end;
		CreateCone(hor, vtxBegin[-1]);
		TPlane* coneEnd = end;
		//	�R�[���ɕ����߂��钸�_��vtxEnd�̌��Ɉړ�
		TVtx** inner = DivideByPlane(-cur->normal, -cur->dist+HULL_EPSILON, vtxBegin, vtxEnd);
		for(TPlane* it=coneBegin; it!=coneEnd; ++it){
			if (it->deleted) continue;
			HULL_DEBUG_EVAL(
				std::cout << "Inner:";
				for(TVtx** v = inner; v != vtxEnd; ++v){
					std::cout << **v;
				}
				std::cout << std::endl;
			);
			inner = DivideByPlane(it->normal, it->dist+HULL_EPSILON, inner, vtxEnd);
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
};
template <class TVtx>
std::ostream& operator << (std::ostream& os, const TPlanes<TVtx>& pls){
	pls.Print(os);
	return os;
}
template <class TVtx>
inline std::ostream& operator << (std::ostream& os, const TPlanes<TVtx>::TVtxs& f){
	f.Print(os); return os;
}


///	���_�N���X�̗�
class TVtx{
public:
	//	QuickHull���g�p���郁���o�D�K�{�D
	///  ���_�̍��W
	const Vec3d Vtx() const { return vtx_; }

	/**	QuickHull�A���S���Y���p���[�N�G���A�D
		��ԉ������_���猩����ʂ��폜�������Ǝc�����`��̃G�b�W����
		������ڑ����Ă������߂̃|�C���^�D
		���_���ʂ̐ڑ��D	�ʁ����_�͒��_�̕��я����番����D	*/
	TPlane<TVtx>* horizon;

	//	���̑��̃����o
	Vec3d vtx_;	//	���_�̃f�[�^
	int id_;

	///	�f�o�b�O�p�\��
	template <class T>
	void Print(T& os) const {
//		os << Vtx();
		os << id_ << " ";
	}
};
inline std::ostream& operator << (std::ostream& os, const TVtx& f){
	f.Print(os); return os;
}

}

#endif
