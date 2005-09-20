// TGeoNodeVash.cpp: TGeoNodeVash �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodeVash.h"
#include "GLView.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
const int TGeoNodeVash::BaseLine= 101+2;

TGeoNodeVash::TGeoNodeVash()
{
	TGeoNode();
	isDraw = true;
	isTextDraw = false;
	pp_VashPoint = 0;
	val = -200;
	dr = 0.1; //0.3��]���x�����߂�p�����[�^
	radius_ratio = 2.0;
}

TGeoNodeVash::~TGeoNodeVash()
{

}

void TGeoNodeVash::Draw()
{
	/*
	// ������`�� -->>
	static int draw_count=0;
	if(isTextDraw){

		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
		//glMultMatrixf(Affinef(1,0,0)*posture);	// �ʒu�E�p����ύX����B
		glMultMatrixf(this->p_TGLView->afBody*Affinef(0,100,-1000));
		char buf[8];
		sprintf(buf,"+%d",this->val);
		for(int i=0;i<strlen(buf);i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN,buf[i]);
		}
		glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )
		
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		draw_count++;
		if(draw_count>50){
			draw_count=0;
			isTextDraw = false;
		}
	}
	// <<-- ������`��
	*/

	if(!isDraw) return;

	static double j=0;
	j+=dr;
	
	Affinef R = Affinef(Rad(j),'y');

	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	Vec3f av = posture.Pos();
	Affinef atr = Affinef(av.X(),av.Y(),av.Z());
	Affinef aff = posture;
	aff.Pos() = Vec3f();

	glMultMatrixf( atr * R * aff);	// �ʒu�E�p����ύX����B
	glCallList(displayList);
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )

	if(isGetVash()){ // ���ł��炷
		p_TGLView->PlaySound( SOUND_COIN );
	}
}


// V�}�[�N�̓����蔻��
bool TGeoNodeVashV::isGetVash()
{
	if(!isDraw) return false;
	Affinef RX90 = Affinef(Rad(90),'x');
	Vec3f aObj = p_TGLView->GetLaserNode()->GetPosition().Pos();
	Affinef af = this->p_TGLView->afBody * afTrans * RX90;
	Vec3f bObj = af.Pos();
	Vec3f dis = aObj-bObj;

	// �|�C���^�Ƃ̓����蔻��
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

// vash�̓����蔻��
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
			(*this->pp_VashPoint)->isDraw = true; //�ǉ�
		}
		return true;
	}

	// �|�C���^�Ƃ̓����蔻��
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

	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	glMultMatrixf(   this->p_TGLView->afBody * afTrans*RX90);	// �ʒu�E�p����ύX����B
	glCallList(displayList);
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )

	if(isGetVash()){ // ���ł��炷
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
//DEL 	// �|�C���^�Ƃ̓����蔻��
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
//DEL 	// �|�C���^�Ƃ̓����蔻��
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
//DEL 	// �|�C���^�Ƃ̓����蔻��
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
