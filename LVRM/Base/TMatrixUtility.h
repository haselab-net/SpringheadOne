#ifndef PTMATRIX_TMATRIXUTILITY
#define PTMATRIX_TMATRIXUTILITY
#include "TVector.h"
#include "TVector.h"
/**	@file TMatrixUtility.h
	行列初期化ユーティリティー．*/

namespace PTM {

inline void getAxisMap2D(int& x, int& y, int axis){
	switch(axis){
	case 'x': case 'X': case '0':
		x = 0; y = 1;
		break;
	case 'y': case 'Y': case '1':
		x = 1; y = 0;
		break;
	default:
		x = 0; y = 1;
		break;
	}
}
///	x/y軸を指定して2×2行列を回転行列に初期化
template <class RI, class CI, class BM, class ITPA, class BUFA, class ITPP, class BUFP>
void init_direct(TMatrixBaseOp<DIMENC(2),DIMENC(2),RI,CI,BM>& m,
				const TVectorBaseOp<DIMENC(2),ITPA, BUFA>& a,
				char axis){
	int	x, y;
	getAxisMap2D(x, y, axis);
	col(x) = exi.unit();
	col(y)[0] = -col(x)[1];
	col(y)[1] =  col(x)[0];
}

///	2×2行列を回転行列に初期化
template <class RI, class CI, class BM>
void init_rot(TMatrixBaseOp<DIMENC(2),DIMENC(2),RI,CI,BM>& m, TYPENAME CI::element_type th){
	TYPENAME CI::element_type c = cos(th);
	TYPENAME CI::element_type s = sin(th);
	item(0, 0) = c; item(0, 1) = -s;
	item(1, 0) = s; item(1, 1) = c;
}

inline void getAxisMap3D(int& x, int& y, int& z, int axis){
	switch(axis){
	case 'x': case 'X': case '0':
		x = 0; y = 1; z = 2;
		break;
	case 'y': case 'Y': case '1':
		x = 1; y = 2; z = 0;
		break;
	case 'z': case 'Z': case '2':
		x = 2; y = 0; z = 1;
		break;
	default:
		x = 0; y = 1; z = 2;
		break;
	}
}
///	axis軸, axis++軸を指定して3×3行列を回転行列に初期化
template <class RI, class CI, class BM, class ITPA, class BUFA, class ITPB, class BUFB>
void init_direct(TMatrixBaseOp<DIMENC(3),DIMENC(3),RI,CI,BM>& m,
				const TVectorBaseOp<DIMENC(3),ITPA, BUFA>& a,
				const TVectorBaseOp<DIMENC(3),ITPB, BUFB>& b,
				char axis){
	int x,y,z;
	getAxisMap3D(x, y, z, axis);
	m.col(x) = a.unit();
	m.col(y) = (b - (b*m.col(x))*m.col(x)).unit();
	m.col(z) = m.col(x) % m.col(y);
}
///	3×3行列をx/y/z軸まわり回転行列に初期化
template <class RI, class CI, class BM>
void init_rot(TMatrixBaseOp<DIMENC(3),DIMENC(3),RI,CI,BM>& m, TYPENAME CI::element_type th, char axis){
	int x,y,z;
	getAxisMap3D(x, y, z, axis);
	TYPENAME CI::element_type c = cos(th); 
	TYPENAME CI::element_type s = sin(th); 
	m.item(x,x) = 1; m.item(x, y) = 0; m.item(x, z) = 0;
	m.item(y,x) = 0; m.item(y, y) = c; m.item(y, z) = -s;
	m.item(z,x) = 0; m.item(z, y) = s; m.item(z, z) = c;
}
/**	3×3行列を任意軸まわり回転行列に初期化
		+																	   +
		|u^2+(1-u^2)cos(th)      uv(1-cos(th))-wsin(th)  wu(1-cos(th))+vsin(th)|
	R =	|uv(1-cos(th))+wsin(th)  v^2+(1-v^2)cos(th)      vw(1-cos(th))-usin(th)|
		|wu(1-cos(th))-vsin(th)  vw(1-cos(th))+usin(th)  w^2+(1-w^2)cos(th)    |
		+																	   +*/
template <class RI, class CI, class BM, class ITPA, class BUFA>
void init_rot(TMatrixBaseOp<DIMENC(3),DIMENC(3),RI,CI,BM>& m, TYPENAME CI::element_type th,
			 const TVectorBaseOp<DIMENC(3),ITPA, BUFA>& axis){
	TYPENAME CI::element_type s = sin(th), c = cos(th);
	TYPENAME CI::element_type u = axis[0], v = axis[1], w = axis[2];
	m.item(0,0) = u*u + (1-u*u)*c;
	m.item(1,0) = u*v*(1-c) + w*s;
	m.item(2,0) = w*u*(1-c) - v*s;

	m.item(0,1)  = u*v*(1-c) - w*s;
	m.item(1,1) = v*v + (1-v*v)*c;
	m.item(2,1) = v*w*(1-c) + u*s;
	
	m.item(0,2) = w*u*(1-c) + v*s;
	m.item(1,2) = v*w*(1-c) - u*s;
	m.item(2,2) = w*w + (1-w*w)*c;
}

/**	3×3行列をクォータニオンから任意軸まわり回転行列に初期化	*/
template <class RI, class CI, class BM, class ITPA, class BUFA>
void init_rot(TMatrixBaseOp<DIMENC(3),DIMENC(3),RI,CI,BM>& m, const TVectorBaseOp<DIMENC(4),ITPA, BUFA>& q){
	typedef TMatrixBaseOp<DIMENC(3),DIMENC(3),RI,CI,BM> MAT;
	const int W = 0;
	const int X = 1;
	const int Y = 2;
	const int Z = 3;
	MAT::element_type d = q*q;
    assert(d);
    MAT::element_type s = 2 / d;
    MAT::element_type xs = q[X] * s,   ys = q[Y] * s,   zs = q[Z] * s;
    MAT::element_type wx = q[W] * xs,  wy = q[W] * ys,  wz = q[W] * zs;
    MAT::element_type xx = q[X] * xs,  xy = q[X] * ys,  xz = q[X] * zs;
    MAT::element_type yy = q[Y] * ys,  yz = q[Y] * zs,  zz = q[Z] * zs;
	m.item(0,0) = 1 - (yy + zz);	m.item(0,1) = xy - wz;			m.item(0,2) = xz + wy;
	m.item(1,0) = xy + wz;			m.item(1,1) = 1 - (xx + zz);	m.item(1,2) = yz - wx;
	m.item(2,0) = xz - wy;			m.item(2,1) = yz + wx;			m.item(2,2) = 1 - (xx + yy);
}

///	N×N行列を単位行列に初期化
template <DIMTYPE N, class RI, class CI, class BM>
void init_unitize(TMatrixBaseOp<N,N,RI,CI,BM>& m){
	m.clear();
	for(int i=0; i<DIMDEC(N); i++){ m.item(i,i) = 1; }
}
///	3×3行列をベクトルの外積計算になるように初期化(m*b == v^b).
template <class RI, class CI, class BM, class I, class B>
void init_cross(TMatrixBaseOp<DIMENC(3),DIMENC(3),RI,CI,BM>& m, const TVectorBaseOp<DIMENC(3), I, B>& v){
	m.item(0,0) = 0;		m.item(0,1) = -v[2];	m.item(0,2) =  v[1];
	m.item(1,0) = v[2];		m.item(1,1) = 0;		m.item(1,2) = -v[0];
	m.item(2,0) = -v[1];	m.item(2,1) =  v[0];	m.item(2,2) = 0;
}

///	4×4行列をある点を注視する視点行列に初期化する．
template <class RI, class CI, class B, class IP, class BP>
void init_look_at(TMatrixBaseOp<DIMENC(4),DIMENC(4),RI,CI,B>& a, const TVectorBaseOp<DIMENC(3), IP, BP>& posi){
	typedef TMatrixBaseOp<DIMENC(4),DIMENC(4),RI,CI,B> TAf;
	typedef TVectorBaseOp<DIMENC(3), IP, BP> TVec;
	TVec relv = posi - a.col(3).sub_vector(TVecDim<3>());
	typename CI::element_type sx = a.col(0).sub_vector(TVecDim<3>()).norm();
	typename CI::element_type sy = a.col(1).sub_vector(TVecDim<3>()).norm();
	typename CI::element_type sz = a.col(2).sub_vector(TVecDim<3>()).norm();

	// y -> x
	TVec tv;
	tv[0] = relv[0];
	tv[1] = 0;
	tv[2] = relv[2];
	TVec ey = VecInit << 0.0f << 1.0f << 0.0f;
	a.col(0).sub_vector(TVecDim<3>()) = (ey % tv.unit()).unit();
	a.col(2).sub_vector(TVecDim<3>()) = relv.unit();
	a.col(1).sub_vector(TVecDim<3>()) = a.col(2).sub_vector(TVecDim<3>()) % a.col(0).sub_vector(TVecDim<3>());
	if (a.item(1,1) < 0){
		TAf rot;
		init_rot(rot.sub_matrix(TMatDim<3,3>()), M_PI, 'z');
		a = a * rot;
	}
	a.col(0).sub_vector(TVecDim<3>()) *= sx;
	a.col(1).sub_vector(TVecDim<3>()) *= sy;
	a.col(2).sub_vector(TVecDim<3>()) *= sz;
}
template <class RI, class CI, class B, class IP, class BP, class IT, class BT>
void init_look_at(TMatrixBaseOp<DIMENC(4),DIMENC(4),RI,CI,B>& a, const TVectorBaseOp<DIMENC(3), IP, BP>& posz, const TVectorBaseOp<DIMENC(3), IT, BT>& posy){
	typedef TMatrixBaseOp<DIMENC(4),DIMENC(4),RI,CI,B> TAf;
	typedef TVectorBaseOp<DIMENC(3), IP, BP> TVec;
	typename CI::element_type sx = a.col(0).sub_vector(TVecDim<3>()).norm();
	typename CI::element_type sy = a.col(1).sub_vector(TVecDim<3>()).norm();
	typename CI::element_type sz = a.col(2).sub_vector(TVecDim<3>()).norm();

	TVec relvz = posz - a.col(3).sub_vector(TVecDim<3>());
	TVec relvy = posy - a.col(3).sub_vector(TVecDim<3>());
	
	a.col(2).sub_vector(TVecDim<3>()) = relvz.unit();
	relvy = relvy - (relvy * a.col(2).sub_vector(TVecDim<3>())) * a.col(2).sub_vector(TVecDim<3>());
	a.col(1).sub_vector(TVecDim<3>()) = relvy.unit();
	a.col(0).sub_vector(TVecDim<3>()) = a.col(1).sub_vector(TVecDim<3>()) % a.col(2).sub_vector(TVecDim<3>());
	
	a.col(0).sub_vector(TVecDim<3>()) *= sx;
	a.col(1).sub_vector(TVecDim<3>()) *= sy;
	a.col(2).sub_vector(TVecDim<3>()) *= sz;
}
///	4×4行列をある点を注視する視点行列に初期化する．
template <class RI, class CI, class B, class IP, class BP>
void init_look_at_gl(TMatrixBaseOp<DIMENC(4),DIMENC(4),RI,CI,B>& a, TVectorBaseOp<DIMENC(3), IP, BP> posi){
	posi -= 2 * (posi - a.col(3).sub_vector(TVecDim<3>()));
	init_look_at(a, posi);
}
template <class RI, class CI, class B, class IZ, class BZ, class IY, class BY>
void init_look_at_gl(TMatrixBaseOp<DIMENC(4),DIMENC(4),RI,CI,B>& a,
					 TVectorBaseOp<DIMENC(3), IZ, BZ> posi,
					 const TVectorBaseOp<DIMENC(3), IY, BY>& posy){
	posi -= 2 * (posi - a.col(3).sub_vector(TVecDim<3>()));
	init_look_at(a, posi, posy);
}

template <class RI, class CI, class B, class ITPS, class BUFS, class ITPZ, class BUFZ>
void init_projection_gl(TMatrixBaseOp<DIMENC(4),DIMENC(4),RI,CI,B>& a,
						TVectorBaseOp<DIMENC(3),ITPS, BUFS> screen,
						TVectorBaseOp<DIMENC(2),ITPZ, BUFZ> size,
						typename CI::element_type front=1.0f, typename CI::element_type back=10000.0f){
	if (screen[2] <= 0) screen[2] = size[0];
	assert(screen[2] > 0);			//	Check screen's position.
	assert(front > 0);				//	Check front clipping plane.
	assert(back > front);				//	Check back clipping plane.
	
	typedef TYPENAME CI::element_type ET;
	TSubVector<TVectorBaseOp<DIMENC(3),ITPS, BUFS>, 2>::type center = screen.sub_vector(TVecDim<2>());
	
	center *= front / screen[2];
	size *= front / screen[2];
	
	typename CI::element_type Q = back/(back-front);
	a.item(0,0) = 2*front/size[0];	a.item(1,0) = 0;				a.item(2,0) = 0;			a.item(3,0) = 0;
	a.item(0,1) = 0;				a.item(1,1) = 2*front/size[1];	a.item(2,1) = 0;			a.item(3,1) = 0;
	a.item(0,2) = center[0];		a.item(1,2) = center[1];		a.item(2,2) = -2*Q+1;		a.item(3,2) = -1;
	a.item(0,3) = 0;				a.item(1,3) = 0;				a.item(2,3) = -2*Q*front;	a.item(3,3) = 0;
}
template <class RI, class CI, class B, class ITPS, class BUFS, class ITPZ, class BUFZ>
void init_projection_d3d(TMatrixBaseOp<DIMENC(4),DIMENC(4),RI,CI,B>& a,
						 TVectorBaseOp<DIMENC(3),ITPS, BUFS> screen,
						 TVectorBaseOp<DIMENC(2),ITPZ, BUFZ> size,
						 typename CI::element_type front=1.0f, typename CI::element_type back=10000.0f){
	if (screen[2] <= 0) screen[2] = size[0];
	assert(screen[2] > 0);			//	Check screen's position.
	assert(front > 0);				//	Check front clipping plane.
	assert(back > front);				//	Check back clipping plane.
	
	typedef TYPENAME CI::element_type ET;
	TSubVector<TVectorBaseOp<DIMENC(3),ITPS, BUFS>, 2>::type center = screen.sub_vector(TVecDim<2>());
	
	center *= front / screen[2];
	size *= front / screen[2];

	typename CI::element_type Q = back/(back-front);
	a.item(0,0) = 2*front/size[0];	a.item(1,0) = 0;				a.item(2,0) = 0;			a.item(3,0) = 0;
	a.item(0,1) = 0;				a.item(1,1) = 2*front/size[1];	a.item(2,1) = 0;			a.item(3,1) = 0;
	a.item(0,2) = center[0];		a.item(1,2) = center[1];		a.item(2,2) = Q;			a.item(3,2) = 1;
	a.item(0,3) = 0;				a.item(1,3) = 0;				a.item(2,3) = -Q*front;		a.item(3,3) = 0;
}


/**	4行ベクトルを回転をあらわすクォータニオンとして初期化	*/
template <class IQ, class BQ, class T, class IA, class BA>
void init_quaternion(TVectorBaseOp<DIMENC(4), IQ, BQ>& q, T angle, const TVectorBaseOp<DIMENC(3),IA, BA>& axis){
	typedef TVectorBaseOp<DIMENC(4), IQ, BQ> qtype;
	qtype::element_type d = axis.norm();
	assert(d);
	qtype::element_type s = (qtype::element_type)(sin(angle / 2) / d);
	q[0] = cos(angle / 2);
	q.sub_vector(1, axis) = axis*s;
}

/**	4行ベクトルを回転をあらわすクォータニオンとして初期化	*/
template <class IQ, class BQ, class T>
void init_quaternion(TVectorBaseOp<DIMENC(4), IQ, BQ>& q, T angle, char axis){
	typedef TVectorBaseOp<DIMENC(4), IQ, BQ> qtype;
	q[0] = (qtype::element_type) cos(angle / 2);
	int x,y,z;
	getAxisMap3D(x,y,z, axis);
	q[x+1] = (qtype::element_type) sin(angle / 2);
	q[y+1] = 0;
	q[z+1] = 0;
}

/**	4行ベクトルを回転をあらわすクォータニオンとして初期化	*/
template <class ITPA, class BUFA, class RI, class CI, class BM>
void init_quaternion(const TVectorBaseOp<DIMENC(4),ITPA, BUFA>& q, TMatrixBaseOp<DIMENC(3),DIMENC(3),RI,CI,BM>& m){
	typedef TMatrixBaseOp<DIMENC(3),DIMENC(3),RI,CI,BM> mtype;
	typedef TVectorBaseOp<DIMENC(4), IQ, BQ> qtype;
	// check the diagonal
	double tr = m[0][0] + m[1][1] + m[2][2];
	if (tr > 0){
		double s = sqrt(tr+1);
		W() = qtype::element_type(s/2);
		s =	 qtype::element_type(0.5/s);
		X() = qtype::elemet_type((m[1][2] - m[2][1]) * s);
		Y() = qtype::elemet_type((m[2][0] - m[0][2]) * s);
		Z() = qtype::elemet_type((m[0][1] - m[1][0]) * s);
	}else{	// diagonal is negative
		double q[4];
		int	i, j, k;
		int nxt[3] = {1, 2, 0};
		i = 0;
		if (m[1][1] > m[0][0]) i = 1;
		if (m[2][2] > m[i][i]) i = 2;
		j = nxt[i];
		k = nxt[j];
		s = sqrt((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
		q[i] = s * 0.5f;
		if (s){
			s = 0.5 / s;
			q[3] = (m[j][k] - m[k][j]) * s;
			q[j] = (m[i][j] + m[j][i]) * s;
			q[k] = (m[i][k] + m[k][i]) * s;			
			W() = q[0];
			X() = q[1];
			Y() = q[2];
			Z() = q[3];
		}else{
			W() = 0;
			X() = 1;
			Y() = 0;
			Z() = 0;
		}
	}
}

}	//	namespace PTM
#endif
