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
		0.3;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frPointer1}
	}
	Solid Pointer2{	//	spidar[1]
		0.3;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frPointer2}
	}
	Solid soBody{
		0.4;
		0.4, 0.0, 0.0,
		0.0, 0.4, 0.0,
		0.0, 0.0, 0.4;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frBody}
	}
	Solid soLegFLS{
		0.1;
		0.1, 0.0, 0.0,
		0.0, 0.1, 0.0,
		0.0, 0.0, 0.1;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegFLS}
	}
	Solid soLegFLU{
		0.13;
		0.13, 0.0, 0.0,
		0.0, 0.13, 0.0,
		0.0, 0.0, 0.13;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegFLU}
	}
	Solid soLegFLL{
		0.13;
		0.13, 0.0, 0.0,
		0.0, 0.13, 0.0,
		0.0, 0.0, 0.13;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegFLL}
	}
	Solid soLegFRS{
		0.1;
		0.1, 0.0, 0.0,
		0.0, 0.1, 0.0,
		0.0, 0.0, 0.1;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegFRS}
	}
	Solid soLegFRU{
		0.13;
		0.13, 0.0, 0.0,
		0.0, 0.13, 0.0,
		0.0, 0.0, 0.13;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegFRU}
	}
	Solid soLegFRL{
		0.13;
		0.13, 0.0, 0.0,
		0.0, 0.13, 0.0,
		0.0, 0.0, 0.13;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegFRL}
	}
	
	Solid soLegRLS{
		0.1;
		0.1, 0.0, 0.0,
		0.0, 0.1, 0.0,
		0.0, 0.0, 0.1;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegRLS}
	}
	Solid soLegRLU{
		0.13;
		0.13, 0.0, 0.0,
		0.0, 0.13, 0.0,
		0.0, 0.0, 0.13;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegRLU}
	}
	Solid soLegRLL{
		0.13;
		0.13, 0.0, 0.0,
		0.0, 0.13, 0.0,
		0.0, 0.0, 0.13;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegRLL}
	}
	Solid soLegRRS{
		0.1;
		0.1, 0.0, 0.0,
		0.0, 0.1, 0.0,
		0.0, 0.0, 0.1;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegRRS}
	}
	Solid soLegRRU{
		0.13;
		0.13, 0.0, 0.0,
		0.0, 0.13, 0.0,
		0.0, 0.0, 0.13;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegRRU}
	}
	Solid soLegRRL{
		0.13;
		0.13, 0.0, 0.0,
		0.0, 0.13, 0.0,
		0.0, 0.0, 0.13;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frLegRRL}
	}
}
GravityEngine{
	0.0; -98.0; 0.0;;
	{soLegFLU}
	{soLegFLL}
	{soLegFRU}
	{soLegFRL}
	{soLegRLU}
	{soLegRLL}
	{soLegRRU}
	{soLegRRL}
	{soBody}
}

ContactEngine{
	{Pointer1}
	{Pointer2}
	{soBody}
	{soLegFLU}
	{soLegFLL}
	{soLegFRU}
	{soLegFRL}
	{soLegRLU}
	{soLegRLL}
	{soLegRLU}
	{soLegRLL}
	{soLegRRU}
	{soLegRRL}
	{frFloor}
	ContactInactive{
		1;
		0,1;
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
PhysicalMaterial pmPointer{
		0.2; 0.1; 0.2; 0.1;             # 抗力のバネ係数，ダンパ係数，摩擦力のバネ係数，ダンパ係数
                                          # (デフォルト値の何倍かで設定する．つまりデフォルト値は1.0)
		0.6;                            # 静止摩擦係数(デフォルト値は0.6)
		0.3;                            # 動摩擦係数(デフォルト値は0.3)
}
Frame frPointer2{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		3.0, 1.0,0.0,1.0;;
	}
	Import{ "cubeSmall2.xi"; }
#	Sphere{ 0.3; 10; 10; {matPointer1} {pmPointer} }
}
Frame frPointer1{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		-3.0, 1.0,0.0,1.0;;
	}
	Import{ "cubeSmall1.xi"; }
#	Sphere{ 0.3; 10; 10; {matPointer2} {pmPointer} }
}
Frame frBody{
	FrameTransformMatrix  {
		1.0,  0.0, 0.0, 0.0,
		0.0,  1.0, 0.0, 0.0,
		0.0,  0.0, 1.0, 0.0,
		0.0, 3.0, 0.0, 1.0;;
	}
	Frame{
		FrameTransformMatrix  {
			2.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}

Frame frLegFLS{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
	}
}
Frame frLegFLU{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}
Frame frLegFLL{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}

Frame frLegFRS{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
	}
}
Frame frLegFRU{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}
Frame frLegFRL{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}

Frame frLegRLS{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
	}
}
Frame frLegRLU{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}
Frame frLegRLL{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}
Frame frLegRRS{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
	}
}
Frame frLegRRU{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}
Frame frLegRRL{
	Frame{
		FrameTransformMatrix  {
			0.2,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,0.2,0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Import{ "cube.xi"; }
	}
}

Frame frFloor{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0, 0.0, 0.0, 1.0;;
	}
	Import{ "floor.xi";}
}
JointEngine{
	{soBody}
	Joint joLegFLU{
		0;						#関節の種類	0:蝶番, 1:スライド
		0.8; 0.0; 0.6;;			#関節から見た親剛体の位置
		1.0, 0.0, 0.0,			#関節から見た親剛体の向き
		0.0, 1.0, 0.0,			
		0.0, 0.0, 1.0;;			
		0.0; 0.4; 0.0;;			#関節から見た子剛体の位置
		1.0, 0.0, 0.0,			#関節から見た子剛体の向き
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;			
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.1; 100.0; 0.0; 10.0;		#位置入力の例
#		2; 2.0; 100.0; 0.01; 0.5;		#速度入力の例
#		3; 0.0; 0.0; 0.0; 0.0;
		{soLegFLS}
		Joint joLegFLS{
			0;						#関節の種類	0:蝶番, 1:スライド
			0.0; 0.0; 0.0;;			#関節から見た親剛体の位置
			0.0, 0.0, 1.0,			#関節から見た親剛体の向き
			0.0, 1.0, 0.0,			
			-1.0, 0.0, 0.0;;			
			0.0; 0.0; 0.0;;			#関節から見た子剛体の位置
			0.0, 0.0, 1.0,			#関節から見た子剛体の向き
			0.0, 1.0, 0.0,			
			-1.0, 0.0, 0.0;;			
			#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
			3; 0.3; 100.0; 0.0; 10.0;		#位置入力の例
			{soLegFLU}
			Joint joLegFLL{
				0;						#関節の種類	0:蝶番, 1:スライド
				0.0; -0.4; 0.2;;		#関節から見た親剛体の位置
				1.0, 0.0, 0.0,			#関節から見た親剛体の向き
				0.0, 1.0, 0.0,			
				0.0, 0.0, 1.0;;			
				0.0; 0.4; 0.0;;			#関節から見た子剛体の位置
				1.0, 0.0, 0.0,			#関節から見た子剛体の向き
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0;;			
				#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
				3; 0.0; 50.0; 0.0; 5.0;			#位置入力の例
				{soLegFLL}
			}
		}
	}
	Joint joLegFRU{
		0;						#関節の種類	0:蝶番, 1:スライド
		0.8; 0.0; -0.6;;			#関節から見た親剛体の位置
		1.0, 0.0, 0.0,			#関節から見た親剛体の向き
		0.0, 1.0, 0.0,			
		0.0, 0.0, 1.0;;			
		0.0; 0.4; 0.0;;			#関節から見た子剛体の位置
		1.0, 0.0, 0.0,			#関節から見た子剛体の向き
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;			
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.1; 100.0; 0.0; 10.0;		#位置入力の例
#		2; 2.0; 100.0; 0.01; 0.5;		#速度入力の例
#		3; 0.0; 0.0; 0.0; 0.0;
		{soLegFRS}
		Joint joLegFRS{
			0;						#関節の種類	0:蝶番, 1:スライド
			0.0; 0.0; 0.0;;			#関節から見た親剛体の位置
			0.0, 0.0, 1.0,			#関節から見た親剛体の向き
			0.0, -1.0, 0.0,			
			1.0, 0.0, 0.0;;			
			0.0; 0.0; 0.0;;			#関節から見た子剛体の位置
			0.0, 0.0, 1.0,			#関節から見た子剛体の向き
			0.0, -1.0, 0.0,			
			1.0, 0.0, 0.0;;			
			#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
			3; 0.3; 100.0; 0.0; 10.0;		#位置入力の例
			{soLegFRU}
			Joint joLegFRL{
				0;						#関節の種類	0:蝶番, 1:スライド
				0.0; -0.4; -0.2;;			#関節から見た親剛体の位置
				1.0, 0.0, 0.0,			#関節から見た親剛体の向き
				0.0, 1.0, 0.0,			
				0.0, 0.0, 1.0;;			
				0.0; 0.4; 0.0;;			#関節から見た子剛体の位置
				1.0, 0.0, 0.0,			#関節から見た子剛体の向き
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0;;			
				#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
				3; 0.0; 50.0; 0.0; 5.0;			#位置入力の例
	#			2; 2.0; 100.0; 0.01; 0.5;		#速度入力の例
	#			3; 0.0; 0.0; 0.0; 0.0;
				{soLegFRL}
			}
		}
	}
	Joint joLegRLU{
		0;						#関節の種類	0:蝶番, 1:スライド
		-0.8; 0.0; 0.6;;			#関節から見た親剛体の位置
		1.0, 0.0, 0.0,			#関節から見た親剛体の向き
		0.0, 1.0, 0.0,			
		0.0, 0.0, 1.0;;			
		0.0; 0.4; 0.0;;			#関節から見た子剛体の位置
		1.0, 0.0, 0.0,			#関節から見た子剛体の向き
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;			
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; -0.1; 100.0; 0.0; 10.0;		#位置入力の例
#		2; 2.0; 100.0; 0.01; 0.5;		#速度入力の例
#		3; 0.0; 0.0; 0.0; 0.0;
		{soLegRLS}
		Joint joLegRLS{
			0;						#関節の種類	0:蝶番, 1:スライド
			0.0; 0.0; 0.0;;			#関節から見た親剛体の位置
			0.0, 0.0, 1.0,			#関節から見た親剛体の向き
			0.0, 1.0, 0.0,			
			-1.0, 0.0, 0.0;;			
			0.0; 0.0; 0.0;;			#関節から見た子剛体の位置
			0.0, 0.0, 1.0,			#関節から見た子剛体の向き
			0.0, 1.0, 0.0,			
			-1.0, 0.0, 0.0;;			
			#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
			3; 0.3; 100.0; 0.0; 10.0;		#位置入力の例
			{soLegRLU}
			Joint joLegRLL{
				0;						#関節の種類	0:蝶番, 1:スライド
				0.0; -0.4; 0.2;;		#関節から見た親剛体の位置
				1.0, 0.0, 0.0,			#関節から見た親剛体の向き
				0.0, 1.0, 0.0,			
				0.0, 0.0, 1.0;;			
				0.0; 0.4; 0.0;;			#関節から見た子剛体の位置
				1.0, 0.0, 0.0,			#関節から見た子剛体の向き
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0;;			
				#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
				3; 0.0; 50.0; 0.0; 5.0;			#位置入力の例
	#			2; 2.0; 100.0; 0.01; 0.5;		#速度入力の例
	#			3; 0.0; 0.0; 0.0; 0.0;
				{soLegRLL}
			}
		}
	}
	Joint joLegRRU{
		0;						#関節の種類	0:蝶番, 1:スライド
		-0.8; 0.0; -0.6;;			#関節から見た親剛体の位置
		1.0, 0.0, 0.0,			#関節から見た親剛体の向き
		0.0, 1.0, 0.0,			
		0.0, 0.0, 1.0;;			
		0.0; 0.4; 0.0;;			#関節から見た子剛体の位置
		1.0, 0.0, 0.0,			#関節から見た子剛体の向き
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;			
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; -0.1; 100.0; 0.0; 10.0;		#位置入力の例
#		2; 2.0; 100.0; 0.01; 0.5;		#速度入力の例
#		3; 0.0; 0.0; 0.0; 0.0;
		{soLegRRS}
		Joint joLegRRS{
			0;						#関節の種類	0:蝶番, 1:スライド
			0.0; 0.0; 0.0;;			#関節から見た親剛体の位置
			0.0, 0.0, 1.0,			#関節から見た親剛体の向き
			0.0, -1.0, 0.0,			
			1.0, 0.0, 0.0;;			
			0.0; 0.0; 0.0;;			#関節から見た子剛体の位置
			0.0, 0.0, 1.0,			#関節から見た子剛体の向き
			0.0, -1.0, 0.0,			
			1.0, 0.0, 0.0;;			
			#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
			3; 0.3; 100.0; 0.0; 10.0;		#位置入力の例
			{soLegRRU}
			Joint joLegRRL{
				0;						#関節の種類	0:蝶番, 1:スライド
				0.0; -0.4; -0.2;;			#関節から見た親剛体の位置
				1.0, 0.0, 0.0,			#関節から見た親剛体の向き
				0.0, 1.0, 0.0,			
				0.0, 0.0, 1.0;;			
				0.0; 0.4; 0.0;;			#関節から見た子剛体の位置
				1.0, 0.0, 0.0,			#関節から見た子剛体の向き
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0;;			
				#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
				3; 0.0; 50.0; 0.0; 5.0;			#位置入力の例
	#			2; 2.0; 100.0; 0.01; 0.5;		#速度入力の例
	#			3; 0.0; 0.0; 0.0; 0.0;
				{soLegRRL}
			}
		}
	}
}

