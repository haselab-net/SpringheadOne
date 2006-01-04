xof 0303txt 0032
template Header {
	<3d82ab43-62da-11cf-ab39-0020af71e433>
	WORD major;
	WORD minor;
	DWORD flags;
}

template Vector {
	<3d82ab5e-62da-11cf-ab39-0020af71e433>
	FLOAT x;
	FLOAT y;
	FLOAT z;
}

template Coords2d {
	<f6f23f44-7686-11cf-8f52-0040333594a3>
	FLOAT u;
	FLOAT v;
}

template Matrix4x4 {
	<f6f23f45-7686-11cf-8f52-0040333594a3>
	array FLOAT matrix[16];
}

template ColorRGBA {
	<35ff44e0-6c7c-11cf-8f52-0040333594a3>
	FLOAT red;
	FLOAT green;
	FLOAT blue;
	FLOAT alpha;
}

template ColorRGB {
	<d3e16e81-7835-11cf-8f52-0040333594a3>
	FLOAT red;
	FLOAT green;
	FLOAT blue;
}

template IndexedColor {
	<1630b820-7842-11cf-8f52-0040333594a3>
	DWORD index;
	ColorRGBA indexColor;
}

template Boolean {
	<4885ae61-78e8-11cf-8f52-0040333594a3>
	WORD truefalse;
}

template Boolean2d {
	<4885ae63-78e8-11cf-8f52-0040333594a3>
	Boolean u;
	Boolean v;
}

template MaterialWrap {
	<4885ae60-78e8-11cf-8f52-0040333594a3>
	Boolean u;
	Boolean v;
}

template TextureFilename {
	<a42790e1-7810-11cf-8f52-0040333594a3>
	STRING filename;
}

template Material {
	<3d82ab4d-62da-11cf-ab39-0020af71e433>
	ColorRGBA faceColor;
	FLOAT power;
	ColorRGB specularColor;
	ColorRGB emissiveColor;
	[...]
}

template MeshFace {
	<3d82ab5f-62da-11cf-ab39-0020af71e433>
	DWORD nFaceVertexIndices;
	array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
	<4885ae62-78e8-11cf-8f52-0040333594a3>
	DWORD nFaceWrapValues;
	Boolean2d faceWrapValues;
}

template MeshTextureCoords {
	<f6f23f40-7686-11cf-8f52-0040333594a3>
	DWORD nTextureCoords;
	array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
	<f6f23f42-7686-11cf-8f52-0040333594a3>
	DWORD nMaterials;
	DWORD nFaceIndexes;
	array DWORD faceIndexes[nFaceIndexes];
	[Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template MeshNormals {
	<f6f23f43-7686-11cf-8f52-0040333594a3>
	DWORD nNormals;
	array Vector normals[nNormals];
	DWORD nFaceNormals;
	array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
	<1630b821-7842-11cf-8f52-0040333594a3>
	DWORD nVertexColors;
	array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
	<3d82ab44-62da-11cf-ab39-0020af71e433>
	DWORD nVertices;
	array Vector vertices[nVertices];
	DWORD nFaces;
	array MeshFace faces[nFaces];
	[...]
}

template FrameTransformMatrix {
	<f6f23f41-7686-11cf-8f52-0040333594a3>
	Matrix4x4 frameMatrix;
}

template Frame {
	<3d82ab46-62da-11cf-ab39-0020af71e433>
	[...]
}

template FloatKeys {
	<10dd46a9-775b-11cf-8f52-0040333594a3>
	DWORD nValues;
	array FLOAT values[nValues];
}

template TimedFloatKeys {
	<f406b180-7b3b-11cf-8f52-0040333594a3>
	DWORD time;
	FloatKeys tfkeys;
}

template AnimationKey {
	<10dd46a8-775b-11cf-8f52-0040333594a3>
	DWORD keyType;
	DWORD nKeys;
	array TimedFloatKeys keys[nKeys];
}

template AnimationOptions {
	<e2bf56c0-840f-11cf-8f52-0040333594a3>
	DWORD openclosed;
	DWORD positionquality;
}

template Animation {
	<3d82ab4f-62da-11cf-ab39-0020af71e433>
	[...]
}

template AnimationSet {
	<3d82ab50-62da-11cf-ab39-0020af71e433>
	[Animation <3d82ab4f-62da-11cf-ab39-0020af71e433>]
}

template Light8 {
	<8840e4ac-5c01-4e6a-9df9-880a45fb9e56>
	DWORD Type;
	ColorRGBA Diffuse;
	ColorRGBA Specular;
	ColorRGBA Ambient;
	Vector Position;
	Vector Direction;
	FLOAT Range;
	FLOAT Falloff;
	FLOAT Attenuation0;
	FLOAT Attenuation1;
	FLOAT Attenuation2;
	FLOAT Theta;
	FLOAT Phi;
	[Frame <3d82ab46-62da-11cf-ab39-0020af71e433>]
}

template Scene {
	<5b47a445-ce94-4ba6-8fe4-5ca6c46bb28c>
	[...]
}

template Matrix3x3 {
	<d70232c1-ef74-42f8-b768-7083a2a939eb>
	array FLOAT matrix[9];
}

template Solid {
	<a9c20358-edeb-4c0b-97ab-37d330b6bfa4>
	FLOAT mass;
	Matrix3x3 inertia;
	Vector velocity;
	Vector angularVelocity;
	Vector center;
	[Frame <3d82ab46-62da-11cf-ab39-0020af71e433>]
}

template SolidContainer {
	<582e937c-3fed-4175-a0bf-295c6faea0cc>
	[Solid <a9c20358-edeb-4c0b-97ab-37d330b6bfa4>]
}

template Penalty {
	<01d5ff3c-6d41-4ea5-8e89-45f5c14eda5f>
	[Solid <a9c20358-edeb-4c0b-97ab-37d330b6bfa4>, Frame <3d82ab46-62da-11cf-ab39-0020af71e433>]
}

template PenaltyEngine {
	<de9f4f45-8631-4107-93ff-a2b8bea3121b>
	[Penalty <01d5ff3c-6d41-4ea5-8e89-45f5c14eda5f>]
}

template Include {
	<dd0c497d-53c3-4888-a529-3bbfab9d9216>
	STRING filename;
}

template GravityEngine {
 <fa58a120-587e-4e15-a023-61cf996858e6>
 Vector accel;
 [Solid]
}
Scene {
	SolidContainer{
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
	}
	GravityEngine{
		0.0; -9.8; 0.0;;
		{soBlock1}
		{soBlock2}
		{soBlock3}
	}
	PenaltyEngine{
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
			{frBlock1}
			{frBlock2}
			{soBlock1}
			{soBlock2}
		}
		Penalty{
			{frBlock2}
			{frBlock3}
			{soBlock2}
			{soBlock3}
		}
		Penalty{
			{frBlock1}
			{frBlock3}
			{soBlock1}
			{soBlock3}
		}
	}
}
Frame frTop{
  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   0, 0, 0, 1.000000;;
  }
	 Frame frBlock3 {
	  FrameTransformMatrix  {
	   1.000000,0.000000,0.000000,0.000000,
	   0.000000,1.000000,0.000000,0.000000,
	   0.000000,0.000000,1.000000,0.000000,
	   0.000000,0.600000,0.000000,1.000000;;
	  }
	 Mesh {
	  8;
	  -1.00000;0.50000;1.00000;,
	  1.00000;0.50000;1.00000;,
	  1.00000;0.50000;-1.00000;,
	  -1.00000;0.50000;-1.00000;,
	  1.00000;-0.50000;-1.00000;,
	  -1.00000;-0.50000;-1.00000;,
	  1.00000;-0.50000;1.00000;,
	  -1.00000;-0.50000;1.00000;;
	 
	  6;
	  4;0,1,2,3;,
	  4;3,2,4,5;,
	  4;5,4,6,7;,
	  4;7,6,1,0;,
	  4;1,6,4,2;,
	  4;7,0,3,5;;
	  
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
	   0.019608;1.000000;0.721569;1.000000;;
	   22.000000;
	   0.430000;0.430000;0.430000;;
	   0.004902;0.250000;0.180392;;
	  }
	  }
	 }
	 }

	 Frame frBlock2 {
	  FrameTransformMatrix  {
	   1.000000,0.000000,0.000000,0.000000,
	   0.000000,1.000000,0.000000,0.000000,
	   0.000000,0.000000,1.000000,0.000000,
	   0.000000,1.700000,0.000000,1.000000;;
	  }

	Mesh {
	 8;
	 -1.00000;0.50000;1.00000;,
	 1.00000;0.50000;1.00000;,
	 1.00000;0.50000;-1.00000;,
	 -1.00000;0.50000;-1.00000;,
	 1.00000;-0.50000;-1.00000;,
	 -1.00000;-0.50000;-1.00000;,
	 1.00000;-0.50000;1.00000;,
	 -1.00000;-0.50000;1.00000;;
	 
	 6;
	 4;0,1,2,3;,
	 4;3,2,4,5;,
	 4;5,4,6,7;,
	 4;7,6,1,0;,
	 4;1,6,4,2;,
	 4;7,0,3,5;;
	 
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
	   1.000000;0.125490;0.078431;1.000000;;
	   25.000000;
	   0.830000;0.830000;0.830000;;
	   0.280000;0.035137;0.021961;;
	  }
	 }
	}
	}

	Frame frBlock1 {
	  FrameTransformMatrix  {
	   1.000000,0.000000,0.000000,0.000000,
	   0.000000,1.000000,0.000000,0.000000,
	   0.000000,0.000000,1.000000,0.000000,
	   0.000000,2.800000,0.000000,1.000000;;
	  }

	Mesh {
	 8;
	 -1.00000;0.50000;1.00000;,
	 1.00000;0.50000;1.00000;,
	 1.00000;0.50000;-1.00000;,
	 -1.00000;0.50000;-1.00000;,
	 1.00000;-0.50000;-1.00000;,
	 -1.00000;-0.50000;-1.00000;,
	 1.00000;-0.50000;1.00000;,
	 -1.00000;-0.50000;1.00000;;
	 
	 6;
	 4;0,1,2,3;,
	 4;3,2,4,5;,
	 4;5,4,6,7;,
	 4;7,6,1,0;,
	 4;1,6,4,2;,
	 4;7,0,3,5;;
	 
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
	   1.000000;0.933333;0.003922;1.000000;;
	   25.000000;
	   0.670000;0.670000;0.670000;;
	   0.310000;0.289333;0.001216;;
	  }
	 }
	}
	}
	 Frame frFloor{
	  FrameTransformMatrix  {
	   1.000000,0.000000,0.000000,0.000000,
	   0.000000,1.000000,0.000000,0.000000,
	   0.000000,0.000000,1.000000,0.000000,
	   0.000000,0.000000,0.000000,1.000000;;
	  }
		Mesh {
		 8;
		 -5.00000;0.00000;5.00000;,
		 5.00000;0.00000;5.00000;,
		 5.00000;0.00000;-5.00000;,
		 -5.00000;0.00000;-5.00000;,
		 5.00000;-1.00000;-5.00000;,
		 -5.00000;-1.00000;-5.00000;,
		 5.00000;-1.00000;5.00000;,
		 -5.00000;-1.00000;5.00000;;
		 
		 6;
		 4;0,1,2,3;,
		 4;3,2,4,5;,
		 4;5,4,6,7;,
		 4;7,6,1,0;,
		 4;1,6,4,2;,
		 4;7,0,3,5;;
		 
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
		   1.000000;1.000000;1.000000;1.000000;;
		   0.000000;
		   0.000000;0.000000;0.000000;;
		   0.000000;0.000000;0.000000;;
		  }
		 }
		}
	 }
	}
