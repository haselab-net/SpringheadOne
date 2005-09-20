xof 0303txt 0032

Frame frGear{
	Frame frGearCore{
		FrameTransformMatrix  {
			1.4,0.0,0.0,0.0,
			0.0,0.0,1.2,0.0,
			0.0,-1.4,0.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Mesh {
		 26;
		 0.00000;1.00000;-1.00000;,
		 0.50000;1.00000;-0.86603;,
		 0.50000;-1.00000;-0.86603;,
		 0.00000;-1.00000;-1.00000;,
		 0.86603;1.00000;-0.50000;,
		 0.86603;-1.00000;-0.50000;,
		 1.00000;1.00000;0.00000;,
		 1.00000;-1.00000;0.00000;,
		 0.86603;1.00000;0.50000;,
		 0.86603;-1.00000;0.50000;,
		 0.50000;1.00000;0.86603;,
		 0.50000;-1.00000;0.86603;,
		 0.00000;1.00000;1.00000;,
		 0.00000;-1.00000;1.00000;,
		 -0.50000;1.00000;0.86603;,
		 -0.50000;-1.00000;0.86603;,
		 -0.86603;1.00000;0.50000;,
		 -0.86603;-1.00000;0.50000;,
		 -1.00000;1.00000;0.00000;,
		 -1.00000;-1.00000;0.00000;,
		 -0.86603;1.00000;-0.50000;,
		 -0.86603;-1.00000;-0.50000;,
		 -0.50000;1.00000;-0.86603;,
		 -0.50000;-1.00000;-0.86603;,
		 0.00000;1.00000;0.00000;,
		 0.00000;-1.00000;-0.00000;;
		 
		 36;
		 4;0,1,2,3;,
		 4;1,4,5,2;,
		 4;4,6,7,5;,
		 4;6,8,9,7;,
		 4;8,10,11,9;,
		 4;10,12,13,11;,
		 4;12,14,15,13;,
		 4;14,16,17,15;,
		 4;16,18,19,17;,
		 4;18,20,21,19;,
		 4;20,22,23,21;,
		 4;22,0,3,23;,
		 3;24,1,0;,
		 3;24,4,1;,
		 3;24,6,4;,
		 3;24,8,6;,
		 3;24,10,8;,
		 3;24,12,10;,
		 3;24,14,12;,
		 3;24,16,14;,
		 3;24,18,16;,
		 3;24,20,18;,
		 3;24,22,20;,
		 3;24,0,22;,
		 3;25,3,2;,
		 3;25,2,5;,
		 3;25,5,7;,
		 3;25,7,9;,
		 3;25,9,11;,
		 3;25,11,13;,
		 3;25,13,15;,
		 3;25,15,17;,
		 3;25,17,19;,
		 3;25,19,21;,
		 3;25,21,23;,
		 3;25,23,3;;
		 
		PhysicalMaterial{
			0.10; 0.05; 0.10; 0.05;
			0.0; 0.0;
		}
		 MeshMaterialList {
		  1;
		  36;
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0,
		  0;;
		  Material {
		   1.000000;1.000000;1.000000;1.000000;;
		   0.000000;
		   0.000000;0.000000;0.000000;;
		   0.000000;0.000000;0.000000;;
		  }
		 }
		}
	}
	Frame frBit0{ 
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{ "GearBit.xi"; }
	}
	Frame frBit30{ 
		FrameTransformMatrix  {
			0.866025,-0.500000,0.000000,0.000000,
			0.500000,0.866025,0.000000,0.000000,
			0.000000,0.000000,1.00000,0.000000,
			0.000000,0.000000,0.000000,1.00000;;
		}
		Import{ "GearBit.xi"; }
	}
	Frame frBit60{ 
		FrameTransformMatrix  {
			0.500000,-0.866025,0.000000,0.000000,
			0.866025,0.500000,0.000000,0.000000,
			0.000000,0.000000,1.00000,0.000000,
			0.000000,0.000000,0.000000,1.00000;;
		}
		Import{ "GearBit.xi"; }
	}
	Frame frBit90{ 
		FrameTransformMatrix  {
			0.00000,-1.00000,0.000000,0.000000,
			1.00000,0.00000,0.000000,0.000000,
			0.000000,0.000000,1.00000,0.000000,
			0.000000,0.000000,0.000000,1.00000;;
		}
		Import { "GearBit.xi"; }
	}
	Frame frBit120{ 
		FrameTransformMatrix  {
			-0.500000,-0.866025,0.000000,0.000000,
			0.866025,-0.500000,0.000000,0.000000,
			0.000000,0.000000,1.00000,0.000000,
			0.000000,0.000000,0.000000,1.00000;;
		}
		Import{ "GearBit.xi"; }
	}
	Frame frBit150{ 
		FrameTransformMatrix  {
			-0.866025,-0.500000,0.000000,0.000000,
			0.500000,-0.866025,0.000000,0.000000,
			0.000000,0.000000,1.00000,0.000000,
			0.000000,0.000000,0.000000,1.00000;;
		}
		Import{ "GearBit.xi"; }
	}
}

