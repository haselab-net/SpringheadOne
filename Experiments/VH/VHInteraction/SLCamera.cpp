// SLCamera.cpp: SLCamera �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SLCamera.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

SLCamera::SLCamera(){
	lCamera = NULL;
	bStep   = false;
	bCalib  = false;
}

SLCamera::~SLCamera()
{
	CloseSLCamera();
}

bool SLCamera::InitSLC(){
	lCamera = OpenSLC("SLC-C01-00023",NULL,NULL);//���x�����O�J�����g���͂���
	if (lCamera == NULL){
		//std::cout << " LabelingCamera Not Existing !" << std::endl;
		return false;
	}
	//else std::cout << " Succeed in getting LabelingCamera " << std::endl; //add osada
	labelNum = LABEL_NUM;
	lastLabelNum = 0;

	return true;
}

void SLCamera::Init(){
	bStep = true;
}

void SLCamera::CloseSLCamera(){
	if(lCamera) CloseSLC(lCamera);
}

bool SLCamera::Draw(Spr::GRRender* render){
	if(!lCamera) return false;
	
	render->SetModelMatrix(Spr::Affinef());
	render->SetLineWidth(10);
	for(int i = 0; i < MaxMarkerNum; i++){
		int m = MaxMarkerNum - 1;
		if(m == 0) m = 1;

		Spr::Vec4f color = Spr::Vec4f(((float)i / m), 1 - ((float)i / m), 1, 1);	// �F����
		if(marker[i].match == -1) color = Spr::Vec4f(1, 1, 1, 1);

		Spr::Vec3f pos = marker[i].pos;
		render->SetMaterial(Spr::GRMaterialData(color));
		render->DrawDirect(Spr::GRRender::POINTS, &pos, &pos+1);
	}
	return true;
}

void SLCamera::Calib(SLCOBJECT label){
	// �S��_�ɂ��"�J�������W�n��Springhead���W�n"�̕ϊ��s��̌v�Z
	// �S��_�F(1,0,0)(0,1,0)(0,0,1)(-1,0,0)
	Vec4f c[4], p[4];
	PTM::TMatrixRow<4,4,float> c44, p44;

	c[0] = Vec4f( 1.0f, 0.0f, 0.0f, 1.0f);
	c[1] = Vec4f( 0.0f, 1.0f, 0.0f, 1.0f);
	c[2] = Vec4f( 0.0f, 0.0f, 1.0f, 1.0f);
	c[3] = Vec4f(-1.0f, 0.0f, 0.0f, 1.0f);

	for(int i = 0; i < 4; ++i){
		p[i].x = label.dX;
		p[i].y = label.dY;
		p[i].z = label.dX;
		p[i].w = 1.0f;
		c44.col(i) = c[i];
		p44.col(i) = p[i];
	}
	T = p44 * c44.inv();
	bCalib = true;
}

Spr::Vec3f SLCamera::Convert(SLCOBJECT label){
	Spr::Vec3f lpos;
	if(bCalib){
		lpos.x = label.dX;
		lpos.y = label.dY;
		lpos.z = label.dZ;
		return T * lpos;
	}
	// ���x�����O�J�����̍��W�ϊ�����(�菑��)
	lpos.x = -label.dY;
	lpos.y = -label.dX + 100;
	lpos.z =  label.dZ - 50;
	lpos  /= 100;
	return lpos;
}

void SLCamera::SaveLabelPos(){
	for(int i = 0; i < markerNum; ++i) tmpLabelPos[i] = Convert(labelPos[i]);
}

void SLCamera::SaveDistance(){
	for(int i = 0; i < markerNum; ++i){
		for(int j = 0; j < MaxMarkerNum; ++j){
			if(i == 0) marker[j].dist.clear();
			marker[j].dist.push_back((tmpLabelPos[i] - marker[j].pos).norm());
		}
	}
}

void SLCamera::IncorrectRecognitionCheck(){
	float d;
	for(int i = 0; i < markerNum - 1; ++i){
		d  = (tmpLabelPos[i] - tmpLabelPos[i+1]).norm();	// �ׂ̃��x���Ƃ̋���
		if(d < 0.001f || tmpLabelPos[i].norm() > 10){		// �ׂ̃��x���Ƌ߂�����E�ʒu����������
//			DSTR << tmpLabelPos[i] << std::endl;
//			DSTR << tmpLabelPos[i].norm() << std::endl;
//			DSTR << d << std::endl;
			for(int j = i; j < markerNum - 1; ++j){
				tmpLabelPos[j] = tmpLabelPos[j+1];
			}
			markerNum--;
		}
	}
}

/*void SLCamera::SearchNeighborhood(){
	for(int j = 0; j < MaxMarkerNum; j++){
		for(int i = 0; i < markerNum; ++i) neighborhood[i][j] = i;	// ������
		// �o�u���\�[�g
		for(int i = 0; i < markerNum - 1; ++i){
			for(int k = markerNum - 1; k > i; --k){
				if(dist[neighborhood[j][k]][j] < dist[neighborhood[j][k-1]][j]){
					int tmp = neighborhood[j][k];
					neighborhood[j][k] = neighborhood[j][k-1];
					neighborhood[j][k-1] = tmp;
				}
			}
		}
	}
}*/

float SLCamera::CalcTotalCost(int l, float cost, float min){
	float tmp;
	if(l == markerNum){					// �ċA�I��
		if(cost <= min) return cost;	// �ŏ��R�X�g��Ԃ�
		else			return min;	
	}

	for(int i = 0; i < MaxMarkerNum; i++){
		if(marker[i].match == -1){					// ���܂łɑI�΂�Ă��Ȃ��}�[�J�[
			if(cost + marker[i].dist[l] <= min){	// �ŏ��R�X�g���X�V����\���L��
				marker[i].match = l;				// �}�[�J�[[i]�����x��[l]�ƑΉ��t����
				tmp = CalcTotalCost(l + 1, cost + marker[i].dist[l], min);

				if(tmp <= min){		// ���܂ł̍ŏ��R�X�g�X�V
					min = tmp;
					match[l] = i;	// ���x��[l]�̓}�[�J�[[i]�ƌ��΂�܂���
				}
			}
			marker[i].match = -1;
		}
	}
	return min;
}

void SLCamera::Matching(){
	for(int i = 0; i < MaxMarkerNum; ++i) marker[i].match = -1;
	CalcTotalCost(0, 0, 10000);
	for(int i = 0; i < markerNum; ++i){
		marker[match[i]].match = i;	// �}�[�J�[�ƃ��x���̃}�b�`���O
		marker[match[i]].state = labelPos[i].nStatus;
		marker[match[i]].pos = tmpLabelPos[i];
	}
}

void SLCamera::SetInitMarkerPos(Spr::Vec3f* soPos){
	for(int i = 0; i < MaxMarkerNum; ++i){
		marker[i].pos = soPos[i];
	}
}

/*void SLCamera::CaseOfErasure(){
	int match[MaxMarkerNum];
	int label;
	for(int i = 0; i < MaxMarkerNum; i++) match[MaxMarkerNum] = -1;
	for(int i = 0; i < MaxMarkerNum; i++){
		label = neighborhood[0][i];
		if(match[label] == -1){	// �}�b�`���O���Ă��Ȃ�

		}
	}
}*/

void SLCamera::SetUserPos(CRUser* user){
	user->positionSprings[2].SetTarget(marker[0].pos, marker[0].vel, false);
	user->positionSprings[3].SetTarget(marker[1].pos, marker[1].vel, false);
	user->positionSprings[4].SetTarget(marker[2].pos, marker[2].vel, false);
}

bool SLCamera::Step(Spr::SGScene* scene){ 
	if(lCamera){
		markerNum = 0;
		if(GetSLCData(lCamera, labelPos, &labelNum)){ ////���x�����O�J��������l���擾
			if(bStep){
				markerNum = labelNum;			// �擾�������x���̐�
				SaveLabelPos();					// ���x���ʒu�����ۑ�
				IncorrectRecognitionCheck();	// ���x�����d�����ĔF�����Ă��Ȃ����`�F�b�N
				SaveDistance();					// ���x���ƃ}�[�J�[�̋������v�Z

				Matching();						// �}�[�J�[�Ǐ]
				
				Spr::GRRender* render;
				scene->GetRenderers().Find(render);
				Draw(render);

				labelNum = LABEL_NUM;	// labelNum �͏��2�ɂ��ĂȂ��ƃ_���Ȃ悤��.
			}
		}
		else{
			//std ::cout << GetSLCErrorCode() << std::endl;	// ���Ń��x��������Ȃ��̂���m�邽��.
			labelNum = LABEL_NUM;
			return false;
		}
		//SetSLCDebugItem(lCamera,NULL,NULL);	// ���邩�ǂ���������Ȃ�.��Ńe�X�g<-����Ȃ��݂�����
		return true;
	}
	//else DSTR << GetSLCErrorCode() << std::endl;
	return false;
}

}
