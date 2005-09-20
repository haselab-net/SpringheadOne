#include "LDRenderer.h"

namespace Spr {;

LDRenderer::LDRenderer(){
	k = 1.0f;
	b = 0.0f;
	x = prevX = 0.0f;
	center.clear();
	pos.clear();
	prevPos.clear();
	currentPos.clear();
	normal.clear();
	prevNormal.clear();
	currentNormal.clear();
	f_external.clear();
	torque_ex.clear();
	force.clear();
	impulse.clear();
	velocities.clear();
	Iinv.clear();
	calc_ld = true;
	bCalc   = true;
	bExperiment = false;
//	pointer  = prevPointer = Vec3f(0.0f, 0.3f, 0.05f);
	pointer  = prevPointer = Vec3f(0.8f, -0.05f, 0.0f);

	ldDT   = 0.0001f;
	simuDT = 0.001f;

	posScale   = 1.0f;
	forceScale = 1.0f;
	qtOffset.clear();
	qtOffsetInv.clear();
	v3Offset.clear();
}

LDRenderer::~LDRenderer(){
}

void LDRenderer::Step(){

#ifndef USE_IHC
	CSEnter();
#endif

	if(bCalc)
		LDHapticRendering();

	if(calc_ld  && !bExperiment)
		LDLocalDynamics();
	else
		LocalUpdate();

	prevPointer = pointer;
	prevX       = x;

#ifndef USE_IHC
	CSLeave();
#endif
}

void LDRenderer::RegisterPreviousParameters(){
	prevNormal  = normal;
	prevPos     = pos;
}

void LDRenderer::LDHapticRendering(){
	// 力覚提示ポインタの位置取得
	device->Update(ldDT);
	pointer = device->GetPos();
#ifdef USE_IHC
//	pointer = qtOffset * (pointer * posScale) + v3Offset;
	pointer = (pointer*posScale) + v3Offset;
#endif

	CalcForce(); // 力計算

	// 力提示(force)
#ifdef USE_IHC
//	device->SetForce(qtOffsetInv*force / forceScale, Vec3f(0.0f, 0.0f, 0.0f));
	device->SetForce(force / forceScale, Vec3f(0.0f, 0.0f, 0.0f));
#else
	device->SetForce(force, Vec3f(0.0f, 0.0f, 0.0f));
#endif
}

void LDRenderer::CalcForce(){
	if(calc_ld && !bExperiment)
		x = (pos - pointer) * normal;        // ポインタの侵入量
	else
		x = (currentPos - pointer) * normal;

	if(x > 1e-6f && normal.norm() > 1e-6f){
		float v = (x - prevX) / ldDT;
		force = (k*x - b*v) * normal.unit();
	}
	else{
		force.clear();
	}

}

void LDRenderer::LDLocalDynamics(){
	Vec3f f_internal = -force;
	Vec3f torque_in;
	Vec6f acc;
	Vec6f f;	
	Vec6f diff;

	torque_in = (pos - center) % f_internal;
	
	f.sub_vector(0, Vec3f()) = f_internal + f_external;
	f.sub_vector(3, Vec3f()) = torque_in  + torque_ex;
	acc = Iinv * f;

	diff = velocities*ldDT + 0.5*acc*ldDT*ldDT;

	displacement += diff;
	velocities   += acc*ldDT;

	center += diff.sub_vector(0, Vec3f());
	normal += diff.sub_vector(3, Vec3f()) % normal;
	pos    += diff.sub_vector(0, Vec3f()) + diff.sub_vector(3, Vec3f())%(pos-center);
}

void LDRenderer::LocalUpdate(){
	if(bCalc){
		currentPos    += (pos-prevPos)      *ldDT/simuDT;
		currentNormal += (normal-prevNormal)*ldDT/simuDT;
		impulse       -= force * ldDT;
	}

}

void LDRenderer::AdjustPos(){
	pos = pointer + x * normal.unit(); // 侵入量を変えず、法線だけ変化
}

void LDRenderer::SetParameters(IHPacketLD packet){
	RegisterPreviousParameters();

	bCalc		= true;
	calc_ld		= packet.calc_ld;
	posScale    = packet.posScale;
	forceScale  = packet.forceScale;
	qtOffset    = packet.qtOffset;
	qtOffsetInv = packet.qtOffsetInv;
	v3Offset    = packet.v3Offset;

	normal     = packet.normal;
	Iinv       = packet.Iinv;
	f_external = packet.f_external;
	torque_ex  = packet.torque_ex;

#ifndef ADJUST_POS
	pos = packet.pos;
#endif

	if(calc_ld && !bExperiment){
		if(packet.bChanged){
			k          = packet.k;
			b          = packet.b;
			pos        = packet.pos;
			center     = packet.center;
			velocities = packet.velocities;
			
		}
		else{
#ifdef ADJUST_POS
			AdjustPos();
#endif
		}
	}
	else{
		k          = packet.k;
		b          = packet.b;
		pos        = packet.pos;
		velocities = packet.velocities;
		bCalc      = !packet.bChanged;
		if(bExperiment)
			currentPos = pos;
		else
			SetCurrentPos();
	}
}

} //namespase Spr

