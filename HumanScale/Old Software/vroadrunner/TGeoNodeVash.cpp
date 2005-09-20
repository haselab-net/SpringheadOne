// TGeoNodeVash.cpp: TGeoNodeVash クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodeVash.h"
#include "GLView.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
const int TGeoNodeVash::BaseLine= 101+2;

TGeoNodeVash::TGeoNodeVash()
{
	TGeoNode();
	isDraw = true;
	isTextDraw = false;
	pp_VashPoint = 0;
	val = -200;
	dr = 0.1; //0.3回転速度を決めるパラメータ
	radius_ratio = 2.0;
}

TGeoNodeVash::~TGeoNodeVash()
{

}

void TGeoNodeVash::Draw()
{
	/*
	// 文字列描画 -->>
	static int draw_count=0;
	if(isTextDraw){

		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
		//glMultMatrixf(Affinef(1,0,0)*posture);	// 位置・姿勢を変更する。
		glMultMatrixf(this->p_TGLView->afBody*Affinef(0,100,-1000));
		char buf[8];
		sprintf(buf,"+%d",this->val);
		for(int i=0;i<strlen(buf);i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN,buf[i]);
		}
		glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
		
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		draw_count++;
		if(draw_count>50){
			draw_count=0;
			isTextDraw = false;
		}
	}
	// <<-- 文字列描画
	*/

	if(!isDraw) return;

	static double j=0;
	j+=dr;
	
	Affinef R = Affinef(Rad(j),'y');

	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	Vec3f av = posture.Pos();
	Affinef atr = Affinef(av.X(),av.Y(),av.Z());
	Affinef aff = posture;
	aff.Pos() = Vec3f();

	glMultMatrixf( atr * R * aff);	// 位置・姿勢を変更する。
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )

	if(isGetVash()){ // 音でも鳴らす
		p_TGLView->PlaySound( SOUND_COIN );
	}
}


// Vマークの当たり判定
bool TGeoNodeVashV::isGetVash()
{
	if(!isDraw) return false;
	Affinef RX90 = Affinef(Rad(90),'x');
	Vec3f aObj = p_TGLView->GetLaserNode()->GetPosition().Pos();
	Affinef af = this->p_TGLView->afBody * afTrans * RX90;
	Vec3f bObj = af.Pos();
	Vec3f dis = aObj-bObj;

	// ポインタとの当たり判定
	if( p_TGLView->GetLaserNode()->IsDraw() ){
		if(dis.norm() <= p_TGLView->GetLaserNode()->GetRadius()){
			isDraw = false;
			p_TGLView->GetLaserNode()->Hide();
			p_TGLView->vash_val+=val;
			return true;
		}
	}
	return false;
}

// vashの当たり判定
bool TGeoNodeVash::isGetVash()
{
	if(!isDraw) return false;
	Vec3f aObj = p_TGLView->afBody.Pos();
	Vec3f bObj = posture.Pos();
	Vec3f dis = aObj-bObj;

	if(dis.norm() <= p_TGLView->radius * radius_ratio){
		isDraw = false;
		isTextDraw = true;
		p_TGLView->vash_val+=val;
		if(pp_VashPoint != 0){
			(*this->pp_VashPoint)->isDraw = true; //追加
		}
		return true;
	}

	// ポインタとの当たり判定
	if( p_TGLView->GetLaserNode()->IsDraw() ){
		aObj = p_TGLView->GetLaserNode()->GetPosition().Pos();
		dis = aObj - bObj;
		if(dis.norm() <= p_TGLView->GetLaserNode()->GetRadius()){
			isDraw = false;
			isTextDraw = true;
			p_TGLView->GetLaserNode()->Hide();
			p_TGLView->vash_val+=val;
			return true;
		}
	}

	return false;
}

void TGeoNodeVash::InitInstance()
{
	isDraw = true;
}

void TGeoNodeVashV::Draw()
{
	if(!isDraw) return;
	static double j=0;
	j+=dr;

	Affinef RX90 = Affinef(Rad(90),'x');

	afTrans.Rot() = Affinef(Rad(j),'y').Rot();
	afTrans.Pos() = Vec3f(10, 14.9, -40);

	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf(   this->p_TGLView->afBody * afTrans*RX90);	// 位置・姿勢を変更する。
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )

	if(isGetVash()){ // 音でも鳴らす
		strcpy(p_TGLView->GameCommentStr,"You get vRoadRunner!");
	}
}

//DEL bool TGeoNodeVash10::isGetVash()
//DEL {
//DEL 	if(!isDraw) return false;
//DEL 	Vec3f aObj = p_TGLView->afBody.Pos();
//DEL 	Vec3f bObj = posture.Pos();
//DEL 	Vec3f dis = aObj-bObj;
//DEL 
//DEL 	if(dis.norm() <= p_TGLView->radius * radius_ratio){
//DEL 		isDraw = false;
//DEL 		isTextDraw = true;
//DEL 		p_TGLView->vash_val+=val;
//DEL 		return true;
//DEL 	}
//DEL 
//DEL 	// ポインタとの当たり判定
//DEL 	if( p_TGLView->GetLaserNode()->IsDraw() ){
//DEL 		aObj = p_TGLView->GetLaserNode()->GetPosition().Pos();
//DEL 		dis = aObj - bObj;
//DEL 		if(dis.norm() <= p_TGLView->GetLaserNode()->GetRadius()){
//DEL 			isDraw = false;
//DEL 			isTextDraw = true;
//DEL 			p_TGLView->GetLaserNode()->Hide();
//DEL 			p_TGLView->vash_val+=val;
//DEL 			return true;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	return false;
//DEL 
//DEL }

//DEL bool TGeoNodeVash100::isGetVash()
//DEL {
//DEL 	if(!isDraw) return false;
//DEL 	Vec3f aObj = p_TGLView->afBody.Pos();
//DEL 	Vec3f bObj = posture.Pos();
//DEL 	Vec3f dis = aObj-bObj;
//DEL 
//DEL 	if(dis.norm() <= p_TGLView->radius * radius_ratio){
//DEL 		isDraw = false;
//DEL 		isTextDraw = true;
//DEL 		p_TGLView->vash_val+=val;
//DEL 		return true;
//DEL 	}
//DEL 
//DEL 	// ポインタとの当たり判定
//DEL 	if( p_TGLView->GetLaserNode()->IsDraw() ){
//DEL 		aObj = p_TGLView->GetLaserNode()->GetPosition().Pos();
//DEL 		dis = aObj - bObj;
//DEL 		if(dis.norm() <= p_TGLView->GetLaserNode()->GetRadius()){
//DEL 			isDraw = false;
//DEL 			isTextDraw = true;
//DEL 			p_TGLView->GetLaserNode()->Hide();
//DEL 			p_TGLView->vash_val+=val;
//DEL 			return true;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	return false;
//DEL 
//DEL }

//DEL bool TGeoNodeVash50::isGetVash()
//DEL {
//DEL 	if(!isDraw) return false;
//DEL 	Vec3f aObj = p_TGLView->afBody.Pos();
//DEL 	Vec3f bObj = posture.Pos();
//DEL 	Vec3f dis = aObj-bObj;
//DEL 
//DEL 	if(dis.norm() <= p_TGLView->radius * radius_ratio){
//DEL 		isDraw = false;
//DEL 		isTextDraw = true;
//DEL 		p_TGLView->vash_val+=val;
//DEL 		return true;
//DEL 	}
//DEL 
//DEL 	// ポインタとの当たり判定
//DEL 	if( p_TGLView->GetLaserNode()->IsDraw() ){
//DEL 		aObj = p_TGLView->GetLaserNode()->GetPosition().Pos();
//DEL 		dis = aObj - bObj;
//DEL 		if(dis.norm() <= p_TGLView->GetLaserNode()->GetRadius()){
//DEL 			isDraw = false;
//DEL 			isTextDraw = true;
//DEL 			p_TGLView->GetLaserNode()->Hide();
//DEL 			p_TGLView->vash_val+=val;
//DEL 			return true;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	return false;
//DEL 
//DEL }
