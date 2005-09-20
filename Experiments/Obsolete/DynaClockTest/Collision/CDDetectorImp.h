#ifndef CDDETECTORIMP_H
#define CDDETECTORIMP_H
#include "CDQuickHull3D.h"

namespace Spr {;

#define CD_EPSILON	HULL_EPSILON
#define CD_INFINITE	HULL_INFINITE

class CDFace;
class CDConvex;
class CDConvexPair;
class CDConvexPairWithNormal;
class SGScene;

///	BBox���m�̌�������D�������Ă���� true�D
bool BBoxIntersection(Affinef postureA, Vec3f centerA, Vec3f extentA,
					  Affinef postureB, Vec3f centerB, Vec3f extentB);

///	GJK�ŋ��L�_��������
bool FindCommonPoint(const CDConvex& a, const CDConvex& b,
					 const Affinef& a2w, const Affinef& b2w,
					 Vec3d& v, Vec3d& pa, Vec3d& pb);

/// GJK�ōŋߖT�_�΂�������
void FindClosestPoints(const CDConvex& a, const CDConvex& b,
					   const Affinef& a2w, const Affinef& b2w,
					   Vec3d& pa, Vec3d& pb);

/**	�Փ˔��茋��(���������̌`������߂�@�\/�@�������߂�@�\�t��)	*/
class CDContactAnalysis{
public:
	/// \addtogroup quickHull QuickHull�̂��߂̒��_�ƕ���
	//@{
	///	�ʂ�o�Εϊ��������_��QuickHull������̂ŁCCDFace��Vtxs.
	typedef std::vector<CDFace*> Vtxs;	
	static Vtxs vtxs;					///<	QuickHull�̒��_
	static CDQHPlanes<CDFace> planes;	///<	��
	bool isValid;						///<	���������̃|���S���͗L���H
	//@}
	/**	���ʕ����̌`������߂�D
		���ʂ́C���ʕ������\������ʂ� vtxs.begin() ����Ԃ�l�܂łɁC
		���ʕ������\�����钸�_���C planes.begin ���� planes.end �̂�����
		deleted==false �̂��̂ɓ���ĕԂ��D
		convex[0], convex[1]�̒��_��World�n�ɕϊ�����ϊ��s���
		af0, af1 �ɓn���Ȃ���΂Ȃ�Ȃ��D	*/
	CDFace** FindIntersection(CDConvexPair& cp, Affinef* af);
	/**	���������̌`��̖@����ϕ����āC�Փ˂̖@�������߂�D
		����A��B�̏Փ˂̖@���́C���������̖ʂ̂����CA�̖ʂ̖@���̐ϕ�
		����B�̖ʂ̖@���̐ϕ������������̂ɂȂ�D	*/
	void IntegrateNormal(CDConvexPairWithNormal& cp, Affinef* af);
	/**	�@���̌v�Z�D�O��̖@���̌����ɕ��̂𓮂����C
		���̂𗣂��čŋߖT�_�����߂�D	*/
	void CalcNormal(CDConvexPairWithNormal& cp,  Affinef* af);
	/**	���ʓ_�ƌ��������̌`��̕`��	*/
	void Draw(CDConvexPair& cp, Affinef afw, SGScene* s);
};

}
#endif
