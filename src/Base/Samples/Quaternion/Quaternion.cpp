#include <iostream>
#include <Base/Affine.h>

using namespace Spr;
using namespace std;
void test(){
	Quaternionf q;
	Matrix3f m = Matrix3f::Rot((float)Rad(90), 'z');
	cout << m;
	q.from_matrix(m);
	cout << "Z90 rotation:" << q.rotation() << endl;
	q.to_matrix(m);
	cout << m;
//	Quaternionf q2 = Quaternionf::Rot((float)Rad(90), 'z');
	Quaternionf q2;
	q2.euler(0, 0, Rad(90));
	cout << "q " << q << endl;
	cout << "q2" << q2 << endl;
	cout << "Z180:" << (q*q2).rotation() << endl;
}
void randTest(){
	Vec3f axis;
	axis.X() = float(rand() - RAND_MAX/2);
	axis.Y() = float(rand() - RAND_MAX/2);
	axis.Z() = float(rand() - RAND_MAX/2);
	axis.unitize();
	float rotation = rand() / float(RAND_MAX);
	const float rotationMAX = (float)M_PI-0.1f;
	rotation *= rotationMAX;
	if (rotation > rotationMAX) rotation = rotationMAX;
//	if (rotation < -rotationMAX) rotation = -rotationMAX;
	cout << "------------------------------------" << endl;
	cout << axis << rotation << endl;
	
	Quaternionf q;
	Matrix3f m = Matrix3f::Rot(rotation, axis);
	q.from_matrix(m);
	cout << q.rotation().unit() << q.rotation().norm() << endl;
	Vec3f deltarotation = (q.rotation() - axis*rotation);
	assert(deltarotation.norm() < 1e-4f);
	
	Matrix3f m2;
	q.to_matrix(m2);
	Matrix3f diff = m-m2;
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			if (! (abs(diff.item(i,j)) < 1e-3) ){
				cout << m << m2;
				cout << diff;
				assert(abs(diff.item(i,j)) < 1e-3);
			}
		}
	}
	Quaternionf q2 = Quaternionf::Rot(rotation, axis);
	assert((q2-q).norm() < 1e-5);
}

int main(){
	{
		Quaternionf qu;
		Matrix3f rot;
		rot = Matrix3f::Rot((float)Rad(60), 'x');
		qu.from_matrix(rot);
		cout << qu << std::endl;
		rot = Matrix3f::Rot((float)Rad(179), 'x');
		qu.from_matrix(rot);
		cout << qu << std::endl;
		rot = Matrix3f::Rot((float)Rad(180), 'x');
		qu.from_matrix(rot);
		cout << qu << std::endl;
	}
	for(int i=0; i<100; ++i){
		randTest();
	}
	
	Quaternionf q;// = Quaternionf::Rot(Rad(30), Vec3f(0,0,1));
	Vec3f axis(1,0,0);
	q.sub_vector(PTM::TSubVectorDim<1,3>()) = axis * 0.2f;

	cout << q << endl;

	cout << "Matrix3 --------------------------" << endl;
	Matrix3f mat = Matrix3f::Rot(q*q);
	cout << mat;
	Matrix3f r = Matrix3f::Rot((float)Rad(60), 'z');
	cout << r;
	cout << "Vec3f --------------------------" << endl;

	Vec3f v(1,0,0);
	cout << q*q * v << endl;
	cout << mat * v << endl;
	Vec3f a = (q*q.inv()).rotation();
	cout << Deg(a.norm()) << "," << a << endl;
	Quaternionf q1 = Quaternionf::Rot((float)Rad(30), Vec3f(0,0,1));
	Quaternionf q2 = Quaternionf::Rot((float)Rad(-30), Vec3f(0,0,1));
	cout << q1+q2 << endl;
	cout << q1-q2 << endl;
	cout << q1*q2 << endl;
	cout << q1*q2.inv() << endl;
	test();
	return 0;
}
