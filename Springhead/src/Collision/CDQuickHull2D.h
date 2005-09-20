#ifndef CDQUICKHULL2D_H
#define CDQUICKHULL2D_H

#include <Base/Affine.h>
#include <Base/TQuaternion.h>
#include <vector>

//#define HULL_DEBUG		//	�f�o�b�O�o��
#ifdef HULL_DEBUG
 #define HULL_DEBUG_EVAL(x) x
#else
 #define HULL_DEBUG_EVAL(x)
#endif

namespace Spr{;

///	QuickHull�ō�����
template <class TVtx>
class CDQHLine{
public:
	Vec2d normal;			///<	�ʂ̖@��
	double dist;			///<	�ʂ̌��_����̋���
	
	TVtx* vtx[2];			///<	�ʂ��\�����钸�_
	CDQHLine* neighbor[2];	///<	�ׂ̖� vtx[0] �ׂ̗� neighbor[0]
	bool deleted;			///<	�폜���ꂽ�ʂ�true
	void Clear();			///<	�������N���A�D�g���O�ɌĂԁD
	void Reverse();			///<	�ӂ̗��\���Ђ�����Ԃ��D
	bool Visible(TVtx* p);	///<	���_ v ����\���������邩�ǂ���
	///	v�̒��_�ԍ���Ԃ�(0..1��Ԃ�)�D������Ȃ����3��Ԃ��D
	int GetVtxID(TVtx* v);
	void CalcNormal();		///<	�@���x�N�g���Ƌ������v�Z����D
	///	�_�Ƃ̋������v�Z����D���x���l�����Ĉ�ԋ߂��_�Ōv�Z����D
	double CalcDist(TVtx* v);
	///	�f�o�b�O�p�\��
	void Print(std::ostream& os) const;
};

///	�ʂ̃o�b�t�@
template <class TVtx>
class CDQHLines{
public:
	double epsilon;
	double infinite;

	TVtx** vtxBeginInput;	///<	�c���Ă��钸�_�̐擪
	TVtx** vtxEndInput;		///<	�c���Ă��钸�_�̍Ō�̎�
	///	���_��Vector
	class TVtxs: public std::vector<TVtx*>{
	public:
		void Print(std::ostream& os) const{
			for(TVtxs::const_iterator it = begin(); it != end(); ++it){
				(*it)->Print(os);
			}
		}
 	};
	typedef CDQHLine<TVtx> CDQHLine;

	CDQHLine* buffer;	///<	�o�b�t�@�ւ̃|�C���^ new ����D
	int len;			///<	�o�b�t�@�̒���
	CDQHLine* begin;	///<	�ŏ��̕�
	CDQHLine* end;		///<	�Ō�̕ӂ̎�
	TVtx** vtxBegin;	///<	�c���Ă��钸�_�̐擪
	TVtx** vtxEnd;		///<	�c���Ă��钸�_�̍Ō�̎�
	int nLines;			///<	�ӂ̐�
	unsigned size();	///<	�g�p�ς݃o�b�t�@�̃T�C�Y
	CDQHLines(int l);
	void Clear();
	~CDQHLines();
	/**	b����e�܂ł̒��_����ʕ�����D�g�p�������_��b����vtxBegin�C
		�g�p���Ȃ��������_�́CvtxBegin����e�Ɉړ�����D	
		begin����end�͒��_��3�܂ޖʂɂȂ�D�����̖ʂ����ʕ�Ɏg��ꂽ��
		�� CDQHLine::deleted �� false �ɂȂ��Ă���D	*/
	void CreateConvexHull(TVtx** b, TVtx** e);
	void Print(std::ostream& os) const;

private:
	/**	�ŏ��̓ʑ��ʑ́�2�{�̕�(�\��)�����D
		�ł��邾���傫���ӂ����ƌ������ǂ��D	*/
	void CreateFirstConvex();
	/**	��cur�ƁC���̖ʂ����ԉ������_ top ���󂯎��C
		cur�Ƃ��̎��͂̕ӂ��폜���C�ʕ��top���܂߂�D
		end[-1], end[-2]���V���ɍ��ꂽ�ӂɂȂ�D	*/
	void CreateCone(CDQHLine* cur, TVtx* top);
	/**	��ԉ����̒��_��������D�������炻��𒸓_���X�g����͂���	*/
	bool FindFarthest(CDQHLine* plane);
	/*	�O�� ���� �̏��ɕ��ׂ�D
		�O���̏I��聁�����̎n�܂肪 inner	*/
	TVtx** DivideByPlaneR(CDQHLine* plane, TVtx** start, TVtx** end);
	TVtx** DivideByPlane(CDQHLine* plane, TVtx** start, TVtx** end);
	/**	��̖ʂɑ΂��鏈�����s���D��ԉ����̒��_�������C
		�n�����𒲂ׁC�R�[�������C�����̒��_���͂����D*/
	void TreatPlane(CDQHLine* cur);
};

///	���_�N���X�̗�
class CDQHVtx2DSample{
public:
	///@name QuickHull���g�p���郁���o�D�K�{�D
	//@{
	///  ���_�̈ʒu
	Vec2f GetPos() const { return pos; }
	//@}
public:
	Vec2f pos;		///<	�ʒu
	int id_;
	void Print(std::ostream& os) const;
};

}

#endif
