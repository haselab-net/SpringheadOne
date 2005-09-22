#ifndef PH_GEOMETRY2D_H
#define PH_GEOMETRY2D_H

///////////////////////////////////////////////////////////////////////
//Geometric Constraint System 2D
/* �􉽍S���\���o�[
	- �g���� ----------------------------------------------
	//�\���o�𐶐�
	PHGeometry3D geo;

	//�S����ǉ�
	geo.Add(new ***(frame1, frame2)); //***�͍S���̎��
	...

	//�S��������
	//	���ʂɂ��frame�̈ʒu�p�����X�V�����
	geo.Solve();
	-------------------------------------------------------
	//�S���̎��
	

	//����
	�S���͓��frame�Ԃ̓����S����frame���m�[�h�A�S�����G�b�W�Ƃ���O���t
	�G�b�W�͎��R�x�����i�Ⴆ�΃{�[���W���C���g�S���Ȃ�3�C�q���W�֐ߍS���Ȃ�1�j
	�S�G�b�W�̎��R�x������ϐ��Ƃ��C�S�G�b�W�̍S���𖞑�����l�����߂�
		����frame�̈ʒu�p��������ϐ��ɂ���Ǝ������傫���Ȃ�̂�NG

	�s�����S���̖��F
	����frame�̌`�󂪌������Ȃ��S���͎���������
		���y�i���e�B�@
		

	���֐ߎ��̎w������^�Z�R�ŐF���̐����Ŏw�肷��͕̂֗������I
 */


//�J���ӔC�ҁF�c��E�� tazaki@cyb.mei.titech.ac.jp

#include <Base/Affine.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Graphics/GRVisual.h>
#include <Physics/PHSolid.h>
//#include <algorithm>
//#include <list>
#include <vector>

using namespace PTM;
namespace Spr{;

//�[�������Ȓl
static const double PHG2Epsilon = 1.0e-5;

//���R�x
//	���݂̎����ł�T1R1�ƂȂ�P�[�X�͑��݂��Ȃ�
enum PHG2_DOF{
	PHG2_T0R0,
	PHG2_T0R1,
	PHG2_T1R0,
	PHG2_T2R0,
	PHG2_T2R1
};

//�S���̎��
enum PHConstraint2DType{
	PHG2_POINT_TO_POINT,	//�_�|�_�S���F�Q�_����v������
	PHG2_POINT_TO_LINE,		//�_�|���S���F�_�����ɍS������
	PHG2_POINT_TO_ARC,
	PHG2_LINE_TO_LINE,
	PHG2_LINE_TO_ARC,
	PHG2_ARC_TO_ARC,
	PHG2_PARALLEL,			//���s�S���F�Q�����Ȃ�ׂ������ȉ�]�ŕ��s�ɂ���
	PHG2_ANGLE,				//�p�x�S���Flhs�̃x�N�g��p0->p1����Ƃ���rhs�̃x�N�g��q0->q1�̊p�x��
							//	�C�ӂ̒l�i�����t���j�ɍS������
	PHG2_FIX,				//�Œ�S���F
};

//PHConstraint2D::Solve�̖߂�l
enum PHG2Result{
	PHG2_AGAIN,		//������x�]����]
	PHG2_SOLVED,	//������
	PHG2_NEW,		//�V�����S�����P������
	PHG2_ILLPOSED,	//�����s�\
	PHG2_OVER,		//�ߏ�S��
	PHG2_REDUNDANT,	//�璷�S��
	PHG2_ERROR		//���̑��̃G���[
};

//�O���R���|�[�l���g�ƃ����N��������N���X
class PHG2Frame : public UTRefCount{
public:
	UTRef<SGFrame>	frame;
	PHG2_DOF	dof;			//���s�ړ��A��]�̊e���R�x
	Vec2d		center;			//T0R1���̉�]���S
	Vec2d		range0, range1;	//T1R0���̕��s�ړ��͈�

	Vec2d		position;		//�ʒu
	double		angle;			//�p�x

	//���[�J�����W�̃��[���h���W
	Vec2d toWorld(const Vec2d& pl){
		return Matrix2d::Rot(angle) * pl + position;
	}
	Vec2d toLocal(const Vec2d& pw){
		return Matrix2d::Rot(-angle) * (pw - position);
	}

	PHG2Frame& operator=(const PHG2Frame& src){
		dof = src.dof;
		center = src.center;
		range0 = src.range0;
		range1 = src.range1;
		position = src.position;
		angle = src.angle;
		return *this;
	}

	//operator SGFrame*(){return frame;}
	//operator const SGFrame*()const{return frame;}

	PHG2Frame(SGFrame* f):frame(f){
		dof = PHG2_T2R1;
		angle = 0.0;
	}
	PHG2Frame(const PHG2Frame& src){*this = src;}
};

//�I�u�W�F�N�g�̔z��
class PHG2FrameList : public std::vector<UTRef<PHG2Frame> >{
public:
	PHG2FrameList& operator=(const PHG2FrameList& src){
		for(const_iterator it = src.begin(); it != src.end(); it++)
			push_back(new PHG2Frame(**it));
		return *this;
	}
	PHG2FrameList(){}
	PHG2FrameList(const PHG2FrameList& src){*this = src;}
};

//�S���N���X
class PHConstraintList2D;
class PHConstraint2D : public SGObject{
public:		
	SGOBJECTDEFABST(PHConstraint2D);

	virtual bool AddChildObject(SGObject* o, SGScene* s);
	virtual bool DelChildObject(SGObject* o, SGScene* s);

	PHConstraint2DType	type;	//�S���̎��
	UTRef<PHG2Frame> lhs, rhs;	//�S���Ώ�

	//�������쐬
	virtual PHConstraint2D* dup(){return 0;}
	//�S��������
	virtual PHG2Result Solve(PHConstraintList2D& newcon){return PHG2_ERROR;}

	PHConstraint2D(){}
	PHConstraint2D(PHConstraint2DType _type, SGFrame* _lhs, SGFrame* _rhs){
		type = _type;
		lhs = new PHG2Frame(_lhs);
		rhs = new PHG2Frame(_rhs);
	}
	virtual ~PHConstraint2D(){}
};

//�S���̃��X�g
class PHConstraintList2D : public std::list<UTRef<PHConstraint2D> >{
public:
	PHConstraintList2D& operator=(const PHConstraintList2D& src){
		resize(src.size());
		iterator it0;
		const_iterator it1;
		for(it0 = begin(), it1 = src.begin();
			it0 != end();
			it0++, it1++){
			*it0 = (*it1)->dup();
		}
		return *this;
	}
	PHConstraintList2D(){}
	PHConstraintList2D(const PHConstraintList2D& src){*this = src;}
};
		
//�{��
class PHGeometry2DEngine : public SGBehaviorEngine{
public:		
	SGOBJECTDEF(PHGeometry2DEngine);

	virtual bool AddChildObject(SGObject* o, SGScene* s);
	virtual bool DelChildObject(SGObject* o, SGScene* s);
	virtual int GetPriority() const {return SGBP_GEOMETRYENGINE;}
	virtual void Step(SGScene* s);
	virtual void Loaded(SGScene* scene);
	virtual void Clear(SGScene* s);
	virtual size_t NChildObjects(){ return frames.size(); }
	virtual SGObject* ChildObject(size_t i){ return (SGObject*)&*(frames[i]); }

	///�S����ǉ�����
	PHConstraint2D*		Add(PHConstraint2D* con);

	///�S�����폜����
	void				Remove(PHConstraint2D*);

	///�S�����X�g�̎擾
	const PHConstraintList2D& Constraints()const{return cons;}

	///�S�N���A
	void Clear();

	///�S������������
	void Solve();

	PHGeometry2DEngine(){}
	~PHGeometry2DEngine(){}
protected:
	UTRef<SGFrame>		plane;
	PHG2FrameList		frames;
	PHConstraintList2D	cons, cons_tmp;
	
	void		_Preprocess();
	void		_Postprocess();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	�F�X�ȍS��

//Vector2��Float�^�������J�������Ȃ��̂œ���namespace
namespace PHG2{
	typedef float Float;
	typedef Vec2f Vector2;
	/*class Vector2 : public SGObject, public Vec2f{
	public:
		SGOBJECTDEF(Vector2);
		Vector2& operator=(const Vec2f& v){
			*(Vec2f*)this = v;
			return *this;
		}
	};*/
	
	DEF_RECORD(XVector2, {
		GUID Guid(){return WBGuid("ec045bba-b265-4511-973d-500656055f40");}
		Float x;
		Float y;
	});

//#define array

	//�q�m�[�h�F
	//Frame x 1�D����Frame��xy���ʂ�����ʂƂȂ�D���w��Ȃ�O���[�o���t���[��
	//�S�� x N > 0�D
	DEF_RECORD(XGeometry2DEngine, {
		GUID Guid(){ return WBGuid("a59cd5af-d032-421f-a3ce-24920fd84222"); }
	});

	DEF_RECORD(XPointToPoint2D, {
		GUID Guid(){ return WBGuid("e4fa6c65-eddb-473c-96e9-9300d63875b0"); }
		Vector2	p;
		Vector2 q;
		//array Float p[2];
		//array Float q[2];
	});

	DEF_RECORD(XPointToLine2D, {
		GUID Guid(){ return WBGuid("1b0bbd07-7d79-4f95-ae7a-660e8fe05ed5"); }
		Vector2	p;
		Vector2 q0;
		Vector2 q1;
		//array Float p[2];
		//array Float q0[2];
		//array Float q1[2];
	});

	DEF_RECORD(XPointToArc2D, {
		GUID Guid(){ return WBGuid("6669a0c5-6ad0-4f58-84e5-98673b2cd03d"); }
		Vector2	p;
		Vector2 c;
		//array Float p[2];
		//array Float c[2];
		Float	r;
		Float	s0;
		Float	s1;
	});

	DEF_RECORD(XLineToLine2D, {
		GUID Guid(){ return WBGuid("2e675bc7-c8a2-42bd-9d18-02e60e8f8b6a"); }
		//array Float p0[2];
		//array Float p1[2];
		//array Float q0[2];
		//array Float q1[2];
		Vector2	p0;
		Vector2 p1;
		Vector2 q0;
		Vector2 q1;
	});

	DEF_RECORD(XLineToArc2D, {
		GUID Guid(){ return WBGuid("f9e509e9-3010-41dd-9cf1-c9c6f6fb76ae"); }
		Vector2 p0;
		Vector2 p1;
		Vector2 c;
		//array Float p0[2];
		//array Float p1[2];
		//array Float c[2];
		Float	r;
		Float	s0;
		Float	s1;
	});

	DEF_RECORD(XArcToArc2D, {
		GUID Guid(){ return WBGuid("41bc8040-7536-4bf2-b66d-da3f5a8f2ffc"); }
		Vector2 c0;
		Vector2 c1;
		//array Float c0[2];
		//array Float c1[2];
		Float	r0;
		Float	r1;
		Float	s00;
		Float	s01;
		Float	s10;
		Float	s11;
	});

	DEF_RECORD(XParallel2D, {
		GUID Guid(){ return WBGuid("cfd3c326-485a-42b7-8311-63bbbf21c58c"); }
		Float	theta0;
		Float	theta1;
	});

	DEF_RECORD(XAngle2D, {
		GUID Guid(){ return WBGuid("66cc9fe9-b9b6-46a8-9c04-6049a6c2b782"); }
		Float	theta0;
		Float	theta1;
		Float	phi;
	});

	DEF_RECORD(XFix2D, {
		GUID Guid(){ return WBGuid("8a3c63cf-639e-408f-8075-44c35f78c539"); }
		Vector2	p;
		//array Float p0[2];
		Float	theta;
	});
}
	
//�_�|�_�S��
class PHPointToPoint2D : public PHConstraint2D, public PHG2::XPointToPoint2D{
public:
	SGOBJECTDEF(PHPointToPoint2D);
	PHPointToPoint2D(){}
	PHPointToPoint2D(
		SGFrame* _lhs, const Vec2f& _p,
		SGFrame* _rhs, const Vec2f& _q):
		PHConstraint2D(PHG2_POINT_TO_POINT, _lhs, _rhs)
		{p = _p; q = _q;}
	PHConstraint2D* dup(){return new PHPointToPoint2D(lhs->frame, p, rhs->frame, q);}
	PHG2Result Solve(PHConstraintList2D&);
};

//�_�|���S��
class PHPointToLine2D : public PHConstraint2D, public PHG2::XPointToLine2D{
public:
	SGOBJECTDEF(PHPointToLine2D);
	PHPointToLine2D(){}
	PHPointToLine2D(
		SGFrame* _lhs, const Vec2f& _p,
		SGFrame* _rhs, const Vec2f& _q0, const Vec2f& _q1):
		PHConstraint2D(PHG2_POINT_TO_LINE, _lhs, _rhs)
		{p = _p; q0 = _q0; q1 = _q1;}
	PHConstraint2D* dup(){return new PHPointToLine2D(lhs->frame, p, rhs->frame, q0, q1);}
	PHG2Result Solve(PHConstraintList2D&);
};

//�_�|�~�S��
class PHPointToArc2D : public PHConstraint2D, public PHG2::XPointToArc2D{
public:
	SGOBJECTDEF(PHPointToArc2D);
	PHPointToArc2D(){}
	PHPointToArc2D(
		SGFrame* _lhs, const Vec2f& _p,
		SGFrame* _rhs, const Vec2f& _c, float _r, float _s0, float _s1):
		PHConstraint2D(PHG2_POINT_TO_ARC, _lhs, _rhs)
		{p = _p; c = _c; r = _r; s0 = _s0; s1 = _s1;}
	PHConstraint2D* dup(){return new PHPointToArc2D(lhs->frame, p, rhs->frame, c, r, s0, s1);}
	PHG2Result Solve(PHConstraintList2D&);
};

//���|��
class PHLineToLine2D : public PHConstraint2D, public PHG2::XLineToLine2D{
public:
	SGOBJECTDEF(PHLineToLine2D);
	PHLineToLine2D(){}
	PHLineToLine2D(
		SGFrame* _lhs, const Vec2f& _p0, const Vec2f& _p1,
		SGFrame* _rhs, const Vec2f& _q0, const Vec2f& _q1):
		PHConstraint2D(PHG2_LINE_TO_LINE, _lhs, _rhs)
		{p0 = _p0; p1 = _p1; q0 = _q0; q1 = _q1;}
	PHConstraint2D* dup(){return new PHLineToLine2D(lhs->frame, p0, p1, rhs->frame, q0, q1);}
	PHG2Result Solve(PHConstraintList2D&);
};

//���|�~�S��
class PHLineToArc2D : public PHConstraint2D, public PHG2::XLineToArc2D{
public:
	SGOBJECTDEF(PHLineToArc2D);
	PHLineToArc2D(){}
	PHLineToArc2D(
		SGFrame* _lhs, const Vec2f& _p0, const Vec2f& _p1,
		SGFrame* _rhs, const Vec2f& _c, float _r, float _s0, float _s1):
		PHConstraint2D(PHG2_LINE_TO_ARC, _lhs, _rhs)
		{p0 = _p0; p1 = _p1; c = _c; r = _r; s0 = _s0; s1 = _s1;}
	PHConstraint2D* dup(){return new PHLineToArc2D(lhs->frame, p0, p1, rhs->frame, c, r, s0, s1);}
	PHG2Result Solve(PHConstraintList2D&);
};

//�~�|�~�S��
class PHArcToArc2D : public PHConstraint2D, public PHG2::XArcToArc2D{
public:
	SGOBJECTDEF(PHArcToArc2D);
	PHArcToArc2D(){}
	PHArcToArc2D(
		SGFrame* _lhs, const Vec2f& _c0, float _r0, float _s00, float _s01,
		SGFrame* _rhs, const Vec2f& _c1, float _r1, float _s10, float _s11):
		PHConstraint2D(PHG2_ARC_TO_ARC, _lhs, _rhs){
		c0 = _c0; r0 = _r0; s00 = _s00; s01 = _s01;
		c1 = _c1; r1 = _r1; s10 = _s10; s11 = _s11;
	}
	PHConstraint2D* dup(){return new PHArcToArc2D(lhs->frame, c0, r0, s00, s01, rhs->frame, c1, r1, s10, s11);}
	PHG2Result Solve(PHConstraintList2D&);
};

//���s�S��
class PHParallel2D : public PHConstraint2D, public PHG2::XParallel2D{
public:
	SGOBJECTDEF(PHParallel2D);
	PHParallel2D(){}
	PHParallel2D(
		SGFrame* _lhs, float _theta0,
		SGFrame* _rhs, float _theta1):
		PHConstraint2D(PHG2_PARALLEL, _lhs, _rhs)
		{theta0 = _theta0; theta1 = _theta1;}
	PHConstraint2D* dup(){return new PHParallel2D(lhs->frame, theta0, rhs->frame, theta1);}
	PHG2Result Solve(PHConstraintList2D&);
};

//�p�x�S��
class PHAngle2D : public PHConstraint2D, public PHG2::XAngle2D{
public:
	SGOBJECTDEF(PHAngle2D);
	PHAngle2D(){}
	PHAngle2D(
		SGFrame* _lhs, float _theta0,
		SGFrame* _rhs, float _theta1,
		float _phi):
		PHConstraint2D(PHG2_ANGLE, _lhs, _rhs)
		{theta0 = _theta0; theta1 = _theta1; phi = _phi;}
	PHConstraint2D* dup(){return new PHAngle2D(lhs->frame, theta0, rhs->frame, theta1, phi);}
	PHG2Result Solve(PHConstraintList2D&);
};

//�Œ�S��
class PHFix2D : public PHConstraint2D, public PHG2::XFix2D{
public:
	SGOBJECTDEF(PHFix2D);
	PHFix2D(){}
	PHFix2D(
		SGFrame* _lhs,
		SGFrame* _rhs, const Vec2d& _p, float _theta):
		PHConstraint2D(PHG2_FIX, _lhs, _rhs)
		{p = _p; theta = _theta;}
	PHConstraint2D* dup(){return new PHFix2D(lhs->frame, rhs->frame, p, theta);}
	PHG2Result Solve(PHConstraintList2D&);
};

///////////////////////////////////////////////////////////////////////////

/*
enum GC2_GeometryType{
	GC2_Point,
	GC2_Line,
	GC2_Circle
};

class GC2Geometry
{
protected:
	GC2_GeometryType	type;
};

class GC2Point : public GC2Geometry
{
public:
	Vec2d	p;
	operator Vec2d&(){return p;}
	operator Vec2d()const{return p;}
	GC2Point(const Vec2d& _p):GC2Geometry(GC2_Point), p(_p){}
};

class GC2Line : public GC2Geometry
{
public:
	Vec2d p0, p1;
	GC2Line(const Vec2d& _p0, const Vec2d& _p1):GC2Geometry(GC2_Line), p0(_p0), p1(_p1){}
};

class GC2Circle : public GC2Geometry
{
public:
	Vec2d c;
	double r, theta0, theta1;
	GC2Circle(const Vec2d& _c, double _r, double _theta0, double _theta1):
		GC2Geometry(GC2_Circle), r(_r), theta0(_theta0), theta1(_theta1){}
};
*/

////////////////////////////////////////////////////////////////////////	
//�Q�����􉽊֐�

///����_�܂��̉�]
inline void RotationAlong(Vec2d* position, double* angle, const Vec2d& center, double rot){
	*angle += rot;
	*position = Matrix2d::Rot(rot) * (*position - center) + center;
}

//����_�ɍł��߂�������̓_
inline void NearestPoint(
	Vec2d* y,
	const Vec2d& p,
	const Vec2d& q, const Vec2d& u)
{
	*y = q + PTM::dot(u, p - q) / u.square() * u;
}

//�Q�����̊p�x
inline double Line_Line_Angle(
	const Vec2d& u0,
	const Vec2d& u1)
{
	return atan2(PTM::cross(u0, u1), PTM::dot(u0, u1));
}

//�Q�����̌�_
inline bool Line_Line_Intersect(
	Vec2d* y,
	const Vec2d& p0, const Vec2d& u0,
	const Vec2d& p1, const Vec2d& u1)
{
	//�A��������
	Matrix2d A(u0, -u1);
	if(fabs(A.det()) < 0.0)return false;
	Vec2d s = A.inv() * (-p0 + p1);
	
	*y = p0 + s[0] * u0;
	return true;
}

//�~�ƒ����̌�_
//�������Ȃ��ꍇ�݂͌��ɍł��߂��_��Ԃ�
inline bool Line_Circle_Intersect(
	Vec2d* y0, Vec2d* y1,
	const Vec2d& p, const Vec2d& u,
	const Vec2d& c, double r)
{
	//�񎟕������̌W��
	double A = u.square();
	double B = PTM::dot(u, (p - c));
	double C = (p - c).square() - r * r;
	//���ʎ�
	double det = B * B - A * C;
	if(det < 0){
		//�������Ȃ��̂ōŋߓ_�����߂�
		NearestPoint(y0, c, p, u);			//������̍ŋߓ_
		Vec2d c_y0 = *y0 - c;
		*y1 = c + r / c_y0.norm() * c_y0;	//�~��̍ŋߓ_
		return false;
	}
	//�������̉��ƌ�_
	double det_root = sqrt(det);
	double s0 = (-B + det_root) / A, s1 = (-B - det_root) / A;
	*y0 = p + s0 * u, *y1 = p + s1 * u;
	return true;
}

//�Q�~�̌�_
//�������Ȃ��ꍇ�݂͌��ɍł��߂��_��Ԃ�
inline bool Circle_Circle_Intersect(
	Vec2d* y0, Vec2d* y1,
	const Vec2d& c0, double r0,
	const Vec2d& c1, double r1)
{
	//���S�Ԃ��Ȃ��x�N�g��
	Vec2d d = c1 - c0;
	double d_norm = d.norm();
	//�~�̔��a
	//�~���m�̌X���ƑΉ������]�s��
	double phi = atan2(d.Y(), d.X());
	Matrix2d rot = Matrix2d::Rot(phi);
	//���ʎ�
	double det = 
		( r0 + r1 + d_norm) * 
		( r0 + r1 - d_norm) * 
		( r0 - r1 + d_norm) * 
		(-r0 + r1 + d_norm);
	if(det < 0){
		//�������Ȃ��̂ōŋߓ_�����߂�
		*y0 = c0 + r0 / d_norm * d;
		*y1 = c1 - r1 / d_norm * d;
		return false;
	}
	//c0�����_�Ad��x���Ƃ���t���[���ł̌�_���W
	double h = sqrt(det) / (2 * d_norm);
	double w = sqrt(r0 * r0 - h * h);
	//��_
	*y0 = rot * Vec2d(w,  h) + c0;
	*y1 = rot * Vec2d(w, -h) + c0;
	return true;
}

//�Q�~�̋��ʐڐ�
//�~c0��̐ړ_��p0, p1, p2, p3�ɁAc1��̐ړ_��q0, q1, q2, q3��
//�ڐ��̖{����Ԃ�(0, 2, 4�̂����ꂩ��Ԃ��B1, 3�{�͂Q�~���ڂ��Ă����Ԃ�
//�N���邪
inline int Circle_Circle_Tangent(
	const Vec2d& c0, double r0,
	const Vec2d& c1, double r1,
	Vec2d* p0, Vec2d* p1, Vec2d* p2, Vec2d* p3,
	Vec2d* q0, Vec2d* q1, Vec2d* q2, Vec2d* q3)
{
	Vec2d d = c1 - c0;
	double d_norm = d.norm();
	double phi = atan2(d.Y(), d.X());
	//�Q�~�����S�����L����ꍇ
	if(d_norm < PHG2Epsilon)return 0;
	//���ʎ�
	double det1 = (r0 - r1) / d_norm;
	double det2 = (r0 + r1) / d_norm;
	int nline;
	double theta;
	double _cos, _sin;
	//c0�����_�Ƃ��āAd�������Ƃ�����W�Ōv�Z��i�߂�F
	//�Е��̉~���������������ꍇ
	if(fabs(det1) > 1.0 + PHG2Epsilon)
		return 0;
	//���ڂ���ꍇ
	if(fabs(det1) > 1.0 - PHG2Epsilon){
		//�ǂ������O����
		*p0 = *q0 = Vec2d(r0 > r1 ? r0 : -r0, 0);
		nline = 1;
	}
	//�Q�_�Ō�������ꍇ
	else if(det2 > 1.0 + PHG2Epsilon){
		theta = acos(det1);
		_cos = cos(theta), _sin = sin(theta);
		*p0 = Vec2d(r0 * _cos,  r0 * _sin);
		*p1 = Vec2d(r0 * _cos, -r0 * _sin);
		*q0 = *p0 + d_norm * _sin * Vec2d( _sin, _cos);
		*q1 = *p1 - d_norm * _sin * Vec2d(-_sin, _cos);
		nline = 2;
	}
	//�O�ڂ���ꍇ
	else if(det2 < PHG2Epsilon){
		*p0 = *q0 = Vec2d(r0, 0);
		theta = acos(det1);
		_cos = cos(theta), _sin = sin(theta);
		*p1 = Vec2d(r0 * _cos,  r0 * _sin);
		*p2 = Vec2d(r0 * _cos, -r0 * _sin);
		*q1 = *p0 + d_norm * _sin * Vec2d( _sin, _cos);
		*q2 = *p1 - d_norm * _sin * Vec2d(-_sin, _cos);
		nline = 3;
	}
	//����Ă���ꍇ
	else{
		theta = acos(det1);
		_cos = cos(theta), _sin = sin(theta);
		*p0 = Vec2d(r0 * _cos,  r0 * _sin);
		*p1 = Vec2d(r0 * _cos, -r0 * _sin);
		*q0 = *p0 + d_norm * _sin * Vec2d( _sin, _cos);
		*q1 = *p1 - d_norm * _sin * Vec2d(-_sin, _cos);
		theta = acos(det2);
		_cos = cos(theta), _sin = sin(theta);
		*p2 = Vec2d(r0 * _cos,  r0 * _sin);
		*p3 = Vec2d(r0 * _cos, -r0 * _sin);
		*q2 = *p0 + d_norm * _sin * Vec2d( _sin, _cos);
		*q3 = *p1 - d_norm * _sin * Vec2d(-_sin, _cos);
		nline = 4;
	}
	//�Ō�ɍ��W�ϊ�
	Matrix2d rot = Matrix2d::Rot(phi);
	*p0 = c0 + rot * *p0;
	*q0 = c0 + rot * *q0;
	*p1 = c0 + rot * *p1;
	*q1 = c0 + rot * *q1;
	*p2 = c0 + rot * *p2;
	*q2 = c0 + rot * *q2;
	*p3 = c0 + rot * *p3;
	*q3 = c0 + rot * *q3;
	return nline;
}

//p��q0, q1�̊Ԃɂ��邩�𔻒�
inline bool Point_On_LineSegment(
	const Vec2d& p,
	const Vec2d& q0, const Vec2d& q1)
{
	Vec2d u = p - q0, v = p - q1;
	return 
		fabs(PTM::cross(u, v)) < u.norm() * v.norm() * PHG2Epsilon &&
		PTM::dot(u, v) < PHG2Epsilon;
}

//p��c, r�ŕ\�����~��ɂ�����̂Ƃ��āAp���p�x�͈�[s0, s1]���ɂ��邩�𔻒�
//������s0, s1�͂�������ɔ����v������
inline bool Point_On_Arc(
	const Vec2d& p,
	const Vec2d& c, double r, double s0, double s1)
{
	while(s0 < -M_PI)s0 += 2 * M_PI;
	while(s0 >  M_PI)s0 -= 2 * M_PI;
	while(s1 < -M_PI)s1 += 2 * M_PI;
	while(s1 >  M_PI)s1 -= 2 * M_PI;
	if(s1 < s0)s1 += 2 * M_PI;
	
	Vec2d u = p - c;
	double s = atan2(u.Y(), u.X());
	if(s < s0)s += M_PI;
	if(s < s1)return true;
	return false;
}

}

#endif