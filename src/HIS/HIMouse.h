#ifndef HIMOUSE_H
#define HIMOUSE_H

#include "HIHapticDevice.h"
#include <Device/DVDeviceManager.h>

namespace Spr {;

class SPR_DLL HIMouse: public HIForceDevice6D{
public:
	/// 押されているボタンの状態を示すフラグ
	enum ButtonState{
		NONE,
		// #define MK_LBUTTON          0x0001
		LBUTTON = MK_LBUTTON,
		// #define MK_RBUTTON          0x0002
		RBUTTON = MK_RBUTTON,
		// #define MK_SHIFT            0x0004
		SHIFT   = MK_SHIFT,
		// #define MK_CONTROL          0x0008
		CONTROL = MK_CONTROL,
		// #define MK_MBUTTON          0x0010
		MBUTTON = MK_MBUTTON,
	} btnState;	

	/// 
	HIMouse(){ bGood = false; }
	virtual ~HIMouse(){}
	///	デバイスのタイプ
	HI_DEVICE_TYPE_DEF("Mouse")

	bool IsGood(){return bGood;}

	/// マウスの初期化
	bool Init();

	///	押されているボタンに応じてpos,oriを更新する。Shiftが押されているときはaxisを更新する。
	void Update(float dt);

	///	まとめて，メッセージを処理する場合
	bool PreviewMessage(void* m);
	///@name 個別のメッセージハンドラで呼び出す場合
	//@{
	///	キー処理
	bool OnKeyDown(unsigned nChar);
	/// ドラッグ時の処理
	void OnButtonMove(UINT state, int x, int y);
	/// ボタンを押したときの処理
	void OnButtonDown(int x, int y);
	/// ボタンを離したときの処理
	void OnButtonUp();
	///	ダブルクリック時の処理
	void OnDblClick(unsigned fwKeys, int x, int y);
	/// 左ダブルクリック時の処理(ポインタの姿勢を水平に初期化)
	void OnLButtonDClick();
	/// 右ダブルクリック時の処理(ポインタの位置を原点に初期化)
	void OnRButtonDClick();
	/// Shift+左ダブルクリック時の処理(基準座標軸の位置・姿勢を初期化)
	void OnShiftLButtonDClick();
	/// ホイールをまわしたときの処理(ポインタor基準座標軸をZ軸方向に水平移動)
	void OnWheel(UINT state, short dz);
	//@}

	///	位置の取得
	Vec3f GetPos(){ return pos; }
	///	力の取得
	Vec3f GetForce(){ return force; }
	///	トルクの取得
	Vec3f GetTorque(){ return torque; }
	///	角度の取得
	Quaternionf GetOri(){ return ori; }
	/// 基準座標軸の取得
	Affinef GetAxis(){ return axis; }

	/// スケールの設定
	void SetScale(float tp, float rp, float ta, float ra);

	/// 質量の設定
	void SetMass(float m){ mass = m; }
	/// 位置の設定
	void SetPos(Vec3f p){ pos = p; }
	/// 姿勢の設定
	void SetOri(Quaternionf o){ ori = o; }
	///	並進力・回転力の設定
	void SetForce(const Vec3f& f, const Vec3f& t){ force = f; torque = t; }
	void SetForce(const Vec3f& f){ force = f; }
	///	最低の力を設定する
	void SetMinForce(){ force = torque = Vec3f(); }	

	/// 基準座標軸の設定
	void SetAxis(Affinef a){ axis = a; }
	/// 初期化用座標軸の設定
	void SetAxisInit(Affinef ai){ axisInit = ai; }


	///	キャリブレーション(特に何もしない)
	bool Calib(){ return true; }

protected:
	// マウスがアクティブかどうかのフラグ
	bool bGood;
	// マウスの座標(old:前ステップ，new:現ステップ)
	int oldX, oldY, newX, newY;
	// マウスの座標と仮想世界とのスケール調整用パラメータ
	float scaleTransPointer, scaleRotPointer, scaleTransAxis, scaleRotAxis;

	// ポインタに関する変数
	// 質量
	float mass;
	// ポインタにかかる並進力
	Vec3f force;
	// ポインタにかかる回転力
	Vec3f torque;
	// ポインタの位置。Update()で更新される
	Vec3f pos;
	// ポインタの姿勢。Update()で更新される
	Quaternionf ori;

	// 基準座標軸(ドラッグ操作時はこの座標軸にもどづいて動かされる)
	Affinef axis;
	// 初期化用座標軸
	Affinef axisInit;
};

}	//	namespace Spr
#endif
