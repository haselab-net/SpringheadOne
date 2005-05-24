#include "Creature.h"
#pragma hdrstop
// CRBallVirtualHuman.cpp: CRBallVirtualHuman �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRBallVirtualHuman.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;
SGOBJECTIMP(CRBallVirtualHuman, CRBallHuman);

CRBallVirtualHuman::CRBallVirtualHuman(){
}

CRBallVirtualHuman::~CRBallVirtualHuman(){
}

void CRBallVirtualHuman::SetBodyNum(){
	// �o�����X�ȂǂɕK�v��Solid,Joint�̔ԍ��̓o�^
	for(unsigned i = 0; i < ANKLE_NUM; i++){
		supportSolidNum[i].clear();
		ankleJointNum[i].clear();
	}
	// �E��
	supportSolidNum[0].push_back(12);	// ��
	supportSolidNum[0].push_back(13);	// �ܐ�
	ankleJointNum[0].push_back(11);		// joRAnkle

	// ����
	supportSolidNum[1].push_back(16);	// ��
	supportSolidNum[1].push_back(17);	// �ܐ�
	ankleJointNum[1].push_back(15);		// joLAnkle
}

void CRBallVirtualHuman::Draw(GRRender* render){
/*
	balanceQDPR.Draw(render,&supportArea);
	rMovment.Draw(render);
	rMovmentPD.Draw(render);

	//�@�d�S�̕`��
	render->SetModelMatrix(Affinef());
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,0,1)));
	render->SetLineWidth(10);
	Vec3f v = GetCOG();
	render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
*/
/*
	// �S�g��ZMNP
	render->SetModelMatrix(Affinef());
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0.4,0.5,0.8,1)));
	render->SetLineWidth(10);
	Vec3f x = balanceQDPR.cog_zmp;
	render->DrawDirect(Spr::GRRender::POINTS, &x , &x+1);
*/
/*	// ������ZMNP
	render->SetModelMatrix(Affinef());
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,1,1)));
	render->SetLineWidth(10);
	Vec3f y = balanceQDPR.footR_zmp;
	render->DrawDirect(Spr::GRRender::POINTS, &y , &y+1);

	render->SetModelMatrix(Affinef());
	render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,1,1)));
	render->SetLineWidth(10);
	Vec3f z = balanceQDPR.footL_zmp;
	render->DrawDirect(Spr::GRRender::POINTS, &z , &z+1);
	*/
}
void CRBallVirtualHuman::OnKeyDown(unsigned int nChar,SGScene* scene){

	// �����O��(����+x�ɉ�])
	if(nChar == '3'){
		jointPids[9]->goal += Rad(1);
		jointPids[16]->goal += Rad(1);
	}
	// ���������(����+y�ɉ�])
	else if(nChar == '4'){
		jointPids[10]->goal += Rad(1);
		jointPids[17]->goal += Rad(1);
	}
	// �������L����
	else if(nChar == '5'){
		jointPids[24]->goal += Rad(1);
		jointPids[32]->goal += Rad(1);
		jointPids[28]->goal -= Rad(1);
		jointPids[36]->goal -= Rad(1);
	}
	// �O��(����+X�ɉ�])
	else if(nChar == '6'){
		jointPids[0]->goal += Rad(1);
	}
	// ���(����-X�ɉ�])
	else if(nChar == '7'){
		jointPids[0]->goal -= Rad(1);
	}
	else if (nChar == 'K'){
		jointPids[23]->goal += Rad(1);
		jointPids[26]->goal += 2*Rad(1);
		jointPids[27]->goal += Rad(1);

		jointPids[31]->goal += Rad(1);
		jointPids[34]->goal += 2*Rad(1);
		jointPids[35]->goal += Rad(1);
	}
}

void CRBallVirtualHuman::LoadDerivedModel(SGScene* scene){

	RegistNoUSeJoint();
	//balanceQDPR.Load(scene,this);
	//rMovment.Load();
	//rMovmentPD.Load(this);

	//stayPost[0].SetSolid(solids[supportSolidNum[0][0]],100.0,100.0);
	//stayPost[1].SetSolid(solids[supportSolidNum[1][0]],100.0,100.0);
	//stayPost[0].SetTarget(Quaterniond(1.0,0.0,0.0,0.0),Vec3f(),true);
	//stayPost[1].SetTarget(Quaterniond(1.0,0.0,0.0,0.0),Vec3f(),true);


	// ���B�^������Solid�̓o�^��k,b,���B�^�����Ԃ̐ݒ�
	std::vector<int> joints0,joints1;
	for(int i = 3; i <=5; i++)
		joints0.push_back(i);
	for(int i = 6; i <=8; i++)
		joints1.push_back(i);
	//rMovmentPD.RegistMovmentSolid(scene,solids[6],0.0005,0.3,0.5,joints0);
	//rMovmentPD.RegistMovmentSolid(scene,solids[9],0.0002,0.3,0.3,joints1);

}

void CRBallVirtualHuman::RegistNoUSeJoint(){
	noUseJoint.clear();
	
#if 1	//	�㔼�g�𐧌䂷��Ȃ�1�C�����g�����Ȃ�0
	// �㔼�g
	// (��:X,Z,Y)
	//noUseJoint.push_back(0);

	// (��:X,Z,Y�j
	noUseJoint.push_back(1);

	// (��:X,Z,Y�j
	noUseJoint.push_back(2);

	// (�E��:X,Z,Y�j
	noUseJoint.push_back(3);

	// (�E�I:X,Y�j
	noUseJoint.push_back(4);

	//	(�E���:Z,X)
	noUseJoint.push_back(5);

	// (����:X,Z,Y�j
	noUseJoint.push_back(6);

	// (���I:X,Y�j
	noUseJoint.push_back(7);

	//	(�����:Z,X)
	noUseJoint.push_back(8);

#endif
	// �����g
	// (�E�Ҋ֐�:X,Z,Y�j
	//noUseJoint.push_back(23);
	//noUseJoint.push_back(24);
	//noUseJoint.push_back(25);

	// (�E�G:X�j
	//noUseJoint.push_back(26);

	// (���Ҋ֐�:X,Z,Y�j
	//noUseJoint.push_back(31);
	//noUseJoint.push_back(32);
	//noUseJoint.push_back(33);

	// (���G:X�j
	//noUseJoint.push_back(34);
	
	// �ȉ��͕K�{
	//	(�E����)
	//noUseJoint.push_back(27);
	//noUseJoint.push_back(28);
	//noUseJoint.push_back(29);
	//	(�E�y���܂�)
	//noUseJoint.push_back(30);

	//	(������)
	//noUseJoint.push_back(35);
	//noUseJoint.push_back(36);
	//noUseJoint.push_back(37);

	//	(���y���܂�)
	//noUseJoint.push_back(38);
}

void CRBallVirtualHuman::MaintainBalance(SGScene* scene){

	// ����̈�v�Z
	for(unsigned i = 0; i < supportObject.size(); i++){
		supportObject[i].Step();
	}
	supportArea.Step();

	// ��������p�x���z������A��͓]�|����̂�
	/*
	if(balanceQDPR.BalanceCheck(this)){
		// �񎟌v��@�Ńo�����X���Ƃ�
		balanceQDPR.Step(scene,this);
	}
	*/
}

void CRBallVirtualHuman::ReachingMovment(SGScene* scene){
	//rMovment.Step(scene,this);
	//rMovmentPD.Step(scene);
}
void CRBallVirtualHuman::RegistSupportParts(UTRef<SGScene> scene){

	if(IsLoaded()){
		UTRef<CRContactDetector> contacts[2];

		// �̂��x������I�u�W�F�N�g(�E��,����)�̐�����CRContactDetector��CRSupportArea�����
		supportObject.resize(2);

		// �E���Œn�ʂƐڐG����Solid
		contacts[0] = new CRContactDetector;
		contacts[0]->AddSubject(solids[supportSolidNum[0][0]]->GetFrame());	// ��
		contacts[0]->AddSubject(solids[supportSolidNum[0][1]]->GetFrame());	// �ܐ�

		// �����Œn�ʂƐڐG����Solid
		contacts[1] = new CRContactDetector;
		contacts[1]->AddSubject(solids[supportSolidNum[1][0]]->GetFrame());	// ��
		contacts[1]->AddSubject(solids[supportSolidNum[1][1]]->GetFrame());	// �ܐ�

		for(unsigned i = 0; i < 2; i++){
			ExceptContactParts(contacts[i]);
			contacts[i]->Loaded(scene);
		}

		for(unsigned i = 0; i < 2; i++){
			// �Б�����
			supportObject[i].SetContacts(contacts[i]);
			// �̑S�̂Ȃ̂ŗ������Z�b�g����
			supportArea.SetContacts(contacts[i]);
		}
	}
}

void CRBallVirtualHuman::ExceptContactParts(CRContactDetector* contact){
	for(unsigned i = 0; i < solids.size(); i++){
		if(!(solids[i] == NULL)){
			contact->AddExcept(solids[i]->GetFrame());
		}
	}
}


float CRBallVirtualHuman::GetChildMass(PHJointBase* j){
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
void CRBallVirtualHuman::OffSpring(){
	//JointPIDMul(jointPids[9], 0.01f, 1.0f);
	//JointPIDMul(jointPids[16], 0.01f, 1.0f);
//	JointPIDMul(jointPids[26], 0.09f, 1.0f);
//	JointPIDMul(jointPids[34], 0.09f, 1.0f);

	/*
	//�@�����_�炩������(�E)
	for(int i = 9; i <= 11; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.05f, 0.05f);
	}

	//�@�����_�炩������(��)
	for(int i = 16; i <= 18; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.05f, 0.05f);
	}
	
	// �I���_�炩������(�E)
	for(int i = 12; i <= 13; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.01f, 0.5f);
	}

	// �I���_�炩������(��)
	for(int i = 19; i <= 20; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.01f, 0.5f);
	}
	*/
	/*
	// ��
	for(int i = 0; i <= 2; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.05f, 0.5f);
	}
*/
}

void CRBallVirtualHuman::SetJointSpring(float dt){
	const float SAFETYRATE = 0.001f;
	float k = 0.2f * SAFETYRATE;
	float b = 0.6f*10 * SAFETYRATE;
	for(int i=0; i<joints.size(); ++i){
		if(jointPids[i] != NULL){
			float mass = GetChildMass(joints[i]);
			jointPids[i]->proportional = k * 2 * mass / (dt*dt);
			jointPids[i]->differential = b * mass / dt;
			jointPids[i]->integral = k * 2 * mass / (dt*dt) / 5000.0f;
		}
		else if(jointBallPids[i] != NULL){
			float mass = GetChildMass(joints[i]);
			jointBallPids[i]->proportional = k * 2 * mass / (dt*dt);
			jointBallPids[i]->differential = b * mass / dt;
			jointBallPids[i]->integral = k * 2 * mass / (dt*dt) / 5000.0f;
		}
	}

	/*
	// �����d������(�E)
	if(jointBallPids[5] != NULL)
			JointBallPIDMul(jointBallPids[5], 10.0f, 10.0f);
	// �����d������(��)
	if(jointBallPids[8] != NULL)
			JointBallPIDMul(jointBallPids[8], 10.0f, 10.0f);
	*/

	//�@�G���_�炩������
	//JointPIDMul(jointPids[10], 0.1f, 0.8f);
	//JointPIDMul(jointPids[14], 0.1f, 0.8f);


	//�@�����_�炩������(�E)
	if(jointBallPids[3] != NULL){
		JointBallPIDMul(jointBallPids[3], 0.9f, 1.0f);
	}

	//�@�����_�炩������(��)
	if(jointBallPids[6] != NULL){
		JointBallPIDMul(jointBallPids[6], 0.9f, 1.0f);
	}

	// �I���_�炩������(�E)
	if(jointPids[4] != NULL){
		JointPIDMul(jointPids[4], 0.1f, 1.0f);
	}

	// �I���_�炩������(��)
	if(jointPids[7] != NULL){
			JointPIDMul(jointPids[7], 0.1f, 1.0f);
	}

	// �ܐ���d������(�E��)
	if(jointPids[12] != NULL)
			JointPIDMul(jointPids[12], 10.0f, 10.0f);
	
	// �ܐ���d������(����)
	if(jointPids[16] != NULL)
			JointPIDMul(jointPids[16], 10.0f, 10.0f);

}

void CRBallVirtualHuman::SetJointInitAngle(){
	CRBallHuman::SetJointInitAngle();

	float angle = Rad(5);

	/*
	// �㔼�g
	((PHJoint1D*)joints[9])->position = jointPids[9]->goal = 5*angle;
	((PHJoint1D*)joints[12])->position = jointPids[12]->goal = 20*angle;

	((PHJoint1D*)joints[16])->position = jointPids[16]->goal = 5*angle;
	((PHJoint1D*)joints[19])->position = jointPids[19]->goal = 20*angle;

	// �����g
	((PHJoint1D*)joints[23])->position = jointPids[23]->goal = angle;
	((PHJoint1D*)joints[26])->position = jointPids[26]->goal = 2*angle;
	((PHJoint1D*)joints[27])->position = jointPids[27]->goal = angle;

	((PHJoint1D*)joints[31])->position = jointPids[31]->goal = angle;
	((PHJoint1D*)joints[34])->position = jointPids[34]->goal = 2*angle;
	((PHJoint1D*)joints[35])->position = jointPids[35]->goal = angle;
	*/
}


DEF_RECORD(XBallVirtualHuman,{
	GUID Guid(){ return WBGuid("272E1906-9D5D-4c62-913F-EA05BDD3E7F2"); }
});
class CRBallVirtualHumanLoader : public FIObjectLoader<CRBallVirtualHuman>{
public:
	CRBallVirtualHumanLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XBallVirtualHuman);
	}
	bool LoadData(FILoadScene* ctx, CRBallVirtualHuman* h){
		return true;
	}
};
class CRBallVirtualHumanSaver : public FIObjectSaver<CRBallVirtualHuman>{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CRBallVirtualHuman* h){
	}
};
DEF_REGISTER_BOTH(CRBallVirtualHuman);

}		// end namespace