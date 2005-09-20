// CRVH.cpp: CRVH クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRVH.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;
SGOBJECTIMP(CRVH, CRHuman);

CRVH::CRVH()
{

}

CRVH::~CRVH()
{

}


bool CRVH::Connect(UTRef<SGScene> scene){
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
	ConnectSolid("soRFoot", scene);

	ConnectSolid("soLThigh", scene);
	ConnectSolid("soLLThigh", scene);
	ConnectSolid("soLFoot", scene);

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

	ConnectJoint("joLHip1", scene);
	ConnectJoint("joLHip2", scene);
	ConnectJoint("joLHip3", scene);
	ConnectJoint("joLKnee", scene);
	ConnectJoint("joLAnkle1", scene);
	ConnectJoint("joLAnkle2", scene);
	ConnectJoint("joLAnkle3", scene);

	for(int i=0; i<joints.size(); ++i){
		jointPids.push_back(PHJointPid::Find((PHJoint1D*)joints[i], scene));
	}
	return bLoaded;
}

void CRVH::SetJointInfo(){
	// 腰[0] (-X)
	jinfo[0].rangeMin	= -51.00f;
	jinfo[0].rangeMax	= 102.65f;
	jinfo[0].axis		= -1.0;
	jinfo[0].initPos	= 0.0f;

	// 腰[1] (+Z) 可動域データ無し
	jinfo[1].rangeMin	= -10.00f;
	jinfo[1].rangeMax	= 10.00f;
	jinfo[1].axis		= 1.0;
	jinfo[1].initPos	= 0.0f;

	// 腰[2] (-Y)
	jinfo[2].rangeMin	= -49.00f;
	jinfo[2].rangeMax	= 49.00f;
	jinfo[2].axis		= -1.0;
	jinfo[2].initPos	= 0.0f;

	// 胸[3] (-X) 可動域データ無し
	jinfo[3].rangeMin	= -5.00f;
	jinfo[3].rangeMax	= 5.00f;
	jinfo[3].axis		= -1.0;
	jinfo[3].initPos	= 0.0f;

	// 胸[4] (+Z) 可動域データ無し
	jinfo[4].rangeMin	= -5.00f;
	jinfo[4].rangeMax	= 5.00f;
	jinfo[4].axis		= 1.0;
	jinfo[4].initPos	= 0.0f;

	// 胸[5] (-Y) 可動域データ無し
	jinfo[5].rangeMin	= -5.00f;
	jinfo[5].rangeMax	= 5.00f;
	jinfo[5].axis		= -1.0;
	jinfo[5].initPos	= 0.0f;

	// 首[6] (-X)
	jinfo[6].rangeMin	= -101.00f;
	jinfo[6].rangeMax	= 70.00f;
	jinfo[6].axis		= -1.0;
	jinfo[6].initPos	= 0.0f;

	// 首[7] (+Z)
	jinfo[7].rangeMin	= -55.00f;
	jinfo[7].rangeMax	= 59.35f;
	jinfo[7].axis		= 1.0;
	jinfo[7].initPos	= 0.0f;

	// 首[8] (-Y)
	jinfo[8].rangeMin	= -78.00f;
	jinfo[8].rangeMax	= 82.00f;
	jinfo[8].axis		= -1.0;
	jinfo[8].initPos	= 0.0f;

	// 右肩[9] (+X)
	jinfo[9].rangeMin	= -66.00f;
	jinfo[9].rangeMax	= 186.00f;
	jinfo[9].axis		= 1.0;
	jinfo[9].initPos	= 0.0f;

	// 右肩[10] (+Z)
	jinfo[10].rangeMin	= -60.00f;
	jinfo[10].rangeMax	= 164.40f;
	jinfo[10].axis		= 1.0;
	jinfo[10].initPos	= 0.0f;

	// 右肩[11] (-Y) 可動域データ無し
	jinfo[11].rangeMin	= -90.00f;
	jinfo[11].rangeMax	= 90.00f;
	jinfo[11].axis		= -1.0;
	jinfo[11].initPos	= 0.0f;

	// 右肘[12] (+X)
	jinfo[12].rangeMin	= -26.00f;
	jinfo[12].rangeMax	= 154.00f;
	jinfo[12].axis		= 1.0;
	jinfo[12].initPos	= 0.0f;

	// 右肘[13] (-Y) 可動域データ無し
	jinfo[13].rangeMin	= -90.00f;
	jinfo[13].rangeMax	= 90.00f;
	jinfo[13].axis		= -1.0;
	jinfo[13].initPos	= 0.0f;

	// 右手首[14] (-Z)
	jinfo[14].rangeMin	= -96.25f;
	jinfo[14].rangeMax	= 100.05f;
	jinfo[14].axis		= -1.0;
	jinfo[14].initPos	= 0.0f;

	// 右手首[15] (+X)
	jinfo[15].rangeMin	= -64.00f;
	jinfo[15].rangeMax	= 48.00f;
	jinfo[15].axis		= 1.0;
	jinfo[15].initPos	= 0.0f;

	// 左肩[16] (+X)
	jinfo[16].rangeMin  = -66.00f;
	jinfo[16].rangeMax  = 186.00f;
	jinfo[16].axis		= 1.0;
	jinfo[16].initPos	= 0.0f;

	// 左肩[17] (-Z)
	jinfo[17].rangeMin	= -60.00f;
	jinfo[17].rangeMax	= 164.40f;
	jinfo[17].axis		= -1.0;
	jinfo[17].initPos	= 0.0f;

	// 左肩[18] (Y) 可動域データ無し
	jinfo[18].rangeMin	= -90.00f;
	jinfo[18].rangeMax	= 90.00f;
	jinfo[18].axis		= 1.0;
	jinfo[18].initPos	= 0.0f;

	// 左肘[19] (+X)
	jinfo[19].rangeMin	= -26.00f;
	jinfo[19].rangeMax	= 154.00f;
	jinfo[19].axis		= 1.0;
	jinfo[19].initPos	= 0.0f;

	// 左肘[20] (Y) 可動域データ無し
	jinfo[20].rangeMin	= -90.00f;
	jinfo[20].rangeMax	= 90.00f;
	jinfo[20].axis		= 1.0;
	jinfo[20].initPos	= 0.0f;

	// 左手首[21] (+Z)
	jinfo[21].rangeMin	= -96.25f;
	jinfo[21].rangeMax	= 100.05f;
	jinfo[21].axis		= 1.0;
	jinfo[21].initPos	= 0.0f;

	// 左手首[22] (+X)
	jinfo[22].rangeMin	= -64.00f;
	jinfo[22].rangeMax	= 48.00f;
	jinfo[22].axis		= 1.0;
	jinfo[22].initPos	= 0.0f;

	// 右股[23] (+X)
	jinfo[23].rangeMin	= -36.15f;
	jinfo[23].rangeMax	= 128.00f;
	jinfo[23].axis		= 1.0;
	jinfo[23].initPos	= 0.0f;

	// 右股[24] (+Z)
	jinfo[24].rangeMin	= -33.00f;
	jinfo[24].rangeMax	= 69.00f;
	jinfo[24].axis		= 1.0;
	jinfo[24].initPos	= 0.0f;

	// 右股[25] (-Y) 可動域データ無し
	jinfo[25].rangeMin	= -90.00f;
	jinfo[25].rangeMax	= 90.00f;
	jinfo[25].axis		= -1.0;
	jinfo[25].initPos	= 0.0f;

	// 右膝[26] (-X)
	jinfo[26].rangeMin	= -8.00f;
	jinfo[26].rangeMax	= 164.00f;
	jinfo[26].axis		= -1.0;
	jinfo[26].initPos	= 0.0f;

	// 右足首[27] (X)
	jinfo[27].rangeMin	= -39.00f;
	jinfo[27].rangeMax	= 73.00f;
	jinfo[27].axis		= 1.0;
	jinfo[27].initPos	= 0.0f;

	// 右足首[28] (Z)
	jinfo[28].rangeMin	= -20.00f;
	jinfo[28].rangeMax	= 20.00f;
	jinfo[28].axis		= 1.0;
	jinfo[28].initPos	= 0.0f;

	// 右足首[29] (-Y) 可動域データ無し
	jinfo[29].rangeMin	= -20.00f;
	jinfo[29].rangeMax	= 20.00f;
	jinfo[29].axis		= -1.0;
	jinfo[29].initPos	= 0.0f;

	// 左股[30] (+X)
	jinfo[30].rangeMin	= -36.15f;
	jinfo[30].rangeMax	= 128.00f;
	jinfo[30].axis		= 1.0;
	jinfo[30].initPos	= 0.0f;

	// 左股[31] (-Z)
	jinfo[31].rangeMin	= -33.00f;
	jinfo[31].rangeMax	= 69.00f;
	jinfo[31].axis		= -1.0;
	jinfo[31].initPos	= 0.0f;
	
	// 左股[32] (Y) 可動域データ無し
	jinfo[32].rangeMin	= -90.00f;
	jinfo[32].rangeMax	= 90.00f;
	jinfo[32].axis		= 1.0;
	jinfo[32].initPos	= 0.0f;

	// 左膝[33] (-X)
	jinfo[33].rangeMin	= -8.00f;
	jinfo[33].rangeMax	= 164.00f;
	jinfo[33].axis		= -1.0;
	jinfo[33].initPos	= 0.0f;

	// 左足首[34] (-X)
	jinfo[34].rangeMin	= -39.00f;
	jinfo[34].rangeMax	= 73.00f;
	jinfo[34].axis		= -1.0;
	jinfo[34].initPos	= 0.0f;

	// 左足首[35] (-Z)
	jinfo[35].rangeMin	= -20.00f;
	jinfo[35].rangeMax	= 20.00f;
	jinfo[35].axis		= -1.0;
	jinfo[35].initPos	= 0.0f;

	// 左足首[36] (+Y) 可動域データ無し
	jinfo[36].rangeMin	= -20.00f;
	jinfo[36].rangeMax	= 20.00f;
	jinfo[36].axis		= 1.0;
	jinfo[36].initPos	= 0.0f;

}

void CRVH::SetSolidInfo(){
	// その内ファイルからロードするようにしよう
	// soWaist(腰)
	sinfo[0].scaleRatio = Vec3f(0.190, 0.0768, 0.137) / 2;
	sinfo[0].massRatio  = 0.17f;

	// soAbdomen(腹)
	sinfo[1].scaleRatio = Vec3f(0.151, 0.0201, 0.108) / 2;
	sinfo[1].massRatio  = 0.028f;

	// soChest(胸)
	sinfo[2].scaleRatio = Vec3f(0.184, 0.202, 0.124) / 2;
	sinfo[2].massRatio  = 0.252f;

	// soHead(頭)
	sinfo[3].scaleRatio = Vec3f(0.0949, 0.140, 0.111) / 2;
	sinfo[3].massRatio  = 0.07f;

	// soRUArm(右上腕)
	sinfo[4].scaleRatio = Vec3f(0.0487, 0.181, 0.0487) / 2;
	sinfo[4].massRatio  = 0.04f;

	// soRFArm(右前腕)
	sinfo[5].scaleRatio = Vec3f(0.046, 0.141, 0.046) / 2;
	sinfo[5].massRatio  = 0.02f;

	// soRHand(右手)
	sinfo[6].scaleRatio = Vec3f(0.0169, 0.106, 0.0479) / 2;
	sinfo[6].massRatio  = 0.01f;

	// soLUArm(左上腕)
	sinfo[7].scaleRatio = Vec3f(0.0487, 0.181, 0.0487) / 2;
	sinfo[7].massRatio  = 0.04f;

	// soLFArm(左前腕)
	sinfo[8].scaleRatio = Vec3f(0.046, 0.141, 0.046) / 2;
	sinfo[8].massRatio  = 0.02f;

	// soLHand(左手)
	sinfo[9].scaleRatio = Vec3f(0.0169, 0.106, 0.0479) / 2;
	sinfo[9].massRatio  = 0.01f;

	// soRThigh(右大腿)
	sinfo[10].scaleRatio = Vec3f(0.0961, 0.233, 0.0961) / 2;
	sinfo[10].massRatio  = 0.1f;

	// soRLThigh(右下腿)
	sinfo[11].scaleRatio = Vec3f(0.0664, 0.233, 0.0664) / 2;
	sinfo[11].massRatio  = 0.05f;

	// soRFoot(右足)
	sinfo[12].scaleRatio = Vec3f(0.0582, 0.0408, 0.1484) / 2;
	sinfo[12].massRatio  = 0.02f;

	// soLThigh(左大腿)
	sinfo[13].scaleRatio = Vec3f(0.0961, 0.233, 0.0961) / 2;
	sinfo[13].massRatio  = 0.1f;

	// soLLThigh(左下腿)
	sinfo[14].scaleRatio = Vec3f(0.0664, 0.233, 0.0664) / 2;
	sinfo[14].massRatio  = 0.05f;

	// soLFoot(左足)
	sinfo[15].scaleRatio = Vec3f(0.0582, 0.0408, 0.1484) / 2;
	sinfo[15].massRatio  = 0.02f;

	//for(int i= 0; i < solids.size(); ++i){
	for(int i= 0; i < 15; ++i){
		sinfo[i].scale = sinfo[i].scaleRatio * totalHeight;
	}
}

void CRVH::SetBodyNum(){
	// バランスなどに必要なSolid,Jointの番号の登録
	for(unsigned i = 0; i < ANKLE_NUM; i++){
		supportSolidNum[i].clear();
		ankleJointNum[i].clear();
	}
	// 右足
	supportSolidNum[0].push_back(12);	// 足
	ankleJointNum[0].push_back(27);		// joRAnkle1(X)
	ankleJointNum[0].push_back(28);		// joRAnkle2(Z)
	ankleJointNum[0].push_back(29);		// joRAnkle3(-Y)

	// 左足
	supportSolidNum[1].push_back(15);	// 足
	ankleJointNum[1].push_back(34);		// joLAnkle1(X)
	ankleJointNum[1].push_back(35);		// joLAnkle2(-Z)
	ankleJointNum[1].push_back(36);		// joLAnkle3(Y)
}



void CRVH::RegistNoUSeJoint(){
	noUseJoint.clear();
	
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

	// 下半身
	// (右股関節:X,Z,Y）
	//noUseJoint.push_back(23);
	//noUseJoint.push_back(24);
	//noUseJoint.push_back(25);

	// (右膝:X）
	//noUseJoint.push_back(26);

	// (左股関節:X,Z,Y）
	//noUseJoint.push_back(30);
	//noUseJoint.push_back(31);
	//noUseJoint.push_back(32);

	// (左膝:X）
	//noUseJoint.push_back(33);
	
	// 以下は必須
	//	(右足首)
	//noUseJoint.push_back(27);
	//noUseJoint.push_back(28);
	//noUseJoint.push_back(29);

	//	(左足首)
	//noUseJoint.push_back(34);
	//noUseJoint.push_back(35);
	//noUseJoint.push_back(36);

}

void CRVH::RegistSupportParts(UTRef<SGScene> scene){

	if(IsLoaded()){
		UTRef<CRContactDetector> contacts[2];

		// 体を支持するオブジェクト(右足,左足)の数だけCRContactDetectorとCRSupportAreaを作る
		supportObject.resize(2);

		// 右足で地面と接触するSolid
		contacts[0] = new CRContactDetector;
		contacts[0]->AddSubject(solids[supportSolidNum[0][0]]->GetFrame());	// 足
		//contacts[0]->AddSubject(solids[supportSolidNum[0][1]]->GetFrame());	// 爪先

		// 左足で地面と接触するSolid
		contacts[1] = new CRContactDetector;
		contacts[1]->AddSubject(solids[supportSolidNum[1][0]]->GetFrame());	// 足
		//contacts[1]->AddSubject(solids[supportSolidNum[1][1]]->GetFrame());	// 爪先

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


void CRVH::SetJointSpring(float dt){
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
	for(int i = 9; i < 22; i++){
		if(jointPids[i] != NULL)
			JointPIDMul(jointPids[i], 0.1f, 0.1f);
	}
*/
}

void CRVH::SetJointInitAngle(){
	CRHuman::SetJointInitAngle();

	float angle = Rad(5);

	// 上半身
	/*
	((PHJoint1D*)joints[0])->position = jointPids[0]->goal = 0.2f;

	((PHJoint1D*)joints[9])->position = jointPids[9]->goal = 0.8f;
	((PHJoint1D*)joints[10])->position = jointPids[10]->goal = 0.0f;
	((PHJoint1D*)joints[11])->position = jointPids[11]->goal = 0.5f;
	((PHJoint1D*)joints[12])->position = jointPids[12]->goal = 2.5f;

	((PHJoint1D*)joints[16])->position = jointPids[16]->goal = 1.2f;
	((PHJoint1D*)joints[17])->position = jointPids[17]->goal = 0.0f;
	((PHJoint1D*)joints[18])->position = jointPids[18]->goal = 0.5f;
	((PHJoint1D*)joints[19])->position = jointPids[19]->goal = 2.3f;
	*/
	// 下半身
	((PHJoint1D*)joints[23])->position = jointPids[23]->goal = angle;
	((PHJoint1D*)joints[26])->position = jointPids[26]->goal = 2*angle;
	((PHJoint1D*)joints[27])->position = jointPids[27]->goal = angle;

	((PHJoint1D*)joints[30])->position = jointPids[30]->goal = angle;
	((PHJoint1D*)joints[33])->position = jointPids[33]->goal = 2*angle;
	((PHJoint1D*)joints[34])->position = jointPids[34]->goal = angle;

}


}		// end namespace