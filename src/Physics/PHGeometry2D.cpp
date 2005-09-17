#include "PHGeometry2D.h"
#include <algorithm>

using namespace std;
using namespace Spr;
using namespace PHG2;

double PHG2_Epsilon = 1.0e-3f;
const double PHG2_PI = 3.14159265358979f;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// PHGeometry2DEngine

bool PHGeometry2DEngine::AddChildObject(SGObject* o, SGScene* s){
	PHG2Constraint* con = DCAST(PHG2Constraint, o);
	if(con){
		Add(con);
		return true;
	}
	return false;
}

bool PHGeometry2DEngine::DelChildObject(SGObject* o, SGScene* s){
	PHG2Constraint* con = DCAST(PHG2Constraint, o);
	if(con){
		Remove(con);
		return true;
	}
	return false;
}

void PHGeometry2DEngine::Loaded(SGScene* scene){

}

//全クリア
void PHGeometry2DEngine::Clear(SGScene* s){
	frames.clear();
	cons.clear();
}

//拘束を登録
PHG2Constraint* PHGeometry2DEngine::Add(PHG2Constraint* con){
	cons.push_back(con);
	return con;
}

//拘束を削除
void PHGeometry2DEngine::Remove(PHG2Constraint* con){
	cons.erase(find(cons.begin(), cons.end(), con));
}

//Solveの前処理
void PHGeometry2DEngine::_Preprocess(){
	cons_tmp = cons;
}

//Solveの後処理
void PHGeometry2DEngine::_Postprocess(){
	//自由度をリセット
	PHG2FrameList::iterator it;
	for(it = frames.begin(); it != frames.end(); it++)
		(*it)->dof = PHG2_T2R1;
}

//拘束を解決
void PHGeometry2DEngine::Step(SGScene* s){
 	_Preprocess();

	//計算本体：
	bool bChanged;
	PHG2ConstraintList	newcon;
	PHG2Result result;
	PHG2ConstraintList::iterator itcon, itcon_tmp;

	//変化が生じなくなるまで繰り返す
	do{
		bChanged = false;
		for(itcon = cons_tmp.begin(); itcon != cons_tmp.end();){
			//拘束を評価
			result = (*itcon)->Solve(newcon);

			//変化したか
			if(result != PHG2_REDUNDANT)
				bChanged = true;
			
			//新しい拘束
			switch(result){
			case PHG2_NEW:
				itcon_tmp = itcon;
				cons_tmp.insert(++itcon_tmp, newcon.begin(), newcon.end());
				newcon.clear();
				break;
			}

			//評価した拘束をどうするか
			switch(result){
			//もう一度評価
			case PHG2_AGAIN: break;
			//後回し
			case PHG2_REDUNDANT: itcon++; break;
			//消去
			default: itcon = cons_tmp.erase(itcon);
			}
		}
	}while(bChanged);
	
	_Postprocess();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// PHG2Constraint

SGOBJECTIMP(PHG2Constraint, SGObject);

PHG2Result PHG2PointToPoint::Solve(PHG2ConstraintList& newcon){
	Vec2d pw = lhs ? lhs->toWorld(p) : p;
	Vec2d qw = rhs ? rhs->toWorld(q) : q;
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	switch(dof_lhs){
	case PHG2_T0R0:
		switch(dof_rhs){
		case PHG2_T0R0:
			return (pw - qw).norm() < PHG2_Epsilon ? PHG2_SOLVED : PHG2_OVER;
		case PHG2_T0R1:{
			//rhsを拘束中心まわりに回転して拘束を満たす
			Vec2d c = rhs->center;
			Vec2d r0 = pw - c, r1 = qw - c;
			//拘束中心と拘束点のいずれかが一致していてはならない
			if( r0.square() > 0 && r1.square() > 0){
				double phi = Line_Line_Angle(r1, r0);
				RotationAlong(&(rhs->position), &(rhs->angle), c, phi);
				rhs->dof = PHG2_T0R0;
				return PHG2_SOLVED;
			}
			else return PHG2_ILLPOSED;
		}
		case PHG2_T1R0:{
			//拘束誤差が最小になる位置までrhsを拘束直線に沿って平行移動させる
			Vec2d y, pos;
			NearestPoint(&y, pw, qw, rhs->range1 - rhs->range0);
			//新しい位置が拘束線分上にある場合はそこに移動
			pos = rhs->position + (y - qw);
			if(Point_On_LineSegment(pos, rhs->range0, rhs->range1)){
				rhs->position = pos;
				rhs->dof = PHG2_T0R0;
				return PHG2_SOLVED;
			}
			return PHG2_ILLPOSED;
		}
		case PHG2_T2R0:
		case PHG2_T2R1:
			//rhsを拘束を満たす位置に平行移動
			rhs->position += pw - qw;
			if(rhs->dof == PHG2_T2R1){
				rhs->center = pw;
				rhs->dof = PHG2_T0R1;
			}
			else rhs->dof = PHG2_T0R0;
			return PHG2_SOLVED;
		}
	case PHG2_T0R1:
		switch(dof_rhs){
		case PHG2_T0R0: swap(lhs, rhs); swap(p, q); return PHG2_AGAIN;
		case PHG2_T0R1:{
			//２円の交点を求める:
			Vec2d y0, y1;
			if(!Circle_Circle_Intersect(
				&y0, &y1,
				lhs->center, (lhs->center - pw).norm(),
				rhs->center, (rhs->center - qw).norm()))
				return PHG2_ILLPOSED;
			double d0 = (y0 - pw).square() + (y0 - qw).square();
			double d1 = (y1 - pw).square() + (y1 - qw).square();
			if(d0 > d1)y0 = y1;
			newcon.push_back(new PHG2PointToPoint(0, y0, lhs->frame, p));
			newcon.push_back(new PHG2PointToPoint(0, y0, rhs->frame, q));
			return PHG2_NEW;
		}
		case PHG2_T1R0:{
			//円と直線の交点を求める：
			Vec2d y0, y1;
			if(!Line_Circle_Intersect(
				&y0, &y1,
				qw, rhs->range1 - rhs->range0,
				lhs->center, (lhs->center - pw).norm()))
				return PHG2_ILLPOSED;
			double d0 = (y0 - pw).square() + (y0 - qw).square();
			double d1 = (y1 - pw).square() + (y1 - qw).square();
			if(d0 > d1)y0 = y1;
			newcon.push_back(new PHG2PointToPoint(0, y0, lhs->frame, p));
			newcon.push_back(new PHG2PointToPoint(0, y0, rhs->frame, q));
			return PHG2_NEW;
		}
		case PHG2_T2R0:
		case PHG2_T2R1:
			return PHG2_REDUNDANT;
		}
	case PHG2_T1R0:
		switch(dof_rhs){
		case PHG2_T0R0:
		case PHG2_T0R1: 
			swap(lhs, rhs); swap(p, q); return PHG2_AGAIN;
		case PHG2_T1R0:{
			//二直線の交点を求める：
			Vec2d y;
			if(!Line_Line_Intersect(
				&y,
				pw, lhs->range1 - lhs->range0,
				qw, rhs->range1 - rhs->range0))
				return PHG2_ILLPOSED;
			//交点が両者の可動範囲内になければならない
			newcon.push_back(new PHG2PointToPoint(0, y, lhs->frame, p));
			newcon.push_back(new PHG2PointToPoint(0, y, rhs->frame, q));
			return PHG2_NEW;
		}
		default: return PHG2_REDUNDANT;
		}
	case PHG2_T2R0:
		switch(dof_rhs){
		case PHG2_T0R0: swap(lhs, rhs); swap(p, q); return PHG2_AGAIN;
		default: return PHG2_REDUNDANT;
		}
	case PHG2_T2R1:
		switch(dof_rhs){
		case PHG2_T0R0: swap(lhs, rhs); swap(p, q); return PHG2_AGAIN;
		default: return PHG2_REDUNDANT;
		}
	}
	return PHG2_ERROR;
}

PHG2Result PHG2PointToLine::Solve(PHG2ConstraintList& newcon){
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	Vec2d pw  = lhs ? lhs->toWorld(p) : p;
	Vec2d qw0 = rhs ? rhs->toWorld(q0) : q0;
	Vec2d qw1 = rhs ? rhs->toWorld(q1) : q1;
	//両方ともT0R0の場合
	if(dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R0)
		return Point_On_LineSegment(pw, qw0, qw1) ? PHG2_SOLVED : PHG2_OVER;
	//片方がT0R0でもう一方がT0R1の場合
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R1) ||
		(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R0)){
		//円と直線の交点:
		Vec2d y0, y1, c = (dof_lhs == PHG2_T0R1) ? lhs->center : rhs->center;
		if(!Line_Circle_Intersect(
			&y0, &y1,
			qw0, qw1 - qw0,
			c, (pw - c).norm()))
			return PHG2_ILLPOSED;
		bool y0_online = Point_On_LineSegment(y0, qw0, qw1);
		bool y1_online = Point_On_LineSegment(y1, qw0, qw1);
		if((y0 - pw).square() > (y1 - pw).square())
			y0 = y1;
		if(y0_online){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHG2PointToPoint(0, y0, lhs->frame, p) : new PHG2PointToPoint(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_online){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHG2PointToPoint(0, y1, lhs->frame, p) : new PHG2PointToPoint(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	//片方がT0R0でもう一方がT1R0の場合
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T1R0) ||
		(dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R0)){
		//二直線の交点を求める：
		Vec2d y, dir = (dof_lhs == PHG2_T1R0) ? (lhs->range1 - lhs->range0) : (rhs->range1 - rhs->range0);
		if(!Line_Line_Intersect(&y, pw, dir, qw0, qw1 - qw0))
			return PHG2_ILLPOSED;
		//yが線分上になければならない
		if(!Point_On_LineSegment(y, qw0, qw1))
			return PHG2_ILLPOSED;
		if(dof_lhs == PHG2_T1R0)
			 newcon.push_back(new PHG2PointToPoint(0, y, lhs->frame, p));
		else newcon.push_back(new PHG2PointToPoint(0, pw, rhs->frame, rhs->toLocal(y)));
		return PHG2_NEW;
	}
	//片方がT0R0でもう一方がT2R0の場合
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T2R0) ||
		(dof_lhs == PHG2_T2R0 && dof_rhs == PHG2_T0R0)){
		//点が直線に乗るように平行移動
		PHG2Frame* obj = (dof_lhs == PHG2_T2R0) ? lhs : rhs;
		obj->position += (dof_lhs == PHG2_T2R0) ? (qw0 - pw) : (pw - qw0);
		obj->dof = PHG2_T1R0;
		obj->range0 = obj->position;
		obj->range1 = obj->position + (qw1 - qw0);
		return PHG2_SOLVED;
	}
	return PHG2_REDUNDANT;
}

PHG2Result Spr::PHG2PointToArc::Solve(PHG2ConstraintList& newcon){
	//簡単のためいずれかがT0R0で、かつもう一方がT0R1かT1R0の場合のみ解く
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	if(dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R0)return PHG2_OVER;
	
	Vec2d pw = lhs ? lhs->toWorld(p) : p;
	Vec2d cw = rhs ? rhs->toWorld(c) : c;
	double sw0 = rhs ? (s0 + rhs->angle) : s0;
	double sw1 = rhs ? (s1 + rhs->angle) : s1;
	Vec2d y0, y1, y;

	if((dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R1) || (dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R0)){
		//２円の交点
		Vec2d c = (dof_lhs == PHG2_T0R1 ? lhs->center : rhs->center);
		if(!Circle_Circle_Intersect(
			&y0, &y1,
			c, (pw - c).norm(),
			cw, r))
			return PHG2_ILLPOSED;
		bool y0_onarc = Point_On_Arc(y0, cw, r, sw0, sw1);
		bool y1_onarc = Point_On_Arc(y1, cw, r, sw0, sw1);
		if((y0 - pw).square() > (y1 - pw).square())
			y0 = y1;
		if(y0_onarc){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHG2PointToPoint(0, y0, lhs->frame, p) : new PHG2PointToPoint(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_onarc){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHG2PointToPoint(0, y1, lhs->frame, p) : new PHG2PointToPoint(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	if((dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T1R0) || (dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R0)){
		//円と直線との交点
		Vec2d dir = (dof_lhs == PHG2_T1R0 ? lhs->range1 - lhs->range0 : rhs->range1 - rhs->range0);
		Vec2d y0, y1;
		if(!Line_Circle_Intersect(
			&y0, &y1,
			pw, dir,
			cw, r))
			return PHG2_ILLPOSED;
		bool y0_onarc = Point_On_Arc(y0, cw, r, sw0, sw1);
		bool y1_onarc = Point_On_Arc(y1, cw, r, sw0, sw1);
		if((y0 - pw).square() > (y1 - pw).square())
			y0 = y1;
		if(y0_onarc){
			newcon.push_back(dof_lhs == PHG2_T1R0 ? new PHG2PointToPoint(0, y0, lhs->frame, p) : new PHG2PointToPoint(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_onarc){
			newcon.push_back(dof_lhs == PHG2_T1R0 ? new PHG2PointToPoint(0, y1, lhs->frame, p) : new PHG2PointToPoint(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	return PHG2_REDUNDANT;
}

PHG2Result PHG2LineToLine::Solve(PHG2ConstraintList& newcon){
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	Vec2d pw0 = lhs ? lhs->toWorld(p0) : p0;
	Vec2d pw1 = lhs ? lhs->toWorld(p1) : p1;
	Vec2d qw0 = rhs ? rhs->toWorld(q0) : q0;
	Vec2d qw1 = rhs ? rhs->toWorld(q1) : q1;
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T2R1) ||
		(dof_lhs == PHG2_T2R1 && dof_rhs == PHG2_T0R0)){
		//未実装
		return PHG2_REDUNDANT;
	}
	if( (dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R1) ||
		(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T1R0)){
		//未実装
		return PHG2_REDUNDANT;
	}
	if(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R1){
		//２円の接線を求める：
		Vec2d u, v;		//線分上の拘束点に対する最近点
		Vec2d a0, a1, a2, a3, b0, b1, b2, b3;	//２円の接点
		NearestPoint(&u, lhs->center, pw0, pw1 - pw0);
		NearestPoint(&v, rhs->center, qw0, qw1 - qw0);
		int nline = Circle_Circle_Tangent(
			lhs->center, (u - lhs->center).norm(),
			rhs->center, (v - rhs->center).norm(),
			&a0, &a1, &a2, &a3,
			&b0, &b1, &b2, &b3);
		if(nline == 0)
			return PHG2_ILLPOSED;
		//接線の内最も近いものを選んで接点へのPTP拘束に変換
		double d0, d1;
		Vec2d a, b;
		a = a0, b = b0;
		d0 = (u - a).square() + (v - b).square();
		if(nline >= 2){
			d1 = (u - a1).square() + (v - b1).square();
			if(d0 > d1)a = a1, b = b1, d0 = d1;
		}
		if(nline >= 3){
			d1 = (u - a2).square() + (v - b2).square();
			if(d0 > d1)a = a2, b = b2, d0 = d1;
		}
		if(nline == 4){
			d1 = (u - a3).square() + (v - b3).square();
			if(d0 > d1)a = a3, b = b3;
		}
		newcon.push_back(new PHG2PointToPoint(0, a, lhs->frame, lhs->toLocal(u)));
		newcon.push_back(new PHG2PointToPoint(0, b, rhs->frame, rhs->toLocal(v)));
		return PHG2_NEW;
	}
	
	return PHG2_REDUNDANT;
}

PHG2Result PHG2LineToArc::Solve(PHG2ConstraintList& newcon){
	return PHG2_REDUNDANT;
}

PHG2Result PHG2ArcToArc::Solve(PHG2ConstraintList& newcon){
	return PHG2_REDUNDANT;
}

PHG2Result PHG2Parallel::Solve(PHG2ConstraintList& newcon){
	//lhs, rhs共に回転自由度を持つ場合は冗長
	//lhs, rhs共に回転自由度を持たない場合は過剰
	//いずれかのみ回転自由度を持つ場合はそちらを回転して拘束を解決
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	bool revolutive_lhs = (dof_lhs == PHG2_T0R1 || dof_lhs == PHG2_T2R1);
	bool revolutive_rhs = (dof_rhs == PHG2_T0R1 || dof_rhs == PHG2_T2R1);
	if( revolutive_lhs &&  revolutive_rhs)return PHG2_REDUNDANT;
	if(!revolutive_lhs && !revolutive_rhs)return PHG2_OVER;
	if( revolutive_lhs && !revolutive_rhs){
		swap(lhs, rhs);
		swap(theta0, theta1);
		return PHG2_AGAIN;
	}
	double thetaw0 = lhs ? theta0 + lhs->angle : theta0;
	double thetaw1 = rhs ? theta1 + rhs->angle : theta1;
	
	//角度を求める
	double diff = thetaw1 - thetaw0;
	if(diff >  PHG2_PI / 2.0)diff -= PHG2_PI;
	if(diff < -PHG2_PI / 2.0)diff += PHG2_PI;
	switch(dof_rhs){
	case PHG2_T0R1:
		RotationAlong(&(rhs->position), &(rhs->angle), rhs->center, -diff);
		rhs->dof = PHG2_T0R0;
		return PHG2_SOLVED;
	case PHG2_T2R1:
		RotationAlong(&(rhs->position), &(rhs->angle), rhs->position, -diff);
		rhs->dof = PHG2_T2R0;
		return PHG2_SOLVED;
	}
	
	return PHG2_ERROR;
}

PHG2Result PHG2Angle::Solve(PHG2ConstraintList& newcon){
	//lhs, rhs共に回転自由度を持つ場合は冗長
	//lhs, rhs共に回転自由度を持たない場合は過剰
	//いずれかのみ回転自由度を持つ場合はそちらを回転して拘束を解決
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	bool revolutive_lhs = (dof_lhs == PHG2_T0R1 || dof_lhs == PHG2_T2R1);
	bool revolutive_rhs = (dof_rhs == PHG2_T0R1 || dof_rhs == PHG2_T2R1);
	if( revolutive_lhs &&  revolutive_rhs)return PHG2_REDUNDANT;
	if(!revolutive_lhs && !revolutive_rhs)return PHG2_OVER;
	if( revolutive_lhs && !revolutive_rhs){
		swap(lhs, rhs), swap(theta0, theta1), phi = -phi;
		return PHG2_AGAIN;
	}
	double thetaw0 = lhs ? theta0 + lhs->angle : theta0;
	double thetaw1 = rhs ? theta1 + rhs->angle : theta1;
	
	//角度を求める
	double diff = phi - (thetaw1 - thetaw0);
	switch(dof_rhs){
	case PHG2_T0R1:
		RotationAlong(&(rhs->position), &(rhs->angle), rhs->center, diff);
		rhs->dof = PHG2_T0R0;
		return PHG2_SOLVED;
	case PHG2_T2R1:
		RotationAlong(&(rhs->position), &(rhs->angle), rhs->position, diff);
		rhs->dof = PHG2_T2R0;
		return PHG2_SOLVED;
	}
	
	return PHG2_ERROR;
}

PHG2Result PHG2Fix::Solve(PHG2ConstraintList& newcon){
	//いずれかがT2R1で他方がT0R0の場合のみ扱う
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	if(dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T2R1){
		rhs->position	= lhs ? lhs->toWorld(p) : p;
		rhs->angle		= lhs ? lhs->angle + theta : theta;
		rhs->dof = PHG2_T0R0;
		return PHG2_SOLVED;
	}
	else if(dof_lhs == PHG2_T2R1 && dof_rhs == PHG2_T0R0){
		swap(lhs, rhs);
		p = -Matrix2d::Rot(-theta) * p;
		theta = -theta;
		return PHG2_AGAIN;
	}
	else return PHG2_REDUNDANT;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Loader & Saver

class PHGeometry2DEngineLoader:public FIObjectLoader<PHGeometry2DEngine>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHGeometry2DEngine* engine){
		return true;
	}
	PHGeometry2DEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_RECORD_PROTO(XGeometry2DEngine);
	}
};

#define DEF_LOADER(NAME)											\
class PH##NAME##2DLoader : public FIObjectLoader<PHG2##NAME>{		\
public:	typedef X##NAME##2D template_type;							\
	virtual bool LoadData(FILoadScene* ctx, PHG2PointToPoint *con){	\
	FIDocNodeBase* doc = ctx->docs.Top();							\
	doc->GetWholeData(*(template_type*)con);						\
	return true;													\
	}																\
	PH##NAME##2DLoader(){											\
		UTRef<FITypeDescDb> db = new FITypeDescDb;					\
		db->SetPrefix("X");											\
		db->REG_FIELD(Float);										\
		db->REG_FIELD(Vector2);										\
		db->REG_RECORD_PROTO(X##NAME##2D);							\
	}																\
};	

DEF_LOADER(PointToPoint)
DEF_LOADER(PointToLine)
DEF_LOADER(PointToArc)
DEF_LOADER(LineToLine)
DEF_LOADER(LineToArc)
DEF_LOADER(ArcToArc)
DEF_LOADER(Parallel)
DEF_LOADER(Angle)
DEF_LOADER(Fix)

#undef DEF_LOADER