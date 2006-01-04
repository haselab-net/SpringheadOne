xof 0302txt 0064

Scene {
	SolidContainer{
		Solid Pointer{
			0.25;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frPointer}
		}
		Solid Pointer2{
			0.25;
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
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock1}
		}
		Solid soBlock2{
			1.0;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock2}
		}
		Solid soBlock3{
			1.0;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock3}
		}
		Solid soBlock4{
			1.0;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock4}
		}
		Solid soBlock5{
			1.0;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock5}
		}
		Solid soBlock6{
			1.0;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock6}
		}
	}
	GravityEngine{
		0.0; -9.8; 0.0;;
		{soBlock1}
		{soBlock2}
		{soBlock3}
		{soBlock4}
		{soBlock5}
		{soBlock6}
	}
	ContactEngine{
		{frFloor}
		{Pointer}
		{Pointer2}
		{soBlock1}
		{soBlock2}
		{soBlock3}
		{soBlock4}
		{soBlock5}
		{soBlock6}
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

  Frame frPointer{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     -3.0, 1.0, 0.0, 1.000000;;
    }
    Mesh {
 24;
 -0.35;0.35;0.35;,	0.35;0.35;0.35;,	0.35;0.35;-0.35;,	-0.35;0.35;-0.35;,
 -0.35;0.35;-0.35;,	0.35;0.35;-0.35;,	0.35;-0.35;-0.35;,	-0.35;-0.35;-0.35;,
 0.35;0.35;0.35;,	0.35;-0.35;0.35;,	0.35;-0.35;-0.35;,	0.35;0.35;-0.35;,
 -0.35;-0.35;0.35;,	0.35;-0.35;0.35;,	0.35;0.35;0.35;,	-0.35;0.35;0.35;,
 -0.35;-0.35;0.35;,	-0.35;0.35;0.35;,	-0.35;0.35;-0.35;,	-0.35;-0.35;-0.35;,
 -0.35;-0.35;-0.35;,0.35;-0.35;-0.35;,	0.35;-0.35;0.35;,	-0.35;-0.35;0.35;;
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
		   0.313726;0.945098;0.992157;1.000000;;
		   5.000000;
		   0.000000;0.000000;0.000000;;
		   0.078431;0.236275;0.248039;;
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
		}
  }
  Frame frPointer2{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     3.0, 1.0, 0.0, 1.000000;;
    }
    Mesh {
 24;
 -0.35;0.35;0.35;,	0.35;0.35;0.35;,	0.35;0.35;-0.35;,	-0.35;0.35;-0.35;,
 -0.35;0.35;-0.35;,	0.35;0.35;-0.35;,	0.35;-0.35;-0.35;,	-0.35;-0.35;-0.35;,
 0.35;0.35;0.35;,	0.35;-0.35;0.35;,	0.35;-0.35;-0.35;,	0.35;0.35;-0.35;,
 -0.35;-0.35;0.35;,	0.35;-0.35;0.35;,	0.35;0.35;0.35;,	-0.35;0.35;0.35;,
 -0.35;-0.35;0.35;,	-0.35;0.35;0.35;,	-0.35;0.35;-0.35;,	-0.35;-0.35;-0.35;,
 -0.35;-0.35;-0.35;,0.35;-0.35;-0.35;,	0.35;-0.35;0.35;,	-0.35;-0.35;0.35;;
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
		   0.313726;0.945098;0.992157;1.000000;;
		   5.000000;
		   0.000000;0.000000;0.000000;;
		   0.078431;0.236275;0.248039;;
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
		}
  }
  Frame frBlock1{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     0.5, 0.5, 0.0, 1.000000;;
    }
    Mesh meshBlock{
 24;
 -0.5;0.5;1.0;,		0.5;0.5;1.0;,		0.5;0.5;-1.0;,	-0.5;0.5;-1.0;,
 -0.5;0.5;-1.0;,	0.5;0.5;-1.0;,		0.5;-0.5;-1.0;,	-0.5;-0.5;-1.0;,
 0.5;0.5;1.0;,		0.5;-0.5;1.0;,		0.5;-0.5;-1.0;,	0.5;0.5;-1.0;,
 -0.5;-0.5;1.0;,	0.5;-0.5;1.0;,		0.5;0.5;1.0;,	-0.5;0.5;1.0;,
 -0.5;-0.5;1.0;,	-0.5;0.5;1.0;,		-0.5;0.5;-1.0;,	-0.5;-0.5;-1.0;,
 -0.5;-0.5;-1.0;	,0.5;-0.5;-1.0;,	0.5;-0.5;1.0;,	-0.5;-0.5;1.0;;
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
		   1.000000;0.121569;0.043137;1.000000;;
		   5.000000;
		   0.000000;0.000000;0.000000;;
		   0.250000;0.030392;0.010784;;
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
		}
  }
  Frame frBlock2{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     -0.5, 0.5, 0.0, 1.000000;;
    }
	Mesh {
 24;
 -0.5;0.5;1.0;,		0.5;0.5;1.0;,		0.5;0.5;-1.0;,	-0.5;0.5;-1.0;,
 -0.5;0.5;-1.0;,	0.5;0.5;-1.0;,		0.5;-0.5;-1.0;,	-0.5;-0.5;-1.0;,
 0.5;0.5;1.0;,		0.5;-0.5;1.0;,		0.5;-0.5;-1.0;,	0.5;0.5;-1.0;,
 -0.5;-0.5;1.0;,	0.5;-0.5;1.0;,		0.5;0.5;1.0;,	-0.5;0.5;1.0;,
 -0.5;-0.5;1.0;,	-0.5;0.5;1.0;,		-0.5;0.5;-1.0;,	-0.5;-0.5;-1.0;,
 -0.5;-0.5;-1.0;	,0.5;-0.5;-1.0;,	0.5;-0.5;1.0;,	-0.5;-0.5;1.0;;
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
		   1.000000;0.992157;0.062745;1.000000;;
		   5.000000;
		   0.000000;0.000000;0.000000;;
		   0.250000;0.248039;0.015686;;
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
		}
  }
  Frame frBlock3{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     0.0, 1.5, 0.5, 1.000000;;
    }
    Mesh {
 24;
 -1.0; 0.5; 0.5;,	 1.0; 0.5; 0.5;,	 1.0; 0.5;-0.5;,-1.0; 0.5;-0.5;,
 -1.0; 0.5;-0.5;,	 1.0; 0.5;-0.5;,	 1.0;-0.5;-0.5;,-1.0;-0.5;-0.5;,
  1.0; 0.5; 0.5;,	 1.0;-0.5; 0.5;,	 1.0;-0.5;-0.5;, 1.0; 0.5;-0.5;,
 -1.0;-0.5; 0.5;,	 1.0;-0.5; 0.5;,	 1.0; 0.5; 0.5;,-1.0; 0.5; 0.5;,
 -1.0;-0.5; 0.5;,	-1.0; 0.5; 0.5;,	-1.0; 0.5;-0.5;,-1.0;-0.5;-0.5;,
 -1.0;-0.5;-0.5;,	 1.0;-0.5;-0.5;,	 1.0;-0.5;0.5;,	-1.0;-0.5; 0.5;;
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
		   0.058824;0.152941;1.000000;1.000000;;
		   5.000000;
		   0.000000;0.000000;0.000000;;
		   0.014706;0.038235;0.250000;;
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
		}
  }
  Frame frBlock4{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     0.0, 1.5, -0.5, 1.000000;;
    }
    Mesh {
 24;
 -1.0; 0.5; 0.5;,	 1.0; 0.5; 0.5;,	 1.0; 0.5;-0.5;,-1.0; 0.5;-0.5;,
 -1.0; 0.5;-0.5;,	 1.0; 0.5;-0.5;,	 1.0;-0.5;-0.5;,-1.0;-0.5;-0.5;,
  1.0; 0.5; 0.5;,	 1.0;-0.5; 0.5;,	 1.0;-0.5;-0.5;, 1.0; 0.5;-0.5;,
 -1.0;-0.5; 0.5;,	 1.0;-0.5; 0.5;,	 1.0; 0.5; 0.5;,-1.0; 0.5; 0.5;,
 -1.0;-0.5; 0.5;,	-1.0; 0.5; 0.5;,	-1.0; 0.5;-0.5;,-1.0;-0.5;-0.5;,
 -1.0;-0.5;-0.5;,	 1.0;-0.5;-0.5;,	 1.0;-0.5;0.5;,	-1.0;-0.5; 0.5;;
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
		   0.109804;1.000000;0.278431;1.000000;;
		   5.000000;
		   0.000000;0.000000;0.000000;;
		   0.027451;0.250000;0.069608;;
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
		}
  }
  Frame frBlock5{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     0.5, 2.5, 0.0, 1.000000;;
    }
    Mesh {
 24;
 -0.5;0.5;1.0;,		0.5;0.5;1.0;,		0.5;0.5;-1.0;,	-0.5;0.5;-1.0;,
 -0.5;0.5;-1.0;,	0.5;0.5;-1.0;,		0.5;-0.5;-1.0;,	-0.5;-0.5;-1.0;,
 0.5;0.5;1.0;,		0.5;-0.5;1.0;,		0.5;-0.5;-1.0;,	0.5;0.5;-1.0;,
 -0.5;-0.5;1.0;,	0.5;-0.5;1.0;,		0.5;0.5;1.0;,	-0.5;0.5;1.0;,
 -0.5;-0.5;1.0;,	-0.5;0.5;1.0;,		-0.5;0.5;-1.0;,	-0.5;-0.5;-1.0;,
 -0.5;-0.5;-1.0;	,0.5;-0.5;-1.0;,	0.5;-0.5;1.0;,	-0.5;-0.5;1.0;;
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
		   1.000000;0.121569;0.043137;1.000000;;
		   5.000000;
		   0.000000;0.000000;0.000000;;
		   0.250000;0.030392;0.010784;;
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
		}
  }
  Frame frBlock6{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     -0.5, 4.5, 0.0, 1.000000;;
    }
    Mesh {
 24;
 -0.5;0.5;1.0;,		0.5;0.5;1.0;,		0.5;0.5;-1.0;,	-0.5;0.5;-1.0;,
 -0.5;0.5;-1.0;,	0.5;0.5;-1.0;,		0.5;-0.5;-1.0;,	-0.5;-0.5;-1.0;,
 0.5;0.5;1.0;,		0.5;-0.5;1.0;,		0.5;-0.5;-1.0;,	0.5;0.5;-1.0;,
 -0.5;-0.5;1.0;,	0.5;-0.5;1.0;,		0.5;0.5;1.0;,	-0.5;0.5;1.0;,
 -0.5;-0.5;1.0;,	-0.5;0.5;1.0;,		-0.5;0.5;-1.0;,	-0.5;-0.5;-1.0;,
 -0.5;-0.5;-1.0;	,0.5;-0.5;-1.0;,	0.5;-0.5;1.0;,	-0.5;-0.5;1.0;;
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
		   1.000000;0.992157;0.062745;1.000000;;
		   5.000000;
		   0.000000;0.000000;0.000000;;
		   0.250000;0.248039;0.015686;;
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
	}
  }
  Frame frFloor{
    FrameTransformMatrix {
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     0.0, -1.0, 0.0, 1.000000;;
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
	   0.600000;0.600000;0.600000;0.600000;;
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
	}
  }
}
