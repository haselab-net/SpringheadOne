#include "Physics.h"
#pragma hdrstop
// PHMagneticEngine.cpp: PHMagneticEngine �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "PHMagneticEngine.h"
#include <Base/BaseDebug.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
//#include <Framework/FWGraphics.h>

#include <Base/TMatrix.h>    //  �s�񃉃C�u�����̃C���N���[�h����D
#include <Base/TVector.h>

#define MAG_NUM 5.0f

namespace Spr{;
SGOBJECTIMP(PHMagnet, SGObject);		//�h���֌W���������߂̃}�N��

bool PHMagnet::AddChildObject(SGObject *o, SGScene *s)
{
	if (DCAST(PHSolid, o)){
		solid=(PHSolid*)o;
		return true;
	}
	return false;
}

void PHMagnet::Clear()
{
	solid=NULL;
}


//�Z�C�o�[�ƃ��[�_�[
class PHMagnetLoader:public FIObjectLoader<PHMagnet>{
public:
	PHMagnetLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Vector);
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XMagnet);
	}
	virtual bool LoadData(FILoadScene* ctx, PHMagnet* magnet){
		ctx->docs.Top()->GetWholeData(magnet->magn);
		return true;
	}
};

class PHMagnetSaver:public FIObjectSaver<PHMagnet>{
	public:
	virtual UTString GetType() const{ return "PHMagnet"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, PHMagnet* magnet){
			doc->SetWholeData(magnet->magn);
			doc->AddChild(ctx->CreateDocNode("REF", magnet->solid));
	}
};
DEF_REGISTER_BOTH(PHMagnet);	//���[�_�[,�Z�C�o�[�̓o�^

//�h���֌W���������߂̃}�N��
SGOBJECTIMP(PHMagneticEngine, SGBehaviorEngine);

//////////////////////////////////////////////////////////////////////
//���͂̕����G���W��
//////////////////////////////////////////////////////////////////////
PHMagneticEngine::PHMagneticEngine(){}

//PHMagneticEngine��1�X�e�b�v�̏���
void PHMagneticEngine::Step(SGScene *s)
{
	int size=mags.size();
	//���ʂ̎��΂̌v�Z
	for(int i=0;i<size-1;i++){
		for(int j=i+1;j<size;j++){
			MagnetForce(mags[i], mags[j], s);
		}
	}
	/*
	//�P���Ɏ��΂̌v�Z
	for(int i=0;i<size-1;i++){
		for(int j=i+1;j<size;j++){
			AddForce(mags[i], mags[j], -1);
		}
	}
	*/
	//�n���̎��͂�������A���΂ł���I�u�W�F�N�g�̋���
	for(int i=0;i<size;i++){
		//DSTR << size;
		EarthPower(mags[i]);
		LineDraw(mags[i]->solid->GetCenterPosition(),s);
	}

	 //  �s��N���X��PTM���O��Ԃ̒��Ő錾����Ă���D
	PTM::TMatrixCol<2,2,float> mat;     //  2�s2��̍s���錾
    mat[0][0] = 1;  mat[0][1] = 2;
    mat[1][0] = 3;  mat[1][1] = 4;
	const Vec2f vec(1,0);       //  2�����̃x�N�g����錾
    //vec[0] = 1; vec[1] = 0;
	PTM::TVector<2,float> xx;

	//PTM::gauss(mat,xx,vec,ip);
	DSTR << mat*vec;
    
}
//���΂����\���b�h�̃|�C���^���擾���邽�߂̊֐�
bool PHMagneticEngine::AddChildObject(SGObject *o, SGScene *s)
{
	if (DCAST(PHMagnet, o)){
		mags.push_back((PHMagnet*)o);
		return true;
	}else{
		if (DCAST(GRMesh,o)){
			mesh=(GRMesh*)o;
			return true;
		}
	}
	return false;
}
void PHMagneticEngine::EarthPower(PHMagnet *one)
{
	Vec3f onePosi		= one->solid->GetCenterPosition();	//one���S�_���擾
	Quaterniond quat1	= one->solid->GetOrientation();		//one�������擾

	//���ׂĂ̎��ɂ̍��W�̌v�Z
	Vec3f onePosiS = onePosi+quat1*one->magn.sPos;
	Vec3f onePosiN = onePosi+quat1*one->magn.nPos;

	one->solid->AddForce(mag.earthMagnet,onePosiS);
	one->solid->AddForce(-mag.earthMagnet,onePosiN);

}
//1���ɂ������΂̗͂̌v�Z
void PHMagneticEngine::AddForce(PHMagnet *one, PHMagnet *two,int flag)
{
	Vec3f onePosi		= one->solid->GetCenterPosition();	//one���S�_���擾
	Vec3f twoPosi		= two->solid->GetCenterPosition();	//two���S�_���擾

	Vec3f distance;
	float dis;
	Vec3f power;

	distance = onePosi-twoPosi;
	dis		 = distance*distance;
	//���͂̌v�Z
	dis=(float)sqrt(dis);
	dis+=0.1f;
	dis=dis*dis*dis;
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce((float)flag*power);
	two->solid->AddForce(-(float)flag*power);
}
//2���ɂ������΂̗͂̌v�Z
void PHMagneticEngine::MagnetForce(PHMagnet *one, PHMagnet *two, SGScene *s)
{
	Vec3f onePosi		= one->solid->GetCenterPosition();	//one���S�_���擾
	Vec3f twoPosi		= two->solid->GetCenterPosition();	//two���S�_���擾
	Quaterniond quat1	= one->solid->GetOrientation();		//one�������擾
	Quaterniond quat2	= two->solid->GetOrientation();		//two�������擾
	
	Vec3f power;
	Vec3f hyouji;
	Vec3f distance;
	float dis;
	Vec3f onePosiS, twoPosiS;
	Vec3f onePosiN, twoPosiN;
	//���ׂĂ̎��ɂ̍��W�̌v�Z
	onePosiS = onePosi+quat1*one->magn.sPos;
	twoPosiS = twoPosi+quat2*two->magn.sPos;
	onePosiN = onePosi+quat1*one->magn.nPos;
	twoPosiN = twoPosi+quat2*two->magn.nPos;

//	FWGraphics graph(s);
//	graph.DrawVec(onePosiS,Vec3f(0.1,0.1,0.1),Vec4f(1,0,0,1));
//	graph.DrawVec(onePosiN,Vec3f(0.1,0.1,0.1),Vec4f(0,1,0,1));

//---S�ɂ�S�ɂ̌v�Z--------------------------
	distance = onePosiS-twoPosiS;
	dis		 = distance*distance;
	//���͂̌v�Z
	dis=(float)sqrt(dis);
	//�G���[����̂��ߋ��������Z����
	dis+=0.1f;
	dis=dis*dis*dis;
	//�͂��v�Z���A�\���b�h�ɉ�����
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce(power,onePosiS);
	two->solid->AddForce(-power,twoPosiS);

//---N�ɂ�S�ɂ̌v�Z--------------------------
	distance = onePosiN-twoPosiS;
	dis		 = distance*distance;
	//���͂̌v�Z
	dis=(float)sqrt(dis);
	dis+=0.1f;
	dis=dis*dis*dis;
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce(-power,onePosiN);
	two->solid->AddForce(power,twoPosiS);
	
//---S�ɂ�N�ɂ̌v�Z--------------------------
	distance = onePosiS-twoPosiN;
	dis		 = distance*distance;
	//���͂̌v�Z
	dis=(float)sqrt(dis);
	dis+=0.1f;
	dis=dis*dis*dis;
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce(-power,onePosiS);
	two->solid->AddForce(power,twoPosiN);

//---N�ɂ�N�ɂ̌v�Z--------------------------
	distance = onePosiN-twoPosiN;
	dis		 = distance*distance;
	//���͂̌v�Z
	dis=(float)sqrt(dis);
	dis+=0.1f;
	dis=dis*dis*dis;
	power=(two->magn.magPow*one->magn.magPow*MAG_NUM/dis)*distance;
	one->solid->AddForce(power,onePosiN);
	two->solid->AddForce(-power,twoPosiN);
}

void PHMagneticEngine::LineDraw(Vec3f center,SGScene *s)
{
//�`�ʂ̐ݒ�/////
	GRRender* render;
	s->GetRenderers().Find(render);
	if (render->CanDraw()){
		Affinef afw;
		SGFrame* baseFrame=s->GetWorld();
		afw = baseFrame->GetWorldPosture();
		afw.Ex().unitize();
		afw.Ey().unitize();
		afw.Ez().unitize();
		render->SetModelMatrix(afw);
	}
	GRMaterialData mat1(Vec4f(1, 0, 0, 1),Vec4f(1, 0, 0, 1),Vec4f(1, 0, 0, 1),Vec4f(1, 0, 0, 1), 0);
	mat1.ambient = Vec4f(1, 1, 0, 1);
	mat1.diffuse = Vec4f(1, 1, 0, 1);
	mat1.specular = Vec4f(1, 1, 0, 1);
	mat1.emissive = Vec4f(1, 1, 0, 1);
	mat1.power = 0;
	render->SetDepthFunc(GRRender::DF_ALWAYS);//Z�o�b�t�@�̃`�F�b�N�����Ȃ��D
	render->SetLineWidth(8);                  //���̑���(OpenGL�ŗL��)�C�_�̑傫��(DirectX�ŗL��)
	render->SetMaterial(mat1);                //�}�e���A���ݒ�
/////////////////
	//���b�V���̏�񂪂������Ƃ������`�ʂ��s��
	if(mesh)
	for(int i=0;i<mesh->triangles.size()-1;i++){
		Vec3f vec[2];
		vec[0]=mesh->vertices[mesh->triangles[i]]+center;
		vec[1]=mesh->vertices[mesh->triangles[i+1]]+center;
		render->DrawDirect(GRRender::LINES, vec, vec+2);
	}
	render->SetDepthFunc(GRRender::DF_LESS);  //Z�o�b�t�@�̃`�F�b�N��߂�
}
///////////////////////////////////////////////
//���̓G���W���̃��[�_�[�ƃZ�C�o�[
//
//X�t�@�C������V�[���O���t�ɕϊ��A�܂��t�̕ϊ����s�����߂̃N���X
class PHMagneticEngineLoader:public FIObjectLoader<PHMagneticEngine>{
public:
	PHMagneticEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Vector);
		db->REG_RECORD_PROTO(XMagneticEngine);
	}
	virtual bool LoadData(FILoadScene* ctx, PHMagneticEngine* magnet){
		ctx->docs.Top()->GetWholeData(magnet->mag);
		return true;
	}
};

class PHMagneticEngineSaver:public FIObjectSaver<PHMagneticEngine>{
public:
	virtual UTString GetType() const{ return "PHMagneticEngine"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, PHMagneticEngine* magnet){
			doc->SetWholeData(magnet->mag);
			for(PHMagnets::iterator it = magnet->mags.begin(); it != magnet->mags.end(); it++){
				doc->AddChild(ctx->CreateDocNode("REF", *it));
			}
	}
};
DEF_REGISTER_BOTH(PHMagneticEngine);	//���[�_�[,�Z�C�o�[�̓o�^�}�N��

}	//	namespace Spr





