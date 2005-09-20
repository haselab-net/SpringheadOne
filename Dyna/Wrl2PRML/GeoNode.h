#ifndef	_TGeoNode_H_
#define	_TGeoNode_H_

#include "CalcPenalty.h"
#include <penalty.h>
#include <Base/Utilities.h>
#include <Base/VRBase.h>
#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>
#include <SceneGraph/Wrl2view/Wrl2view.h>
#include <algorithm>

class Object;
using namespace LVRM;

/**	�����̃V�[���O���t���\������m�[�h�D���O����(DEF������)VRML�m�[�h
	�P�ɕt��1�m�[�h���쐬�����D*/
class TGeoNode : public UTRefCount{
public:
	///	���̃m�[�h�̎p���D(5/6�ύX)
	Affined	posture;
protected:
	///	�m�[�h��
	string name;
	///	�Ή�����VRML�m�[�h 
	UTRef<TNode> vrmlNode;
	///	OpenGL��DisplayList��ID
	int	displayList;

public:
	//	�`�� --------------------------------------------------------
	///	���_�̍��W
	TCoords vertices;
	///	���ʑ̂���
	TPolytopes polytopes;
	///	SOLID�̃I�u�W�F�N�g�ւ̃|�C���^
	Object* solid;

	//	�_�C�i�~�N�X ------------------------------------------------
	///	����
	double mass;
	///	�����e���\��
	Matrix3d inertia;
	///	�^����
	Vec3d momentum;
	///	�p�^����
	Vec3d angularMomentum;
	///	�d�S�̈ʒu
	Vec3d center;
	///
	Vec3d penalty, angularPenalty;
	///
	Vec3d lastPenalty, lastAngularPenalty;
	/// ���C�̓x�[�X�{�b�N�X
/*	class FricBase{
	public:
		Vec3d friction;
		TGeoNode* g;
		double dt;
	};*/
//	typedef std::pair<TPenaltyDetector*, Vec3d> FricBase;
//	vector<FricBase> fricBases;	//(5/9�ǉ�)
	// --------------------------------------------------------------
	///
	TGeoNode();
	///
	TGeoNode(const TGeoNode& n);
	///	�Ή�����VRML�m�[�h
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̎擾
	Affined GetPosture(){ return posture; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̐ݒ�
	void SetPosture(Affined a){ posture = a; }
	///	�d�S�̈ʒu
	Vec3d Center(){ return center; }
	///	���x

	// --------------------------------------------------------------
	/// ���x
	Vec3d Vel(){ return momentum / mass; }
	///	�p���x
	Vec3d AngVel(){
		Matrix3d rot = posture.Rot();
		return rot.trans() * inertia.inv() * rot * angularMomentum;
	}
	///	�p���̍X�V
	void UpdatePosture(double dt);
	///	�y�i���e�B��������D���[���h�n���_����Ƃ���D
	void AddPenalty(Vec3d p, Vec3d a);
	///	�y�i���e�B��͂ɕϊ�����D
	void ConvertToForce(double dt);
	///	�d�͂�������
	void AddGravity(Vec3d gravity, double dt);
	///	��C��R
	void AirResistance(double a, double dt){
		momentum = momentum * (1 - a*dt);
		angularMomentum = angularMomentum * (1 - a*dt);
	}

 	// --------------------------------------------------------------
	///	�m�[�h��`��
	void Draw();
	void DrawGeometry();
	///	���O
	string Name() { return name; }
	void Name(string n) { name = n; }

	friend class TGeoNodeBuilder;
};
typedef vector< UTRef<TGeoNode> > TGeoNodes;
/**	VRML�V�[���O���t����CTGeoNode�̃V�[���O���t�����o���D
	VRML�V�[���O���t�̃��X�i�N���X�ɂȂ��Ă���D*/
class TGeoNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affined> TAffineStack;
	typedef vector< UTRef<TGeoNode> > TNodeStack;
	TAffineStack afStack;
	TGeoNode* curNode;
public:
	///	���������m�[�h	
	TGeoNodes allNodes;
	///
	Vec3d clearColor;
	///
	Affined viewPoint;
	///
	Affined projection;
	///
	TGeoNodeBuilder();
	///	TGeoNode�V�[���O���t�̍쐬�D
	void Build(TSts* sts);
	///	VRML�̃m�[�h����TGeoNode�����o���֐��D
	TGeoNode* CreateTGeoNode(TNode* n);
protected:
	///	���O�t���m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundNamed(TNode *pn);
	///	���ł��R�[���o�b�N
	bool FoundTypeNode(TNode *pn);
	///	Transform�m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundTransformBefore(TNode *pn);
	///	Transform�m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundTransformAfter(TNode *pn);
};

#endif // _TGeoNode_H_
