#ifndef FWFILEIOD3D_H
#define FWFILEIOD3D_H
#include "FWFileIO.h"

namespace Spr{;
class FILoadScene;
class FISaveScene;

/**	ファイル入出力．
	Direct3Dで描画するためのシーングラフをXファイルから作る．*/
class FWFileIOD3D{
protected:
	UTRef<FILoadScene> loader;
	UTRef<FISaveScene> saver;
public:
	///	ファイルをロードし，sceneに含まれるfrの子とする．
	virtual bool Load(const char* fn, SGScene* scene, SGObject* obj=0);
	///	sceneに含まれるfrをファイルにセーブする．
	virtual bool Save(const char* fn, SGScene* scene, SGObject* obj=0);
protected:
	virtual bool PrepareLoader();
	virtual bool PrepareSaver();
};

}
#endif
