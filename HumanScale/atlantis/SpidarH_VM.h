#ifndef SPIDARH_VM_H
#define SPIDARH_VM_H
//SPIDAR-H
/////////////////////////////////////////////
#include <His/HISpidar4quad.h> //LVRM
#include <Device/DRUsb20Simple.h>
//////////////////////////////////////
//#include <HIS/HISpidar4.h> //spring head
//////////////////////////////////////////
// 動作確認SPIDAR用 モータの取り付け座標
#define PX	0.3125f			//	x方向の辺の長さ/2 [m]
#define PY	0.23f			//	y方向の辺の長さ/2 [m]
#define PZ	0.265f			//	z方向の辺の長さ/2 [m]
#define MAXON	0.3776f
#define LPP		2.924062107079e-5f
const float K = 10;	// 5弱く 10強く
#define SpidarUpdateRate	2

namespace LVRM {;

/**	Spidar-H
	フレームのサイズやモータの符号を含む	*/

class SpidarH_VM: public HISpidar4quad{

public:
	///	デバイスのタイプ
//	HI_DEVICE_TYPE_DEF("Spidar4VM")

	DVDeviceManager devMan;
	HISpidar4quad h_spidar[2];
	SpidarH_VM();
	virtual bool Init();
	virtual void SetMinForce(int idx);

	virtual Vec3f GetForce(int idx);
	virtual Vec3f GetPos(int idx);
		
	virtual void Update(int idx);
	//SPIDAR-H
/////////////////////////////////////
  	bool bMinForce;
	// 状態遷移カウンタ
 	void reset_spidar();
    void spidarCheck();

	Vec3f convertEuclidToPole(Vec3f euclid);
	Vec3f convertPoleToEuclid(Vec3f pole);
};

}	//	namespace
#endif
