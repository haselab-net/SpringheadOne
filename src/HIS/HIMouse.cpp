#include "HIS.h"
#pragma hdrstop
#include "HIMouse.h"

namespace Spr {;

HIOBJECTIMP(HIMouse, HIForceDevice6D);

bool HIMouse::Init(){
	oldX = oldY = newX = newY = 0;
	btnState = NONE;

	// デフォルトの設定
	scaleTransPointer = 0.01f;
	scaleRotPointer = 0.008f;
	scaleTransAxis = 0.03f;
	scaleRotAxis = 0.008f;
	mass = 0.25f;
	pos = Vec3f();
	ori = Quaternionf();
	axis.Pos() = Vec3f(0, 0, -10);
    axis.LookAt(Vec3f(), Vec3f(0,1,0));
	axis = axis.inv();
	axisInit = axis;
	// マウスがアクティブかどうかの検出方法がよくわからないのでとりあえずtrueにしてある
	bGood = true;	

//-> 関口による変更 (2005/1/05)
	SetViewSize(640, 480);
//<-

	return true;
}

void HIMouse::SetScale(float tp, float rp, float ta, float ra){
	scaleTransPointer = tp;
	scaleRotPointer = rp;
	scaleTransAxis = ta;
	scaleRotAxis = ra;
}

//-> 関口による変更 (2005/1/05)
void HIMouse::SetViewSize(float inWidth, float inHeight){
	//	VirtualTrackBallにはViewの大きさのパラメータが
	//	必須なため、それをセットするためのメンバ関数

	viewCenterX = inWidth / 2;
	viewCenterY = inHeight / 2;

	//	Viewの短軸側をVirtualTrackBallの半径とする
	float	d;
	if (viewCenterX < viewCenterY)
		d = viewCenterX;
	else
		d = viewCenterY;
	trackBallRadius = d * 1.0;
}

Vec3f HIMouse::GetTrackBallRotation(float inX, float inY){
	//	クリックされたポイントをVirtualTrackBall内部の
	//	点にマッピングするためのメンバ関数

	Vec3f	v;
	float	d;

	//	TrackBallの中心が原点となるようにし、なおかつ
	//	半径が1となるように座標を正規化
	v.x = 1.0 * (inX - viewCenterX) / trackBallRadius;
	v.y = -1.0 * (inY - viewCenterY) / trackBallRadius;
	v.z = 0.0;

	//	x, yが求まったのでzの決定
	d = v.x * v.x + v.y * v.y;
	if (d < 1.0)
	{
		v.z = sqrt(1.0 - d);
	}
	else
	{
		d = sqrt(d);
		v.x = v.x / d;
		v.y = v.y / d;
		v.z = 0;
	}

	return v;
}
//<-

void HIMouse::Update(float dt){

	// force,torqueに基づいてpos,oriを更新
	// もっときちんと計算したほうがいいかもしれない
	pos += force * dt * dt / mass /2;
	torque	= ori.inv() * torque;
	Vec3f drot;
	drot = torque * dt * dt / mass;
	float norm = drot.norm();
	if(norm > 1.0e-10){
		ori = Quaternionf::Rot(norm, drot) * ori;
		ori.unitize();
	}

	if(btnState != NONE){
		// 前ステップのマウスの位置との差
		float dx, dy;
		dx = float(newX - oldX);
		dy = float(newY - oldY);

		// ドラッグ操作によって動かす方向の基準
		Affinef afAxis = axis.inv();		

		if (btnState == (MK_SHIFT + MK_LBUTTON) ){

//-> 関口による変更 (2005/1/05)

			//	Virtual Track Ballの実装
			//		透視変換の影響を無視した（つまり平行投影を仮定）
			//		微妙にいんちきなVirtual Track Ballであるが、
			//		ちゃんと動いているように感じるのでよしとする。
			//		（おそらく平行投影を仮定したことによる誤差は
			//		無視できるくらいであろう...）
			Vec3f	oldRot = GetTrackBallRotation(oldX, oldY);
			Vec3f	newRot = GetTrackBallRotation(newX, newY);
			Vec3f	rotAxis, rotDirection;
			float	rotVelocity;

			rotDirection = newRot - oldRot;
			rotVelocity = rotDirection.norm();

			if (rotVelocity > 0.001)
			{
				rotAxis = cross(newRot, oldRot);
				rotAxis = afAxis.Rot() * rotAxis;
				rotAxis.unitize();

				afAxis = Affinef::Rot(rotVelocity * 1.0, rotAxis) * afAxis;
				axis = afAxis.inv();
			}

			//-> オリジナル
			// Shift + 左ドラッグのときは基準座標軸をx軸,y軸を中心にカメラを回転
			//afAxis = Affinef::Rot(dx*scaleRotAxis, afAxis.Ey()) * Affinef::Rot(dy*scaleRotAxis, afAxis.Ex()) * afAxis;
			//axis = afAxis.inv();
			//<- ここまで　（オリジナル）
//<-
		}
		else if (btnState == (MK_SHIFT + MK_MBUTTON) ){
			// Shift + 中ドラッグのときは基準座標軸をx-z平面で平行移動
			afAxis.Pos() += (-afAxis.Ex() * dx - afAxis.Ez() * dy) * scaleTransAxis;	
			axis = afAxis.inv();
		}
		else if (btnState == (MK_SHIFT + MK_LBUTTON + MK_RBUTTON) ) {
			// Shift + 左右ドラッグのときは基準座標軸のx-z平面で平行移動
			afAxis.Pos() += (-afAxis.Ex() * dx - afAxis.Ez() * dy) * scaleTransAxis;
			axis = afAxis.inv();
		}
		else if (btnState == (MK_SHIFT + MK_RBUTTON + MK_MBUTTON) ) {
			// Shift + 右中ドラッグのときは基準座標軸のy-z平面で平行移動
			afAxis.Pos() += (-afAxis.Ey() * dx - afAxis.Ez() * dy) * scaleTransAxis;
			axis = afAxis.inv();
		}
		else if (btnState == (MK_SHIFT + MK_RBUTTON) ){
			// Shift + 右ドラッグのときは基準座標軸をx-y平面で平行移動
			afAxis.Pos() += (-afAxis.Ex() * dx + afAxis.Ey() * dy) * scaleTransAxis;	
			axis = afAxis.inv();
		}
		else if (btnState == MK_LBUTTON) {
			// 左ドラッグのときは基準座標軸のx軸,y軸を中心にポインタを回転
			ori = Quaternionf::Rot(dx*scaleRotPointer, -afAxis.Ey()) * Quaternionf::Rot(dy*scaleRotPointer, -afAxis.Ex()) * ori;
		}
		else if (btnState == MK_MBUTTON) {
			// 中ドラッグのときは基準座標軸のx-z平面でポインタを平行移動
			Vec3f up(0,1,0);
			Vec3f ex = afAxis.Ex();
			ex = (ex - (ex*up)*ex).unit();
			Vec3f ez = afAxis.Ez();
			ez = (ez - (ez*up)*ez).unit();
			pos += (ex * dx + ez * dy) * scaleTransPointer;
		}
		else if (btnState == MK_LBUTTON + MK_RBUTTON) {
			// 左右ドラッグのときは基準座標軸のx-z平面でポインタを平行移動
			Vec3f up(0,1,0);
			Vec3f ex = afAxis.Ex();
			ex = (ex - (ex*up)*ex).unit();
			Vec3f ez = afAxis.Ez();
			ez = (ez - (ez*up)*ez).unit();
			pos += (ex * dx + ez * dy) * scaleTransPointer;
		}
		else if (btnState == MK_RBUTTON + MK_MBUTTON) {
			// 右中ドラッグのときは基準座標軸のy-z平面でポインタを平行移動
			Vec3f up(0,1,0);
			Vec3f ez = afAxis.Ez();
			ez = (ez - (ez*up)*ez).unit();
			pos += (up * dx + ez * dy) * scaleTransPointer;
		}
		else if (btnState == MK_RBUTTON) {
			Vec3f up(0,1,0);
			Vec3f ex = afAxis.Ex();
			ex = (ex - (ex*up)*ex).unit();
			// 右ドラッグのときは基準座標軸のx-y平面でポインタを平行移動
			pos += (ex * dx - up * dy) * scaleTransPointer;
		}

		oldX = newX;
		oldY = newY;

		//DSTR << "(x, y, z) = (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	}
	HIForceDevice6D::Update(dt);
}

bool HIMouse::PreviewMessage(void* m){
	MSG* msg = (MSG*)m;
	switch (msg->message){
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:{
		int x = (short)LOWORD(msg->lParam);  // horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);  // vertical position of cursor 
		OnButtonDown(x, y);
		SetCapture(msg->hwnd);
		}break;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		OnButtonUp();
		break;
	case WM_MOUSEMOVE:{
		unsigned fwKeys = msg->wParam;  // key flags 
		int x = (short)LOWORD(msg->lParam);	// horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);	// vertical position of cursor 
		OnButtonMove(fwKeys, x, y);
		//DSTR << "mouse:" << x << " " << y << std::endl;
		}break;
	case WM_MOUSEWHEEL:{
		unsigned fwKeys = LOWORD(msg->wParam);    // key flags
		int zDelta = (short) HIWORD(msg->wParam);    // wheel rotation
		int xPos = (short) LOWORD(msg->lParam);    // horizontal position of pointer
		int yPos = (short) HIWORD(msg->lParam);    // vertical position of pointer
		OnWheel(fwKeys, zDelta);
		}break;
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:{
		unsigned fwKeys = msg->wParam;        // key flags 
		int x = (short)LOWORD(msg->lParam);  // horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);  // vertical position of cursor 
		OnDblClick(fwKeys, x, y);
		}break;
	case WM_KEYDOWN:{
		int nVirtKey = (int) msg->wParam;
		long lKeyData = msg->lParam;
		return OnKeyDown(nVirtKey);
		}break;
	default:
		return false;
	}
	return true;
}
bool HIMouse::OnKeyDown(unsigned nChar){
	//	視点移動関係
	
	Affinef afBody = GetAxis().inv();
	float df = 0.2f ; //移動変化量
	// カメラの平行移動
	if ( nChar == 'W' ) afBody.Pos() = afBody.Pos() + afBody.Ex() * df;
	else if ( nChar == 'Q' ) afBody.Pos() = afBody.Pos() - afBody.Ex() * df;
	else if ( nChar == 'A' ) afBody.Pos() = afBody.Pos() + afBody.Ey() * df;
	else if ( nChar == 'Z' ) afBody.Pos() = afBody.Pos() - afBody.Ey() * df;
	else if ( nChar == VK_DOWN ) afBody.Pos() = afBody.Pos() + afBody.Ez() * df;
	else if ( nChar == VK_UP ) afBody.Pos() = afBody.Pos() - afBody.Ez() * df;
	// カメラの回転
	else if ( nChar == VK_LEFT ) afBody = Affinef::Rot(Radf(5), 'y') * afBody;
	else if ( nChar == VK_RIGHT ) afBody = Affinef::Rot(Radf(-5), 'y') * afBody;
	else if ( nChar == 'S' ) afBody = Affinef::Rot(Radf(5),afBody.Ex()) * afBody;
	else if ( nChar == 'X' ) afBody = Affinef::Rot(Radf(-5),afBody.Ex()) * afBody;
	else if ( nChar == 'C' ){	// ポインタとカメラをリセット
		afBody = axisInit.inv();
		afBody.Ex().unitize();
		afBody.Ey().unitize();
		afBody.Ez().unitize();
		pos = Vec3f();
		ori = Quaternionf();
	}
	else return false;	
	SetAxis(afBody.inv());
	return true;
}
void HIMouse::OnButtonDown(int x, int y){
	oldX = newX = x;
	oldY = newY = y;
}	

void HIMouse::OnButtonUp(){ 
	btnState = NONE;
	oldX = newX;
	oldY = newY;
}

void HIMouse::OnButtonMove(UINT state, int x, int y){ 
	// 押されているボタンの状態を設定
	btnState = (ButtonState)state; 

	newX = x;
	newY = y;
}

void HIMouse::OnDblClick(unsigned fwKeys, int x, int y){
	if (fwKeys & MK_LBUTTON) OnLButtonDClick();
	if (fwKeys & MK_RBUTTON) OnRButtonDClick();
}

/// ポインタの姿勢を初期化
void HIMouse::OnLButtonDClick(){
	ori = Quaternionf();
}

/// ポインタの位置を初期化
void HIMouse::OnRButtonDClick(){
	pos = Vec3f();
}

/// 基準座標軸の位置・姿勢を初期化
void HIMouse::OnShiftLButtonDClick(){
	axis = axisInit;
}

/// ホイールをまわしたときの処理(ポインタor基準座標軸をZ軸方向に水平移動)
void HIMouse::OnWheel(UINT state, short dz){
	btnState = (ButtonState)state;	

	Affinef afAxis = axis.inv();		
	if (btnState == MK_SHIFT){
		afAxis.Pos() += afAxis.Ez() * dz * scaleTransAxis;	
		axis = afAxis.inv();
	} else if (btnState == NONE){
		pos += -afAxis.Ez() * dz * scaleTransPointer;
	}
}

}	//	namespace Spr

