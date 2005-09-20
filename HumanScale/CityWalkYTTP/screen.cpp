// mkurahashi wrote... /////
// VM室の描画　と　各プロジェクション領域の中心を計算

#include "GLView.h"
#include "screen.h"

using namespace LVRM;

DrawScreen::DrawScreen(){
	accuracy = 80;
	a = 2*M_PI / accuracy;
	accQ1 = accuracy / 4;
	accQ2 = 2 * accQ1;
	accQ3 = 3 * accQ1;
	count = 0;
}
void DrawScreen::makeDList(void){
	d_vision = glGenLists(1);
	glNewList(d_vision, GL_COMPILE);

	glBegin(GL_TRIANGLES);

	panel1(); // 正面
	panel2(); // 床面
	upper_cylinder_parts(1500,1650); // 上
	lower_cylinder_parts(300,1650); // 下
	right_cylinder_parts(1500,1100); // 右
	left_cylinder_parts(1500,1100); // 左
	right_sphere_parts(1500); // 右上
	left_sphere_parts(1500); // 左上
	circle_parts1(1200); // 右下
	doughnut_parts1(1200, 300);
	circle_parts2(1200); // 左下
	doughnut_parts2(1200, 300);

	glEnd();

	glEndList();
}
void DrawScreen::panel1(void){
	glNormal3f( 0.0, 0.0, -1.0 );
	glVertex3f(-1650.0, 2500.0, -1500.0);
	glVertex3f( 1650.0,  300.0, -1500.0);
	glVertex3f( 1650.0, 2500.0, -1500.0);

	glVertex3f( 1650.0,  300.0, -1500.0);
	glVertex3f(-1650.0, 2500.0, -1500.0);
	glVertex3f(-1650.0,  300.0, -1500.0);
	count += 6;
}
void DrawScreen::panel2(void){
	glNormal3f( 0.0, -1.0, 0.0 );
	glVertex3f(-1650.0, 0.0, -1200.0);
	glVertex3f( 1650.0, 0.0,     0.0);
	glVertex3f( 1650.0, 0.0, -1200.0);

	glVertex3f( 1650.0, 0.0,     0.0);
	glVertex3f(-1650.0, 0.0, -1200.0);
	glVertex3f(-1650.0, 0.0,     0.0);
	count += 6;
}
void DrawScreen::upper_cylinder_parts(float r, float length){ 
	glNormal3f( 0.0, 1.0, 0.0 );
	for(int i=accQ3; i<accuracy; i++){ 
		glNormal3f( 0.0, cos(a*(i+1)), sin(a*(i+1)) );
		glVertex3f( length, r*cos(a*(i+1)) +2500.0, r*sin(a*(i+1)) );
		glVertex3f(-length, r*cos(a*(i+1)) +2500.0, r*sin(a*(i+1)) );  
		glNormal3f( 0.0, cos(a*i), sin(a*i) );
		glVertex3f( length, r*cos(a*i) +2500.0,     r*sin(a*i)     ); 

		glVertex3f(-length, r*cos(a*i) +2500.0,     r*sin(a*i)     ); 
		glVertex3f( length, r*cos(a*i) +2500.0,     r*sin(a*i)     );
		glNormal3f( 0.0, cos(a*(i+1)), sin(a*(i+1)) );
		glVertex3f(-length, r*cos(a*(i+1)) +2500.0, r*sin(a*(i+1)) ); 
		count += 6;
	} 
}
void DrawScreen::lower_cylinder_parts(float r, float length){ 
	for(int i=accQ2; i<accQ3; i++){ 
		glNormal3f( 0.0, cos(a*(i+1)), sin(a*(i+1)) );
		glVertex3f( length, r*cos(a*(i+1))+300.0, r*sin(a*(i+1))-1200.0 ); 
		glVertex3f(-length, r*cos(a*(i+1))+300.0, r*sin(a*(i+1))-1200.0 ); 
		glNormal3f( 0.0, cos(a*i), sin(a*i) );
		glVertex3f( length, r*cos(a*i)+300.0,     r*sin(a*i)-1200.0     ); 

		glVertex3f(-length, r*cos(a*i)+300.0,     r*sin(a*i)-1200.0     ); 
		glVertex3f( length, r*cos(a*i)+300.0,     r*sin(a*i)-1200.0     );
		glNormal3f( 0.0, cos(a*(i+1)), sin(a*(i+1)) );
		glVertex3f(-length, r*cos(a*(i+1))+300.0, r*sin(a*(i+1))-1200.0 ); 
		count += 6;
	} 
}
void DrawScreen::right_cylinder_parts(float r, float length){ 
	for(int i=accQ1; i<accQ2; i++){ 
		glNormal3f( sin(a*(i+1)), 0.0, cos(a*(i+1)) );
		glVertex3f( r*sin(a*(i+1))+1650.0, length+1400.0, r*cos(a*(i+1)) ); 
		glVertex3f( r*sin(a*(i+1))+1650.0,-length+1400.0, r*cos(a*(i+1)) ); 
		glNormal3f( sin(a*i), 0.0, cos(a*i) );
		glVertex3f( r*sin(a*i)+1650.0,     length+1400.0, r*cos(a*i)     ); 

		glVertex3f( r*sin(a*i)+1650.0, -length+1400.0,     r*cos(a*i)     ); 
		glVertex3f( r*sin(a*i)+1650.0,  length+1400.0,     r*cos(a*i)     );
		glNormal3f( sin(a*(i+1)), 0.0, cos(a*(i+1)) );
		glVertex3f( r*sin(a*(i+1))+1650.0, -length+1400.0, r*cos(a*(i+1)) ); 
		count += 6;
	} 
}
void DrawScreen::left_cylinder_parts(float r, float length){ 
	for(int i=accQ2; i<accQ3; i++){ 
		glNormal3f( sin(a*(i+1)), 0.0, cos(a*(i+1)) );
		glVertex3f( r*sin(a*(i+1))-1650.0, length+1400.0, r*cos(a*(i+1)) ); 
		glVertex3f( r*sin(a*(i+1))-1650.0,-length+1400.0, r*cos(a*(i+1)) ); 
		glNormal3f( sin(a*i), 0.0, cos(a*i) );
		glVertex3f( r*sin(a*i)-1650.0,     length+1400.0, r*cos(a*i)     ); 

		glVertex3f( r*sin(a*i)-1650.0, -length+1400.0,     r*cos(a*i)     ); 
		glVertex3f( r*sin(a*i)-1650.0,  length+1400.0,     r*cos(a*i)     );
		glNormal3f( sin(a*(i+1)), 0.0, cos(a*(i+1)) );
		glVertex3f( r*sin(a*(i+1))-1650.0, -length+1400.0, r*cos(a*(i+1)) ); 
		count += 6;
	} 
}
void DrawScreen::right_sphere_parts(float r){ 
	for(int i=0; i<accQ1; i++){ //y座標を決める
		for(int j=accQ1; j<accQ2; j++){ //x,z座標を決める
		glNormal3f( cos(a*(i+1))*sin(a*j), sin(a*(i+1))
			, cos(a*(i+1))*cos(a*j) );
		glVertex3f(r*(cos(a*(i+1)))*sin(a*j)+1650.0
			, r*sin(a*(i+1))+2500.0, r*(cos(a*(i+1)))*cos(a*j));
		glNormal3f( cos(a*(i+1))*sin(a*(j+1)), sin(a*(i+1))
			, cos(a*(i+1))*cos(a*(j+1)) );
		glVertex3f(r*(cos(a*(i+1)))*sin(a*(j+1))+1650.0
			, r*sin(a*(i+1))+2500.0, r*(cos(a*(i+1)))*cos(a*(j+1)));
		glNormal3f( cos(a*i)*sin(a*j), sin(a*i)
			, cos(a*i)*cos(a*j) );
		glVertex3f(r*(cos(a*i))*sin(a*j)+1650.0
			, r*sin(a*i)+2500.0, r*(cos(a*i))*cos(a*j));

		glNormal3f( cos(a*i)*sin(a*(j+1)), sin(a*i)
			, cos(a*i)*cos(a*(j+1)) );
		glVertex3f(r*(cos(a*i))*sin(a*(j+1))+1650.0
			, r*sin(a*i)+2500.0, r*(cos(a*i))*cos(a*(j+1)));
		glNormal3f( cos(a*i)*sin(a*j), sin(a*i)
			, cos(a*i)*cos(a*j) );
		glVertex3f(r*(cos(a*i))*sin(a*j)+1650.0
			, r*sin(a*i)+2500.0, r*(cos(a*i))*cos(a*j));
		glNormal3f( cos(a*(i+1))*sin(a*(j+1)), sin(a*(i+1))
			, cos(a*(i+1))*cos(a*(j+1)) );
		glVertex3f(r*(cos(a*(i+1)))*sin(a*(j+1))+1650.0
			, r*sin(a*(i+1))+2500.0, r*(cos(a*(i+1)))*cos(a*(j+1)));
		count += 6;
		}
	} 
}
void DrawScreen::left_sphere_parts(float r){ 
	for(int i=0; i<accQ1; i++){ //y座標を決める
		for(int j=accQ2; j<accQ3; j++){ //x,z座標を決める
		glNormal3f( cos(a*(i+1))*sin(a*j), sin(a*(i+1))
			, cos(a*(i+1))*cos(a*j) );
		glVertex3f(r*(cos(a*(i+1)))*sin(a*j)-1650.0
			, r*sin(a*(i+1))+2500.0, r*(cos(a*(i+1)))*cos(a*j));
		glNormal3f( cos(a*(i+1))*sin(a*(j+1)), sin(a*(i+1))
			, cos(a*(i+1))*cos(a*(j+1)) );
		glVertex3f(r*(cos(a*(i+1)))*sin(a*(j+1))-1650.0
			, r*sin(a*(i+1))+2500.0, r*(cos(a*(i+1)))*cos(a*(j+1)));
		glNormal3f( cos(a*i)*sin(a*j), sin(a*i)
			, cos(a*i)*cos(a*j) );
		glVertex3f(r*(cos(a*i))*sin(a*j)-1650.0
			, r*sin(a*i)+2500.0, r*(cos(a*i))*cos(a*j));

		glNormal3f( cos(a*i)*sin(a*(j+1)), sin(a*i)
			, cos(a*i)*cos(a*(j+1)) );
		glVertex3f(r*(cos(a*i))*sin(a*(j+1))-1650.0
			, r*sin(a*i)+2500.0, r*(cos(a*i))*cos(a*(j+1)));
		glNormal3f( cos(a*i)*sin(a*j), sin(a*i)
			, cos(a*i)*cos(a*j) );
		glVertex3f(r*(cos(a*i))*sin(a*j)-1650.0
			, r*sin(a*i)+2500.0, r*(cos(a*i))*cos(a*j));
		glNormal3f( cos(a*(i+1))*sin(a*(j+1)), sin(a*(i+1))
			, cos(a*(i+1))*cos(a*(j+1)) );
		glVertex3f(r*(cos(a*(i+1)))*sin(a*(j+1))-1650.0
			, r*sin(a*(i+1))+2500.0, r*(cos(a*(i+1)))*cos(a*(j+1)));
		count += 6;
		}
	} 
}
void DrawScreen::circle_parts1(int r1){
	glNormal3f( 0.0, -1.0, 0.0);
	for(int i=accQ1; i<accQ2; i++){ 
		glVertex3f(0.0+1650.0, 0.0, 0.0); 
		glVertex3f(-r1*cos(a*(i+1))+1650.0, 0.0, -r1*sin(a*(i+1))); 
		glVertex3f(-r1*cos(a*i)+1650.0, 0.0, -r1*sin(a*i)); 
		count += 3;
	} 
}
void DrawScreen::circle_parts2(int r1){
	glNormal3f( 0.0, -1.0, 0.0);
	for(int i=0; i<accQ1; i++){ 
		glVertex3f(0.0-1650.0, 0.0, 0.0); 
		glVertex3f(-r1*cos(a*(i+1))-1650.0, 0.0, -r1*sin(a*(i+1))); 
		glVertex3f(-r1*cos(a*i)-1650.0, 0.0, -r1*sin(a*i)); 
		count += 3;
	} 
}
void DrawScreen::doughnut_parts1(int r1, int r2){
	for(int i=accQ3; i<accuracy; i++){ 
		for(int j=0; j<accQ1; j++){
		glNormal3f( -sin(a*(j+1))*sin(a*i), -cos(a*(j+1))
			, -sin(a*(j+1))*cos(a*i) );
		glVertex3f(-r1*sin(a*i)-r2*sin(a*(j+1))*sin(a*i)+1650.0
			, r2-r2*cos(a*(j+1)), -r1*cos(a*i)-r2*sin(a*(j+1))*cos(a*i)); 
		glNormal3f( -sin(a*(j+1))*sin(a*(i+1)), -cos(a*(j+1))
			, -sin(a*(j+1))*cos(a*(i+1)) );
		glVertex3f(-r1*sin(a*(i+1))-r2*sin(a*(j+1))*sin(a*(i+1))+1650.0
			, r2-r2*cos(a*(j+1)), -r1*cos(a*(i+1))-r2*sin(a*(j+1))*cos(a*(i+1))); 
		glNormal3f( -sin(a*j)*sin(a*i), -cos(a*j)
			, -sin(a*j)*cos(a*i) );
		glVertex3f(-r1*sin(a*i)-300*sin(a*j)*sin(a*i)+1650.0
			, r2-r2*cos(a*j), -r1*cos(a*i)-r2*sin(a*j)*cos(a*i)); 

		glNormal3f( -sin(a*j)*sin(a*(i+1)), -cos(a*j)
			, -sin(a*j)*cos(a*(i+1)) );
		glVertex3f(-r1*sin(a*(i+1))-300*sin(a*j)*sin(a*(i+1))+1650.0
			, r2-r2*cos(a*j), -r1*cos(a*(i+1))-r2*sin(a*j)*cos(a*(i+1)));
		glNormal3f( -sin(a*j)*sin(a*i), -cos(a*j)
			, -sin(a*j)*cos(a*i) );
		glVertex3f(-r1*sin(a*i)-300*sin(a*j)*sin(a*i)+1650.0
			, r2-r2*cos(a*j), -r1*cos(a*i)-r2*sin(a*j)*cos(a*i)); 
		glNormal3f( -sin(a*(j+1))*sin(a*(i+1)), -cos(a*(j+1))
			, -sin(a*(j+1))*cos(a*(i+1)) );
		glVertex3f(-r1*sin(a*(i+1))-r2*sin(a*(j+1))*sin(a*(i+1))+1650.0
			, r2-r2*cos(a*(j+1)), -r1*cos(a*(i+1))-r2*sin(a*(j+1))*cos(a*(i+1))); 
		count += 6;
		}
	} 
}
void DrawScreen::doughnut_parts2(int r1, int r2){
	for(int i=0; i<accQ1; i++){ 
		for(int j=0; j<accQ1; j++){
		glNormal3f( -sin(a*(j+1))*sin(a*i), -cos(a*(j+1))
			, -sin(a*(j+1))*cos(a*i) );
		glVertex3f(-r1*sin(a*i)-r2*sin(a*(j+1))*sin(a*i)-1650.0
			, r2-r2*cos(a*(j+1)), -r1*cos(a*i)-r2*sin(a*(j+1))*cos(a*i)); 
		glNormal3f( -sin(a*(j+1))*sin(a*(i+1)), -cos(a*(j+1))
			, -sin(a*(j+1))*cos(a*(i+1)) );
		glVertex3f(-r1*sin(a*(i+1))-r2*sin(a*(j+1))*sin(a*(i+1))-1650.0
			, r2-r2*cos(a*(j+1)), -r1*cos(a*(i+1))-r2*sin(a*(j+1))*cos(a*(i+1))); 
		glNormal3f( -sin(a*j)*sin(a*i), -cos(a*j)
			, -sin(a*j)*cos(a*i) );
		glVertex3f(-r1*sin(a*i)-300*sin(a*j)*sin(a*i)-1650.0
			, r2-r2*cos(a*j), -r1*cos(a*i)-r2*sin(a*j)*cos(a*i)); 

		glNormal3f( -sin(a*j)*sin(a*(i+1)), -cos(a*j)
			, -sin(a*j)*cos(a*(i+1)) );
		glVertex3f(-r1*sin(a*(i+1))-300*sin(a*j)*sin(a*(i+1))-1650.0
			, r2-r2*cos(a*j), -r1*cos(a*(i+1))-r2*sin(a*j)*cos(a*(i+1)));
		glNormal3f( -sin(a*j)*sin(a*i), -cos(a*j)
			, -sin(a*j)*cos(a*i) );
		glVertex3f(-r1*sin(a*i)-300*sin(a*j)*sin(a*i)-1650.0
			, r2-r2*cos(a*j), -r1*cos(a*i)-r2*sin(a*j)*cos(a*i)); 
		glNormal3f( -sin(a*(j+1))*sin(a*(i+1)), -cos(a*(j+1))
			, -sin(a*(j+1))*cos(a*(i+1)) );
		glVertex3f(-r1*sin(a*(i+1))-r2*sin(a*(j+1))*sin(a*(i+1))-1650.0
				, r2-r2*cos(a*(j+1)), -r1*cos(a*(i+1))-r2*sin(a*(j+1))*cos(a*(i+1)));
		count += 6;
		}
	} 
}

void DrawScreen::create_screen(void){

//	accuracy = 60;
	glColor3d(0.0, 0.0, 0.0);

	glScalef(0.001, 0.001, 0.001);
	glTranslatef(0.0, -1600.0, -900.0);
	// T2の中心から想定視点まで移動

	glTranslatef(0.0, 0.0, 66.0);
	glRotatef(3.9, 1.0, 0.0, 0.0);
	glRotatef(-2.3, 0.0, 1.0, 0.0);
	
	glCallList(d_vision);
}
/*
Vec3d DrawScreen::GetRotCenter(double * cameraParam, int number){

	// number : マシン番号
	// cameraParam : カメラパラメータ
	double a, b, c; // 想定されている視線ベクトル

	a = -cameraParam[0];
	b =  cameraParam[1];
	c = -cameraParam[2];
	// 座標空間を一致させる

	double t; // 直線のパラメータ
	rotCenter = Vec3f(0.0, 0.0, 0.0);

	switch (number){
	case 0: case 1: case 2: case 3:
	case 4: case 5: case 6: case 7:
		// 正面の平面スクリーンに交点が存在するとき
		rotCenter.Z() = -2.4;
		t = rotCenter.Z() / c;
		rotCenter.X() = a * t;
		rotCenter.Y() = b * t;
		break;
	case 12: case 30: case 31: case 16: 
	case 32: case 33:
		// 下面の平面スクリーンに交点が存在するとき
		rotCenter.Y() = -1.6;
		t = rotCenter.Y() / b;
		rotCenter.X() = a * t;
		rotCenter.Z() = c * t;
		break;
	case 11:
		// 左上の球面スクリーンに交点が存在するとき
		static double a1 = a*a + b*b + c*c;
		static double b1 = -(a*(-1.650) + b*0.900 + c*(-0.900));
		static double c1;
		c1 = (-1.650)*(-1.650) + 0.900*0.900 + (-0.900)*(-0.900) - 1.500*1.500;
		t = (-b1+sqrt(b1*b1 -a1*c1)) / a1;
		rotCenter.X() = a*t;
		rotCenter.Y() = b*t;
		rotCenter.Z() = c*t;
		break;
	case 15:
		// 右上の球面スクリーンに交点が存在するとき
		static double a2 = a*a + b*b + c*c;
		static double b2 = -(a*1.650 + b*0.900 + c*(-0.900));
		static double c2;
		c2 = 1.650*1.650 + 0.900*0.900 + (-0.900)*(-0.900) - 1.500*1.500;
		t = (-b2+sqrt(b2*b2 -a2*c2)) / a2;
		rotCenter.X() = a*t;
		rotCenter.Y() = b*t;
		rotCenter.Z() = c*t;
		break;
	case 20: case 21: case 22: case 23:
		// 上の円筒面スクリーンに交点が存在するとき
		static double a3 = b*b + c*c;
		static double b3 = -(b*0.900 + c*(-0.900));
		static double c3;
		c3 = 0.900*0.900 + (-0.900)*(-0.900) - 1.500*1.500;
		t = (-b3+sqrt(b3*b3 -a3*c3)) / a3;
		rotCenter.X() = a*t;
		rotCenter.Y() = b*t;
		rotCenter.Z() = c*t;
		break;
	case 10: case 13:
		// 左の円筒面スクリーンに交点が存在するとき
		static double a4 = a*a + c*c;
		static double b4 = -(a*(-1.650) + c*(-0.900));
		static double c4;
		c4 = (-1.650)*(-1.650) + (-0.900)*(-0.900) - 1.500*1.500;
		t = (-b4+sqrt(b4*b4 -a4*c4)) / a4;
		rotCenter.X() = a*t;
		rotCenter.Y() = b*t;
		rotCenter.Z() = c*t;
		break;
	case 14: case 17:
		// 右の円筒面スクリーンに交点が存在するとき
		static double a5 = a*a + c*c;
		static double b5 = -(a*1.650 + c*(-0.900));
		static double c5;
		c5 = 1.650*1.650 + (-0.900)*(-0.900) - 1.500*1.500;
		t = (-b5+sqrt(b5*b5 -a5*c5)) / a5;
		rotCenter.X() = a*t;
		rotCenter.Y() = b*t;
		rotCenter.Z() = c*t;
		break;
	default:
		break;
	}
//	for(int j=0;j<6;j++)
//		cout << "setting[" << j << "] = " << setting[j] << endl;
	cout << "rotCenter = (" << rotCenter.X() << ", " << rotCenter.Y() 
		<< ", " << rotCenter.Z() << "), t = " << t << endl;

	return(rotCenter);
}
*/