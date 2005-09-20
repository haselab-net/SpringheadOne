#include "Stdafx.h"
#include "TForceFile.h"
#include <Graphics/GRMaterial.h>
#include <sstream>
#include <fstream>

#include <float.h>
using namespace Spr;

inline bool IsValid(const Vec3f& v){
	return v.square() < Square(900);
}

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


TForceFile::~TForceFile()
{

}

/////////////////////////////////////////////////////////////////////

float TForceFile::TForceRecord::LoadField(std::istream& is){
	if (!is.good()){
		is.setf(std::ios::badbit);
		return 0;
	}
	float rv;
	is >> rv;
	return rv;
}
bool TForceFile::TForceRecord::Load(std::istream& isAll){
	char line[10*1024];
	isAll.getline(line, sizeof(line));
	std::istringstream is(line, strlen(line));

	if (!is.good()) return false;

	time = LoadField(is);
	for(int i = 0; i < 2; i++){
		Vec3f v;
		for(int i = 0; i < 3; i++){
			v[i] = LoadField(is);
		}
		if(is.good()){
			if(i == 0)forceDataL.push_back(v);
			if(i == 1)forceDataR.push_back(v);
		}
	}
	while(is.good()){
		double v;
		v = LoadField(is);
		torqueData.push_back(v);
	}
	is.clear();
	return true;
}

bool TForceFile::Load(std::istream& is){
	while(is.good()){
		TForceRecord rec;
		rec.Load(is);
		jointTorque.push_back(rec);
	}
	bLoaded = true;
	return true;
}

bool TForceFile::SetVHTorque(THuman& human, float time, float dt){
	if (!IsLoaded() || !human.IsLoaded()) return false;

	static int d = 0;
	int c = time / dt;
	if ( c == d && c != 0){
		c = d+1;
	}
	d = c;
	if (c >= jointTorque.size())return false;

	//�S�Ă̊֐߂Ƀg���N����͂���
	if(!human.checkPart){
		for(int i=0; i<human.joints.size(); ++i){
			//human.joints[i]->SetTorque(human.angles[c+1].jointTorque[i]);	//�t�@�C�������o���O�̃f�[�^
			human.joints[i]->SetTorque(jointTorque[c].torqueData[i]);		//�t�@�C������擾�����f�[�^
			//human.joints[i]->SetTorque(human.jojo[i][c]);					//�֐߂��璼�ړ����f�[�^
		}
	//�����̊֐߂ɂ����g���N����͂���
	}else{
		//�g���N���͂ɂ���֐߂����߂�B
		human.joLHip1->SetTorque(human.jojo[18][c]);
		//���ɂ����̂悤�ɂ��ē��͂���B
//		human.joWaist1->SetTorque(human.jojo[0][c]);
//		human.joWaist2->SetTorque(human.jojo[1][c]);
//		human.joWaist3->SetTorque(human.jojo[2][c]);
//		human.joNeck1->SetTorque(human.jojo[3][c]);
//		human.joNeck2->SetTorque(human.jojo[4][c]);
	}
	return true;
}