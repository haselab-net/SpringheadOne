#ifndef CDQUICKHULL3D_H
#define CDQUICKHULL3D_H

#include <Base/Affine.h>
#include <Base/TQuaternion.h>
#include <iosfwd>
#include <vector>

#define HULL_EPSILON 1e-6f	///<	QuickHull�ň����ŏ��̐��l
#define HULL_INFINITE 1e8f	///<	QuickHull�ň����ő�̐��l

//#define HULL_DEBUG		//	�f�o�b�O�o��
#ifdef HULL_DEBUG
 #define HULL_DEBUG_EVAL(x) x
#else
 #define HULL_DEBUG_EVAL(x)
#endif

namespace Spr{;

///	QuickHull�ō�����
template <class TVtx>
class CDQHPlane{
public:
	Vec3d normal;			///<	�ʂ̖@��
	double dist;			///<	�ʂ̌��_����̋���
	
	TVtx* vtx[3];			///<	�ʂ��\�����钸�_
	CDQHPlane* neighbor[3];	///<	�ׂ̖� vtx[0]-vtx[1] �ׂ̗� neighbor[0]
	bool deleted;			///<	�폜���ꂽ�ʂ�true
	void Clear();			///<	�������N���A�D�g���O�ɌĂԁD
	bool Visible(TVtx* p);	///<	���_ v ����\���������邩�ǂ���
	///	v�̒��_�ԍ���Ԃ�(0..2��Ԃ�)�D������Ȃ����3��Ԃ��D
	int GetVtxID(TVtx* v);
	void CalcNormal();		///<	�@���x�N�g���Ƌ������v�Z����D
	void Reverse();			///<	�\�����Ђ�����Ԃ��D		
	///	�_�Ƃ̋������v�Z����D���x���l�����Ĉ�ԋ߂��_�Ōv�Z����D
	double CalcDist(TVtx* v);
	///	�f�o�b�O�p�\��
	void Print(std::ostream& os) const;
};

///	�ʂ̃o�b�t�@
template <class TVtx>
class CDQHPlanes{
public:
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
	typedef CDQHPlane<TVtx> CDQHPlane;

	CDQHPlane* buffer;		///<	�o�b�t�@�ւ̃|�C���^ new ����D
	int len;			///<	�o�b�t�@�̒���
	CDQHPlane* begin;		///<	�ŏ��̖�
	CDQHPlane* end;		///<	�Ō�̖ʂ̎�
	TVtx** vtxBegin;	///<	�c���Ă��钸�_�̐擪
	TVtx** vtxEnd;		///<	�c���Ă��钸�_�̍Ō�̎�
	int nPlanes;		///<	�ʂ̐�
	unsigned size();	///<	�g�p�ς݃o�b�t�@�̃T�C�Y
	CDQHPlanes(int l);
	void Clear();
	~CDQHPlanes();
	/**	b����e�܂ł̒��_����ʕ�����D�g�p�������_��b����vtxBegin�C
		�g�p���Ȃ��������_�́CvtxBegin����e�Ɉړ�����D	
		begin����end�͒��_��3�܂ޖʂɂȂ�D�����̖ʂ����ʕ�Ɏg��ꂽ��
		�� CDQHPlane::deleted �� false �ɂȂ��Ă���D	*/
	void CreateConvexHull(TVtx** b, TVtx** e);
	void Print(std::ostream& os) const;

private:
	/**	�ŏ��̓ʑ��ʑ́�2���̎O�p�`(�\��)�����D
		�ł��邾���傫��3�p�`�����ƌ������ǂ��̂ŁCx�ő�Cx�ŏ��C
		2�_����钼�������ԉ������_��3�p�`�����D
		�g�p�������_�́CvtxBegin�̎�O�Ɉړ����āC���_���X�g����͂����D
	*/
	void CreateFirstConvex();
	/**	horizon �����D cur ������������ʁCvtx ���V�������_�D
	rv ��horizon��ӂɎ���3�p�`��1�Ԃ��D*/
	void FindHorizon(TVtx*& rv, CDQHPlane* cur, TVtx* vtx);
	/**	���_��horizon�̊ԂɃR�[�������D*/
	void CreateCone(TVtx* firstVtx, TVtx* top);
	/**	��ԉ����̒��_��������D�������炻��𒸓_���X�g����͂���	*/
	bool FindFarthest(CDQHPlane* plane);
	/*	�O�� ���� �̏��ɕ��ׂ�D
		�O���̏I��聁�����̎n�܂肪 inner	*/
	TVtx** DivideByPlaneR(CDQHPlane* plane, TVtx** start, TVtx** end);
	TVtx** DivideByPlane(CDQHPlane* plane, TVtx** start, TVtx** end);
	/**	��̖ʂɑ΂��鏈�����s���D��ԉ����̒��_�������C
		�n�����𒲂ׁC�R�[�������C�����̒��_���͂����D*/
	void TreatPlane(CDQHPlane* cur);
};

///	���_�N���X�̗�
class TVtx{
public:
	///@name QuickHull���g�p���郁���o�D�K�{�D
	//@{
	/**	QuickHull�A���S���Y���p���[�N�G���A�D
		��ԉ������_���猩����ʂ��폜�������Ǝc�����`��̃G�b�W����
		������ڑ����Ă������߂̃|�C���^�D
		���_���ʂ̐ڑ��D	�ʁ����_�͒��_�̕��я����番����D	*/
	CDQHPlane<TVtx>* horizon;
	///  ���_�̈ʒu
	Vec3f GetPos() const;
	//@}
	///	�ʒu�̐ݒ�
	void SetPos(Vec3f p);
public:
	Vec3f dir;		///<	���_����̌���
	float dist;		///<	���_����̋���
public:
	int id_;

	///	�f�o�b�O�p�\��
	void Print(std::ostream& os) const;
};

}

#endif
