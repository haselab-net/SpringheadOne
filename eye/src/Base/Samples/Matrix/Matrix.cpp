#include <Base/TMatrix.h>
#include <Base/TMatrixUtility.h>
#include <Base/TinyVec.h>
#include <Base/TinyMat.h>
#include <iostream>


using namespace PTM;
using namespace Spr;
using namespace std;
void main2(){
	TMatrixCol<3,3,float> m33;
#ifdef __BORLANDC__
	TMatrixRow<3,3,float> work_around_for_bcb;
#endif
	m33.row(0) = Vec3f(1,2,3);
	m33.row(1) = Vec3f(10,20,30)*2;
	m33.row(2) = Vec3f(100,200,300)*3;
	cout << m33;
	cout << m33.trans();
	m33 = m33.trans();
	cout << m33;
	cout << endl;
	
	TMatrixCol<6,6,float> m66;
	int v=1;
	for(unsigned i=0; i<6; ++i){
		for(unsigned j=0; j<6; ++j){
			m66.item(i,j) = (float)v++;
		}
	}
	cout << m66;
	typedef TSubMatrixCol<4,4,TMatrixCol<6,6,float>::desc> Sub;
	Sub& sm = m66.sub_matrix(1,2,Sub());
	cout << sm;
#ifdef __BORLANDC__
	Sub::desc::trans_type smt = sm.trans();
#endif
	cout << sm.trans();
	typedef TSubMatrixCol<2,2,Sub::desc> SubSub;
	SubSub&ssm = sm.sub_matrix(1,1,SubSub());
	cout << ssm;
#ifdef __BORLANDC__
	SubSub::desc::trans_type ssmt = ssm.trans();
#endif
	cout << ssm.trans();

	typedef ESubMatrixCol<float> ESub;
	ESub esm = m66.vsub_matrix(1,2,4,4);
//	TMatrixCol<6,6,float>::desc::row_vector_type rrow = m66.row(1);
	cout << esm;
	cout << esm.trans();
	typedef ESubMatrixCol<float> ESubSub;
	ESubSub essm = esm.vsub_matrix(1,1,2,2);
	cout << essm;
	cout << essm.trans();

	VMatrixCol<float> vmat;
	TMatrixCol<2,4,float> tmat2;
	tmat2.clear(3);
	float data[8] = {1,2,3,4,5,6,7,8};
	TMatrixRow<4,2,float>& tmat1 = *(TMatrixRow<4,2,float>*)data;
	std::cout << "size:" << sizeof(tmat1) / sizeof(float) << std::endl;

	vmat = tmat1 * tmat2;
	std::cout << tmat1;
	std::cout << tmat2;
	std::cout << vmat;

	TMatrixRow<4,3,float> A;
	TMatrixRow<3,4,float> B;
	A.clear(2);
	B.clear(3);
	std::cout << "--------------------------------------" << std::endl;
	std::cout << A*B;
	std::cout << B*A;
}


template <class A, class B>
void test(A& a, B& b){
	a = b;
	assert(b == a);
	assert(b*3 == a+a+a);
	assert(b == a+a-a);
	cout << a;
}
template <class M>
void initMat(M& a){
	size_t v=0;
	for(size_t i=0; i<a.height(); ++i){
		for(size_t j=0; j<a.width(); ++j){
			a.item(i,j) = ++v;
		}
	}
}

int main3(){
	VMatrixCol<float> va;
	VMatrixCol<float> vb;
	va.resize(4,4);
	vb.resize(4,4);
	initMat(va);
	initMat(vb);
#if 0
	test(va.vsub_matrix(1,1,3,3), vb.vsub_matrix(1,1,3,3));
	test(va.vsub_matrix(1,1,3,3).vsub_matrix(1,1,2,2), vb.vsub_matrix(1,1,3,3).vsub_matrix(1,1,2,2));
	test(va.trans(), vb.trans());
	test(va.trans().vsub_matrix(1,1,3,3), vb.trans().vsub_matrix(1,1,3,3));
	test(va.trans().vsub_matrix(1,1,3,3).vsub_matrix(1,1,2,2), vb.trans().vsub_matrix(1,1,3,3).vsub_matrix(1,1,2,2));
	test(va, vb);
#endif
	TMatrixCol<4,4,float> ta;
	TMatrixCol<4,4,float> tb;
	initMat(ta);
	initMat(tb);
	test(ta.sub_matrix(1,1, TMatDim<3,3>()), tb.sub_matrix(1,1, TMatDim<3,3>()));
	test(ta.sub_matrix(1,1, TMatDim<3,3>()).sub_matrix(1,1, TMatDim<2,2>()),
		 tb.sub_matrix(1,1, TMatDim<3,3>()).sub_matrix(1,1, TMatDim<2,2>()));
	test(ta.trans(), tb.trans());
	test(ta.trans().sub_matrix(1,1,TMatDim<3,3>()), tb.trans().sub_matrix(1,1,TMatDim<3,3>()));
	test(ta.trans().sub_matrix(1,1, TMatDim<3,3>()).trans().sub_matrix(1,1, TMatDim<2,2>()),
		 tb.trans().sub_matrix(1,1, TMatDim<3,3>()).trans().sub_matrix(1,1, TMatDim<2,2>()));
	test(ta, tb);
	
	TVector<2, float> a;
	TVector<4, float> b;
	TMatrixCol<4,2, float> matA;
	TMatrixCol<2,5, float> matB;
	for(unsigned i=0; i<a.size(); ++i) a[i] = i+1;
	for(unsigned i=0; i<b.size(); ++i) b[i] = i+1;
	for(unsigned i=0; i<matA.height(); ++i){
		for(unsigned j=0; j<matA.width(); ++j) matA[i][j] = 10*i+j;
	}
	for(unsigned i=0; i<matB.height(); ++i){
		for(unsigned j=0; j<matB.width(); ++j) matB[i][j] = 10*i+j;
	}
//	a = b * matA;
	cout << a << endl << endl;
	cout << matA << " * " << endl;
	cout << matB << " = " << endl;
	cout << matA * matB;

	main2();
	
	TMatrixCol<5,5,float> m;
	init_unitize(m);
	m*=2;
	std::swap(m[0], m[1]);
	std::swap(m[2], m[4]);
	cout << m << "det:" << m.det() << std::endl;
	return 0;
}

int main(){
	Matrix3f rot;
	rot = Matrix3f::Rot(Vec3f(1,0,0), Vec3f(0,1,0), 'y');
	Matrix2f rot2;
	rot2 = Matrix2f::Rot(Vec2f(1,0), 'y');

	TMatrixCol<2,2,Matrix2f> vm1, vm2;
	vm1.clear();vm2.clear();
	vm1=Matrix2f(2);
	vm2=Matrix2f(4);
	DSTR << vm1*vm2 << std::endl;

	
	main3();
	const float p[]={
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};
	TMatrixCol<3,3,float> mat = (TMatrixCol<3,3,float>&)p;
	TVector<3, float> x, b;
	b[0] = 1;
	b[1] = 2;
	b[2] = 3;
	x.clear();
	TVector<3, int> ip;
	ip.clear();
	mat.gauss(x, b, ip);
	cout << "mat:" << mat << std::endl;
	cout << "x:" << x << std::endl;
	cout << "b:" << b << std::endl;
	return 0;
}

