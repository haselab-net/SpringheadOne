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
		Solid soBlock0{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock0}
		}
		Solid soBlock1{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock1}
		}
		Solid soBlock2{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock2}
		}
		Solid soBlock3{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock3}
		}
		Solid soBlock4{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock4}
		}
		Solid soBlock5{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; -10.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock5}
		}
		Solid soBlock6{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; -10.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock6}
		}
		Solid soBlock7{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; -10.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock7}
		}
		Solid soBlock8{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; -10.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock8}
		}
		Solid soBlock9{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; -10.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock9}
		}
		Solid soBlock10{
			10.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; -10.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock10}
		}
	}
	GravityEngine{
		0.0; -9.8; 0.0;;
		{soBlock0}
		{soBlock1}
		{soBlock2}
		{soBlock3}
		{soBlock4}
		{soBlock5}
	}
	GravityEngine{
		0.0; 5.8; 0.0;;
		{soBlock6}
		{soBlock7}
		{soBlock8}
		{soBlock9}
		{soBlock10}
	}
	
	ContactEngine{
		{Pointer1}
		{soBlock0}
		{soBlock1}
		{soBlock2}
		{soBlock3}
		{soBlock4}
		{soBlock5}
		{soBlock6}
		{soBlock7}
		{soBlock8}
		{soBlock9}
		{soBlock10}
		{frFloor}
	}
	Spring{
		-2.01; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		2.01; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		1000.0; 1000.0; 1000.0;;	100.0; 100.0; 100.0;;	#	kt, bt
		0.0; 0.0; 0.0;;				0.0; 0.0; 0.0;;		#	kr, br
		{soBlock0}
		{soBlock10}
	}
	JointEngine{
		//胴体がルート
		{soBlock0}
		Joint jnt01{
			0;						#関節の種類	0:蝶番, 1:スライド
			2.01; 0.0; 0.0;;			#関節から見た親剛体の位置
			0.0, 1.0, 0.0,			#関節から見た親剛体の向き
			-1.0, 0.0, 0.0,			
			0.0, 0.0, 1.0;;			
			-2.01; 0.0; 0.0;;		#関節から見た子剛体の位置
			0.0, -1.0, 0.0,			#関節から見た子剛体の向き
			1.0, 0.0, 0.0,
			0.0, 0.0, 1.0;;			
			#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
			3.1415; 0.0; 200000.0; 0.0; 0.0;	
			#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
#			3; 0.0; 100000.0; 0.0; 1000.0;		#位置入力の例
#			2; -0.1; 4000.0; 100.0; 10.0;		#速度入力の例
			3; 0.0; 0.0; 0.0; 0.0;
			{soBlock1}
			Joint jnt12{
				0;
				2.01; 0.0; 0.0;;
				0.0, 1.0, 0.0,
				-1.0, 0.0, 0.0,
				0.0, 0.0, 1.0;;
				-2.01; 0.0; 0.0;;
				0.0, -1.0, 0.0,
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0;;
				3.1415; 0.0; 20.0; 0.0; 0.0;
				3; 0.0; 0.0; 0.0; 0.0;
				{soBlock2}
				Joint jnt23{
					0;
					2.01; 0.0; 0.0;;
					0.0, 1.0, 0.0,
					-1.0, 0.0, 0.0,
					0.0, 0.0, 1.0;;
					-2.01; 0.0; 0.0;;
					0.0, -1.0, 0.0,
					1.0, 0.0, 0.0,
					0.0, 0.0, 1.0;;
					3.1415; 0.0; 20.0; 0.0; 0.0;
					3; 0.0; 0.0; 0.0; 0.0;
					{soBlock3}
					Joint jnt34{
						0;
						2.01; 0.0; 0.0;;
						0.0, 1.0, 0.0,
						-1.0, 0.0, 0.0,
						0.0, 0.0, 1.0;;
						-2.01; 0.0; 0.0;;
						0.0, -1.0, 0.0,
						1.0, 0.0, 0.0,
						0.0, 0.0, 1.0;;
						1.0; 0.0; 20.0; 0.0; 0.0;
						3; 0.0; 0.0; 0.0; 0.0;
						{soBlock4}
						Joint jnt45{
							0;
							2.01; 0.0; 0.0;;
							0.0, 1.0, 0.0,
							-1.0, 0.0, 0.0,
							0.0, 0.0, 1.0;;
							-2.01; 0.0; 0.0;;
							0.0, -1.0, 0.0,
							1.0, 0.0, 0.0,
							0.0, 0.0, 1.0;;
							3.1415; 0.0; 20.0; 0.0; 0.0;
							3; 0.0; 0.0; 0.0; 0.0;
							{soBlock5}
							Joint jnt56{
								0;
								2.01; 0.0; 0.0;;
								0.0, 1.0, 0.0,
								-1.0, 0.0, 0.0,
								0.0, 0.0, 1.0;;
								-2.01; 0.0; 0.0;;
								0.0, -1.0, 0.0,
								1.0, 0.0, 0.0,
								0.0, 0.0, 1.0;;
								3.1415; 0.0; 20.0; 0.0; 0.0;
								3; 0.0; 0.0; 0.0; 0.0;
								{soBlock6}
								Joint jnt67{
									0;
									2.01; 0.0; 0.0;;
									0.0, 1.0, 0.0,
									-1.0, 0.0, 0.0,
									0.0, 0.0, 1.0;;
									-2.01; 0.0; 0.0;;
									0.0, -1.0, 0.0,
									1.0, 0.0, 0.0,
									0.0, 0.0, 1.0;;
									3.1415; 0.0; 20.0; 0.0; 0.0;
									3; 0.0; 0.0; 0.0; 0.0;
									{soBlock7}
									Joint jnt78{
										0;
										2.01; 0.0; 0.0;;
										0.0, 1.0, 0.0,
										-1.0, 0.0, 0.0,
										0.0, 0.0, 1.0;;
										-2.01; 0.0; 0.0;;
										0.0, -1.0, 0.0,
										1.0, 0.0, 0.0,
										0.0, 0.0, 1.0;;
										1.0; 0.0; 20.0; 0.0; 0.0;
										3; 0.0; 0.0; 0.0; 0.0;
										{soBlock8}
										Joint jnt89{
											0;
											2.01; 0.0; 0.0;;
											0.0, 1.0, 0.0,
											-1.0, 0.0, 0.0,
											0.0, 0.0, 1.0;;
											-2.01; 0.0; 0.0;;
											0.0, -1.0, 0.0,
											1.0, 0.0, 0.0,
											0.0, 0.0, 1.0;;
											3.1415; 0.0; 20.0; 0.0; 0.0;
											3; 0.0; 0.0; 0.0; 0.0;
											{soBlock9}
											Joint jnt910{
												0;
												2.01; 0.0; 0.0;;
												0.0, 1.0, 0.0,
												-1.0, 0.0, 0.0,
												0.0, 0.0, 1.0;;
												-2.01; 0.0; 0.0;;
												0.0, -1.0, 0.0,
												1.0, 0.0, 0.0,
												0.0, 0.0, 1.0;;
												3.1415; 0.0; 20.0; 0.0; 0.0;
												3; 0.0; 0.0; 0.0; 0.0;
												{soBlock10}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
Camera  {
  1.000000,0.000000,0.000000,0.000000,0.000000,0.852046,-0.523464,0.000000,0.000000,0.523466,0.852045,0.000000,0.000000,3.999867,34.416405,1.000000;;
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

		Sphere{ 0.5; 10; 10; {matPointer}}
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
			-5.0, 5.0, 0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock1 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock2 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock3 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock4 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock5 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock6 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock7 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock8 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock9 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frBlock10 {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "chainFreeBlock.xi"; }
	}
	Frame frFloor{
		Frame{
			FrameTransformMatrix  {
				10.0,0.0,0.0,0.0,
				0.0,10.0,0.0,0.0,
				0.0,0.0,10.0,0.0,
				0.0, -10.0, 0.0, 1.0;;
			}
			Import{ "floor.xi"; }
		}
	}
}
