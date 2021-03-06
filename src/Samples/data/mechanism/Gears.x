xof 0303txt 0032

Simulator{
	0.002;
}
Frame frLight {
	FrameTransformMatrix  {
		0.707107,0.000000,-0.707107,0.000000,
		0.500000,0.707107,0.500000,0.000000,
		0.500000,-0.707107,0.500000,0.000000,
		0.000000,0.000000,0.000000,1.00000;;
	}
}
Light8{
	3;
	0.900000;0.900000;0.900000;0.900000;;
	0.900000;0.900000;0.900000;0.900000;;
	0.500000;0.500000;0.500000;0.500000;;
	0.000000;0.000000;0.000000;;
	-0.3 ;-0.3 ; -0.3;;
	20.000000;
	1.000000;
	0.800000;
	0.800000;
	0.800000;
	0.300000;
	0.330000;
	{ frLight }
}

SolidContainer{
	Solid Pointer1{	//	spidar[0]
		3.0;
		3.0, 0.0, 0.0,
		0.0, 3.0, 0.0,
		0.0, 0.0, 3.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frPointer1}
	}
	Solid Pointer2{	//	spidar[1]
		0.07;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frPointer2}
	}
	Solid soCube{
		2.0;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 20.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frCube}
	}
	Solid soGear1{
		10.0;
		10.0, 0.0, 0.0,
		0.0, 10.0, 0.0,
		0.0, 0.0, 10.0;;
		0.0; -10.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frGear1}
	}
	Solid soGear2{
		10.0;
		10.0, 0.0, 0.0,
		0.0, 10.0, 0.0,
		0.0, 0.0, 10.0;;
		0.0; -10.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frGear2}
	}
	Solid soGear3{
		10.0;
		10.0, 0.0, 0.0,
		0.0, 10.0, 0.0,
		0.0, 0.0, 10.0;;
		0.0; -10.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frGear3}
	}
	Solid soGear4{
		10.0;
		10.0, 0.0, 0.0,
		0.0, 10.0, 0.0,
		0.0, 0.0, 10.0;;
		0.0; -10.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frGear4}
	}
}
GravityEngine{
	0.0; -40.0; 0.0;;
	{soCube}
	{soGear1}
	{soGear2}
	{soGear3}
	{soGear4}
}

ContactEngine{
	{Pointer1}
	{Pointer2}
	{soCube}
	{soGear1}
	{soGear2}
	{soGear3}
	{soGear4}
}

JointEngine{
	{frPin1}
	Joint joGear1{
		0;						#関節の種類	0:蝶番, 1:スライド
		0.0; 0.0; 0.0;;			#関節から見た親剛体の位置
		0.0, 1.0, 0.0,			#関節から見た親剛体の向き
		-1.0, 0.0, 0.0,			
		0.0, 0.0, 1.0;;			
		0.0; 0.0; 0.0;;			#関節から見た子剛体の位置
		0.0, -1.0, 0.0,			#関節から見た子剛体の向き
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0;;			
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
#		3; 0.0; 100000.0; 0.0; 1000.0;		#位置入力の例
		2; -3.0; 40.0; 0.03; 3.5;		#速度入力の例
#		3; 0.0; 0.0; 0.0; 0.0;
		{soGear1}
	}
}

JointEngine{
	{frPin2}
	Joint joGear2{
		0;						#関節の種類	0:蝶番, 1:スライド
		0.0; 0.0; 0.0;;			#関節から見た親剛体の位置
		0.0, 1.0, 0.0,			#関節から見た親剛体の向き
		-1.0, 0.0, 0.0,			
		0.0, 0.0, 1.0;;			
		0.0; 0.0; 0.0;;			#関節から見た子剛体の位置
		0.0, -1.0, 0.0,			#関節から見た子剛体の向き
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0;;			
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 200000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.0; 0.0; 0.0; 0.0;
		{soGear2}
	}
}


JointEngine{
	{frPin3}
	Joint joGear3{
		0;						#関節の種類	0:蝶番, 1:スライド
		0.0; 0.0; 0.0;;			#関節から見た親剛体の位置
		0.0, 1.0, 0.0,			#関節から見た親剛体の向き
		-1.0, 0.0, 0.0,			
		0.0, 0.0, 1.0;;			
		0.0; 0.0; 0.0;;			#関節から見た子剛体の位置
		0.0, -1.0, 0.0,			#関節から見た子剛体の向き
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0;;			
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 200000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.0; 0.0; 0.0; 0.0;
		{soGear3}
	}
}


JointEngine{
	{frPin4}
	Joint joGear4{
		0;						#関節の種類	0:蝶番, 1:スライド
		0.0; 0.0; 0.0;;			#関節から見た親剛体の位置
		0.0, 1.0, 0.0,			#関節から見た親剛体の向き
		-1.0, 0.0, 0.0,			
		0.0, 0.0, 1.0;;			
		0.0; 0.0; 0.0;;			#関節から見た子剛体の位置
		0.0, -1.0, 0.0,			#関節から見た子剛体の向き
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0;;			
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		-0.1; 0.0; 200000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		2; 0.0; 0.0; 0.0; 0.0;
		{soGear4}
	}
}

Frame frPin1{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		2.75 ,-1.0, -2.5, 1.0;;
	}
}
Frame frPin2{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0, -1.0, 0.0, 1.0;;
	}
}
Frame frPin3{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		-2.75,-1.0,0.0,1.0;;
	}
}
Frame frPin4{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		-3.00,1.74,0.0,1.0;;
	}
}

Frame frGear1{
	Import{ "Gear.xi"; }
}
Frame frGear2{
	Frame{
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0, 0.0, -2.2,1.0;;
		}
		Import{ "GearSmall.xi"; }
	}
	Import{ "Gear.xi"; }
}
Frame frGear3{
	Import{ "GearSmall.xi"; }
}
Frame frGear4{
	Import{ "Gear.xi"; }
}

Frame frCube{
	FrameTransformMatrix {
		0.9, 0.0, 0.0, 0.0,
		0.0, 0.9, 0.0, 0.0,
		0.0, 0.0, 0.9, 0.0,
		0.5, 2.0, 0.0, 1.0;;
	}
	Frame{
		FrameTransformMatrix {
			0.94, 0.0, 0.0, 0.0,
			0.0, 0.94, 0.0, 0.0,
			0.0, 0.0, 0.94, 0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}

Material matPointer1{
   1.000000;0.380000;0.398000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.095000;0.174500;0.250000;;
}
Material matPointer2{
   0.080000;1.000000;0.098000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.095000;0.174500;0.250000;;
}
Frame frPointer1{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		8.0,2.0,0.0,1.0;;
	}
#	Sphere{ 0.3; 10; 10; {matPointer1}}
#	Import{ "Gear.xi"; }
}
Frame frPointer2{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		-8.0,2.0,0.0,1.0;;
	}
	Sphere{ 0.3; 10; 10; {matPointer2}}
}
