// TGeoNodeNum.cpp: TGeoNodeNum クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodeNum.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

/* タイマーの名前例 
nc_t1_2 → タイマーの1の位(左から一桁目)が2
nc_t2_4 → タイマーの10の位(左から二桁目)が4
nc_t3_1 → タイマーの100の位(左から三桁目)が1
*/

/* 得点の名前例 
nc_s1_2 → 得点の1の位(左から一桁目)が2
nc_s2_4 → 得点の10の位(左から二桁目)が4
nc_s3_1 → 得点の100の位(左から三桁目)が1
nc_s4_m → 得点の1000の位(左から四桁目)が-(マイナス)
*/

TGeoNodeNum::TGeoNodeNum()
{
	TGeoNode();
}

TGeoNodeNum::~TGeoNodeNum(){ }

void TGeoNodeNum::Draw()
{
	control();
//	isOffDraw();

	if(!isDraw) return;

	// タイマー描画 -->>
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( p_TGLView->afBody * afTrans);	// アバターからの相対位置に表示			
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}

void TGeoNodeNum::control(){ }

/*
void TGeoNodeNum::isOffDraw()
{
	if(!isDraw) return;
	if(vn != number){ isDraw = false; }	
}*/

Affinef TGeoNodeNum::setTrans(int digit)
{
	float h = 1.0;			//水平移動の横
	float v = 6.53;			//水平移動の縦
	float d = -12;			//水平移動の奥行き
	
	switch(digit){
	case 1:
		h += 1.0;
		break;
	case 2:
		h += 0.5;
		break;
	case 3:
		h += 0.0;
		break;
	default:
		h += 0;
		break;
	}

	Affinef t = Affinef(Rad(90), 'x');
	t.Pos() = Vec3f(h,v,d);
	return t;
}

void TGeoNodeNum::InitInstance(string name)
{
	const char *pObjectNameStr = name.c_str();
	digit = atoi(pObjectNameStr+4);

	if(pObjectNameStr[6] == 'm'){ number = 10; }
	else{ number = atoi(pObjectNameStr+6); }

	afTrans = setTrans(digit);
	isDraw = false;
}


//***** TGeoNodeScore *****//
void TGeoNodeScore::InitInstance(string name)
{
	TGeoNodeNum::InitInstance(name);
	afTrans = afTrans * Affinef(3.0, 0, 0);
}

void TGeoNodeScore::control()
{
//***	if(isDraw) return;

	char c = *(p_TGLView->vash_char + 4 - digit);		 
	vn = atoi(&c);

//	std::cout << "vc = " << p_TGLView->vash_char << std::endl;
//	std::cout << "vn = " << vn << std::endl;


//	if(vn == number && !(digit == 4 && vn ==0))	{
	if(vn == number && !( (abs(p_TGLView->vash_val) < pow(10,digit-1)) && digit != 1 ))	{
		if(isDraw == false){
			isDraw = true;
			size = 10;
//			afTrans = afTrans * Affinef(0, 0, size * 0.05); 
			afTrans = afTrans * Affinef(0, size * 0.05, 0); 
		} 
		else {
			if(--size >= 0){
//				afTrans = afTrans * Affinef(0, 0, -0.05);
				afTrans = afTrans * Affinef(0, -0.05, 0); 
			}
		}
	}
	else{ isDraw = false; }
	
//	if((c == '-'  && number == 10){ isDraw = true; }
	if(p_TGLView->vash_val < 0  && c == '-' && number == 10){ isDraw = true; }

}

Affinef TGeoNodeScore::setTrans(int digit)
{
	Affinef af = TGeoNodeNum::setTrans(digit);
	if(digit == 4){
		af = af * Affinef(-0.4, 0, 0);
	}
	return af;
}

//***** TGeoNodeTimer *****//
void TGeoNodeTimer::control()
{
//***	if(isDraw) return;

	char c = *(p_TGLView->timer_char + 3 - digit);		 
	vn = atoi(&c);

//	if(vn == number){ isDraw = true; }
	if(vn == number && !( (abs(p_TGLView->timer_val) < pow(10,digit-1)) && digit != 1 ))	{
		isDraw = true; 
	} 
	else { isDraw = false; }

//	std::cout << "vn = " << p_TGLView->timer_char << std::endl;
//	std::cout << "vn = " << vn << std::endl;
}
