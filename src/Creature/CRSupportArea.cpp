// CRSupportArea.cpp: CRSaftyArea �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRSupportArea.h"
#include "CRHuman.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRSupportArea::CRSupportArea()
{
	collision = NULL;
	bAdd = false;
	bClear = false;
}

CRSupportArea::~CRSupportArea()
{

}

void CRSupportArea::Step(){
	if(coVetxs.size() > 0){
		CalcSupportArea();
	}
	bClear = true;	//�`����coVetxs���N���A����B����bool
}

void CRSupportArea::Load(SGScene* scene,CRHuman* crHuman){
	if(bAdd){
		ClearCoVetxs();
		collision = NULL;
		bAdd = false;
	}
	if(!bAdd){
		AddListener(scene);
		SelectCDFrame(crHuman);
	}
}

void CRSupportArea::AddListener(SGScene* scene){
	if(!bAdd && scene){
		scene->GetBehaviors().Find(collision);
		if(collision){
		listenerPos = collision->AddListener(this);
		bAdd = true;
		}
	}
}

void CRSupportArea::Analyzed(SGScene* scene, CDFramePairWithRecord* fr,
								 CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){
	if(bClear){
		ClearCoVetxs();
	}
	bClear = false;

	for(CDQHPlane<CDFace>* it = analyzer->planes.begin; it != analyzer->planes.end;++it){
		//�o�Εϊ������Ă���B
		//it->deleted��true�ɂȂ��Ă���Ǝg��Ȃ��Ȃ����ʂ������B
		if(it->dist != 0 && !it->deleted){
			coVetxs.push_back(it->normal / it->dist + conv->commonPoint);
		}
	}
}

void CRSupportArea::ClearCoVetxs(){
	coVetxs.clear();
}

void CRSupportArea::SelectCDFrame(CRHuman* crHuman){
	if (collision){
		for(CDCollisionEngine::TFrameIt it = collision->FramePairBegin(); it != collision->FramePairEnd(); ++it){
			if (!*it) continue;
			static int iii =0;
			bool b0 = (*it)->frame[0] && HasFrame((*it)->frame[0]->frame,crHuman);
			bool b1 = (*it)->frame[1] && HasFrame((*it)->frame[1]->frame,crHuman);
			if (!((b0 && b1) || (!b0 && !b1))){
				collision->AddActive((*it)->frame[0]->frame,(*it)->frame[1]->frame, listenerPos);
			}
		}
		collision->Init();
	}
}

bool CRSupportArea::HasFrame(SGFrame* f,CRHuman* crHuman){
	for(int i=0; i < crHuman->solids.size(); i++){
		if (crHuman->solids[i] && crHuman->solids[i]->GetFrame() == f) return true;
	}
	return false;
}

void CRSupportArea::DrawCollisionVetxs(GRRender* render){
	if(render){
		render->SetModelMatrix(Spr::Affinef());
		render->SetLineWidth(6);
		render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,0,1)));
		
		//�ڐG�_��`��
		for(int i = 0; i < coVetxs.size(); ++i){
			render->DrawDirect(GRRender::POINTS,&coVetxs[i],&coVetxs[i]+1);
		}
		
		//�@����̈�̕`��
		if(supportVetxs.size() > 0){
			supportVetxs.push_back(supportVetxs[0]);
			for(int i = 0; i < supportVetxs.size()-1; ++i){
				render->DrawDirect(GRRender::LINES, &supportVetxs[i], &supportVetxs[i+2]);
			}
		}
	}
	//bClear = true;	//�`����coVetxs���N���A����B����bool
	if(bClear){
		ClearCoVetxs();
	}
}
/*
void CRSupportArea::CalcSupportArea(){

	supportVetxs.clear();

	// coVetxs��񎟌���
	for(int i = 0; i < coVetxs.size(); i++){
		coVetxs[i].y = 0.0f;
	}

	// coVetxs�̏d�S���W���v�Z
	Vec3f cog = Vec3f();
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
	Vec3f newVetx;	//���̈���̈�̓_

	Vec3f zero_cog = cog - supportVetxs[0]; //���̓_����d�S�ւ̃x�N�g��
	float cos_min=1;				//�]���̍ŏ��l
	for(int i = 0; i < coVetxs.size(); i++){
		if(!(i == startVetx)){	// ���_�ȊO
		//if(!(coVetxs[i] == supportVetxs[0])){	// ���_�ȊO
			//�]�������߂�
			Vec3f zero_other = coVetxs[i] - supportVetxs[0];	//���̓_���炻�ꂼ��̓_�ւ̃x�N�g��
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
		Vec3f supportAreaLine = supportVetxs[i+1] - supportVetxs[i];	//�O�`�̊�ƂȂ�x�N�g��
		cos_min=1.0f;	//cos_min ������
		
		//�����_���猩��
		for(int j = 0; j < coVetxs.size(); j++){
			//���łɌ���������̈�̓_������
			if(coVetxs[i] != supportVetxs[i+1] && coVetxs[i] != supportVetxs[i]){
				// ��̓_���瑼�̓_�ւ̃x�N�g��
				Vec3f  supportAreaLineBuf = coVetxs[i] - supportVetxs[i+1];
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
*/
void CRSupportArea::CalcSupportArea()
{

	supportVetxs.clear();

	// --------------------------
	// �f�[�^��vector�ɓ����@ -
	// --------------------------

	point_number=coVetxs.size();

	for(int j=0;j<coVetxs.size();j++)
	{
		
		M_vec.resize(point_number);
		
		M_vec[j].x = coVetxs[j].x;
		M_vec[j].y = coVetxs[j].z;
		M_vec[j].n =j;
		//dout<<"�|�C���g���F"<< point_number <<eol;
		for(int i=0;i<point_number;i++)
		{
		//	dout<<"X"<<i+1<<":" << M_vec[i].x <<eol;
		//	dout<<"Y"<<i+1<<":" << M_vec[i].y <<eol;
		}
	}
	// --------------------------


	int point_number_calc = point_number-1;
		//�}�E�X�̒l���Ƃ�Ƃ��ɓ_�̌�point_number�͍Ō�ɂP�����Ȃ�
	
	// --------------------------
	// �d�S�̍��W�ʒu���v�Z���� -
	// --------------------------
	M_point sum_point={0,0,0};		//���v�l�����ϒl(�d�S���W)
	for(int i=0;i<point_number_calc;i++)
	{
		sum_point.x += M_vec[i].x;
		sum_point.y += M_vec[i].y;
	}
	sum_point.x = sum_point.x/(double)(point_number_calc);
	sum_point.y = sum_point.y/(double)(point_number_calc);
	coCog.x = sum_point.x;
	coCog.y = sum_point.y;

	//dout<<"�d�SX�F"<<sum_point.x <<"�d�SY�F"<<sum_point.y <<eol;
	// --------------------------

	// --------------------------
	// �d�S�����ԗ���Ă���_ -
	// --------------------------
	double max_p=0;					//��Βl�̍ő�l
	M_point max_val={0,0,0};		//��Βl�̈�ԑ傫�����W�Ƃ��̔ԍ�
	for(int i=0;i<point_number_calc;i++)
	{
		if(max_p < sqrt(pow((sum_point.x - M_vec[i].x),2)+pow((sum_point.y - M_vec[i].y),2)))
		{
			max_val.x = M_vec[i].x;
			max_val.y = M_vec[i].y;
			max_val.n=i;
			max_p = sqrt(pow((sum_point.x - M_vec[i].x),2)+pow((sum_point.y - M_vec[i].y),2));
		}
	}
	search_number=1;				//�d�S���痣��Ă���_���O�`�̑��̓_�Ƃ���
	S_vec.resize(search_number);
	S_vec[0].x=max_val.x;
	S_vec[0].y=max_val.y;
	S_vec[0].n=max_val.n;
//	dout<<"Xmax�F"<<(int)S_vec[0].x <<" Ymax�F"<<(int)S_vec[0].y <<" number�F"<<(int)S_vec[0].n <<eol;
	// --------------------------

	// ------------------------------------------------------
	// �d�S�ƈ�ԗ���Ă���_����̊p�x����ԑ傫���_��T�� -
	// ------------------------------------------------------
	M_point center_vec;				//���̓_����d�S�ւ̃x�N�g��
	M_point each_buf_vec;			//���̓_���炻�ꂼ��̓_�ւ̃x�N�g��

	double cos_min=1;				//�]���̍ŏ��l
					
	//���̓_����d�S�ւ̃x�N�g���@center_vec
	center_vec.x = sum_point.x - S_vec[0].x;
	center_vec.y = sum_point.y - S_vec[0].y;
	
	search_number=2;				//�O�`�ƂȂ�_�������ЂƂ��߂�
	S_vec.resize(search_number);

	for(int i=0;i<point_number_calc;i++)
	{
		if(i != S_vec[0].n)
		{
			//���̓_���炻�ꂼ��̓_�ւ̃x�N�g��	each_buf_vec
			each_buf_vec.x = M_vec[i].x - S_vec[0].x;
			each_buf_vec.y = M_vec[i].y - S_vec[0].y;
			//�]�������߂�
			double cos_buf = ((center_vec.x*each_buf_vec.x)+(center_vec.y*each_buf_vec.y))
				/(sqrt(pow(each_buf_vec.x,2)+pow(each_buf_vec.y,2))*sqrt(pow(center_vec.x,2)+pow(center_vec.y,2)));
			//�]���ŏ��l
			if(cos_buf<cos_min)
			{
				S_vec[1].x=M_vec[i].x;
				S_vec[1].y=M_vec[i].y;
				S_vec[1].n=M_vec[i].n;

				cos_min=cos_buf;
			//	dout<<"cos_min" << cos_min <<eol;
			}
		}
	}
//	dout<<"Xnext1�F" <<eol;
//	dout<<"Xn1�F"<<(int)S_vec[1].x <<" Yn1�F"<<(int)S_vec[1].y <<" nn1�F"<<(int)S_vec[1].n <<eol;

	M_point each_vec;			//�O�`�̊�ƂȂ�x�N�g��
	

	int i=0;
//	int flag_num=0;
	while(1)
	{
		each_vec.x = S_vec[i].x - S_vec[i+1].x;
		each_vec.y = S_vec[i].y - S_vec[i+1].y;
		cos_min=1.0f;				//cos_min ������
		search_number++;			//�O�`�ƂȂ�_�������ЂƂ��߂�
		S_vec.resize(search_number);

		for(int j=0;j<point_number_calc;j++)
		{	
//			flag_num=0;
			if((j!=S_vec[i].n)&&(j!=S_vec[i+1].n))
			{
				each_buf_vec.x = M_vec[j].x - S_vec[i+1].x;
				each_buf_vec.y = M_vec[j].y - S_vec[i+1].y;
				//�]�������߂�
				double cos_buf = ((each_vec.x*each_buf_vec.x)+(each_vec.y*each_buf_vec.y))
					/(sqrt(pow(each_buf_vec.x,2)+pow(each_buf_vec.y,2))*sqrt(pow(each_vec.x,2)+pow(each_vec.y,2)));
				//�]���ŏ��l
				if(cos_buf<cos_min)
				{
					S_vec[i+2].x=M_vec[j].x;
					S_vec[i+2].y=M_vec[j].y;
					S_vec[i+2].n=M_vec[j].n;

					cos_min=cos_buf;
				}
//				flag_num++;
			}
		}
//	dout<<"Xnext�F"<< i <<eol;
//	dout<<"Xni�F"<<(int)S_vec[i].x <<" Yni�F"<<(int)S_vec[i].y <<" nni�F"<<(int)S_vec[i].n <<eol;
	i++;
//	if(flag_num==0)	break;
	if((S_vec[i+1].n==S_vec[0].n)&&(i!=0))	break;
	}


	// --------------------------------
	// �f�[�^��vector�ɂ�����o���@ -
	// --------------------------------

	for(int j=0;j<S_vec.size()-1;j++)
	{
		Vec3f temp = Vec3f();
		temp.x = S_vec[j].x;
		temp.z = S_vec[j].y;
		supportVetxs.push_back(temp);
	}

}




}//	end namespace Spr

