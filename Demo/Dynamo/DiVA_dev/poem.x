xof 0302txt 0064

template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template FrameTransformMatrix {
 <F6F23F41-7686-11cf-8F52-0040333594A3>
 Matrix4x4 frameMatrix;
}

template Frame {
 <3D82AB46-62DA-11cf-AB39-0020AF71E433>
 [...]
}

template GravityEngine {
 <fa58a120-587e-4e15-a023-61cf996858e6>
 Vector accel;
 [Solid]
}


Header {
 1;
 0;
 1;
}


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
		Solid soBlock7{
			1.0;			#　質量
			1.0, 0.0, 0.0,		#
			0.0, 1.0, 0.0,		#　慣性テンソル
			0.0, 0.0, 1.0;;		#
			0.0; 0.0; 0.0;;		#　速度
			0.0; 0.0; 0.0;;		#　角速度
			0.0; 0.0; 0.0;;		#　重心　メッシュの原点が0
			{frBlock7}		# フレームへの参照
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
		{soBlock7}
	}
	PenaltyEngine{
		Penalty{
			{frPointer}
			{frFloor}
			{Pointer}
		}
		Penalty{
			{frPointer2}
			{frFloor}
			{Pointer2}
		}
		Penalty{
			{frBlock1}
			{frFloor}
			{soBlock1}
		}
		Penalty{
			{frBlock2}
			{frFloor}
			{soBlock2}
		}
		Penalty{
			{frBlock3}
			{frFloor}
			{soBlock3}
		}
		Penalty{
			{frBlock4}
			{frFloor}
			{soBlock4}
		}
		Penalty{
			{frBlock5}
			{frFloor}
			{soBlock5}
		}
		Penalty{
			{frBlock6}
			{frFloor}
			{soBlock6}
		}
		Penalty{
			{frBlock1}
			{frPointer}
			{soBlock1}
			{Pointer}
		}
		Penalty{
			{frBlock2}
			{frPointer}
			{soBlock2}
			{Pointer}
		}
		Penalty{
			{frBlock3}
			{frPointer}
			{soBlock3}
			{Pointer}
		}
		Penalty{
			{frBlock4}
			{frPointer}
			{soBlock4}
			{Pointer}
		}
		Penalty{
			{frBlock5}
			{frPointer}
			{soBlock5}
			{Pointer}
		}
		Penalty{
			{frBlock6}
			{frPointer}
			{soBlock6}
			{Pointer}
		}
		Penalty{
			{frBlock1}
			{frPointer2}
			{soBlock1}
			{Pointer2}
		}
		Penalty{
			{frBlock2}
			{frPointer2}
			{soBlock2}
			{Pointer2}
		}
		Penalty{
			{frBlock3}
			{frPointer2}
			{soBlock3}
			{Pointer2}
		}
		Penalty{
			{frBlock4}
			{frPointer2}
			{soBlock4}
			{Pointer2}
		}
		Penalty{
			{frBlock5}
			{frPointer2}
			{soBlock5}
			{Pointer2}
		}
		Penalty{
			{frBlock6}
			{frPointer2}
			{soBlock6}
			{Pointer2}
		}
		Penalty{
			{frBlock1}
			{frBlock2}
			{soBlock1}
			{soBlock2}
		}
		Penalty{
			{frBlock1}
			{frBlock3}
			{soBlock1}
			{soBlock3}
		}
		Penalty{
			{frBlock1}
			{frBlock4}
			{soBlock1}
			{soBlock4}
		}
		Penalty{
			{frBlock1}
			{frBlock5}
			{soBlock1}
			{soBlock5}
		}
		Penalty{
			{frBlock1}
			{frBlock6}
			{soBlock1}
			{soBlock6}
		}
		Penalty{
			{frBlock2}
			{frBlock3}
			{soBlock2}
			{soBlock3}
		}
		Penalty{
			{frBlock2}
			{frBlock4}
			{soBlock2}
			{soBlock4}
		}
		Penalty{
			{frBlock2}
			{frBlock5}
			{soBlock2}
			{soBlock5}
		}
		Penalty{
			{frBlock2}
			{frBlock6}
			{soBlock2}
			{soBlock6}
		}
		Penalty{
			{frBlock3}
			{frBlock4}
			{soBlock3}
			{soBlock4}
		}
		Penalty{
			{frBlock3}
			{frBlock5}
			{soBlock3}
			{soBlock5}
		}
		Penalty{
			{frBlock3}
			{frBlock6}
			{soBlock3}
			{soBlock6}
		}
		Penalty{
			{frBlock4}
			{frBlock5}
			{soBlock4}
			{soBlock5}
		}
		Penalty{
			{frBlock4}
			{frBlock6}
			{soBlock4}
			{soBlock6}
		}
		Penalty{
			{frBlock5}
			{frBlock6}
			{soBlock5}
			{soBlock6}
		}
		Penalty{
			{frBlock6}	#衝突判定対象フレーム１
			{frBlock7}	#衝突判定対象フレーム２
			{soBlock6}	#反力を加える剛体１
			{soBlock7}	#反力を加える剛体２
		}
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
     10.0, 10.0, 20.0, 1.000000;;
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
     10.0, 10.0, 20.0, 1.000000;;
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
     -0.5, 5.5, 0.0, 1.000000;;
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
  2;
  6;
  1,
  0,
  1,
  0,
  0,
  1;;
  Material {
   0.905882;1.000000;0.317647;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.226471;0.250000;0.079412;;
   TextureFilename {
    "earth.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250000;0.250000;0.250000;;
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
 MeshTextureCoords {
  24;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;;
 }
}	#mesh

  }	#frame
############################### ここからフレーム7
  Frame frBlock7{
    FrameTransformMatrix{
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     -0.5, 7.5, 0.0, 1.000000;;
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
  2;
  6;
  1,
  0,
  1,
  0,
  0,
  1;;
  Material {
   0.905882;1.000000;0.317647;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.226471;0.250000;0.079412;;
   TextureFilename {
    "earth.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250000;0.250000;0.250000;;
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
 MeshTextureCoords {
  24;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;
  0.000000;0.000000;
  1.000000;0.000000;
  1.000000;1.000000;
  0.000000;1.000000;;
 }
}	#mesh

  }	#frame
###############################　ここまでフレーム7
  Frame frFloor{
    FrameTransformMatrix {
     1.000000,0.000000,0.000000,0.000000,
     0.000000,1.000000,0.000000,0.000000,
     0.000000,0.000000,1.000000,0.000000,
     0.0, -1.0, 0.0, 1.000000;;
    }
	Mesh {
 24;
 -5.0; 0.0; 5.0;,	 5.0; 0.0; 5.0;,	 5.0; 0.0;-5.0;,	-5.0; 0.0;-5.0;,
 -5.0; 0.0;-5.0;,	 5.0; 0.0;-5.0;,	 5.0;-1.0;-5.0;,	-5.0;-1.0;-5.0;,
  5.0; 0.0; 5.0;,	 5.0;-1.0; 5.0;,	 5.0;-1.0;-5.0;,	 5.0; 0.0;-5.0;,
 -5.0;-1.0; 5.0;,	 5.0;-1.0; 5.0;,	 5.0; 0.0; 5.0;,	-5.0; 0.0; 5.0;,
 -5.0;-1.0; 5.0;,	-5.0; 0.0; 5.0;,	-5.0; 0.0;-5.0;,	-5.0;-1.0;-5.0;,
 -5.0;-1.0;-5.0;,	 5.0;-1.0;-5.0;,	 5.0;-1.0; 5.0;,	-5.0;-1.0; 5.0;;
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
