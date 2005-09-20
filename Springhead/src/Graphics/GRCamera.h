#ifndef GRCAMERA_H
#define GRCAMERA_H
#include <SceneGraph/SGFrame.h>

namespace Spr{;

/// カメラのデータ
class GRCameraData{
public:
	void InitData();
	///	視点行列
	Affinef view;
	///	@name	射影行列の要素
	//@{
	/// スクリーンのサイズ
	Vec2f size;
	/// カメラからのスクリーンのずれ
	Vec2f center;
	/// カメラとクリッピング平面との距離
	float front, back;
	///
	GRCameraData();
	//@}
};
/// カメラの実装
class GRCamera:public SGObject{
public:
	SGOBJECTDEF(GRCamera);
	GRCameraData data;
	///	カメラ位置を示すフレーム
	UTRef<SGFrame> frPosture;
	///	参照フレームの追加
	bool AddChildObject(SGObject* obj, SGScene* s);
	virtual size_t NReferenceObjects(){ return frPosture ? 1 : 0;}
	virtual SGObject* ReferenceObject(size_t i){ return i==0 ? (SGObject*)frPosture : (SGObject*)NULL; }
};
class GRCameras:public std::vector< UTRef<GRCamera> >{
};


}
#endif
