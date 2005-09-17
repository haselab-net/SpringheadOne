#ifndef PH_GEOMETRY2D_H
#define PH_GEOMETRY2D_H

///////////////////////////////////////////////////////////////////////
//Geometric Constraint System 2D
/* 幾何拘束ソルバー
	- 使い方 ----------------------------------------------
	//ソルバを生成
	PHGeometry3D geo;

	//拘束を追加
	geo.Add(new ***(frame1, frame2)); //***は拘束の種類
	...

	//拘束を解く
	//	結果によりframeの位置姿勢が更新される
	geo.Solve();
	-------------------------------------------------------
	//拘束の種類
	

	//メモ
	拘束は二つのframe間の等式拘束→frameをノード、拘束をエッジとするグラフ
	エッジは自由度を持つ（例えばボールジョイント拘束なら3，ヒンジ関節拘束なら1）
	全エッジの自由度を決定変数とし，全エッジの拘束を満足する値を求める
		→＊frameの位置姿勢を決定変数にすると次元が大きくなるのでNG

	不等式拘束の問題：
	特にframeの形状が交差しない拘束は実現したい
		→ペナルティ法
		

	＊関節軸の指定をメタセコで色つきの線分で指定するのは便利そう！
 */


//開発責任者：田崎勇一 tazaki@cyb.mei.titech.ac.jp

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

//充分小さな値
static const double PHG2Epsilon = 1.0e-5;

//自由度
//	現在の実装ではT1R1となるケースは存在しない
enum PHG2_DOF{
	PHG2_T0R0,
	PHG2_T0R1,
	PHG2_T1R0,
	PHG2_T2R0,
	PHG2_T2R1
};

//拘束の種類
enum PHG2ConstraintType{
	PHG2_POINT_TO_POINT,	//点－点拘束：２点を一致させる
	PHG2_POINT_TO_LINE,		//点－線拘束：点を線上に拘束する
	PHG2_POINT_TO_ARC,
	PHG2_LINE_TO_LINE,
	PHG2_LINE_TO_ARC,
	PHG2_ARC_TO_ARC,
	PHG2_PARALLEL,			//平行拘束：２線をなるべく小さな回転で平行にする
	PHG2_ANGLE,				//角度拘束：lhsのベクトルp0->p1を基準とするrhsのベクトルq0->q1の角度を
							//	任意の値（符号付き）に拘束する
	PHG2_FIX,				//固定拘束：
};

//PHG2Constraint::Solveの戻り値
enum PHG2Result{
	PHG2_AGAIN,		//もう一度評価希望
	PHG2_SOLVED,	//解けた
	PHG2_NEW,		//新しい拘束が１つ生じた
	PHG2_ILLPOSED,	//解決不能
	PHG2_OVER,		//過剰拘束
	PHG2_REDUNDANT,	//冗長拘束
	PHG2_ERROR		//その他のエラー
};

//外部コンポーネントとリンクする内部クラス
class PHG2Frame : public UTRefCount{
public:
	UTRef<SGFrame>	frame;
	PHG2_DOF	dof;			//平行移動、回転の各自由度
	Vec2d		center;			//T0R1時の回転中心
	Vec2d		range0, range1;	//T1R0時の平行移動範囲

	Vec2d		position;		//位置
	double		angle;			//角度

	//ローカル座標⇔ワールド座標
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

//オブジェクトの配列
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

//拘束クラス
class PHG2ConstraintList;
class PHG2Constraint : public SGObject{
public:		
	SGOBJECTDEF(PHG2Constraint);
	PHG2ConstraintType	type;	//拘束の種類
	UTRef<PHG2Frame> lhs, rhs;	//拘束対象

	//複製を作成
	virtual PHG2Constraint* dup(){return 0;}
	//拘束を解く
	virtual PHG2Result Solve(PHG2ConstraintList& newcon){return PHG2_ERROR;}

	PHG2Constraint(){}
	PHG2Constraint(PHG2ConstraintType _type, SGFrame* _lhs, SGFrame* _rhs){
		type = _type;
		lhs = new PHG2Frame(_lhs);
		rhs = new PHG2Frame(_rhs);
	}
	virtual ~PHG2Constraint(){}
};

//拘束のリスト
class PHG2ConstraintList : public std::list<UTRef<PHG2Constraint> >{
public:
	PHG2ConstraintList& operator=(const PHG2ConstraintList& src){
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
	PHG2ConstraintList(){}
	PHG2ConstraintList(const PHG2ConstraintList& src){*this = src;}
};
		
//本体
class PHGeometry2DEngine : public SGBehaviorEngine{
public:		
	SGOBJECTDEF(PHGeometry2DEngine);

	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
	int GetPriority() const {return SGBP_GEOMETRYENGINE;}
	virtual void Step(SGScene* s);
	virtual void Loaded(SGScene* scene);
	virtual void Clear(SGScene* s);
	virtual size_t NChildObjects(){ return frames.size(); }
	virtual SGObject* ChildObject(size_t i){ return (SGObject*)&*(frames[i]); }

	//拘束を追加・削除する
	PHG2Constraint*		Add(PHG2Constraint* con);
	void				Remove(PHG2Constraint*);

	//全クリア
	void Clear();

	//拘束を解決する
	void Solve();

	PHGeometry2DEngine(){}
	~PHGeometry2DEngine(){}
protected:
	PHG2FrameList		frames;
	PHG2ConstraintList	cons, cons_tmp;
	
	void		_Preprocess();
	void		_Postprocess();
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	色々な拘束

//Vector2とFloat型名を公開したくないので内部namespace
namespace PHG2{
	typedef Vec2f Vector2;
	typedef float Float;

	DEF_RECORD(XGeometry2DEngine, {
		GUID Guid(){ return WBGuid("a59cd5af-d032-421f-a3ce-24920fd84222"); }
		//子ノード：
		//Frame x 1．このFrameのxy平面が動作面となる．未指定ならグローバルフレーム
		//拘束 x N > 0．
	});

	DEF_RECORD(XPointToPoint2D, {
		GUID Guid(){ return WBGuid("e4fa6c65-eddb-473c-96e9-9300d63875b0"); }
		Vector2	p;
		Vector2 q;
	});

	DEF_RECORD(XPointToLine2D, {
		GUID Guid(){ return WBGuid("1b0bbd07-7d79-4f95-ae7a-660e8fe05ed5"); }
		Vector2	p;
		Vector2 q0;
		Vector2 q1;
	});

	DEF_RECORD(XPointToArc2D, {
		GUID Guid(){ return WBGuid("6669a0c5-6ad0-4f58-84e5-98673b2cd03d"); }
		Vector2	p;
		Vector2 c;
		Float	r;
		Float	s0;
		Float	s1;
	});

	DEF_RECORD(XLineToLine2D, {
		GUID Guid(){ return WBGuid("2e675bc7-c8a2-42bd-9d18-02e60e8f8b6a"); }
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
		Float	r;
		Float	s0;
		Float	s1;
	});

	DEF_RECORD(XArcToArc2D, {
		GUID Guid(){ return WBGuid("41bc8040-7536-4bf2-b66d-da3f5a8f2ffc"); }
		Vector2 c0;
		Vector2 c1;
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
		Float	theta;
	});
}
	
//点－点拘束
class PHG2PointToPoint : public PHG2Constraint, public PHG2::XPointToPoint2D{
public:
	PHG2PointToPoint(){}
	PHG2PointToPoint(
		SGFrame* _lhs, const Vec2f& _p,
		SGFrame* _rhs, const Vec2f& _q):
		PHG2Constraint(PHG2_POINT_TO_POINT, _lhs, _rhs)
		{p = _p; q = _q;}
	PHG2Constraint* dup(){return new PHG2PointToPoint(lhs->frame, p, rhs->frame, q);}
	PHG2Result Solve(PHG2ConstraintList&);
};

//点－線拘束
class PHG2PointToLine : public PHG2Constraint, public PHG2::XPointToLine2D{
public:
	PHG2PointToLine(){}
	PHG2PointToLine(
		SGFrame* _lhs, const Vec2f& _p,
		SGFrame* _rhs, const Vec2f& _q0, const Vec2f& _q1):
		PHG2Constraint(PHG2_POINT_TO_LINE, _lhs, _rhs)
		{p = _p; q0 = _q0; q1 = _q1;}
	PHG2Constraint* dup(){return new PHG2PointToLine(lhs->frame, p, rhs->frame, q0, q1);}
	PHG2Result Solve(PHG2ConstraintList&);
};

//点－円拘束
class PHG2PointToArc : public PHG2Constraint, public PHG2::XPointToArc2D{
public:
	PHG2PointToArc(){}
	PHG2PointToArc(
		SGFrame* _lhs, const Vec2f& _p,
		SGFrame* _rhs, const Vec2f& _c, float _r, float _s0, float _s1):
		PHG2Constraint(PHG2_POINT_TO_ARC, _lhs, _rhs)
		{p = _p; c = _c; r = _r; s0 = _s0; s1 = _s1;}
	PHG2Constraint* dup(){return new PHG2PointToArc(lhs->frame, p, rhs->frame, c, r, s0, s1);}
	PHG2Result Solve(PHG2ConstraintList&);
};

//線－線
class PHG2LineToLine : public PHG2Constraint, public PHG2::XLineToLine2D{
public:
	PHG2LineToLine(){}
	PHG2LineToLine(
		SGFrame* _lhs, const Vec2f& _p0, const Vec2f& _p1,
		SGFrame* _rhs, const Vec2f& _q0, const Vec2f& _q1):
		PHG2Constraint(PHG2_LINE_TO_LINE, _lhs, _rhs)
		{p0 = _p0; p1 = _p1; q0 = _q0; q1 = _q1;}
	PHG2Constraint* dup(){return new PHG2LineToLine(lhs->frame, p0, p1, rhs->frame, q0, q1);}
	PHG2Result Solve(PHG2ConstraintList&);
};

//線－円拘束
class PHG2LineToArc : public PHG2Constraint, public PHG2::XLineToArc2D{
public:
	PHG2LineToArc(){}
	PHG2LineToArc(
		SGFrame* _lhs, const Vec2f& _p0, const Vec2f& _p1,
		SGFrame* _rhs, const Vec2f& _c, float _r, float _s0, float _s1):
		PHG2Constraint(PHG2_LINE_TO_ARC, _lhs, _rhs)
		{p0 = _p0; p1 = _p1; c = _c; r = _r; s0 = _s0; s1 = _s1;}
	PHG2Constraint* dup(){return new PHG2LineToArc(lhs->frame, p0, p1, rhs->frame, c, r, s0, s1);}
	PHG2Result Solve(PHG2ConstraintList&);
};

//円－円拘束
class PHG2ArcToArc : public PHG2Constraint, public PHG2::XArcToArc2D{
public:
	PHG2ArcToArc(){}
	PHG2ArcToArc(
		SGFrame* _lhs, const Vec2d& _c0, double _r0, double _s00, double _s01,
		SGFrame* _rhs, const Vec2d& _c1, double _r1, double _s10, double _s11):
		PHG2Constraint(PHG2_ARC_TO_ARC, _lhs, _rhs){
		c0 = _c0; r0 = _r0; s00 = _s00; s01 = _s01;
		c1 = _c1; r1 = _r1; s10 = _s10; s11 = _s11;
	}
	PHG2Constraint* dup(){return new PHG2ArcToArc(lhs->frame, c0, r0, s00, s01, rhs->frame, c1, r1, s10, s11);}
	PHG2Result Solve(PHG2ConstraintList&);
};

//平行拘束
class PHG2Parallel : public PHG2Constraint, public PHG2::XParallel2D{
public:
	PHG2Parallel(){}
	PHG2Parallel(
		SGFrame* _lhs, double _theta0,
		SGFrame* _rhs, double _theta1):
		PHG2Constraint(PHG2_PARALLEL, _lhs, _rhs)
		{theta0 = _theta0; theta1 = _theta1;}
	PHG2Constraint* dup(){return new PHG2Parallel(lhs->frame, theta0, rhs->frame, theta1);}
	PHG2Result Solve(PHG2ConstraintList&);
};

//角度拘束
class PHG2Angle : public PHG2Constraint, public PHG2::XAngle2D{
public:
	PHG2Angle(){}
	PHG2Angle(
		SGFrame* _lhs, double _theta0,
		SGFrame* _rhs, double _theta1,
		double _phi):
		PHG2Constraint(PHG2_ANGLE, _lhs, _rhs)
		{theta0 = _theta0; theta1 = _theta1; phi = _phi;}
	virtual ~PHG2Angle(){}
	PHG2Constraint* dup(){return new PHG2Angle(lhs->frame, theta0, rhs->frame, theta1, phi);}
	PHG2Result Solve(PHG2ConstraintList&);
};

//固定拘束
class PHG2Fix : public PHG2Constraint, public PHG2::XFix2D{
public:
	PHG2Fix(){}
	PHG2Fix(
		SGFrame* _lhs,
		SGFrame* _rhs, const Vec2d& _p, double _theta):
		PHG2Constraint(PHG2_FIX, _lhs, _rhs)
		{p = _p; theta = _theta;}
	virtual ~PHG2Fix(){}
	PHG2Constraint* dup(){return new PHG2Fix(lhs->frame, rhs->frame, p, theta);}
	PHG2Result Solve(PHG2ConstraintList&);
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
//２次元幾何関数

///ある点まわりの回転
inline void RotationAlong(Vec2d* position, double* angle, const Vec2d& center, double rot){
	*angle += rot;
	*position = Matrix2d::Rot(rot) * (*position - center) + center;
}

//ある点に最も近い直線上の点
inline void NearestPoint(
	Vec2d* y,
	const Vec2d& p,
	const Vec2d& q, const Vec2d& u)
{
	*y = q + PTM::dot(u, p - q) / u.square() * u;
}

//２直線の角度
inline double Line_Line_Angle(
	const Vec2d& u0,
	const Vec2d& u1)
{
	return atan2(PTM::cross(u0, u1), PTM::dot(u0, u1));
}

//２直線の交点
inline bool Line_Line_Intersect(
	Vec2d* y,
	const Vec2d& p0, const Vec2d& u0,
	const Vec2d& p1, const Vec2d& u1)
{
	//連立方程式
	Matrix2d A(u0, -u1);
	if(fabs(A.det()) < 0.0)return false;
	Vec2d s = A.inv() * (-p0 + p1);
	
	*y = p0 + s[0] * u0;
	return true;
}

//円と直線の交点
//交差しない場合は互いに最も近い点を返す
inline bool Line_Circle_Intersect(
	Vec2d* y0, Vec2d* y1,
	const Vec2d& p, const Vec2d& u,
	const Vec2d& c, double r)
{
	//二次方程式の係数
	double A = u.square();
	double B = PTM::dot(u, (p - c));
	double C = (p - c).square() - r * r;
	//判別式
	double det = B * B - A * C;
	if(det < 0){
		//交差しないので最近点を求める
		NearestPoint(y0, c, p, u);			//直線上の最近点
		Vec2d c_y0 = *y0 - c;
		*y1 = c + r / c_y0.norm() * c_y0;	//円上の最近点
		return false;
	}
	//方程式の解と交点
	double det_root = sqrt(det);
	double s0 = (-B + det_root) / A, s1 = (-B - det_root) / A;
	*y0 = p + s0 * u, *y1 = p + s1 * u;
	return true;
}

//２円の交点
//交差しない場合は互いに最も近い点を返す
inline bool Circle_Circle_Intersect(
	Vec2d* y0, Vec2d* y1,
	const Vec2d& c0, double r0,
	const Vec2d& c1, double r1)
{
	//中心間をつなぐベクトル
	Vec2d d = c1 - c0;
	double d_norm = d.norm();
	//円の半径
	//円同士の傾きと対応する回転行列
	double phi = atan2(d.Y(), d.X());
	Matrix2d rot = Matrix2d::Rot(phi);
	//判別式
	double det = 
		( r0 + r1 + d_norm) * 
		( r0 + r1 - d_norm) * 
		( r0 - r1 + d_norm) * 
		(-r0 + r1 + d_norm);
	if(det < 0){
		//交差しないので最近点を求める
		*y0 = c0 + r0 / d_norm * d;
		*y1 = c1 - r1 / d_norm * d;
		return false;
	}
	//c0を原点、dをx軸とするフレームでの交点座標
	double h = sqrt(det) / (2 * d_norm);
	double w = sqrt(r0 * r0 - h * h);
	//交点
	*y0 = rot * Vec2d(w,  h) + c0;
	*y1 = rot * Vec2d(w, -h) + c0;
	return true;
}

//２円の共通接線
//円c0上の接点をp0, p1, p2, p3に、c1上の接点をq0, q1, q2, q3に
//接線の本数を返す(0, 2, 4のいずれかを返す。1, 3本は２円が接している状態で
//起こるが
inline int Circle_Circle_Tangent(
	const Vec2d& c0, double r0,
	const Vec2d& c1, double r1,
	Vec2d* p0, Vec2d* p1, Vec2d* p2, Vec2d* p3,
	Vec2d* q0, Vec2d* q1, Vec2d* q2, Vec2d* q3)
{
	Vec2d d = c1 - c0;
	double d_norm = d.norm();
	double phi = atan2(d.Y(), d.X());
	//２円が中心を共有する場合
	if(d_norm < PHG2Epsilon)return 0;
	//判別式
	double det1 = (r0 - r1) / d_norm;
	double det2 = (r0 + r1) / d_norm;
	int nline;
	double theta;
	double _cos, _sin;
	//c0を原点として、dをｘ軸とする座標で計算を進める：
	//片方の円がもう一方を内包する場合
	if(fabs(det1) > 1.0 + PHG2Epsilon)
		return 0;
	//内接する場合
	if(fabs(det1) > 1.0 - PHG2Epsilon){
		//どっちが外側か
		*p0 = *q0 = Vec2d(r0 > r1 ? r0 : -r0, 0);
		nline = 1;
	}
	//２点で交差する場合
	else if(det2 > 1.0 + PHG2Epsilon){
		theta = acos(det1);
		_cos = cos(theta), _sin = sin(theta);
		*p0 = Vec2d(r0 * _cos,  r0 * _sin);
		*p1 = Vec2d(r0 * _cos, -r0 * _sin);
		*q0 = *p0 + d_norm * _sin * Vec2d( _sin, _cos);
		*q1 = *p1 - d_norm * _sin * Vec2d(-_sin, _cos);
		nline = 2;
	}
	//外接する場合
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
	//離れている場合
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
	//最後に座標変換
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

//pがq0, q1の間にあるかを判定
inline bool Point_On_LineSegment(
	const Vec2d& p,
	const Vec2d& q0, const Vec2d& q1)
{
	Vec2d u = p - q0, v = p - q1;
	return 
		fabs(PTM::cross(u, v)) < u.norm() * v.norm() * PHG2Epsilon &&
		PTM::dot(u, v) < PHG2Epsilon;
}

//pがc, rで表される円上にあるものとして、pが角度範囲[s0, s1]内にあるかを判定
//ただしs0, s1はｘ軸を基準に反時計回り方向
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