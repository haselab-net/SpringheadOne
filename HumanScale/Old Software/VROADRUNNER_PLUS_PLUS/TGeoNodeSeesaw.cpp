// TGeoNodeSeesaw.cpp: TGeoNodeSeesaw クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodeSeesaw.h"
#include "GLView.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TGeoNodeSeesaw::TGeoNodeSeesaw()
{
	startState = 1;
	state = 1;
	shaft = 'x';
	slope = Rad(8);

	l = 39.873;
	w = 13.734;
	h = 1.5;
}

TGeoNodeSeesaw::~TGeoNodeSeesaw()
{

}

void TGeoNodeSeesaw::InitInstance()
{
	state = startState;
}

void TGeoNodeSeesaw::Draw()
{
	/*
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( posture );	// 位置・姿勢を変更する。
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
	*/
	Affinef R;

	SetState();

	if(state < 0){
		 R = Affinef(-slope,shaft);
	}
	else if(state > 0){
		 R = Affinef(slope,shaft);
	}
	else{
		R = Affinef(0,shaft);
	}

/*
	static double j=0;
	j+=0.5;
	
	Affinef R = Affinef(Rad(j),'x');
*/


	Vec3f av = posture.Pos();
	Affinef atr = Affinef(av.X(),av.Y(),av.Z());
	Affinef aff = posture;
	aff.Pos() = Vec3f();
/*
	//cout << this->center << endl;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex3fv(atr * R * aff*Vec3f(0,0,0));
		glVertex3fv(atr * R * aff*Vec3f(0,1000,0));
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
*/
	globalCenter = atr * R * aff*Vec3f(0,0,0);

	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( atr * R * aff);	// 位置・姿勢を変更する。
	
	glCallList(displayList);

	
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )


}

void TGeoNodeSeesaw::SetState()
{
	/*
	if(p_TGLView->afBody.Pos() posture.Pos()){
		state = 1;
	}
	else if(){
		state = 0;
	}
	else if(){
		state = -1;
	}
	*/
	//
	static int i=0;
	int time=400;

	if(i<time){
		state = 1;
	}
	else if(i<time*2){
		state = 0;
	}
	else if (i<time*3){
		state = -1;
	}
	else if(i<time*4){
		state = 0;
	}
	else{
		i=0;
	}
	i++;

	//
}

void TGeoNodeSeesaw::SetCenter(Vec3f center)
{
	this->center = center;
}

void TGeoNodeSeesawX::SetState()
{
	if(globalCenter.X()-w/2 <= p_TGLView->afBody.PosX()  && p_TGLView->afBody.PosX() <= globalCenter.X()+w/2){

		if(state < 0){
			if(globalCenter.Z()+l/2 >= p_TGLView->afBody.PosZ() && globalCenter.Z() <=  p_TGLView->afBody.PosZ()){
				p_TGLView->afBody.Pos().Z() = globalCenter.Z()+l/3*2;
				return ;
			}
		}
		else if(state > 0){
			if(globalCenter.Z()-l/2 <= p_TGLView->afBody.PosZ() && globalCenter.Z() >= p_TGLView->afBody.PosZ()){
				p_TGLView->afBody.Pos().Z() = globalCenter.Z()-l/3*2;
				return;
			}
		}


		if(globalCenter.Z()-l/2 <= p_TGLView->afBody.PosZ() && p_TGLView->afBody.PosZ() <= globalCenter.Z()-l/4){
			state = -startState;
		}
		else if(globalCenter.Z()-l/4 <= p_TGLView->afBody.PosZ() && p_TGLView->afBody.PosZ() <= globalCenter.Z()+l/4){
			state = 0;
		}
		else if(globalCenter.Z()+l/4 <= p_TGLView->afBody.PosZ() && p_TGLView->afBody.PosZ() <= globalCenter.Z()+l/2){
			state = startState;
		}
	}
	/*
	else {
		state = -1;
	}
	*/
}

void TGeoNodeSeesawZ::SetState()
{
	if(globalCenter.Z()-w/2 <= p_TGLView->afBody.PosZ()  && p_TGLView->afBody.PosZ() <= globalCenter.Z()+w/2){
/**/
		if(state > 0){
			if(globalCenter.X()+l/2 >= p_TGLView->afBody.PosX() && globalCenter.X() <=  p_TGLView->afBody.PosX()){
				p_TGLView->afBody.Pos().X() = globalCenter.X()+l/3*2;
				return ;
			}
		}
		else if(state < 0){
			if(globalCenter.X()-l/2 <= p_TGLView->afBody.PosX() && globalCenter.X() >= p_TGLView->afBody.PosX()){
				p_TGLView->afBody.Pos().X() = globalCenter.X()-l/3*2;
				return;
			}
		}

		if(startState > 0){
			if(globalCenter.X()-l/2 <= p_TGLView->afBody.PosX() && p_TGLView->afBody.PosX() <= globalCenter.X()-l/4){
				state = startState;
			}
			else if(globalCenter.X()-l/4 <= p_TGLView->afBody.PosX() && p_TGLView->afBody.PosX() <= globalCenter.X()+l/4){
				state = 0;
			}
			else if(globalCenter.X()+l/4 <= p_TGLView->afBody.PosX() && p_TGLView->afBody.PosX() <= globalCenter.X()+l/2){
				state = -startState;
			}
		}
		else{
			if(globalCenter.X()-l/2 <= p_TGLView->afBody.PosX() && p_TGLView->afBody.PosX() <= globalCenter.X()-l/4){
				state = -startState;
			}
			else if(globalCenter.X()-l/4 <= p_TGLView->afBody.PosX() && p_TGLView->afBody.PosX() <= globalCenter.X()+l/4){
				state = 0;
			}
			else if(globalCenter.X()+l/4 <= p_TGLView->afBody.PosX() && p_TGLView->afBody.PosX() <= globalCenter.X()+l/2){
				state = startState;
			}
		}
	}
}
