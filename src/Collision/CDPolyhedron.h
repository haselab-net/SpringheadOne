#ifndef CDPOLYHEDRON_H
#define CDPOLYHEDRON_H
#include "CDGeometry.h"
#include "CDConvex.h"
#include "CDQuickHull3D.h"

namespace Spr {;

class CDPolyhedron;
class CDFace{
public:
	class DualPlanes: public std::vector<CDQHPlane<CDFace>*>{
	public:
	};
	int vtxs[3];	///< �ʂ̒��_

	//	���������̌`������߂�v�Z�̂��߂̍�Ɨ̈�
	Vec3f normal;	///<	�ʂ̖@���x�N�g��
	float dist;		///<	���_����̋���
	///	QuickHull�ɂƂ��Ă̒��_�D���̖ʂ�o�Εϊ����Ăł��钸�_
	Vec3f GetPos() const { return normal / dist; }
	///
	/**	QuickHull�A���S���Y���p���[�N�G���A�D
		��ԉ������_���猩����ʂ��폜�������Ǝc�����`��̃G�b�W����
		������ڑ����Ă������߂̃|�C���^�D
		���_���ʂ̐ڑ��D	�ʁ����_�͒��_�̕��я����番����D	*/
	CDQHPlane<CDFace>* horizon;

	/**	�o�Εϊ����s���Dbase�ɓn�����_�o�b�t�@�́C�o�Εϊ����\�Ȃ悤��
		�o�Εϊ��̒��S�����_�ƂȂ�悤�ȍ��W�n�łȂ���΂Ȃ�Ȃ��D	*/
	bool CalcDualVtx(Vec3f* base);

	/**	���̖ʂ�o�Εϊ����Ăł��钸�_���܂ޖʁD
		�܂�C���������̌`����\�����钸�_�̂����C
		���̖ʂɂ������	*/
	DualPlanes dualPlanes;
	///	���������̌`����\�����钸�_�̂������̖ʂɂ�����̂̐�.
	size_t NCommonVtx(){ return dualPlanes.size(); }
	///	���������̌`����\�����钸�_�̂������̖ʂɂ������.
	Vec3f CommonVtx(int i);
	///	�f�o�b�O�p�\��
	void Print(std::ostream& os) const;
};
inline std::ostream& operator << (std::ostream& os, const CDFace& f){
	f.Print(os);
	return os;
}
class CDFaces:public std::vector<CDFace>{
};
class CDVertexIDs:public std::vector<int>{
public:
	int FindPos(int id) const;
};

/**	3�p�`��������\�������ʑ��ʑ�	*/
class CDPolyhedron:public CDConvex{
public:
	SGOBJECTDEF(CDPolyhedron);
	///	�T���J�n���_�ԍ�
	mutable int curPos;
	///	GeomtryID
	virtual int GeometryID(){ return CONVEX; }
	///	���_�̍��W(���[�J�����W�n)
	Vec3f* base;
	///	��Ɨ̈�F���_�̍��W(�ϊ���̍��W�n)
	Vec3f* tvtxs;

	///	���̓ʑ��ʑ̂��\�����钸�_�����Dpos�͂��̔z����̈ʒu��\���D
	CDVertexIDs vtxIDs;
	///	���_�ׂ̗̒��_�̈ʒu���������z��
	std::vector< std::vector<int> > neighbor;
	///	��(3�p�`)
	CDFaces faces;

	///	���_�̍��W��Ԃ��D
	Vec3f Vertex(int pos) const { return base[vtxIDs[pos]]; }

	///	���_����ʂ�ڑ����𐶐�����D
	void CalcFace();
	///	���ꕽ�ʏ�Őڑ�����Ă���3�p�`���}�[�W����
	void MergeFace();
	
	///	�ʂ������Ă��邩�ǂ������ׂ�
	bool HasFace(CDFace* face){
		return &*faces.begin() <= face && face <= &*faces.end();
	}
	///	�T�|�[�g�|�C���g�����߂�D
	virtual Vec3f Support(const Vec3f& p) const;
private:
	bool VertexNear(int v1, int v2) const;
};

}
#endif
