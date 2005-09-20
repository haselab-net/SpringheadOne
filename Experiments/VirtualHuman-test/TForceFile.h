// TForceFile.h: TForceFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef TFORCEFILE_H
#define TFORCEFILE_H

#include <Graphics/GRRender.h>
#include <Base/Affine.h>
#include <vector>
#include "THuman.h"

class TForceFile  
{
public:
	class TAngle1{
	public:
		Spr::Vec3f bodyVel;
		Spr::Quaternionf bodyOri;
		Spr::Vec3f bodyAngVel;
		Spr::Vec3f force;
		Spr::Vec3f torque;
		unsigned nAngle;
		float angle[30];
		float angVel[30];
		float jointTorque[30];
		TAngle1():nAngle(0){}
	};
	class TAngles: public std::vector<TAngle1>{
	};

	class TForceRecord{
	public:
		std::string type;
		float time;
		typedef std::vector<Spr::Vec3f> TData;
//		typedef std::vector<TData>;
		TData forceDataR,forceDataL;
		std::vector<double> torqueData;
	
		bool Load(std::istream& is);
		float LoadField(std::istream& is);

		TForceRecord():time(0.0f){}
	};
	class TForceRecords: public std::vector<TForceRecord>{
	};

public:
	TForceFile(): bLoaded(false){}
	virtual ~TForceFile();

	TForceRecords jointTorque;
	bool bLoaded;
	bool IsLoaded(){ return bLoaded; }
	bool Load(std::istream& is);
	bool SetVHTorque(THuman& human, float time, float dt);
};

#endif