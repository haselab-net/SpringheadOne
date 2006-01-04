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

Header {
 1;
 0;
 1;
}

Material MatWhite {
 1.000000;1.000000;1.000000;1.000000;;
 21.333333;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;
}
Mesh {
 16;
 -1.000000;0.000000;0.000000;,
 -0.923880;0.382683;0.000000;,
 -0.707107;0.707107;0.000000;,
 -0.382683;0.923880;0.000000;,
 -0.000000;1.000000;0.000000;,
 0.382683;0.923880;-0.000000;,
 0.707107;0.707107;-0.000000;,
 0.923880;0.382683;-0.000000;,
 1.000000;0.000000;-0.000000;,
 0.923880;-0.382683;-0.000000;,
 0.707107;-0.707107;-0.000000;,
 0.382683;-0.923880;-0.000000;,
 0.000000;-1.000000;-0.000000;,
 -0.382683;-0.923880;0.000000;,
 -0.707107;-0.707107;0.000000;,
 -0.923880;-0.382683;0.000000;;

 28;
 3;1,15,0;,
 3;1,14,15;,
 3;1,13,14;,
 3;1,12,13;,
 3;1,11,12;,
 3;1,10,11;,
 3;1,9,10;,
 3;1,8,9;,
 3;1,7,8;,
 3;1,6,7;,
 3;2,6,1;,
 3;2,5,6;,
 3;5,3,4;,
 3;5,2,3;,
 3;0,15,1;,
 3;15,14,1;,
 3;14,13,1;,
 3;13,12,1;,
 3;12,11,1;,
 3;11,10,1;,
 3;10,9,1;,
 3;9,8,1;,
 3;8,7,1;,
 3;7,6,1;,
 3;1,6,2;,
 3;6,5,2;,
 3;4,3,5;,
 3;3,2,5;;

 MeshMaterialList {
  1;
  1;
  0;;
  {MatWhite}
 }
 MeshNormals {
  28;
  -0.000000;0.000000;-1.000000;,
  -0.000000;0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;0.000000;-1.000000;,
  -0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.000000;1.000000;;

  28;
  3;0,0,0;,
  3;1,1,1;,
  3;2,2,2;,
  3;3,3,3;,
  3;4,4,4;,
  3;5,5,5;,
  3;6,6,6;,
  3;7,7,7;,
  3;8,8,8;,
  3;9,9,9;,
  3;10,10,10;,
  3;11,11,11;,
  3;12,12,12;,
  3;13,13,13;,
  3;14,14,14;,
  3;15,15,15;,
  3;16,16,16;,
  3;17,17,17;,
  3;18,18,18;,
  3;19,19,19;,
  3;20,20,20;,
  3;21,21,21;,
  3;22,22,22;,
  3;23,23,23;,
  3;24,24,24;,
  3;25,25,25;,
  3;26,26,26;,
  3;27,27,27;;
 }
}
