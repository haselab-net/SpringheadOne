// CollisionArea.cpp: CollisionArea �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "virtualhuman.h"
#include "CollisionArea.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CollisionArea::CollisionArea()
{
	render = NULL;
	collision = NULL;
	bAdd = false;
	bClear = false;
}

CollisionArea::~CollisionArea()
{

}

void CollisionArea::ConnectBehaviorEngines(Spr::SGScene* scene,Spr::GRRender* viewRender,Spr::PHHuman& phHuman){

	if(bAdd){
		collision = NULL;
		render = NULL;
		bAdd = false;
		ClearCoVetxs();
	}
	if(!bAdd && scene && viewRender){
		scene->GetBehaviors().Find(collision);
		render = viewRender;
		AddListener();
	}
}

void CollisionArea::Analyzed(Spr::SGScene* scene, Spr::CDFramePairWithRecord* fr,
								 Spr::CDGeometryPair* geom, Spr::CDConvexPairWithRecord* conv, Spr::CDContactAnalysis* analyzer){
	if(bClear){
		ClearCoVetxs();
	}
	bClear = false;

	for(Spr::CDQHPlane<Spr::CDFace>* it = analyzer->planes.begin; it != analyzer->planes.end;++it){
		//�o�Εϊ������Ă���B
		//it->deleted��true�ɂȂ��Ă���Ǝg��Ȃ��Ȃ����ʂ������B
		if(it->dist != 0 && !it->deleted){
			coVetxs.push_back(it->normal / it->dist + conv->commonPoint);
		}
	}
}

void CollisionArea::ClearCoVetxs(){
	coVetxs.clear();
}

void CollisionArea::AddListener(){
	if(collision){
		collision->AddListener(this);
		bAdd = true;
	}
}

void CollisionArea::DrawCollisionVetxs(){
	
	if(render && coVetxs.size() > 0){
		render->SetLineWidth(6);
		render->SetMaterial(Mat(Spr::Vec4f(1,1,0,1)));
		
		//�ڐG�_��`��
		for(int i = 0; i < coVetxs.size(); ++i){
			render->DrawDirect(Spr::GRRender::POINTS,&coVetxs[i],&coVetxs[i]+1);
			//DSTR << coVetxs[i] <<std::endl;
		}
		
		//�ڐG�_����Ō���ł���B
		std::vector<Spr::Vec3f> vert;
		for(int i = 0; i < coVetxs.size(); ++i){
			vert.push_back(coVetxs[i]);
		}
		vert.push_back(coVetxs[0]);
		for(int i = 0; i < vert.size()-1; ++i){
			render->DrawDirect(Spr::GRRender::LINES, &vert[i], &vert[i+2]);
		}
		vert.clear();
	}

	bClear = true;	//�`����coVetxs���N���A����B����bool
}

void CollisionArea::CalcSupportArea(){

	supportVetxs.clear();

	// coVetxs��񎟌���
	for(int i = 0; i < coVetxs.size(); i++){
		coVetxs[i].y = 0.0f;
	}

	// coVetxs�̏d�S���W���v�Z
	Spr::Vec3f cog = Spr::Vec3f();
	for(int i = 0; i < coVetxs.size(); i++){
		cog += coVetxs[i];
	}
	cog /= coVetxs.size();

	// �d�S�����ԗ���Ă���_
	float maxPos = 0;				// ��r�Ɏg�����Βl�̍ő�l
	int startVetx = 0;				// ����̈�̍ŏ��̓_
	for(int i = 0; i < coVetxs.size(); i++){
		float c = (cog - coVetxs[i]).norm();
		if(maxPos < c){
			maxPos = c;
			startVetx = i;
		}
	}
	supportVetxs.push_back(coVetxs[startVetx]);	//�d�S���痣��Ă���_���O�`�̑��̓_�Ƃ���
	

	// �d�S�ƈ�ԗ���Ă���_����̊p�x����ԑ傫���_��T��
	Spr::Vec3f newVetx;	//���̈���̈�̓_

	Spr::Vec3f zero_cog = cog - supportVetxs[0]; //���̓_����d�S�ւ̃x�N�g��
	float cos_min=1;				//�]���̍ŏ��l
	for(int i = 0; i < coVetxs.size(); i++){
		if(!(i == startVetx)){	// ���_�ȊO
		//if(!(coVetxs[i] == supportVetxs[0])){	// ���_�ȊO
			//�]�������߂�
			Spr::Vec3f zero_other = coVetxs[i] - supportVetxs[0];	//���̓_���炻�ꂼ��̓_�ւ̃x�N�g��
			float cos_buf = (zero_cog*zero_other)/(abs(zero_cog.norm())*abs(zero_other.norm()));
			//�]���ŏ��l
			if(cos_buf < cos_min){
				newVetx = coVetxs[i];
				cos_min = cos_buf;
			}
		}
	}
	supportVetxs.push_back(newVetx);	//���_��


	//���̈���̈�̓_������
	for(int i = 0; i < coVetxs.size(); i++){
		Spr::Vec3f supportAreaLine = supportVetxs[i+1] - supportVetxs[i];	//�O�`�̊�ƂȂ�x�N�g��
		cos_min=1.0f;	//cos_min ������
		
		//�����_���猩��
		for(int j = 0; j < coVetxs.size(); j++){
			//���łɌ���������̈�̓_������
			if(coVetxs[i] != supportVetxs[i+1] && coVetxs[i] != supportVetxs[i]){
				// ��̓_���瑼�̓_�ւ̃x�N�g��
				Spr::Vec3f  supportAreaLineBuf = coVetxs[i] - supportVetxs[i+1];
				//�]�������߂�
				double cos_buf = (supportAreaLineBuf*supportAreaLine)/(supportAreaLineBuf.norm()*supportAreaLine.norm());
				//�]���ŏ��l
				if(cos_buf<cos_min)
				{
					newVetx = coVetxs[i];
					cos_min=cos_buf;
				}
			}
		}
		supportVetxs.push_back(newVetx);
		if(newVetx ==supportVetxs[0])
			break;
	}
}
