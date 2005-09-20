#include "Stdafx.h"
#include "TForceFile.h"
#include <Graphics/GRMaterial.h>
#include <sstream>

#include <float.h>
using namespace Spr;

inline bool IsValid(const Vec3f& v){
	return v.square() < Square(900);
}

//////////////////////////////////////////////////////////////////////
// \’z/Á–Å
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
			if(i == 0){
				forceDataL.push_back(v);
			}
			if(i == 1){
				forceDataR.push_back(v);
			}
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

/*
bool TForceFile::TForceRecord::Load(std::istream& isAll){
	char line[10*1024];
	isAll.getline(line, sizeof(line));
	std::istringstream is(line, strlen(line));
	char ch;
	is >> ch;
	if (!is.good()) return false;

	time = LoadField(is);
	for(int i = 0; i < 2; i++){
		Vec3f v;
		for(int i = 0; i < 3; i++){
			v[i] = LoadField(is);
		}
		if(is.good()){
			if(i == 0){
				forceDataL.push_back(v);
			}
			if(i == 1){
				forceDataR.push_back(v);
			}
		}
	}

	while(is.good()){
		Vec3f v;
		for(int i=0; i<3; ++i){
			v[i] = LoadField(is);
		}
		if (is.good()){
			torqueData.push_back(v);
		}
	}
	is.clear();
	return true;
}
*/
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

	int c = (int)(time / dt);
	if (c >= jointTorque.size()) return false;

/*	for(int i=0; i<human.joints.size(); ++i){
		human.joints[i]->SetTorque(human.angles[c+1].jointTorque[i]);
		//human.joints[i]->SetTorque(jointTorque[c].torqueData[i]);
	}*/
		human.joRHip1->SetTorque(human.jojo[0][c+1]);
		human.joRHip2->SetTorque(human.jojo[1][c+1]);
		human.joRHip3->SetTorque(human.jojo[2][c+1]);
		human.joRKnee1->SetTorque(human.jojo[3][c+1]);
		human.joRAnkle1->SetTorque(human.jojo[4][c+1]);
		human.joLHip1->SetTorque(human.jojo[5][c+1]);
		human.joLHip2->SetTorque(human.jojo[6][c+1]);
		human.joLHip3->SetTorque(human.jojo[7][c+1]);
		human.joLKnee1->SetTorque(human.jojo[8][c+1]);
		human.joLAnkle1->SetTorque(human.jojo[9][c+1]);
		human.joLShin->SetTorque(human.jojo[10][c+1]);
		human.joRShin->SetTorque(human.jojo[11][c+1]);

		human.joWaist1->SetTorque(human.jojo[12][c+1]);
		human.joWaist2->SetTorque(human.jojo[13][c+1]);
		human.joWaist3->SetTorque(human.jojo[14][c+1]);
		human.joNeck1->SetTorque(human.jojo[15][c+1]);
		human.joNeck2->SetTorque(human.jojo[16][c+1]);
		human.joNeck3->SetTorque(human.jojo[17][c+1]);
		human.joRShoulder1->SetTorque(human.jojo[18][c+1]);
		human.joRShoulder2->SetTorque(human.jojo[19][c+1]);
		human.joRShoulder3->SetTorque(human.jojo[20][c+1]);
		human.joRElbow1->SetTorque(human.jojo[21][c+1]);
		human.joRWrist1->SetTorque(human.jojo[22][c+1]);
		human.joRWrist2->SetTorque(human.jojo[23][c+1]);

		human.joLShoulder1->SetTorque(human.jojo[24][c+1]);
		human.joLShoulder2->SetTorque(human.jojo[25][c+1]);
		human.joLShoulder3->SetTorque(human.jojo[26][c+1]);
		human.joLElbow1->SetTorque(human.jojo[27][c+1]);
		human.joLWrist1->SetTorque(human.jojo[28][c+1]);
		human.joLWrist2->SetTorque(human.jojo[29][c+1]);

		return true;
}