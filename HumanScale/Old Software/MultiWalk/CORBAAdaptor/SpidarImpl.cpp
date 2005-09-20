// Orbix Version v3.01


#include "SpidarImpl.h"
#include <Base/Affine.h>
#define TYPE 1

extern Affinef afBody;
extern float angle;
extern volatile bool sendFlag;

    CORBA::Long SpidarImpl:: StartSpidar (CORBA::Long ClientNo, 
		CORBA::Environment &IT_env)  {
		cout << "Receive Message StartSpidar"<<endl;
		return(TYPE);
}

    CORBA::Long SpidarImpl:: StopSpidar (CORBA::Long ClientNo, 
		CORBA::Environment &IT_env)  {
		cout << "Receive Message StopSpidar"<<endl;
		return(0);
}

    CORBA::Long SpidarImpl:: GetSpidarStatus (CORBA::Long ClientNo, CORBA::Double& r_x, CORBA::Double& r_y, CORBA::Double& r_z, 
		CORBA::Double& l_x, CORBA::Double& l_y, CORBA::Double& l_z, 
		CORBA::Long& door_num, CORBA::Double& door_ang, CORBA::Environment &IT_env)  {
		r_x = 10;r_y = 20;r_z = 30;l_x = 40;l_y = 50;l_z = 60;
		door_num = 1;door_ang = 90;
		return(0);
}

    CORBA::Long SpidarImpl:: GetMovementStatus (CORBA::Long ClientNo, CORBA::Double& x, CORBA::Double& y, CORBA::Double& z, 
		CORBA::Double& viewang, CORBA::Environment &IT_env)  {
		x = afBody.Pos().X();
		y = afBody.Pos().Y();
		z = afBody.Pos().Z();
		viewang = Deg(angle);
		return(0);
}
CORBA::Long SpidarImpl:: SetMovementStatus (CORBA::Long ClientNo, CORBA::Double& x, CORBA::Double& y, CORBA::Double& z, 
		CORBA::Double& viewang, CORBA::Environment &IT_env)  {
		afBody.Pos().X() = x;
		afBody.Pos().Y() = y;
		afBody.Pos().Z() = z;
		angle = Rad(viewang);
		sendFlag = true;
		return(0);
}

    CORBA::Long SpidarImpl:: SetPosition (CORBA::Double posx, CORBA::Double posy, CORBA::Double posz, 
		CORBA::Double rotx, CORBA::Double roty, CORBA::Double rotz, CORBA::Environment &IT_env)  {
		cout << "Receive Message SetPosition"<<endl;
		cout << "posx = "<<posx<<" posy = "<<posy<<" posz = "<<posz<<endl;
		cout << "rotx = "<<rotx<<" roty = "<<roty<<" rotz = "<<rotz<<endl;
		return(0);
}


