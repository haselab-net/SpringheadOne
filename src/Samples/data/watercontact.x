xof 0303txt 0032
Scene {
	Simulator{0.01; 0.99;}
	SolidContainer{
		Solid soBlock1{
			6.0;
			10.0, 0.0, 0.0,
			0.0, 10.0, 0.0,
			0.0, 0.0, 10.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock1}
		}
	}
	GravityEngine{
		0.0; -9.8; 0.0;;
		{soBlock1}
	}
	WaterEngine{
		{water1}
	}
	WaterContactEngine{
		{water1}
		{soBlock1}
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
	Frame frWater{
		FrameTransformMatrix  {
			1.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, -1.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Water water1{
			60;
			60;
			0.1;	//dh
			1.0;	//depth
			9.8;	//gravity
			1.0;	//hscale
			1000.0;	//density
			0.99;	//loss
			0.0; -3.0;	//	vx, vy
			WaterTrackTarget{
				{frBlock1}
			}
			Material { 
				1.0; 1.0; 1.0; 1.0;;
				5.000000;
				0.30; 0.3; 0.30;;
				0.30;0.30;0.30;;
			TextureFilename { "skydome1.jpg"; }
			}
		}
	}
	Frame frBlock1 {
			FrameTransformMatrix  {
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 2.0, 0.0, 1.0;;
			}
		Import{ "kayak.x"; }
		Frame{
			FrameTransformMatrix  {
				1.0, 0.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, -1.0, 0.0, 0.0,
				0.0, 0.0, 0.0, 1.0;;
			}
#			Import{ "wheel.xi"; }
		}
		Frame{
			FrameTransformMatrix  {
				1.0, 0.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, -0.5, 0.0, 0.0,
				0.0, 0.0, 0.0, 1.0;;
			}
#			Import{ "cube.xi"; }
		}
	}
}

