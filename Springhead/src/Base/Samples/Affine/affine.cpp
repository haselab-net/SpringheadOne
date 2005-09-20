#include <Base/Affine.h>
//#include <iostream>
#include <Base/BaseDebug.h>
#include <Base/BaseUtility.h>

#define cout DSTR

using namespace Spr;
using namespace std;
using namespace PTM;

class Test:public UTRefCount{
	int t;
};

int main(){
	UTRef<Test> test;
	
	
	//	åüç∏
	{
		Affinef r = Affinef::Rot( (float)Rad(60), 'x');
		Affinef af;
		af * r;
		Vec3f v(1,2,3);
		af.Ex() = Vec3f(1,2,3);
		af.Ey() = Vec3f(4,5,6);
		af.Ez() = Vec3f(7,8,9);
		cout << af;
#ifndef __BORLANDC__
		cout << af.sub_matrix(PTM::TSubMatrixDim<0,0,3,3>()).row(0) << endl;
		cout << af.sub_matrix(PTM::TSubMatrixDim<0,0,3,3>()).row(1) << endl;
		cout << af.sub_matrix(PTM::TSubMatrixDim<0,0,3,3>()).row(2) << endl << endl;
#endif

		Affinef u;
		PTM::TMatrixCol<3,3, float>s = u.sub_matrix(PTM::TSubMatrixDim<0,0,3,3>());
		cout << s;
		
		cout << r*r.inv();
		cout << u*v;
		assert(u * v == v);
	}


	Vec3f scr(0.0f, 0.0f, 100.0f);
	Vec2f sz(400.0f, 300.0f);
	Affinef afView = Affinef::ProjectionD3D(scr,sz,1,100);
	cout << afView;
	afView.inv();
	
	Affinef afR = Affinef::Rot((float)Rad(30), 'x');
	afR.Pos() = Vec3f(10,0,0);
	cout << "afR = " << endl << afR;
	cout << "afR.Rot = " << endl << afR.Rot();
	cout <<  "afR.Pos = " << afR.Pos() << endl;

	Vec3f a,b;
	cout << (a-b).X();
	Affinef af, af1, af2;
	cout << (af1-af2).Ex();
	float f1[]={1.1f, 1.2f, 1.3f, 1.4f};
	float f2[]={2.1f, 2.2f, 2.3f, 2.4f};
	float f3[]={3.1f, 3.2f, 3.3f, 3.4f};
	float f4[]={4.1f, 4.2f, 4.3f, 4.4f};
	cout << af;
 	af[0] = f1;
	cout << af;
	af[1] = f2;
	cout << af;
	af[2] = f3;
	cout << af;
	af[3] = f4;
	cout << af;
	Vec3f v(1,0,0);
	v = af*v;
	cout << v << std::endl;


	afView = Affinef::Trn(0,10,10);
	afView.LookAt(Vec3f());
	return 0;

 }


