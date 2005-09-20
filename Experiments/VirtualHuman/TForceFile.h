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
	class TForceRecord{
	public:
		std::string type;
		float time;
		typedef std::vector<Spr::Vec3f> TData;
		TData forceDataR,forceDataL;
		std::vector<double> torqueData;
	
		bool Load(std::istream& is);
		float LoadField(std::istream& is);
		TForceRecord(){
			time = 0.0f;
			torqueData.clear();
		}
	};
	class TForceRecords: public std::vector<TForceRecord>{
	};

public:
	TForceFile(): bLoaded(false){}
	virtual ~TForceFile();

	TForceRecords jointTorque;									//　トルクのデータ
	bool bLoaded;
	bool IsLoaded(){ return bLoaded; }
	bool Load(std::istream& is);								//トルクファイルをロード
	bool SetVHTorque(THuman& human, float time, float dt);		//トルクＶＨにセット
};

#endif