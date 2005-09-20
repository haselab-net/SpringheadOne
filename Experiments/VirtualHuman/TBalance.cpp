// TBalance.cpp: TBalance �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TBalance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

extern Spr::GRMaterialData Mat(Spr::Vec4f c);

TBalance::TBalance()
{
	first = true;

}

TBalance::~TBalance()
{

}

void TBalance::Reload(){
	first = true;
	preVelocity.clear();
}

//void TBalance::CheckBalance(Spr::GRRender* render,THuman& human, double dt){
void TBalance::CheckBalance(Spr::GRRender* render, Spr::PHHuman* phHuman, double dt){

	// ZMP�̌v�Z
	CalcZmp(phHuman,dt);
	// ZMP�̌v�Z2
	phHuman->CalcContactForce();

	//�����̈�̌v�Z
	//CalcFootPoint(human);

	// ZMP�̕`��
	DrawZmp(phHuman,render);
	
	// ZMP���ڐG�i����j�̈���ɂ��邩�ۂ��`�F�b�N
	/*if(ZmpIncludeCheck()){
		DSTR << "����" << std::endl;
	}
	else{
		DSTR <<"�s����"<< std::endl;
	}
	*/
	//�ڐG�̈�̕`��
	collisionArea.DrawCollisionVetxs();

}

void TBalance::CalcZmpTest(THuman& human, double dt){

	GetAccelTest(human, dt);

	double x_zmp1 = 0;
	double x_zmp2 = 0;
	double x_zmp3 = 0;

	double z_zmp1 = 0;
	double z_zmp2 = 0;
	double z_zmp3 = 0;

	for(int i = 0; i < human.solids.size() ; ++i){
		double solidMass = human.solids[i]->GetMass();
		x_zmp1 += solidMass*(Accel[i].y - g_y) * human.solids[i]->GetCenterPosition().x;
		x_zmp2 += solidMass*(Accel[i].x - g_x) * human.solids[i]->GetCenterPosition().y;
		x_zmp3 += solidMass*(Accel[i].y - g_y);

		z_zmp1 += solidMass*(Accel[i].y - g_y) * human.solids[i]->GetCenterPosition().z;
		z_zmp2 += solidMass*(Accel[i].z - g_z) * human.solids[i]->GetCenterPosition().y;
		z_zmp3 += solidMass*(Accel[i].y - g_y);
	}
	vh_zmp.x = (x_zmp1 - x_zmp2)/x_zmp3;
	vh_zmp.z = (z_zmp1 - z_zmp2)/z_zmp3;

	//phHuman->zmp = vh_zmp;
	Accel.clear();


}
void TBalance::CalcZmp(Spr::PHHuman* phHuman, double dt){

	GetAccel(phHuman, dt);

	double x_zmp1 = 0;
	double x_zmp2 = 0;
	double x_zmp3 = 0;

	double z_zmp1 = 0;
	double z_zmp2 = 0;
	double z_zmp3 = 0;

	for(int i = 0; i < phHuman->solids.size() ; ++i){
		double solidMass = phHuman->solids[i]->GetMass();
		x_zmp1 += solidMass*(Accel[i].y - g_y) * phHuman->solids[i]->GetCenterPosition().x;
		x_zmp2 += solidMass*(Accel[i].x - g_x) * phHuman->solids[i]->GetCenterPosition().y;
		x_zmp3 += solidMass*(Accel[i].y - g_y);

		z_zmp1 += solidMass*(Accel[i].y - g_y) * phHuman->solids[i]->GetCenterPosition().z;
		z_zmp2 += solidMass*(Accel[i].z - g_z) * phHuman->solids[i]->GetCenterPosition().y;
		z_zmp3 += solidMass*(Accel[i].y - g_y);
	}
	vh_zmp.x = (x_zmp1 - x_zmp2)/x_zmp3;
	vh_zmp.z = (z_zmp1 - z_zmp2)/z_zmp3;

	phHuman->zmp = vh_zmp;
	Accel.clear();
}

Spr::Vec3d TBalance::GetZmp(){
	return vh_zmp;
}

/*void TBalance::GetAccel(THuman& human, double dt){
	for(int i = 0; i < human.solids.size(); ++i){
		nowVelocityTest = human.solids[i]->GetVelocity();
		Accel.push_back((nowVelocityTest - preVelocityTest)/dt);
		preVelocityTest = nowVelocityTest;
	}
}*/
void TBalance::GetAccel(Spr::PHHuman* phHuman, double dt){
	if(first){
		for(int i = 0; i < phHuman->solids.size(); ++i){
			preVelocity.push_back(Spr::Vec3d(0.0,0.0,0.0));
		}
		first = false;
	}

	for(int i = 0; i < phHuman->solids.size(); ++i){
		nowVelocity = phHuman->solids[i]->GetVelocity();
		Accel.push_back((nowVelocity - preVelocity[i])/dt);
		preVelocity[i] = nowVelocity;
	}
}
void TBalance::GetAccelTest(THuman& human, double dt){
	if(first){
		for(int i = 0; i < human.solids.size(); ++i){
			preVelocity.push_back(Spr::Vec3d(0.0,0.0,0.0));
		}
		first = false;
	}

	for(int i = 0; i < human.solids.size(); ++i){
		nowVelocity = human.solids[i]->GetVelocity();
		Accel.push_back((nowVelocity - preVelocity[i])/dt);
		preVelocity[i] = nowVelocity;
	}
}

//bool TBalance::DrawZmp(THuman& human, Spr::GRRender* render){
bool TBalance::DrawZmp(Spr::PHHuman* phHuman, Spr::GRRender* render){

	//if(!phHuman->IsLoaded()) return false;
	//CalcFootPoint(human);
	
	//ZMP�\��
	render->SetMaterial(Mat(Spr::Vec4f(1,0,0,1)));
	render->SetLineWidth(10);
	Spr::Vec3f v = vh_zmp;
	render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
	

	//ZMP�\��2
	v = phHuman->GetContactPoint();
	render->SetMaterial(Mat(Spr::Vec4f(1,0,1,1)));
	render->SetLineWidth(10);
	render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
	
	//�����̒[�_��\��
	/*render->SetLineWidth(9);
	render->SetMaterial(Mat(Spr::Vec4f(1,0,1,1)));
	for(int i = 0; i < 8; ++i){
		render->DrawDirect(Spr::GRRender::POINTS, &footPoint[i] , &footPoint[i]+1);
	}
	*/
	return true;
}


bool TBalance::ZmpIncludeCheck(){

	bool zmp_in = false;
	if(collisionArea.coVetxs.size() > 3){
		for(int l = 0; l < collisionArea.coVetxs.size() - 2  && !zmp_in; ++l){
			for(int m = l+1; m < collisionArea.coVetxs.size() - 1 && !zmp_in; ++m){
				for(int n = m+1; n < collisionArea.coVetxs.size() && !zmp_in; ++n){
					zmp_in = PosIncludeTriangle(collisionArea.coVetxs[l],
						collisionArea.coVetxs[m], collisionArea.coVetxs[n], vh_zmp);
				}
			}
		}
	}
	return zmp_in;

	// �����̈��ZMP�������Ă��邩�ǂ����B
/*	for(int l = 0; l < 6 && !zmp_in; ++l){
		for(int m = l+1; m < 7 && !zmp_in; ++m){
			for(int n = m+1; n < 8 && !zmp_in; ++n){
				zmp_in = PosIncludeTriangle(footPoint[l], footPoint[m], footPoint[n], vh_zmp);
			}
		}
	}
*/
}

bool TBalance::PosIncludeTriangle(Spr::Vec3f t_a, Spr::Vec3f t_b, Spr::Vec3f t_c, Spr::Vec3f zmp){

	//t_a,t_b,t_c�̎O�_�̒���zmp�������Ă��邩�ǂ���

	float p_a, p_b, a_b, b_a, t, s;
	Spr::Vec3f a_v, b_v;

	a_v = t_b - t_a;
	b_v = t_c - t_a;
	zmp -= t_a;

	p_a = (zmp*a_v)/a_v.norm();
	p_b = (zmp*b_v)/b_v.norm();
	a_b = (a_v*b_v)/b_v.norm();
	b_a = (a_v*b_v)/a_v.norm();

	t = (p_a*a_b - p_b*a_v.norm())/(a_b*b_a - a_v.norm()*b_v.norm());
	s = (p_a*b_v.norm() - p_b*b_a)/(a_v.norm()*b_v.norm() - a_b*b_a);

	if(t >= 0 && t <= 1 && s >= 0 && s <= 1 && s + t >= 0 && s + t <= 1)
		return true;	//����Ă���ꍇ
	else
		return false;	//����Ă��Ȃ��ꍇ
}



//////////////�@�@  �d�v�x���g�p�p�x�Ⴂ     ///////////////////////////////////

void TBalance::CalcFootPoint(THuman& human){

	//���E�̑��̃o�E���f�B���O�{�b�N�X�̌v�Z���擾
	static bool calc = false;
	if(!false){
		human.soLFoot->GetFrame()->CalcBBox();
		human.soRFoot->GetFrame()->CalcBBox();
		
		bBoxMax = human.soLFoot->GetFrame()->bbox.GetBBoxMax();
		bBoxMin = human.soLFoot->GetFrame()->bbox.GetBBoxMin();
		calc = true;
	}

	//���E�̑��̈ʒu���擾
	footLCenter = human.soLFoot->GetCenterPosition();
	footRCenter = human.soRFoot->GetCenterPosition();
	
	if(footLCenter.x < footRCenter.x){	//�ʏ�̑��̈ʒu�i���������j
		footPoint[0] = footLCenter + bBoxMin;
		footPoint[1] = footLCenter + bBoxMin;
		footPoint[1].x = footLCenter.x - bBoxMin.x;
		footPoint[2] = footRCenter + bBoxMin;
		footPoint[3] = footRCenter + bBoxMin;
		footPoint[3].x = footRCenter.x - bBoxMin.x;
		footPoint[4] = footRCenter + bBoxMax;
		footPoint[5] = footRCenter + bBoxMax;
		footPoint[5].x = footRCenter.x - bBoxMax.x;
		footPoint[6] = footLCenter + bBoxMax;
		footPoint[7] = footLCenter + bBoxMax;
		footPoint[7].x = footLCenter.x - bBoxMax.x;

	}else{		//���̈ʒu�����E�t
		footPoint[0] = footRCenter + bBoxMin;
		footPoint[1] = footRCenter + bBoxMin;
		footPoint[1].x = footRCenter.x - bBoxMin.x;
		footPoint[2] = footRCenter + bBoxMax;
		footPoint[3] = footLCenter + bBoxMin;
		footPoint[3].x = footLCenter.x - bBoxMin.x;
		footPoint[4] = footLCenter + bBoxMax;
		footPoint[5] = footLCenter + bBoxMax;
		footPoint[5].x = footLCenter.x - bBoxMax.x;
		footPoint[6] = footLCenter + bBoxMin;
		footPoint[7] = footRCenter + bBoxMax;
		footPoint[7].x = footRCenter.x - bBoxMax.x;
	}

}

Spr::Vec3f* TBalance::GetFootPoint(){
	return footPoint;
}
