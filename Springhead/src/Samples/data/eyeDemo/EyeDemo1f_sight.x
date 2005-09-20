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
	
	Solid soTarget1{ //視標
		0.3;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frTarget1}
	}
	
	Solid soOcularL{
		0.3;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frOcularL}
	}
	Solid soOcularR{
		0.3;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frOcularR}
	}
	Solid soCube3{
		0.3;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{frCube3}
	}
}
	
GravityEngine{
	0.0; -98.0; 0.0;;
	
	#{soCube1}
	#{soCube2}
	#{soCube3}
}



Material matPointer1{
   1.000000;0.380000;0.398000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.095000;0.174500;0.250000;;
}

Material matEye{
   1.000000;0.380000;0.398000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.095000;0.174500;0.250000;;
}

Material matTarget1{
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


Frame frPointer1{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,0.0,1.0;;
		#0.0,2.0,2.0,1.0;;
	}
	#Import{"eye.x";}
	#Import{ "cubeSmall1.xi"; }
	#Sphere{ 0.2; 20; 20; {matPointer1} {pmPointer} }
}

Frame frTarget1{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,-10.0,1.0;;
	}	
	Sphere{ 0.2; 20; 20; {matTarget1} }
}

Frame frOcularL{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,		
		0.0,0.0,1.0,0.0,
		20.0,0.0,0.0,10.0;;
		#-2.0, 2.0, -1.0, 1.0;;
	}
	#Import{ "cube2.xi"; }
	#Import{ "body0.x"; }
	Import{ "./eye/eye0910Bsight.x"; }
}
Frame frOcularR{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
               -20.0,0.0,0.0,10.0;;
		#1.0, 0.3, 1.1,1.0;;
	}
	Import{ "./eye/eye0910Bsight.x"; }
}
Frame frCube3{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,5.0,1.0;;
		#0.0,0.0,0.0,1.0;;
	}
	#Import{ "cube2.xi"; }
}

Frame frFloor{
	FrameTransformMatrix  {
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0, 0.0, 0.0, 1.0;;
	}
	Mesh {
		24;
		-25.0; 0.0; 25.0;,	 25.0; 0.0; 25.0;,	 25.0; 0.0;-25.0;,	-25.0; 0.0;-25.0;,
		-25.0; 0.0;-25.0;,	 25.0; 0.0;-25.0;,	 25.0;-1.0;-25.0;,	-25.0;-1.0;-25.0;,
		25.0; 0.0; 25.0;,	 25.0;-1.0; 25.0;,	 25.0;-1.0;-25.0;,	 25.0; 0.0;-25.0;,
		-25.0;-1.0; 25.0;,	 25.0;-1.0; 25.0;,	 25.0; 0.0; 25.0;,	-25.0; 0.0; 25.0;,
		-25.0;-1.0; 25.0;,	-25.0; 0.0; 25.0;,	-25.0; 0.0;-25.0;,	-25.0;-1.0;-25.0;,
		-25.0;-1.0;-25.0;,	 25.0;-1.0;-25.0;,	 25.0;-1.0; 25.0;,	-25.0;-1.0; 25.0;;
		6;
		4;0,1,2,3;,
		4;4,5,6,7;,
		4;8,9,10,11;,
		4;12,13,14,15;,
		4;16,17,18,19;,
		4;20,21,22,23;;
		MeshMaterialList {
			1;
			6;
			0,
			0,
			0,
			0,
			0,
			0;;
			Material {
				0.300000;0.300000;0.300000;0.600000;;
				0.000000;
				0.000000;0.000000;0.000000;;
				0.000000;0.000000;0.000000;;
			}
		}
		MeshNormals {
			6;
			0.000000;1.000000;0.000000;,
			0.000000;0.000000;-1.000000;,
			1.000000;0.000000;0.000000;,
			0.000000;0.000000;1.000000;,
			-1.000000;0.000000;0.000000;,
			0.000000;-1.000000;-0.000000;;
			6;
			4;0,0,0,0;,
			4;1,1,1,1;,
			4;2,2,2,2;,
			4;3,3,3,3;,
			4;4,4,4,4;,
			4;5,5,5,5;;
		}
		PhysicalMaterial{
			0.4; 0.2; 0.4; 0.2;
			1.0;
			1.0;
		}
	}
 Camera  {

	0.999999,-0.000000,0.000000,0.000000,# 視点行列
	0.000000,0.958496,-0.285103,0.000000,
	0.000000,0.285104,0.958496,0.000000,
	-0.000013,-0.238736,53.182961,1.000000;;

	0.400000;
   0.400000;		#スクリーンの幅,高さ
	0.000000;
   0.000000;           # カメラからのスクリーンのずれ(X,Y)
	0.400000;
   4000.000000;
        # カメラとクリッピング平面の距離(front,back)  
 }


}
	
}