xof 0302txt 0064

Scene {
 Simulator{
 	0.001;
 }
 Camera {
  1.000000,0.000000,0.000000,0.000000,0.000000,0.852045,-0.523465,0.000000,0.000000,0.523466,0.852046,0.000000,0.000000,-0.000002,7.716405,1.000000;;
  0.100000;
  0.100000;
  0.000000;
  0.000000;
  0.100000;
  1000.000000;
 }
 	GravityEngine{
 		0.0; -30.0; 0.0;;
 	}
	ContactEngine{
		{soCube}
	}
	SolidContainer{
		Solid soDrag{
			1000000000000000000.0;
			1000000000000000000.0, 0.0, 0.0,
			0.0, 1000000000000000000.0, 0.0,
			0.0, 0.0, 1000000000000000000.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frDrag}
		}
		Solid soCube{
			1.0;
			1000000000000000000.0, 0.0, 0.0,
			0.0, 1000000000000000000.0, 0.0,
			0.0, 0.0, 1000000000000000000.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frCube}
		}
	}
	Spring{
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;
		# t=0.001, m=1, B=bt /m, K=2k t^2 /m
		# b = mB/t,  k= mK/2t^2
		3900000.0; 3900000.0; 3900000.0;;
		2000.0; 2000.0; 2000.0;;
		0.0; 0.0; 0.0;;
		0.0; 0.0; 0.0;;
		{soCube}
		{soDrag}
	}
}

Frame frTop {
  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   0, 0, 0, 1.000000;;
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
		0.000000;0.000000;0.000000;0.000000;;
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
	Frame frDrag {
		FrameTransformMatrix {
			1.000000,0.000000,0.000000,0.000000,
			0.000000,1.000000,0.000000,0.000000,
			0.000000,0.000000,1.000000,0.000000,
			0.0, 0.0, 0.0, 1.0;;
		}
		Mesh {
			8;
			-0.020; 0.020; 0.020;,
			 0.020; 0.020; 0.020;,
			 0.020; 0.020;-0.020;,
			-0.020; 0.020;-0.020;,
			 0.020;-0.020;-0.020;,
			-0.020;-0.020;-0.020;,
			 0.020;-0.020; 0.020;,
			-0.020;-0.020; 0.020;;

			6;
			4;0,1,2,3;,
			4;3,2,4,5;,
			4;5,4,6,7;,
			4;7,6,1,0;,
			4;1,6,4,2;,
			4;7,0,3,5;;

			MeshMaterialList {
				2;
				6;
				1,
				1,
				1,
				1,
				1,
				1;;
				Material {
					1.000000;0.047059;0.047059;1.000000;;
					5.000000;
					0.000000;0.000000;0.000000;;
					0.250000;0.011765;0.011765;;
				}
				Material {
					0.301961;0.301961;1.000000;1.000000;;
					5.000000;
					0.000000;0.000000;0.000000;;
					0.075490;0.075490;0.250000;;
				}
			}
		}
	}
	Frame frCube {
		FrameTransformMatrix {
			1.000000,0.000000,0.000000,0.000000,
			0.000000,1.000000,0.000000,0.000000,
			0.000000,0.000000,1.000000,0.000000,
			-1.0, 0.0,0.0,1.0;;
		}
		Mesh {
			8;
			-0.5;0.5;0.5;,
			0.5;0.5;0.5;,
			0.5;0.5;-0.5;,
			-0.5;0.5;-0.5;,
			0.5;-0.5;-0.5;,
			-0.5;-0.5;-0.5;,
			0.5;-0.5;0.5;,
			-0.5;-0.5;0.5;;

			6;
			4;0,1,2,3;,
			4;3,2,4,5;,
			4;5,4,6,7;,
			4;7,6,1,0;,
			4;1,6,4,2;,
			4;7,0,3,5;;
			MeshMaterialList {
				2;
				6;
				1,
				1,
				1,
				1,
				1,
				1;;
				Material {
					0.301961;0.301961;1.000000;1.000000;;
					5.000000;
					0.000000;0.000000;0.000000;;
					0.075490;0.075490;0.250000;;
				}
				Material {
					1.000000;1.0;1.0;1.000000;;
					5.000000;
					0.000000;0.000000;0.000000;;
					0.800000;0.800000;0.800000;;
				}
			}
		}
	}
}
