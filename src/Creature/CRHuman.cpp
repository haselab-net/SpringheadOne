// CRHuman.cpp: CRHuman �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRHuman.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

namespace Spr{;

SGOBJECTIMP(CRHuman, SGBehaviorEngine);

CRHuman::CRHuman()
{
	bLoaded = false;

	//�g���A�̏d�̐ݒ�(�f�t�H���g�j
	totalMass = 60.0f;
	totalHeight = 1.7f;
	SetSolidInfo();
	SetJointInfo();
}

CRHuman::~CRHuman()
{

}
void CRHuman::Load(SGScene* scene){
	scene->FindObject(jointEngine, "jeHuman");
	if (!jointEngine) scene->GetBehaviors().Find(jointEngine);
	Loaded(scene);
}
void CRHuman::Loaded(SGScene* scene){
	jointEngine->Loaded(scene);
	Connect(scene);
	SetModel(scene);
	SetBodyNum();
	RegistSupportParts(scene);
	LoadDerivedModel(scene);
}

void CRHuman::SetModel(SGScene* scene){
	if(bLoaded){
		SetScale(scene);
		SetMass();
		SetInertia();
		SetJointSpring((float)scene->GetTimeStep());
		SetJointRange();
		SetJointInitAngle();
		
		totalMass = 0.0;
		for(int i = 0; i < solids.size(); i++){
			if(!(solids[i] == NULL))
				totalMass += solids[i]->GetMass();
		}
	}
}
bool CRHuman::AddChildObject(SGObject* o, SGScene* s){
	PHJointEngine* je = DCAST(PHJointEngine, o);
	if (je){
		jointEngine = je;
	}
	return true;
}

void CRHuman::ConnectSolid(const char* name, UTRef<SGScene> scene){
	SGScene::SetRange range;
	range = scene->RangeObject(name);
	for(;range.first != range.second; ++range.first){
		PHSolid* solid = DCAST(PHSolid, *range.first);
		if (jointEngine->Has(solid)){
			solids.push_back(solid);
			return;
		}
	}
	bLoaded = false;
	DSTR << "Solid '" << name << "' is not found." << std::endl;
}

void CRHuman::ConnectJoint(const char* name, UTRef<SGScene> scene){
	SGScene::SetRange range;
	range = scene->RangeObject(name);
	for(;range.first != range.second; ++range.first){
		PHJointBase* joint = DCAST(PHJointBase, *range.first);
		if (jointEngine->Has(joint)){
			joints.push_back(joint);
			return;
		}
	}
	bLoaded = false;
	DSTR << "Joint '" << name << "' is not found." << std::endl;
}

bool CRHuman::Connect(UTRef<SGScene> scene){
	solids.clear();
	joints.clear();
	jointPids.clear();
	supportSolidNum[0].clear();
	supportSolidNum[1].clear();
	ankleJointNum[0].clear();
	ankleJointNum[1].clear();
	bLoaded = true;

	// Solid Connect
	ConnectSolid("soWaist", scene);
	ConnectSolid("soAbdomen", scene);
	ConnectSolid("soChest", scene);
	ConnectSolid("soHead", scene);

	ConnectSolid("soRUArm", scene);
	ConnectSolid("soRFArm", scene);
	ConnectSolid("soRHand", scene);

	ConnectSolid("soLUArm", scene);
	ConnectSolid("soLFArm", scene);
	ConnectSolid("soLHand", scene);

	ConnectSolid("soRThigh", scene);
	ConnectSolid("soRLThigh", scene);
	ConnectSolid("soRHeel", scene);
	ConnectSolid("soRToe", scene);

	ConnectSolid("soLThigh", scene);
	ConnectSolid("soLLThigh", scene);
	ConnectSolid("soLHeel", scene);
	ConnectSolid("soLToe", scene);

	//Joint Connect	
	ConnectJoint("joWaist1", scene);
	ConnectJoint("joWaist2", scene);
	ConnectJoint("joWaist3", scene);
	ConnectJoint("joChest1", scene);
	ConnectJoint("joChest2", scene);
	ConnectJoint("joChest3", scene);
	ConnectJoint("joNeck1", scene);
	ConnectJoint("joNeck2", scene);
	ConnectJoint("joNeck3", scene);
	
	ConnectJoint("joRShoulder1", scene);
	ConnectJoint("joRShoulder2", scene);
	ConnectJoint("joRShoulder3", scene);
	ConnectJoint("joRElbow1", scene);
	ConnectJoint("joRElbow2", scene);
	ConnectJoint("joRWrist1", scene);
	ConnectJoint("joRWrist2", scene);

	ConnectJoint("joLShoulder1", scene);
	ConnectJoint("joLShoulder2", scene);
	ConnectJoint("joLShoulder3", scene);
	ConnectJoint("joLElbow1", scene);
	ConnectJoint("joLElbow2", scene);
	ConnectJoint("joLWrist1", scene);
	ConnectJoint("joLWrist2", scene);

	ConnectJoint("joRHip1", scene);
	ConnectJoint("joRHip2", scene);
	ConnectJoint("joRHip3", scene);
	ConnectJoint("joRKnee", scene);
	ConnectJoint("joRAnkle1", scene);
	ConnectJoint("joRAnkle2", scene);
	ConnectJoint("joRAnkle3", scene);
	ConnectJoint("joRToe", scene);

	ConnectJoint("joLHip1", scene);
	ConnectJoint("joLHip2", scene);
	ConnectJoint("joLHip3", scene);
	ConnectJoint("joLKnee", scene);
	ConnectJoint("joLAnkle1", scene);
	ConnectJoint("joLAnkle2", scene);
	ConnectJoint("joLAnkle3", scene);
	ConnectJoint("joLToe", scene);

	for(int i=0; i<joints.size(); ++i){
		jointPids.push_back(PHJointPid::Find((PHJoint1D*)joints[i], scene));
	}

	return bLoaded;
}

bool CRHuman::HasFrame(SGFrame* f){
	for(unsigned i=0; i<solids.size(); ++i){
		if (solids[i] && solids[i]->GetFrame() == f) return true;
	}
	return false;
}

void CRHuman::SetMass(){
	for(unsigned i=0; i<solids.size(); ++i){
		if(solids[i] != NULL){
			solids[i]->SetMass(solids[i]->GetMass()*totalMass);
		}
	}
}

void CRHuman::SetOneInertia(Spr::PHSolid* solid, Spr::Vec3f axis){
	Matrix3f solidInertia;

	solidInertia[0][0] = (solid->GetMass() * (axis.y*axis.y + axis.z*axis.z)) / 5;
	solidInertia[1][1] = (solid->GetMass() * (axis.z*axis.z + axis.x*axis.x)) / 5;
	solidInertia[2][2] = (solid->GetMass() * (axis.x*axis.x + axis.y*axis.y)) / 5;

	solid->SetInertia(solidInertia);
}

void CRHuman::SetInertia(){
	for(unsigned i=0; i<solids.size(); ++i){
		if(solids[i] != NULL)
			SetOneInertia(solids[i], sinfo[i].scale);
			//solids[i]->SetInertia(solids[i]->GetInertia()*totalMass);
	}
}

void CRHuman::SetTotalMass(float mass){
	totalMass = mass;
}
void CRHuman::SetTotalHeight(float height){
	totalHeight = height;
}

void CRHuman::SetScale(SGScene* scene){
	SetSolidScale();
	SetJointScale();

	jointEngine = NULL;
	scene->GetBehaviors().Find(jointEngine);
	jointEngine->root->Loaded(scene);
}

void CRHuman::SetSolidScale(){
	SGFrame* transFrame;
	Affinef B;
	B.xx *= totalHeight;
	B.yy *= totalHeight;
	B.zz *= totalHeight;

	for(unsigned i = 0; i < solids.size(); i++){
		if(solids[i] != NULL){
			transFrame = DCAST(SGFrame, solids[i]->GetFrame()->ChildObject(0));
			Affinef A = transFrame->GetPosture();
			A = A*B;
			transFrame->SetPosture(A);
			transFrame = NULL;
			solids[i]->GetFrame()->CalcBBox();	//	Bouding Box�̍Čv�Z�D
		}
	}
}

void CRHuman::SetJointScale(){
	for(unsigned i = 0; i < joints.size(); i++){
		if(joints[i] != NULL){
			joints[i]->v3fPositionParent *= totalHeight; 
			joints[i]->v3fPositionChild  *= totalHeight; 
			//TEST
			if(i == 9 || i == 16){
				joints[i]->v3fPositionParent *= 1.1;
				joints[i]->v3fPositionChild *= 1.1;
			}
		}
	}
}

void CRHuman::SetSolidInfo(){
	// ���̓��t�@�C�����烍�[�h����悤�ɂ��悤
	// soWaist(��)
	sinfo[0].scaleRatio = Vec3f(0.190, 0.0768, 0.137) / 2;
	sinfo[0].massRatio  = 0.17f;

	// soAbdomen(��)
	sinfo[1].scaleRatio = Vec3f(0.151, 0.0201, 0.108) / 2;
	sinfo[1].massRatio  = 0.028f;

	// soChest(��)
	sinfo[2].scaleRatio = Vec3f(0.184, 0.202, 0.124) / 2;
	sinfo[2].massRatio  = 0.252f;

	// soHead(��)
	sinfo[3].scaleRatio = Vec3f(0.0949, 0.140, 0.111) / 2;
	sinfo[3].massRatio  = 0.07f;

	// soRUArm(�E��r)
	sinfo[4].scaleRatio = Vec3f(0.0487, 0.181, 0.0487) / 2;
	sinfo[4].massRatio  = 0.04f;

	// soRFArm(�E�O�r)
	sinfo[5].scaleRatio = Vec3f(0.046, 0.141, 0.046) / 2;
	sinfo[5].massRatio  = 0.02f;

	// soRHand(�E��)
	sinfo[6].scaleRatio = Vec3f(0.0169, 0.106, 0.0479) / 2;
	sinfo[6].massRatio  = 0.01f;

	// soLUArm(����r)
	sinfo[7].scaleRatio = Vec3f(0.0487, 0.181, 0.0487) / 2;
	sinfo[7].massRatio  = 0.04f;

	// soLFArm(���O�r)
	sinfo[8].scaleRatio = Vec3f(0.046, 0.141, 0.046) / 2;
	sinfo[8].massRatio  = 0.02f;

	// soLHand(����)
	sinfo[9].scaleRatio = Vec3f(0.0169, 0.106, 0.0479) / 2;
	sinfo[9].massRatio  = 0.01f;

	// soRThigh(�E���)
	sinfo[10].scaleRatio = Vec3f(0.0961, 0.233, 0.0961) / 2;
	sinfo[10].massRatio  = 0.1f;

	// soRLThigh(�E����)
	sinfo[11].scaleRatio = Vec3f(0.0664, 0.233, 0.0664) / 2;
	sinfo[11].massRatio  = 0.05f;

	// soRHeel(�E��)
	sinfo[12].scaleRatio = Vec3f(0.0379, 0.0408, 0.0942) / 2;
	sinfo[12].massRatio  = 0.015f;

	// soRToe(�E�ܐ�)
	sinfo[13].scaleRatio = Vec3f(0.0582, 0.0288, 0.0542) / 2;
	sinfo[13].massRatio  = 0.005f;

	// soLThigh(�����)
	sinfo[14].scaleRatio = Vec3f(0.0961, 0.233, 0.0961) / 2;
	sinfo[14].massRatio  = 0.1f;

	// soLLThigh(������)
	sinfo[15].scaleRatio = Vec3f(0.0664, 0.233, 0.0664) / 2;
	sinfo[15].massRatio  = 0.05f;

	// soLHeel(����)
	sinfo[16].scaleRatio = Vec3f(0.0379, 0.0408, 0.0942) / 2;
	sinfo[16].massRatio  = 0.015f;

	// soLToe(���ܐ�)
	sinfo[17].scaleRatio = Vec3f(0.0582, 0.0288, 0.0542) / 2;
	sinfo[17].massRatio  = 0.005f;

	//for(int i= 0; i < solids.size(); ++i){
	for(int i= 0; i < 18; ++i){
		sinfo[i].scale = sinfo[i].scaleRatio * totalHeight;
	}
}

void CRHuman::SetJointInfo(){
	// ��[0] (-X)
	jinfo[0].rangeMin	= -51.00f;
	jinfo[0].rangeMax	= 102.65f;
	jinfo[0].axis		= -1.0;
	jinfo[0].initPos	= 0.0f;

	// ��[1] (+Z) ����f�[�^����
	jinfo[1].rangeMin	= -10.00f;
	jinfo[1].rangeMax	= 10.00f;
	jinfo[1].axis		= 1.0;
	jinfo[1].initPos	= 0.0f;

	// ��[2] (-Y)
	jinfo[2].rangeMin	= -49.00f;
	jinfo[2].rangeMax	= 49.00f;
	jinfo[2].axis		= -1.0;
	jinfo[2].initPos	= 0.0f;

	// ��[3] (-X) ����f�[�^����
	jinfo[3].rangeMin	= -5.00f;
	jinfo[3].rangeMax	= 5.00f;
	jinfo[3].axis		= -1.0;
	jinfo[3].initPos	= 0.0f;

	// ��[4] (+Z) ����f�[�^����
	jinfo[4].rangeMin	= -5.00f;
	jinfo[4].rangeMax	= 5.00f;
	jinfo[4].axis		= 1.0;
	jinfo[4].initPos	= 0.0f;

	// ��[5] (-Y) ����f�[�^����
	jinfo[5].rangeMin	= -5.00f;
	jinfo[5].rangeMax	= 5.00f;
	jinfo[5].axis		= -1.0;
	jinfo[5].initPos	= 0.0f;

	// ��[6] (-X)
	jinfo[6].rangeMin	= -101.00f;
	jinfo[6].rangeMax	= 70.00f;
	jinfo[6].axis		= -1.0;
	jinfo[6].initPos	= 0.0f;

	// ��[7] (+Z)
	jinfo[7].rangeMin	= -55.00f;
	jinfo[7].rangeMax	= 59.35f;
	jinfo[7].axis		= 1.0;
	jinfo[7].initPos	= 0.0f;

	// ��[8] (-Y)
	jinfo[8].rangeMin	= -78.00f;
	jinfo[8].rangeMax	= 82.00f;
	jinfo[8].axis		= -1.0;
	jinfo[8].initPos	= 0.0f;

	// �E��[9] (+X)
//	jinfo[9].rangeMin	= -66.00f;
//	jinfo[9].rangeMax	= 186.00f;
	jinfo[9].axis		= 1.0;
	jinfo[9].initPos	= 0.0f;

	// �E��[10] (+Z)
	jinfo[10].rangeMin	= -60.00f;
	jinfo[10].rangeMax	= 164.40f;
	jinfo[10].axis		= 1.0;
	jinfo[10].initPos	= 0.0f;

	// �E��[11] (-Y) ����f�[�^����
	jinfo[11].rangeMin	= -90.00f;
	jinfo[11].rangeMax	= 90.00f;
	jinfo[11].axis		= -1.0;
	jinfo[11].initPos	= 0.0f;

	// �E�I[12] (+X)
	jinfo[12].rangeMin	= -6.00f;
	jinfo[12].rangeMax	= 154.00f;
	jinfo[12].axis		= 1.0;
	jinfo[12].initPos	= 0.0f;

	// �E�I[13] (-Y) ����f�[�^����
	jinfo[13].rangeMin	= -90.00f;
	jinfo[13].rangeMax	= 90.00f;
	jinfo[13].axis		= -1.0;
	jinfo[13].initPos	= 0.0f;

	// �E���[14] (-Z)
	jinfo[14].rangeMin	= -96.25f;
	jinfo[14].rangeMax	= 100.05f;
	jinfo[14].axis		= -1.0;
	jinfo[14].initPos	= 0.0f;

	// �E���[15] (+X)
	jinfo[15].rangeMin	= -64.00f;
	jinfo[15].rangeMax	= 48.00f;
	jinfo[15].axis		= 1.0;
	jinfo[15].initPos	= 0.0f;

	// ����[16] (+X)
//	jinfo[16].rangeMin  = -66.00f;
//	jinfo[16].rangeMax  = 186.00f;
	jinfo[16].axis		= 1.0;
	jinfo[16].initPos	= 0.0f;

	// ����[17] (-Z)
	jinfo[17].rangeMin	= -60.00f;
	jinfo[17].rangeMax	= 164.40f;
	jinfo[17].axis		= -1.0;
	jinfo[17].initPos	= 0.0f;

	// ����[18] (Y) ����f�[�^����
	jinfo[18].rangeMin	= -90.00f;
	jinfo[18].rangeMax	= 90.00f;
	jinfo[18].axis		= 1.0;
	jinfo[18].initPos	= 0.0f;

	// ���I[19] (+X)
	jinfo[19].rangeMin	= -6.00f;
	jinfo[19].rangeMax	= 154.00f;
	jinfo[19].axis		= 1.0;
	jinfo[19].initPos	= 0.0f;

	// ���I[20] (Y) ����f�[�^����
	jinfo[20].rangeMin	= -90.00f;
	jinfo[20].rangeMax	= 90.00f;
	jinfo[20].axis		= 1.0;
	jinfo[20].initPos	= 0.0f;

	// �����[21] (+Z)
	jinfo[21].rangeMin	= -96.25f;
	jinfo[21].rangeMax	= 100.05f;
	jinfo[21].axis		= 1.0;
	jinfo[21].initPos	= 0.0f;

	// �����[22] (+X)
	jinfo[22].rangeMin	= -64.00f;
	jinfo[22].rangeMax	= 48.00f;
	jinfo[22].axis		= 1.0;
	jinfo[22].initPos	= 0.0f;

	// �E��[23] (+X)
	jinfo[23].rangeMin	= -36.15f;
	jinfo[23].rangeMax	= 128.00f;
	jinfo[23].axis		= 1.0;
	jinfo[23].initPos	= 0.0f;

	// �E��[24] (+Z)
	jinfo[24].rangeMin	= -33.00f;
	jinfo[24].rangeMax	= 69.00f;
	jinfo[24].axis		= 1.0;
	jinfo[24].initPos	= 0.0f;

	// �E��[25] (-Y) ����f�[�^����
	jinfo[25].rangeMin	= -90.00f;
	jinfo[25].rangeMax	= 90.00f;
	jinfo[25].axis		= -1.0;
	jinfo[25].initPos	= 0.0f;

	// �E�G[26] (-X)
	jinfo[26].rangeMin	= -8.00f;
	jinfo[26].rangeMax	= 164.00f;
	jinfo[26].axis		= -1.0;
	jinfo[26].initPos	= 0.0f;

	// �E����[27] (X)
	jinfo[27].rangeMin	= -39.00f;
	jinfo[27].rangeMax	= 73.00f;
	jinfo[27].axis		= 1.0;
	jinfo[27].initPos	= 0.0f;

	// �E����[28] (Z)
	jinfo[28].rangeMin	= -20.00f;
	jinfo[28].rangeMax	= 20.00f;
	jinfo[28].axis		= 1.0;
	jinfo[28].initPos	= 0.0f;

	// �E����[29] (-Y) ����f�[�^����
	jinfo[29].rangeMin	= -20.00f;
	jinfo[29].rangeMax	= 20.00f;
	jinfo[29].axis		= -1.0;
	jinfo[29].initPos	= 0.0f;

	// �E�ܐ�[30] (-X) ����f�[�^����
	jinfo[30].rangeMin	= -30.00f;
	jinfo[30].rangeMax	= 45.00f;
	jinfo[30].axis		= -1.0;
	jinfo[30].initPos	= 0.0f;

	// ����[31] (+X)
	jinfo[31].rangeMin	= -36.15f;
	jinfo[31].rangeMax	= 128.00f;
	jinfo[31].axis		= 1.0;
	jinfo[31].initPos	= 0.0f;

	// ����[32] (-Z)
	jinfo[32].rangeMin	= -33.00f;
	jinfo[32].rangeMax	= 69.00f;
	jinfo[32].axis		= -1.0;
	jinfo[32].initPos	= 0.0f;
	
	// ����[33] (Y) ����f�[�^����
	jinfo[33].rangeMin	= -90.00f;
	jinfo[33].rangeMax	= 90.00f;
	jinfo[33].axis		= 1.0;
	jinfo[33].initPos	= 0.0f;

	// ���G[34] (-X)
	jinfo[34].rangeMin	= -8.00f;
	jinfo[34].rangeMax	= 164.00f;
	jinfo[34].axis		= -1.0;
	jinfo[34].initPos	= 0.0f;

	// ������[35] (-X)
	jinfo[35].rangeMin	= -39.00f;
	jinfo[35].rangeMax	= 73.00f;
	jinfo[35].axis		= -1.0;
	jinfo[35].initPos	= 0.0f;

	// ������[36] (-Z)
	jinfo[36].rangeMin	= -20.00f;
	jinfo[36].rangeMax	= 20.00f;
	jinfo[36].axis		= -1.0;
	jinfo[36].initPos	= 0.0f;

	// ������[37] (+Y) ����f�[�^����
	jinfo[37].rangeMin	= -20.00f;
	jinfo[37].rangeMax	= 20.00f;
	jinfo[37].axis		= 1.0;
	jinfo[37].initPos	= 0.0f;

	// ���ܐ�[38] (-X) ����f�[�^����
	jinfo[38].rangeMin	= -30.00f;
	jinfo[38].rangeMax	= 45.00f;
	jinfo[38].axis		= -1.0;
	jinfo[38].initPos	= 0.0f;

}

void CRHuman::SetJointInitAngle(){
	for(unsigned i = 0; i < joints.size(); i++){
		if(joints[i] != NULL){
			((PHJoint1D*)joints[i])->position = jointPids[i]->goal = jinfo[i].initPos;
		}
	}
}

void CRHuman::SetJointRange(){
	for(unsigned i = 0; i < joints.size(); i++){
		if(joints[i] != NULL){
			SetOneJointRange((PHJoint1D*)joints[i], jinfo[i].rangeMin, jinfo[i].rangeMax);
		}
	}
}

void CRHuman::SetOneJointRange(PHJoint1D* j, float min, float max){
	if(j){
		j->minPosition = min * (float)M_PI / 180;
		j->maxPosition = max * (float)M_PI / 180;
	}
}

inline float GetChildMass(PHJointBase* j){
	if (strlen(j->solid->GetName()) != 0){
		return j->solid->GetMass();
	}
	else{
		float rv = 0;
		for(unsigned i=0; i<j->Children().size(); ++i){
			rv += GetChildMass(j->Children()[i]);
		}
		return rv;
	}
}

inline float GetAllChildrenMass(PHJointBase* j){
	float mass = 0;
	if(j->Children().size() == 0){
		return j->solid->GetMass();
	}
	else{
		if(strlen(j->solid->GetName()) != 0){
			mass += j->solid->GetMass();
		}
		for(unsigned i = 0; i < j->Children().size(); ++i){
			mass += GetAllChildrenMass(j->Children()[i]);
		}
		return mass;
	}
}

void CRHuman::JointPIDMul(PHJointPid* jo, float mulP, float mulD){
	jo->proportional *= mulP;
	jo->integral *= mulP;
	jo->differential *= mulD;
}

void CRHuman::SetJointSpring(float dt){
	//const float SAFETYRATE = 0.01f;
	const float SAFETYRATE = 0.001f;
	float k = 0.2f * SAFETYRATE;
	float b = 0.6f * SAFETYRATE;
	//float k = 0.008f * SAFETYRATE;
	//float b = 0.9f * SAFETYRATE;
//	for(int i=0; i<joints.size(); ++i){
	for(int i=0; i<jointPids.size(); ++i){
		//if(joints[i] != NULL){
		if(jointPids[i] != NULL){
			float mass = GetChildMass(joints[i]);
			//float mass = GetAllChildrenMass(joints[i]);
			jointPids[i]->proportional = k * 2 * mass / (dt*dt);
			jointPids[i]->differential = b * mass / dt;
			jointPids[i]->integral = k * 2 * mass / (dt*dt) / 5000.0f;
		}
	}
	// �֐߂��_�炩�߂ɐݒ�(�E�r)
	for(int i = 9; i < 15; ++i){ 
		//if(joints[i] != NULL){
		if(jointPids[i] != NULL){
			JointPIDMul(jointPids[i], 0.01f, 1.0f);
		}
	}
	// �֐߂��_�炩�߂ɐݒ�(���r)
	for(int i = 16; i < 22; ++i){ 
		//if(joints[i] != NULL){
		if(jointPids[i] != NULL){
			JointPIDMul(jointPids[i], 0.01f, 1.0f);
		}
	}
	
}

void CRHuman::AddJointPassivityResistance(){
	float torque;

	////////// ��(���F����) [9],[16] //////////
	// M = exp{3.3671(-�Ɓ~��/180 - 0.2543)} - exp{3.5743(�Ɓ~��/180 - 2.1966)}
	torque = exp(3.3671 * (joints[9]->GetJointPosition(0) - 0.2543))
		- exp(3.5743 * (joints[9]->GetJointPosition(0) - 2.1966));
	//joints[9]->AddTorque(torque);

	torque = exp(3.3671 * (joints[16]->GetJointPosition(0) - 0.2543))
		- exp(3.5743 * (joints[16]->GetJointPosition(0) - 2.1966));
	//joints[16]->AddTorque(torque);

	////////// �I(���F����) [12],[19] //////////
	// M = exp{8.7084(-�Ɓ~��/180 + 0.1201)} - exp{9.4336(�Ɓ~��/180 - 2.3187)}
	torque = exp(8.7084 * (joints[12]->GetJointPosition(0) + 0.1201))
		- exp(9.4336 * (joints[12]->GetJointPosition(0) - 2.3187));
	//joints[12]->AddTorque(torque);

	torque = exp(8.7084 * (joints[20]->GetJointPosition(0) + 0.1201))
		- exp(9.4336 * (joints[20]->GetJointPosition(0) - 2.3187));
	//joints[19]->AddTorque(torque);

	////////// ���(���F����) [14],[21] //////////
	// M = exp{2.8508(-�Ɓ~��/180 - 1.0185)} - exp{5.4930(�Ɓ~��/180 - 1.2374)}
	torque = exp(2.8508 * (joints[13]->GetJointPosition(0) - 1.0185))
		- exp(5.4930 * (joints[13]->GetJointPosition(0) - 1.2374));
	//joints[14]->AddTorque(torque);

	torque = exp(2.8508 * (joints[13]->GetJointPosition(0) - 1.0185))
		- exp(5.4930 * (joints[13]->GetJointPosition(0) - 1.2374));
	//joints[21]->AddTorque(torque);

	////////// �Ҋ֐�(���F����) [23],[31] //////////
	// M = exp{5.3923(-�Ɓ~��/180 + 0.1461)} - exp{2.2578(�Ɓ~��/180 - 0.1270)}
	torque = exp(5.3923 * (joints[23]->GetJointPosition(0) + 0.1461))
		- exp(2.2578 * (joints[23]->GetJointPosition(0) - 0.1270));
	//joints[23]->AddTorque(torque);

	torque = exp(5.3923 * (joints[31]->GetJointPosition(0) - 0.1461))
		- exp(2.2578 * (joints[31]->GetJointPosition(0) - 0.1270));
	//joints[31]->AddTorque(torque);

	////////// �G(���F����) [26],[34] //////////
	// M = exp{8.4262(-�Ɓ~��/180 + 0.2654)} - exp{5.9704(�Ɓ~��/180 - 2.1895)}
	torque = exp(8.4262 * (joints[26]->GetJointPosition(0) + 0.2654))
		- exp(5.9704 * (joints[26]->GetJointPosition(0) - 2.1895));
	//joints[26]->AddTorque(torque);

	torque = exp(8.4262 * (joints[34]->GetJointPosition(0) + 0.2654))
		- exp(5.9704 * (joints[34]->GetJointPosition(0) - 2.1895));
	//joints[34]->AddTorque(torque);

	////////// ����(���F����) [27],[35] //////////
	// M = exp{2.391(-�Ɓ~��/180 + 0.6804)} - exp{5.4401(�Ɓ~��/180 - 0.6706)}
	torque = exp(2.3912 * (joints[27]->GetJointPosition(0) + 0.6804))
		- exp(5.4401 * (joints[27]->GetJointPosition(0) - 0.6706));
	//joints[27]->AddTorque(-torque);

	torque = exp(2.3912 * (joints[35]->GetJointPosition(0) + 0.6804))
		- exp(5.4401 * (joints[35]->GetJointPosition(0) - 0.6706));
	//joints[35]->AddTorque(-torque);
}

Vec3f CRHuman::GetCOG(){
	Vec3f p;
	for(unsigned i=0; i<solids.size(); ++i){
		if( solids[i] != NULL){
			p += solids[i]->GetMass() * solids[i]->GetCenterPosition();
		}
	}
	return p / totalMass;
}
Vec3d CRHuman::GetCogVelocity(){
	Vec3f p;
	for(unsigned i=0; i<solids.size(); ++i){
		if( solids[i] != NULL){
			p += solids[i]->GetMass() * solids[i]->GetVelocity();
		}
	}
	return p / totalMass;
}
void CRHuman::CalcContactForce(){
	supportArea.Step();
	for(int i=0; i<supportObject.size(); ++i){
		supportObject[i].Step();
	}
}
void CRHuman::LoadX(const XHuman& xh){
	totalHeight = xh.totalHeight;
	totalMass = xh.totalMass;
}
void CRHuman::SaveX(XHuman& xh) const{
}

class CRHumanLoader : public FIObjectLoader<CRHuman>
{
public:
	CRHumanLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XHuman);
	}
	bool LoadData(FILoadScene* ctx, CRHuman* h){
		XHuman xh;
		ctx->docs.Top()->GetWholeData(xh);
		h->LoadX(xh);
		return true;
	}
};

class CRHumanSaver : public FIObjectSaver<CRHuman>{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CRHuman* h){
		XHuman xh;
		h->SaveX(xh);
		doc->SetWholeData(xh);		
	}
};

DEF_REGISTER_BOTH(CRHuman);

}	//	end namespace Spr
