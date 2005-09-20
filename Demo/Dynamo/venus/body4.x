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

Header{
1;
0;
1;
}

Mesh {
 36;
 3.23388;2.68331;-1.13046;,
 3.93556;2.65296;-0.56904;,
 4.07338;2.60602;-0.99567;,
 3.12774;2.69430;-0.75960;,
 4.64040;0.86692;2.25427;,
 4.74792;0.70003;3.12679;,
 4.79386;0.35686;3.48878;,
 4.10576;0.67481;3.58726;,
 4.27290;0.88495;2.43445;,
 4.36112;2.23350;0.30706;,
 4.41474;2.20498;-0.36948;,
 3.98371;2.31321;0.45200;,
 3.17510;2.35477;-0.00190;,
 3.15358;2.30369;-0.48783;,
 4.03372;2.23471;-0.73874;,
 4.57662;1.70696;1.40883;,
 4.64348;1.68954;0.60498;,
 4.15816;1.78882;1.59887;,
 3.75421;1.84182;1.10306;,
 3.72529;1.79332;0.58005;,
 4.23258;1.72541;0.20923;,
 4.71411;1.20569;2.29016;,
 4.75690;1.20203;1.58782;,
 4.32556;1.25341;2.55290;,
 3.96838;1.29600;2.17924;,
 3.96448;1.27076;1.67268;,
 4.38856;1.22639;1.30049;,
 4.24185;1.16994;2.15085;,
 4.90029;0.34756;3.83707;,
 4.38547;0.22920;3.90110;,
 4.77751;0.51552;4.34748;,
 5.48762;0.24874;4.59254;,
 4.33205;0.41286;4.40346;,
 4.52360;0.14586;4.49199;,
 4.90330;0.29668;5.21244;,
 5.64009;0.20069;5.15993;;
 
 68;
 3;0,1,2;,
 3;0,3,1;,
 3;4,5,6;,
 3;5,7,6;,
 3;7,8,6;,
 3;8,4,6;,
 3;1,9,10;,
 3;1,10,2;,
 3;1,11,9;,
 3;1,3,12;,
 3;1,12,11;,
 3;3,0,13;,
 3;3,13,12;,
 3;0,14,13;,
 3;2,10,14;,
 3;2,14,0;,
 3;9,15,16;,
 3;9,16,10;,
 3;11,17,15;,
 3;11,15,9;,
 3;11,12,18;,
 3;11,18,17;,
 3;12,13,19;,
 3;12,19,18;,
 3;13,14,20;,
 3;13,20,19;,
 3;10,16,20;,
 3;10,20,14;,
 3;15,21,22;,
 3;15,22,16;,
 3;17,23,21;,
 3;17,21,15;,
 3;17,18,24;,
 3;17,24,23;,
 3;18,19,25;,
 3;18,25,24;,
 3;19,20,26;,
 3;19,26,25;,
 3;16,22,26;,
 3;16,26,20;,
 3;21,5,4;,
 3;21,4,22;,
 3;23,7,5;,
 3;23,5,21;,
 3;23,24,7;,
 3;24,25,8;,
 3;24,8,7;,
 3;25,26,4;,
 3;25,4,8;,
 3;22,4,26;,
 3;27,7,6;,
 3;27,6,7;,
 3;6,28,29;,
 3;6,29,7;,
 3;28,6,30;,
 3;28,30,31;,
 3;6,7,32;,
 3;6,32,30;,
 3;29,33,32;,
 3;29,32,7;,
 3;28,31,33;,
 3;28,33,29;,
 3;31,30,34;,
 3;31,34,35;,
 3;30,32,34;,
 3;33,34,32;,
 3;31,35,34;,
 3;31,34,33;;
 
 MeshMaterialList {
  1;
  68;
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
   1.000000;0.529000;0.220000;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250000;0.132250;0.055000;;
  }
 }
}
