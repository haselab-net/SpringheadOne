#include <Base/env.h>
#include <Base/TVector.h>
#include <Base/TinyVec.h>
#include <iostream>
#include <sstream>
#include <assert.h>

using namespace PTM;
using namespace std;
using namespace Spr;
template <class A, class B>
void test(A a, B b){
	assert(a*b / (a*b) == 1);
	assert(a*2 == a+a);
	assert(a+b == a+b);
};

int SPR_CDECL main(){
	TVector<3, float> pos;
    cout << sizeof(pos) << endl;
	VVector<float> angle;
	pos.clear();
	angle.resize(3);
	angle.clear();
	pos[0] = 1;
	angle[0] = 1;
	test(pos, pos);
	test(angle, angle);
	test(pos, angle);

	typedef TVector<6, float> TVec6f;
	TVec6f posture;

	pos.assign(angle);
	pos = angle;
	posture.sub_vector(3, TSubVector<3, TVec6f::desc>());
	posture.sub_vector(3, TSubVector<3, TVec6f::desc>()) = pos;
	pos = posture.sub_vector(3,TSubVector<3, TVec6f::desc >());
	istringstream is_pos("(1 2 3)");
	pos.input(is_pos);
	istringstream isAngle("(10 20 30)");
	isAngle  >> angle;
//	posture.sub_vector(TSubVectorDim<3,3>()) = pos;
//	posture.sub_vector(TSubVectorDim<0,3>()) = angle;	//	 / 360 * 3.1415;
	posture.v_slice(3,3,1) = pos;
	posture.v_slice(0,3,1) = angle;	//	 / 360 * 3.1415;
	cout << pos << angle << endl;
	cout << posture << endl;
	

	TVector<3, int> a;
	TVector<3, double> b;
	istringstream is("(1 0 0)");
	is >> a;
	istringstream is2("(0.5 1 0)");
	is2 >> b;
	cout << "TVector<3, int> a = " << a << endl;
	cout << "TVector<3, double> b = " << b << endl;
//	cout << "a*b=" << a*b << "\tï‘ÇËílÇÕ int å^" << endl;
	cout << "b*a=" << b*a << "\tï‘ÇËílÇÕ dobule å^" << endl;
	cout << "b^a=" << (b^a) << "\tï‘ÇËílÇÕ double å^" << endl;

	b+=a;
	b-=a;
	a*=2;
	a/=2;
	b==a;
	b!=a;
	TVector<4, float> c;
	cout << "size:" << sizeof(c) << std::endl;
	c.sub_vector(TSubVectorDim<1,3>()) = Spr::Vec3f(1,0,0);
	
	
	//	ââéZéqÇÃåüç∏
	{
		Vec3f a(1,2,3), b(3,2,1);
		assert ( a != b);
		assert ( a*b == 10 );
		assert ( a+b == Vec3f(4,4,4) );
	}
	return 0;
}
