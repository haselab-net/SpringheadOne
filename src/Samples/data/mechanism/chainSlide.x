xof 0303txt 0032

//回転行列は、
//xx xy xz
//yx yy yz
//zx zy zz
//回転方向は、軸に向かって時計周り

Scene {
	SolidContainer{
		Solid Pointer1{	//	spidar[0]
			0.1;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frPointer1}
		}
		Solid Pointer2{	//	spidar[1]
			0.1;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frPointer2}
		}
		Solid soBlock1{
			1.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock1}
		}
		Solid soBlock2{
			1.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock2}
		}
		Solid soBlock3{
			1.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			1.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock3}
		}
		Solid soBlock4{
			1.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			1.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock4}
		}
		Solid soBlock5{
			1.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			1.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock5}
		}
	}
	GravityEngine{
		0.0; -9.8; 0.0;;
		{soBlock1}
		{soBlock2}
		{soBlock3}
		{soBlock4}
		{soBlock5}
	}
	
	ContactEngine{
		{soBlock1}
		{soBlock2}
		{soBlock3}
		{soBlock4}
		{soBlock5}
	}
	JointEngine{
		//胴体がルート
		{frBlock0}
		Joint jnt01{
			1;						#関節の種類	0:蝶番, 1:スライド
			1.3; 0.0; 0.0;;			#関節から見た親剛体の位置
#			0.0, 1.0, 0.0,			#関節から見た親剛体の向き
#			-1.0, 0.0, 0.0,			
#			0.0, 0.0, 1.0;;			
			0.0, 0.0, -1.0,			
			0.0, 1.0, 0.0,			
			1.0, 0.0, 0.0;;			#関節から見た親剛体の向き
			-1.3; 0.0; 0.0;;		#関節から見た子剛体の位置
#			0.0, -1.0, 0.0,			#関節から見た子剛体の向き
#			1.0, 0.0, 0.0,
#			0.0, 0.0, 1.0;;			
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0,			
			1.0, 0.0, 0.0;;			#関節から見た親剛体の向き
			0.0; 0.0; 200000.0; 0.0; 0.0;	#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
			#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
#			3; 0.0; 100000.0; 0.0; 1000.0;		#位置入力の例
#			2; -0.1; 4000.0; 100.0; 10.0;		#速度入力の例
			3; 0.0; 0.0; 0.0; 0.0;
			{soBlock1}
			Joint jnt12{
				0;
				1.3; 0.0; 0.0;;
				0.0, 1.0, 0.0,
				-1.0, 0.0, 0.0,
				0.0, 0.0, 1.0;;
				-1.3; 0.0; 0.0;;
				0.0, -1.0, 0.0,
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0;;
				3.0; 0.0; 20.0; 0.0; 0.0;
				3; 0.0; 0.0; 0.0; 0.0;
				{soBlock2}
				Joint jnt23{
					0;
					1.3; 0.0; 0.0;;
					0.0, 1.0, 0.0,
					-1.0, 0.0, 0.0,
					0.0, 0.0, 1.0;;
					-1.3; 0.0; 0.0;;
					0.0, -1.0, 0.0,
					1.0, 0.0, 0.0,
					0.0, 0.0, 1.0;;
					3.0; 0.0; 20.0; 0.0; 0.0;
					3; 0.0; 0.0; 0.0; 0.0;
					{soBlock3}
					Joint jnt34{
						0;
						1.3; 0.0; 0.0;;
						0.0, 1.0, 0.0,
						-1.0, 0.0, 0.0,
						0.0, 0.0, 1.0;;
						-1.3; 0.0; 0.0;;
						0.0, -1.0, 0.0,
						1.0, 0.0, 0.0,
						0.0, 0.0, 1.0;;
						3.0; 0.0; 20.0; 0.0; 0.0;
						3; 0.0; 0.0; 0.0; 0.0;
						{soBlock4}
						Joint jnt45{
							0;
							1.3; 0.0; 0.0;;
							0.0, 1.0, 0.0,
							-1.0, 0.0, 0.0,
							0.0, 0.0, 1.0;;
							-1.3; 0.0; 0.0;;
							0.0, -1.0, 0.0,
							1.0, 0.0, 0.0,
							0.0, 0.0, 1.0;;
							3.0; 0.0; 20.0; 0.0; 0.0;
							3; 0.0; 0.0; 0.0; 0.0;
							{soBlock5}
						}
					}
				}
			}
		}
	}
	
 Camera  {
  1.000000,0.000000,0.000000,0.000000,0.000000,0.999626,-0.027311,0.000000,0.000000,0.027312,0.999625,0.000000,0.000000,3.999880,25.416491,1.000000;;
  0.400000;
  0.400000;
  0.000000;
  0.000000;
  0.400000;
  4000.000000;
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
	
	#フレームインポート
	Material matPointer{
	   0.380000;0.698000;1.000000;1.000000;;
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
#		Sphere{ 0.5; 10; 10; {matPointer}}
	}
	Frame frPointer2{
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			-8.0,2.0,0.0,1.0;;
		}
#		Sphere{ 0.5; 10; 10; {matPointer}}
	}
	Frame frBlock0 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			-0.5,2.0,0.0,1.0;;
		}
#		Import{ "chainBlock.xi"; }
	}
	Frame frBlock1 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			-0.5,0.0,0.0,1.0;;
		}
		Import{ "chainBlock.xi"; }
	}
	Frame frBlock2 {
		Import{ "chainBlock.xi"; }
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			3.5,-3.0,0.0,1.0;;
		}
	}
	Frame frBlock3 {
		Import{ "chainBlock.xi"; }
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			4.5,-5.0,0.0,1.0;;
		}
	}
	Frame frBlock4 {
		Import{ "chainBlock.xi"; }
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			6.5,-10.0,0.0,1.0;;
		}
	}
	Frame frBlock5 {
		Import{ "chainBlock.xi"; }
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			10.5,-12.0,0.0,1.0;;
		}
	}
}
