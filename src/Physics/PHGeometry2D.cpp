#include "PHGeometry2D.h"
#include <algorithm>

namespace Spr{;

using namespace std;
using namespace PHG2;

double PHG2_Epsilon = 1.0e-3f;
const double PHG2_PI = 3.14159265358979f;

SGOBJECTIMP(PHGeometry2DEngine, SGBehaviorEngine);
SGOBJECTIMPABST(PHConstraint2D, SGObject);
SGOBJECTIMP(PHPointToPoint2D, PHConstraint2D);
SGOBJECTIMP(PHPointToLine2D, PHConstraint2D);
SGOBJECTIMP(PHPointToArc2D, PHConstraint2D);
SGOBJECTIMP(PHLineToLine2D, PHConstraint2D);
SGOBJECTIMP(PHLineToArc2D, PHConstraint2D);
SGOBJECTIMP(PHArcToArc2D, PHConstraint2D);
SGOBJECTIMP(PHParallel2D, PHConstraint2D);
SGOBJECTIMP(PHAngle2D, PHConstraint2D);
SGOBJECTIMP(PHFix2D, PHConstraint2D);

/////////////////////////////////////////////////////////////////////////////////////////////////////
// PHGeometry2DEngine

bool PHGeometry2DEngine::AddChildObject(SGObject* o, SGScene* s){
	PHConstraint2D* con = DCAST(PHConstraint2D, o);
	if(con){
		Add(con);
		return true;
	}
	plane = DCAST(SGFrame, o);	//���약�ʂƂȂ�t���[��
	return false;
}

bool PHGeometry2DEngine::DelChildObject(SGObject* o, SGScene* s){
	PHConstraint2D* con = DCAST(PHConstraint2D, o);
	if(con){
		Remove(con);
		return true;
	}
	if(plane == DCAST(SGFrame, o))
		plane = 0;
	return false;
}

void PHGeometry2DEngine::Loaded(SGScene* scene){
	//�S�����X�g�̊e�v�f���Q�Ƃ��Ă���t���[���̃��X�g���쐬
	PHConstraintList2D::iterator icon;
	for(icon = cons.begin(); icon != cons.end(); icon++){
		if(find(frames.begin(), frames.end(), (*icon)->lhs) == frames.end())
			frames.push_back((*icon)->lhs);
		if(find(frames.begin(), frames.end(), (*icon)->rhs) == frames.end())
			frames.push_back((*icon)->rhs);
	}
}

//�S�N���A
void PHGeometry2DEngine::Clear(SGScene* s){
	frames.clear();
	cons.clear();
}

//�S����o�^
PHConstraint2D* PHGeometry2DEngine::Add(PHConstraint2D* con){
	cons.push_back(con);
	return con;
}

//�S�����폜
void PHGeometry2DEngine::Remove(PHConstraint2D* con){
	cons.erase(find(cons.begin(), cons.end(), con));
}

//Solve�̑O����
void PHGeometry2DEngine::_Preprocess(){
	cons_tmp = cons;
}

//Solve�̌㏈��
void PHGeometry2DEngine::_Postprocess(){
	//�ʒu�C�X����SGFrame�ɔ��f���āC���R�x�����Z�b�g
	//���약�ʃt���[����posture
	Affinef afPlane = plane->GetWorldPosture();

	PHG2FrameList::iterator it;
	PHG2Frame* fr;
	for(it = frames.begin(); it != frames.end(); it++){
		fr = *it;
		fr->frame->SetWorldPosture(
			afPlane * Affinef::Trn(fr->position.x, fr->position.y, 0.0f) * Affinef::Rot(fr->angle, 'z'));
		fr->dof = PHG2_T2R1;
	}
}

//�S��������
void PHGeometry2DEngine::Step(SGScene* s){
 	_Preprocess();

	//�v�Z�{�́F
	bool bChanged;
	PHConstraintList2D	newcon;
	PHG2Result result;
	PHConstraintList2D::iterator itcon, itcon_tmp;

	//�ω��������Ȃ��Ȃ�܂ŌJ��Ԃ�
	do{
		bChanged = false;
		for(itcon = cons_tmp.begin(); itcon != cons_tmp.end();){
			//�S����]��
			result = (*itcon)->Solve(newcon);

			//�ω�������
			if(result != PHG2_REDUNDANT)
				bChanged = true;
			
			//�V�����S��
			switch(result){
			case PHG2_NEW:
				itcon_tmp = itcon;
				cons_tmp.insert(++itcon_tmp, newcon.begin(), newcon.end());
				newcon.clear();
				break;
			}

			//�]�������S�����ǂ����邩
			switch(result){
			//������x�]��
			case PHG2_AGAIN: break;
			//���
			case PHG2_REDUNDANT: itcon++; break;
			//����
			default: itcon = cons_tmp.erase(itcon);
			}
		}
	}while(bChanged);
	
	_Postprocess();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// PHConstraint2D

bool PHConstraint2D::AddChildObject(SGObject* o, SGScene* s){
	SGFrame* fr = DCAST(SGFrame, o);
	if(fr){
		if(!lhs){
			lhs = new PHG2Frame(fr);
			return true;
		}
		if(!rhs){
			rhs = new PHG2Frame(fr);
			return true;
		}
	}
	return false;
}

bool PHConstraint2D::DelChildObject(SGObject* o, SGScene* s){
	SGFrame* fr = DCAST(SGFrame, o);
	if(fr){
		if(fr == lhs->frame){
			lhs = 0;
			return true;
		}
		if(fr == rhs->frame){
			rhs = 0;
			return true;
		}
	}
	return false;
}

PHG2Result PHPointToPoint2D::Solve(PHConstraintList2D& newcon){
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
			//rhs���S�����S�܂��ɉ�]���čS���𖞂���
			Vec2d c = rhs->center;
			Vec2d r0 = pw - c, r1 = qw - c;
			//�S�����S�ƍS���_�̂����ꂩ����v���Ă��Ă͂Ȃ�Ȃ�
			if( r0.square() > 0 && r1.square() > 0){
				double phi = Line_Line_Angle(r1, r0);
				RotationAlong(&(rhs->position), &(rhs->angle), c, phi);
				rhs->dof = PHG2_T0R0;
				return PHG2_SOLVED;
			}
			else return PHG2_ILLPOSED;
		}
		case PHG2_T1R0:{
			//�S���덷���ŏ��ɂȂ�ʒu�܂�rhs���S�������ɉ����ĕ��s�ړ�������
			Vec2d y, pos;
			NearestPoint(&y, pw, qw, rhs->range1 - rhs->range0);
			//�V�����ʒu���S��������ɂ���ꍇ�͂����Ɉړ�
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
			//rhs���S���𖞂����ʒu�ɕ��s�ړ�
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
			//�Q�~�̌�_�����߂�:
			Vec2d y0, y1;
			if(!Circle_Circle_Intersect(
				&y0, &y1,
				lhs->center, (lhs->center - pw).norm(),
				rhs->center, (rhs->center - qw).norm()))
				return PHG2_ILLPOSED;
			double d0 = (y0 - pw).square() + (y0 - qw).square();
			double d1 = (y1 - pw).square() + (y1 - qw).square();
			if(d0 > d1)y0 = y1;
			newcon.push_back(new PHPointToPoint2D(0, y0, lhs->frame, p));
			newcon.push_back(new PHPointToPoint2D(0, y0, rhs->frame, q));
			return PHG2_NEW;
		}
		case PHG2_T1R0:{
			//�~�ƒ����̌�_�����߂�F
			Vec2d y0, y1;
			if(!Line_Circle_Intersect(
				&y0, &y1,
				qw, rhs->range1 - rhs->range0,
				lhs->center, (lhs->center - pw).norm()))
				return PHG2_ILLPOSED;
			double d0 = (y0 - pw).square() + (y0 - qw).square();
			double d1 = (y1 - pw).square() + (y1 - qw).square();
			if(d0 > d1)y0 = y1;
			newcon.push_back(new PHPointToPoint2D(0, y0, lhs->frame, p));
			newcon.push_back(new PHPointToPoint2D(0, y0, rhs->frame, q));
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
			//�񒼐��̌�_�����߂�F
			Vec2d y;
			if(!Line_Line_Intersect(
				&y,
				pw, lhs->range1 - lhs->range0,
				qw, rhs->range1 - rhs->range0))
				return PHG2_ILLPOSED;
			//��_�����҂̉��͈͓��ɂȂ���΂Ȃ�Ȃ�
			newcon.push_back(new PHPointToPoint2D(0, y, lhs->frame, p));
			newcon.push_back(new PHPointToPoint2D(0, y, rhs->frame, q));
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

PHG2Result PHPointToLine2D::Solve(PHConstraintList2D& newcon){
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	Vec2d pw  = lhs ? lhs->toWorld(p) : p;
	Vec2d qw0 = rhs ? rhs->toWorld(q0) : q0;
	Vec2d qw1 = rhs ? rhs->toWorld(q1) : q1;
	//�����Ƃ�T0R0�̏ꍇ
	if(dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R0)
		return Point_On_LineSegment(pw, qw0, qw1) ? PHG2_SOLVED : PHG2_OVER;
	//�Е���T0R0�ł��������T0R1�̏ꍇ
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R1) ||
		(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R0)){
		//�~�ƒ����̌�_:
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
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHPointToPoint2D(0, y0, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_online){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHPointToPoint2D(0, y1, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	//�Е���T0R0�ł��������T1R0�̏ꍇ
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T1R0) ||
		(dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R0)){
		//�񒼐��̌�_�����߂�F
		Vec2d y, dir = (dof_lhs == PHG2_T1R0) ? (lhs->range1 - lhs->range0) : (rhs->range1 - rhs->range0);
		if(!Line_Line_Intersect(&y, pw, dir, qw0, qw1 - qw0))
			return PHG2_ILLPOSED;
		//y��������ɂȂ���΂Ȃ�Ȃ�
		if(!Point_On_LineSegment(y, qw0, qw1))
			return PHG2_ILLPOSED;
		if(dof_lhs == PHG2_T1R0)
			 newcon.push_back(new PHPointToPoint2D(0, y, lhs->frame, p));
		else newcon.push_back(new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y)));
		return PHG2_NEW;
	}
	//�Е���T0R0�ł��������T2R0�̏ꍇ
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T2R0) ||
		(dof_lhs == PHG2_T2R0 && dof_rhs == PHG2_T0R0)){
		//�_�������ɏ��悤�ɕ��s�ړ�
		PHG2Frame* obj = (dof_lhs == PHG2_T2R0) ? lhs : rhs;
		obj->position += (dof_lhs == PHG2_T2R0) ? (qw0 - pw) : (pw - qw0);
		obj->dof = PHG2_T1R0;
		obj->range0 = obj->position;
		obj->range1 = obj->position + (qw1 - qw0);
		return PHG2_SOLVED;
	}
	return PHG2_REDUNDANT;
}

PHG2Result Spr::PHPointToArc2D::Solve(PHConstraintList2D& newcon){
	//�ȒP�̂��߂����ꂩ��T0R0�ŁA�����������T0R1��T1R0�̏ꍇ�̂݉���
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	if(dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R0)return PHG2_OVER;
	
	Vec2d pw = lhs ? lhs->toWorld(p) : p;
	Vec2d cw = rhs ? rhs->toWorld(c) : c;
	double sw0 = rhs ? (s0 + rhs->angle) : s0;
	double sw1 = rhs ? (s1 + rhs->angle) : s1;
	Vec2d y0, y1, y;

	if((dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R1) || (dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R0)){
		//�Q�~�̌�_
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
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHPointToPoint2D(0, y0, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_onarc){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHPointToPoint2D(0, y1, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	if((dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T1R0) || (dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R0)){
		//�~�ƒ����Ƃ̌�_
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
			newcon.push_back(dof_lhs == PHG2_T1R0 ? new PHPointToPoint2D(0, y0, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_onarc){
			newcon.push_back(dof_lhs == PHG2_T1R0 ? new PHPointToPoint2D(0, y1, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	return PHG2_REDUNDANT;
}

PHG2Result PHLineToLine2D::Solve(PHConstraintList2D& newcon){
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	Vec2d pw0 = lhs ? lhs->toWorld(p0) : p0;
	Vec2d pw1 = lhs ? lhs->toWorld(p1) : p1;
	Vec2d qw0 = rhs ? rhs->toWorld(q0) : q0;
	Vec2d qw1 = rhs ? rhs->toWorld(q1) : q1;
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T2R1) ||
		(dof_lhs == PHG2_T2R1 && dof_rhs == PHG2_T0R0)){
		//������
		return PHG2_REDUNDANT;
	}
	if( (dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R1) ||
		(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T1R0)){
		//������
		return PHG2_REDUNDANT;
	}
	if(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R1){
		//�Q�~�̐ڐ������߂�F
		Vec2d u, v;		//������̍S���_�ɑ΂���ŋߓ_
		Vec2d a0, a1, a2, a3, b0, b1, b2, b3;	//�Q�~�̐ړ_
		NearestPoint(&u, lhs->center, pw0, pw1 - pw0);
		NearestPoint(&v, rhs->center, qw0, qw1 - qw0);
		int nline = Circle_Circle_Tangent(
			lhs->center, (u - lhs->center).norm(),
			rhs->center, (v - rhs->center).norm(),
			&a0, &a1, &a2, &a3,
			&b0, &b1, &b2, &b3);
		if(nline == 0)
			return PHG2_ILLPOSED;
		//�ڐ��̓��ł��߂����̂�I��Őړ_�ւ�PTP�S���ɕϊ�
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
		newcon.push_back(new PHPointToPoint2D(0, a, lhs->frame, lhs->toLocal(u)));
		newcon.push_back(new PHPointToPoint2D(0, b, rhs->frame, rhs->toLocal(v)));
		return PHG2_NEW;
	}
	
	return PHG2_REDUNDANT;
}

PHG2Result PHLineToArc2D::Solve(PHConstraintList2D& newcon){
	return PHG2_REDUNDANT;
}

PHG2Result PHArcToArc2D::Solve(PHConstraintList2D& newcon){
	return PHG2_REDUNDANT;
}

PHG2Result PHParallel2D::Solve(PHConstraintList2D& newcon){
	//lhs, rhs���ɉ�]���R�x�����ꍇ�͏璷
	//lhs, rhs���ɉ�]���R�x�������Ȃ��ꍇ�͉ߏ�
	//�����ꂩ�̂݉�]���R�x�����ꍇ�͂��������]���čS��������
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
	
	//�p�x�����߂�
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

PHG2Result PHAngle2D::Solve(PHConstraintList2D& newcon){
	//lhs, rhs���ɉ�]���R�x�����ꍇ�͏璷
	//lhs, rhs���ɉ�]���R�x�������Ȃ��ꍇ�͉ߏ�
	//�����ꂩ�̂݉�]���R�x�����ꍇ�͂��������]���čS��������
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
	
	//�p�x�����߂�
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

PHG2Result PHFix2D::Solve(PHConstraintList2D& newcon){
	//�����ꂩ��T2R1�ő�����T0R0�̏ꍇ�݈̂���
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

//SGOBJECTIMP(Vector2, SGObject);

/*class Vector2Loader:public FIObjectLoader<Vector2>{
public:
	virtual bool LoadData(FILoadScene* ctx, Vector2* v){
		FIDocNodeBase* doc = ctx->docs.Top();
		doc->GetWholeData(*(XVector2*)v);
		return true;
	}
	Vector2Loader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Float);
		db->REG_RECORD_PROTO(XVector2);
	}
};

class Vector2Saver : public FIObjectSaver<Vector2>{
	virtual UTString GetType() const{ return "Vector2"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, Vector2 *v){
		doc->SetWholeData(*(XVector2*)v);
	}
};*/

class PHGeometry2DEngineLoader:public FIObjectLoader<PHGeometry2DEngine>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHGeometry2DEngine* engine){
		return true;
	}
	PHGeometry2DEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Float);
		db->REG_FIELD(Vector2);
		db->REG_RECORD_PROTO(XGeometry2DEngine);
	}
};

class PHGeometry2DEngineSaver : public FIBaseSaver{
	virtual UTString GetType() const{return "PHGeometry2DEngine";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHGeometry2DEngine* e = (PHGeometry2DEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PHGeometry2DEngine", e);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		PHConstraintList2D::const_iterator it;
		for(it = e->Constraints().begin(); it != e->Constraints().end(); ++it){
			ctx->SaveRecursive((SGObject*)&**it);
		}
		ctx->docs.pop_back();
	}
};

#define DEF_LOADER(NAME)										\
class PH##NAME##Loader : public FIObjectLoader<PH##NAME>{		\
public:															\
	typedef X##NAME template_type;								\
	virtual bool LoadData(FILoadScene* ctx, PH##NAME *con){		\
		FIDocNodeBase* doc = ctx->docs.Top();					\
		doc->GetWholeData(*(template_type*)con);				\
		return true;											\
	}															\
	PH##NAME##Loader(){											\
		UTRef<FITypeDescDb> db = new FITypeDescDb;				\
		db->SetPrefix("X");										\
		db->REG_FIELD(Float);									\
		db->REG_FIELD(Vector2);									\
		db->REG_RECORD_PROTO(X##NAME);							\
	}															\
};

#define DEF_SAVER(NAME)															\
class PH##NAME##Saver : public FIObjectSaver<PH##NAME>{							\
	virtual UTString GetType() const{ return "PH"#NAME; }						\
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, PH##NAME *con){	\
		X##NAME dat;															\
		dat = *(X##NAME *)con;													\
		doc->SetWholeData(dat);													\
		doc->AddChild(ctx->CreateDocNode("REF", &*(con->lhs->frame)));			\
		doc->AddChild(ctx->CreateDocNode("REF", &*(con->rhs->frame)));			\
	}																			\
};

DEF_LOADER(PointToPoint2D)
DEF_LOADER(PointToLine2D)
DEF_LOADER(PointToArc2D)
DEF_LOADER(LineToLine2D)
DEF_LOADER(LineToArc2D)
DEF_LOADER(ArcToArc2D)
DEF_LOADER(Parallel2D)
DEF_LOADER(Angle2D)
DEF_LOADER(Fix2D)

DEF_SAVER(PointToPoint2D)
DEF_SAVER(PointToLine2D)
DEF_SAVER(PointToArc2D)
DEF_SAVER(LineToLine2D)
DEF_SAVER(LineToArc2D)
DEF_SAVER(ArcToArc2D)
DEF_SAVER(Parallel2D)
DEF_SAVER(Angle2D)
DEF_SAVER(Fix2D)

//DEF_REGISTER_BOTH(Vector2);
DEF_REGISTER_BOTH(PHGeometry2DEngine);
DEF_REGISTER_BOTH(PHPointToPoint2D);
DEF_REGISTER_BOTH(PHPointToLine2D);
DEF_REGISTER_BOTH(PHPointToArc2D);
DEF_REGISTER_BOTH(PHLineToLine2D);
DEF_REGISTER_BOTH(PHLineToArc2D);
DEF_REGISTER_BOTH(PHArcToArc2D);
DEF_REGISTER_BOTH(PHParallel2D);
DEF_REGISTER_BOTH(PHAngle2D);
DEF_REGISTER_BOTH(PHFix2D);

#undef DEF_LOADER
#undef DEF_SAVER

}