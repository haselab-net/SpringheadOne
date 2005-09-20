#ifndef IHUSBPACKET_H
#define IHUSBPACKET_H
#include <Base/Affine.h>
#include "../../../Experiments/LocalDynamics/Matrix6.h"
#include "../../../Experiments/LocalDynamics/Vector6.h"

class IHPacketBase{
public:
	char id;	//	パケットのID
	short size;	//	パケットのバイト数
	IHPacketBase():id(0){}
	template <class T>
	void Endian(T& t){
		if (sizeof(t) == 2){
			unsigned short& v = (unsigned short&)t;
			v = (v << 8) | (v >> 8);
		}else if(sizeof(t) == 4){
			unsigned long& v = (unsigned long&)t;
			v = (v<<24)&0xFF000000 | (v<<8)&0xFF0000 | (v>>8)&0xFF00 | (v>>24)&0xFF;
		}else{
			printf("Invalid variable size.\n");
		}
	}
	void Endian(Spr::Vec3f& v){
		Endian(v.X()); Endian(v.Y()); Endian(v.Z());
	}
	void Endian(Spr::Matrix3f& v){
		Endian(v.Ex());
		Endian(v.Ey());
		Endian(v.Ez());
	}
	void Endian(Spr::Affinef& v){
		for(int i=0; i<4; ++i){
			for(int j=0; j<4; ++j){
				Endian(v[i][j]);
			}
		}
	}
	void Endian(Vec6f& v){
//		Endian(v.sub_vector(0, Spr::Vec3f()));
//		Endian(v.sub_vector(3, Spr::Vec3f()));
		for(int i = 0; i < 6; i++)
			Endian(v[i]);
	}
	void Endian(Matrix6f& mat){
		for(int i = 0; i < 6; i++)
			for(int j = 0; j < 6; j++)
				Endian(mat[i][j]);
/*		
		Endian(mat.sub_matrix(0, 0, Spr::Matrix3f()));
		Endian(mat.sub_matrix(3, 0, Spr::Matrix3f()));
		Endian(mat.sub_matrix(0, 3, Spr::Matrix3f()));
		Endian(mat.sub_matrix(3, 3, Spr::Matrix3f()));
*/
  }
	void EndianAll(){
		Endian(size);
	}
};

class IHPacketPlane:public IHPacketBase{
public:
	enum PlaneState{ NONE, NEW, CONTINUE };
	IHPacketPlane(){
		size = sizeof(IHPacketPlane);
		if (size > 512){
			printf("Too large packet.\n");
		}
		id = 'i';
	}
	PlaneState planeState;
	Spr::Vec3f position;
	Spr::Vec3f normal;
	float posScale, forceScale;
	Spr::Vec3f offset;
	void EndianAll();
	void Print(std::ostream& os);
};

class IHPacketPlaneReturn: public IHPacketBase{
public:
	IHPacketPlaneReturn(){
		size = sizeof(IHPacketPlane);
		if (size > 512){
			printf("Too large packet.\n");
		}
		id = 'i';
	}
	Spr::Vec3f position;
	Spr::Vec3f force;
	void EndianAll();
	void Print(std::ostream& os);
};

class IHPacketLD: public IHPacketBase{
public:
	IHPacketLD(){
		size = sizeof(IHPacketLD);
		if(size > 512){
			printf("Too large packet : IHPacketLD\n");
		}
		id = 'l';
	}

	bool  bChanged;
	bool  calc_ld;
	float k;
	float b;
	Spr::Vec3f pos;
	Spr::Vec3f center;
	Spr::Vec3f normal;
	Spr::Vec3f f_external;
	Spr::Vec3f torque_ex;
	Vec6f velocities;
	Matrix6f Iinv;

	float posScale;
	float forceScale;
	Spr::Quaternionf qtOffset;
	Spr::Quaternionf qtOffsetInv;
	Spr::Vec3f v3Offset;

	void EndianAll();
};

class IHPacketLDReturn: public IHPacketBase{
public:
	IHPacketLDReturn(){ 
		size = sizeof(IHPacketLD);
		if(size > 512){
			printf("Too large packet : IHPacketLDReturn\n");
		}
		id = 'l';
	}

	bool calc_ld;
	Spr::Vec3f pos;
	Spr::Vec3f pointer;
	Spr::Vec3f impulse;
	Vec6f velocities;
	Vec6f displacement;
	void EndianAll();
};
#endif
