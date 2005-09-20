#ifndef HI_TURNTABLE_H
#define HI_TURNTABLE_H

#include <Device/DVDeviceManager.h>
#include <Device/DVDaBase.h>
#include <Device/DVOrientationBase.h>
#include <vector>

namespace LVRM {;
/**	ターンテーブルと足踏みセンサー
*/

//#define USING_SLC //ラベリングカメラを使うとき

class HITurnTable{
public:
	DVOrientationBase* oriSensor;
	DVDaBase* da;
	bool bControl;

	HITurnTable();
	virtual ~HITurnTable();
	
	bool Init(DVDeviceManager& dev);
	void Stop();
	void Start();
	void Step(float inner);
	void Reset();
	float GetAngle(){return angle;}
	float GetTurnVel(){ return turnVel; }
	float GetStopSpeed() { return stopspeed; }

	void SetVel(float vel){velocity = vel;} 
	void SetEyeAngle(float eyeAng){eyeAngle = eyeAng;}
	void SetIsWalking(bool b) { isWalking = b; }

	void PrintData(std::ostream& os) const;
	bool printTurnData;

private:
	float angle;
	float nowAngle , startAngle;
	float turnVel;

	float velocity;
	float stopspeed;
	float eyeAngle;
	bool isWalking;

	unsigned long dwTime, dwLastTime;

	float printOriAngle;
	float printPressureAngle;
	float printPressureAngle2;
};

}
#endif
