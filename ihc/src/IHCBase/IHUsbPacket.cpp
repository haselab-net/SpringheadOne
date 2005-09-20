#include "IHUsbPacket.h"
void IHPacketPlane::EndianAll(){
	IHPacketBase::EndianAll();
	Endian(planeState);
	Endian(posScale);
	Endian(forceScale);
	Endian(normal);
	Endian(position);
	Endian(offset);
}
void IHPacketPlane::Print(std::ostream& os){
	os << "st:" << planeState;
	os << "p:" << position;
	os << " n:" << normal;
}
void IHPacketPlaneReturn::EndianAll(){	
	IHPacketBase::EndianAll();
	Endian(position);
	Endian(force);
}
void IHPacketPlaneReturn::Print(std::ostream& os){
	os << "p:" << position;
	os << " f:" << force;
}

void IHPacketLD::EndianAll(){
	IHPacketBase::EndianAll();
	Endian(bChanged);
	Endian(calc_ld);
	Endian(k);
	Endian(b);
	Endian(pos);
	Endian(center);
	Endian(normal);
	Endian(f_external);
	Endian(torque_ex);
	Endian(velocities);
	Endian(Iinv);
	Endian(posScale);
	Endian(forceScale);
	Endian(qtOffset);
	Endian(qtOffsetInv);
	Endian(v3Offset);
}

void IHPacketLDReturn::EndianAll(){
	IHPacketBase::EndianAll();
	Endian(pos);
	Endian(pointer);
	Endian(impulse);
	Endian(velocities);
	Endian(displacement);
}

