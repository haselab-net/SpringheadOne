xof 0303txt 0032

Simulator{
	0.005;
}
//回転行列は、
//Exx Exy Exz
//Eyx Eyy Eyz
//Ezx Ezy Ezz
//回転方向は、右手系
Scene {
	JointEngine{
		//胴体がルート
		{frBlock0}					#	関節を取り付け元(親物体)．frame か solid を指定
		JointBall jnt01{			#	ボールジョイント
			#	関節の取り付け位置
			1.0, 0.0, 0.0,			#	関節から見た親剛体の向き
			0.0, 1.0, 0.0,			
			0.0, 0.0, 1.0;;			
			0.0; 0.0; -2.0;;		#	関節から見た親剛体の位置

			1.0, 0.0, 0.0,			#	関節から見た子剛体の向き
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 2.0;;;		#	関節から見た子剛体の位置
			#	関節の初期姿勢
			1.0; 0.0; 0.0; 0.0;;	#	姿勢(Quaternion w x y z)
			0.0; 0.0; 0.0;		#	速度(速度ベクトル)
			ConeLimit{
				#	可動域の設定
				#	子側のz軸が，center を中心とした， cos(theta)=dot となるような
				#	コーンの内部が可動域となる．
				#	z軸周り回転の可動域は，別にラジアンで， minTwist, maxTwist で指定する．
				0.0; 0.0; 1.0;;			#	可動域	center	
				0.8; -0.2; 0.2;			#	可動域	dot minTwist maxTwist
			}
			{soBlock1}				#	子剛体
			JointBallPid {
				0.0; 0.0; 0.0;		#PID
				1.0; 0.0; 0.0; 0.0;;	#目標姿勢(Quaternion w x y z)
				{jnt01}
			}
			JointBall jnt12{			#	ボールジョイント
				1.0, 0.0, 0.0,			#	関節から見た親剛体の向き
				0.0, 1.0, 0.0,			
				0.0, 0.0, 1.0;;			
				0.0; 0.0; -2.0;;		#関節から見た親剛体の位置

				1.0, 0.0, 0.0,			#関節から見た子剛体の向き
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0;;
				0.0; 0.0; 2.0;;;		#関節から見た子剛体の位置

				1.0; 0.0; 0.0; 0.0;;	#姿勢(Quaternion w x y z)
				10.0; 10.0; 20.0;		#速度(速度ベクトル)
				ConeLimit{
					0.0; 0.0; 1.0;;			#可動域	center
					0.3; -0.2; 0.2;			#可動域	dot twist twist
				}
				{soBlock2}
				JointBallPid {
					0.0; 0.0; 0.0;		#PID
					1.0; 0.0; 0.0; 0.0;;	#目標姿勢(Quaternion w x y z)
					{jnt12}
				}
			}
		}
	}
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
	}
	GravityEngine{
		0.0; -98.0; 0.0;;
		{soBlock1}
		{soBlock2}
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
			1.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, -1.0, 0.0, 0.0,
			0.0, 4.0, 0.0, 1.0;;
		}
		Import{ "jointBlock.xi"; }
	}
	Frame frBlock1 {
		Import{ "jointBlock.xi"; }
	}
	Frame frBlock2 {
		Import{ "jointBlock.xi"; }
	}
	Camera  {
		1.000000,0.000000,0.000000,0.000000,0.000000,0.852046,
		-0.523464,0.000000,0.000000,0.523466,0.852045,0.000000,0.000000,3.999867,34.416405,1.000000;;
		0.400000;
		0.400000;
		0.000000;
		0.000000;
		0.400000;
		4000.000000;
	}
}
