#include "Creature.h"
#pragma hdrstop
// CRVirtualHuman.cpp: CRVirtualHuman クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRVirtualHuman.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;
SGOBJECTIMP(CRVirtualHuman, CRHuman);

CRVirtualHuman::CRVirtualHuman(){
}

CRVirtualHuman::~CRVirtualHuman(){
}

void CRVirtualHuman::SetBodyNum(){
	// バランスなどに必要なSolid,Jointの番号の登録
	for(unsigned i = 0; i < ANKLE_NUM; i++){
		supportSolidNum[i].clear();
		ankleJointNum[i].clear();
	}
	// 右足
	supportSolidNum[0].push_back(12);	// 踵
	supportSolidNum[0].push_back(13);	// 爪先
	ankleJointNum[0].push_back(27);		// joRAnkle1(X)
	ankleJointNum[0].push_back(28);		// joRAnkle2(Z)
	ankleJointNum[0].push_back(29);		// joRAnkle3(-Y)

	// 左足
	supportSolidNum[1].push_back(16);	// 踵
	supportSolidNum[1].push_back(17);	// 爪先
	ankleJointNum[1].push_back(35);		// joLAnkle1(X)
	ankleJointNum[1].push_back(36);		// joLAnkle2(-Z)
	ankleJointNum[1].push_back(37);		// joLAnkle3(Y)
}

void CRVirtualHuman::Draw(GRRender* render){
	balanceQDPR.Draw(render,&supportArea);
	rMovment.Draw(render);
	rMovmentPD.Draw(render);

	//　重心の描画
	render->SetModelMatrix(Affinef());
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,0,1)));
	render->SetLineWidth(10);
	Vec3f v = GetCOG();
	render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
/*
	// 全身のZMNP
	render->SetModelMatrix(Affinef());
	render->SetMaterial(GRMaterialData(Spr::Vec4f(0.4,0.5,0.8,1)));
	render->SetLineWidth(10);
	Vec3f x = balanceQDPR.cog_zmp;
	render->DrawDirect(Spr::GRRender::POINTS, &x , &x+1);
*/
/*	// 両足のZMNP
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
void CRVirtualHuman::OnKeyDown(unsigned int nChar,SGScene* scene){

	// 両手を前へ(肩を+xに回転)
	if(nChar == '3'){
		jointPids[9]->goal += Rad(1);
		jointPids[16]->goal += Rad(1);
	}
	// 両手を後ろへ(肩を+yに回転)
	else if(nChar == '4'){
		jointPids[10]->goal += Rad(1);
		jointPids[17]->goal += Rad(1);
	}
	// 両足を広げる
	else if(nChar == '5'){
		jointPids[24]->goal += Rad(1);
		jointPids[32]->goal += Rad(1);
		jointPids[28]->goal -= Rad(1);
		jointPids[36]->goal -= Rad(1);
	}
	// 前屈(腰を+Xに回転)
	else if(nChar == '6'){
		jointPids[0]->goal += Rad(1);
	}
	// 後屈(腰を-Xに回転)
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

void CRVirtualHuman::LoadDerivedModel(SGScene* scene){

	RegistNoUSeJoint();
	balanceQDPR.Load(scene,this);
	rMovment.Load();
	rMovmentPD.Load(this);

	stayPost[0].SetSolid(solids[supportSolidNum[0][0]],100.0,100.0);
	stayPost[1].SetSolid(solids[supportSolidNum[1][0]],100.0,100.0);
	stayPost[0].SetTarget(Quaterniond(1.0,0.0,0.0,0.0),Vec3f(),true);
	stayPost[1].SetTarget(Quaterniond(1.0,0.0,0.0,0.0),Vec3f(),true);


	// 到達運動するSolidの登録とk,b,到達運動時間の設定
	std::vector<int> joints0,joints1;
	for(int i = 9; i <=15; i++)
		joints0.push_back(i);
	for(int i = 16; i <=22; i++)
		joints1.push_back(i);
	rMovmentPD.RegistMovmentSolid(scene,solids[6],0.0005,0.3,0.5,joints0);
	rMovmentPD.RegistMovmentSolid(scene,solids[9],0.0002,0.3,0.3,joints1);

}

void CRVirtualHuman::RegistNoUSeJoint(){
	noUseJoint.clear();
	
#if 1	//	上半身を制御するなら1，下半身だけなら0
	// 上半身
	// (腰:X,Z,Y)
	//noUseJoint.push_back(0);
	//noUseJoint.push_back(1);
	//noUseJoint.push_back(2);

	// (胸:X,Z,Y）
	noUseJoint.push_back(3);
	noUseJoint.push_back(4);
	noUseJoint.push_back(5);

	// (首:X,Z,Y）
	noUseJoint.push_back(6);
	noUseJoint.push_back(7);
	noUseJoint.push_back(8);

	// (右肩:X,Z,Y）
	//noUseJoint.push_back(9);
	//noUseJoint.push_back(10);
	noUseJoint.push_back(11);

	// (右肘:X,Y）
	noUseJoint.push_back(12);
	noUseJoint.push_back(13);

	//	(右手首:Z,X)
	noUseJoint.push_back(14);
	noUseJoint.push_back(15);

	// (左肩:X,Z,Y）
	//noUseJoint.push_back(16);
	//noUseJoint.push_back(17);
	noUseJoint.push_back(18);

	// (左肘:X,Y）
	noUseJoint.push_back(19);
	noUseJoint.push_back(20);

	//	(左手首:Z,X)
	noUseJoint.push_back(21);
	noUseJoint.push_back(22);

#endif
	// 下半身
	// (右股関節:X,Z,Y）
	//noUseJoint.push_back(23);
	//noUseJoint.push_back(24);
	//noUseJoint.push_back(25);

	// (右膝:X）
	//noUseJoint.push_back(26);

	// (左股関節:X,Z,Y）
	//noUseJoint.push_back(31);
	//noUseJoint.push_back(32);
	//noUseJoint.push_back(33);

	// (左膝:X）
	//noUseJoint.push_back(34);
	
	// 以下は必須
	//	(右足首)
	//noUseJoint.push_back(27);
	//noUseJoint.push_back(28);
	//noUseJoint.push_back(29);
	//	(右土踏まず)
	//noUseJoint.push_back(30);

	//	(左足首)
	//noUseJoint.push_back(35);
	//noUseJoint.push_back(36);
	//noUseJoint.push_back(37);

	//	(左土踏まず)
	//noUseJoint.push_back(38);
}

void CRVirtualHuman::MaintainBalance(SGScene* scene){

	// 安定領域計算
	for(unsigned i = 0; i < supportObject.size(); i++){
		supportObject[i].Step();
	}
	supportArea.Step();

	// 腰がある角度を越えたら、後は転倒するのみ
	if(balanceQDPR.BalanceCheck(this)){
		// 二次計画法でバランスをとる
		balanceQDPR.Step(scene,this);
	}
}

void CRVirtualHuman::ReachingMovment(SGScene* scene){
	//rMovment.Step(scene,this);
	rMovmentPD.Step(scene);
}
void CRVirtualHuman::RegistSupportParts(UTRef<SGScene> scene){

	if(IsLoaded()){
		UTRef<CRContactDetector> contacts[2];

		// 体を支持するオブジェクト(右足,左足)の数だけCRContactDetectorとCRSupportAreaを作る
		supportObject.resize(2);

		// 右足で地面と接触するSolid
		contacts[0] = new CRContactDetector;
		contacts[0]->AddSubject(solids[supportSolidNum[0][0]]->GetFrame());	// 踵
		contacts[0]->AddSubject(solids[supportSolidNum[0][1]]->GetFrame());	// 爪先

		// 左足で地面と接触するSolid
		contacts[1] = new CRContactDetector;
		contacts[1]->AddSubject(solids[supportSolidNum[1][0]]->GetFrame());	// 踵
		contacts[1]->AddSubject(solids[supportSolidNum[1][1]]->GetFrame());	// 爪先

		for(unsigned i = 0; i < 2; i++){
			ExceptContactParts(contacts[i]);
			contacts[i]->Loaded(scene);
		}

		for(unsigned i = 0; i < 2; i++){
			// 片足ずつ
			supportObject[i].SetContacts(contacts[i]);
			// 体全体なので両足をセットする
			supportArea.SetContacts(contacts[i]);
		}
	}
}

void CRVirtualHuman::ExceptContactParts(CRContactDetector* contact){
	for(unsigned i = 0; i < solids.size(); i++){
		if(!(solids[i] == NULL)){
			contact->AddExcept(solids[i]->GetFrame());
		}
	}
}


float CRVirtualHuman::GetChildMass(PHJointBase* j){
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
void CRVirtualHuman::OffSpring(){
	//JointPIDMul(jointPids[9], 0.01f, 1.0f);
	//JointPIDMul(jointPids[16], 0.01f, 1.0f);
//	JointPIDMul(jointPids[26], 0.09f, 1.0f);
//	JointPIDMul(jointPids[34], 0.09f, 1.0f);

	//　肩を柔らかくする(右)
	for(int i = 9; i <= 11; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.05f, 0.05f);
	}

	//　肩を柔らかくする(左)
	for(int i = 16; i <= 18; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.05f, 0.05f);
	}
	
	// 肘を柔らかくする(右)
	for(int i = 12; i <= 13; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.01f, 0.5f);
	}

	// 肘を柔らかくする(左)
	for(int i = 19; i <= 20; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.01f, 0.5f);
	}
	/*
	// 腰
	for(int i = 0; i <= 2; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.05f, 0.5f);
	}
*/
}

void CRVirtualHuman::SetJointSpring(float dt){
	const float SAFETYRATE = 0.001f;
	float k = 0.2f * SAFETYRATE;
	float b = 0.6f*10 * SAFETYRATE;
	for(int i=0; i<jointPids.size(); ++i){
		if(jointPids[i] != NULL){
			float mass = GetChildMass(joints[i]);
			jointPids[i]->proportional = k * 2 * mass / (dt*dt);
			jointPids[i]->differential = b * mass / dt;
			jointPids[i]->integral = k * 2 * mass / (dt*dt) / 5000.0f;
		}
	}

	/*
	// 手首を硬くする(右)
	if(jointPids[14] != NULL)
			JointPIDMul(jointPids[14], 10.0f, 10.0f);
	if(jointPids[15] != NULL)
			JointPIDMul(jointPids[15], 10.0f, 10.0f);
	// 手首を硬くする(左)
	if(jointPids[21] != NULL)
			JointPIDMul(jointPids[21], 10.0f, 10.0f);
	if(jointPids[22] != NULL)
			JointPIDMul(jointPids[22], 10.0f, 10.0f);
	*/

	//　膝を柔らかくする
	//JointPIDMul(jointPids[26], 0.1f, 0.8f);
	//JointPIDMul(jointPids[34], 0.1f, 0.8f);


	//JointPIDMul(jointPids[9], 0.01f, 0.01f);
	//JointPIDMul(jointPids[16], 0.01f, 0.01f);

	//　肩を柔らかくする(右)
	for(int i = 9; i <= 11; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.9f, 1.0f);
	}

	//　肩を柔らかくする(左)
	for(int i = 16; i <= 18; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.8f, 1.0f);
	}

	// 肘を柔らかくする(右)
	for(int i = 12; i <= 13; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.1f, 1.0f);
	}

	// 肘を柔らかくする(左)
	for(int i = 19; i <= 20; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.1f, 1.0f);
	}

	// 爪先を硬くする(右足)
	if(jointPids[30] != NULL)
			JointPIDMul(jointPids[30], 10.0f, 10.0f);
	
	// 爪先を硬くする(左足)
	if(jointPids[38] != NULL)
			JointPIDMul(jointPids[38], 10.0f, 10.0f);

}

void CRVirtualHuman::SetJointInitAngle(){
	CRHuman::SetJointInitAngle();

	float angle = Rad(5);

	/*
	// 上半身
	((PHJoint1D*)joints[9])->position = jointPids[9]->goal = 5*angle;
	((PHJoint1D*)joints[12])->position = jointPids[12]->goal = 20*angle;

	((PHJoint1D*)joints[16])->position = jointPids[16]->goal = 5*angle;
	((PHJoint1D*)joints[19])->position = jointPids[19]->goal = 20*angle;
	*/

	// 下半身
	((PHJoint1D*)joints[23])->position = jointPids[23]->goal = angle;
	((PHJoint1D*)joints[26])->position = jointPids[26]->goal = 2*angle;
	((PHJoint1D*)joints[27])->position = jointPids[27]->goal = angle;

	((PHJoint1D*)joints[31])->position = jointPids[31]->goal = angle;
	((PHJoint1D*)joints[34])->position = jointPids[34]->goal = 2*angle;
	((PHJoint1D*)joints[35])->position = jointPids[35]->goal = angle;

}


DEF_RECORD(XVirtualHuman,{
	GUID Guid(){ return WBGuid("272E1906-9D5D-4c62-913F-EA05BDD3E7F2"); }
});
class CRVirtualHumanLoader : public FIObjectLoader<CRVirtualHuman>{
public:
	CRVirtualHumanLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XVirtualHuman);
	}
	bool LoadData(FILoadScene* ctx, CRVirtualHuman* h){
		return true;
	}
};
class CRVirtualHumanSaver : public FIObjectSaver<CRVirtualHuman>{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CRVirtualHuman* h){
	}
};
DEF_REGISTER_BOTH(CRVirtualHuman);

}		// end namespace