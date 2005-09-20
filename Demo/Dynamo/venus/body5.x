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
 31;
 3.21191;0.39621;-6.48632;,
 3.65481;0.17041;-5.83804;,
 3.46812;0.42198;-5.23141;,
 3.14559;0.29366;-5.55139;,
 4.09303;0.42044;-5.95354;,
 3.92300;0.45108;-5.33626;,
 3.72406;0.44808;-7.30243;,
 3.31221;0.55136;-7.25853;,
 3.74910;0.72951;-6.02299;,
 3.17706;0.48456;-5.91498;,
 3.04668;0.74871;-4.88937;,
 3.06857;0.94654;-5.28597;,
 3.28635;0.66050;-4.53185;,
 3.93099;0.93376;-4.61030;,
 3.94894;1.13073;-5.01006;,
 3.70044;1.21409;-5.37835;,
 3.15215;1.41205;-4.19658;,
 3.15586;1.58408;-4.57024;,
 3.40243;1.35815;-3.85326;,
 4.03893;1.81516;-4.27063;,
 3.77422;1.45751;-3.72552;,
 3.76656;1.86491;-4.63973;,
 3.12823;2.18059;-3.51820;,
 3.37768;2.30495;-3.94399;,
 3.92638;2.24298;-3.23814;,
 4.09888;2.51385;-3.68517;,
 3.84980;2.53981;-4.00445;,
 3.78142;2.80271;-3.27165;,
 3.25437;2.83494;-3.48386;,
 3.75354;2.96254;-3.66860;,
 3.91458;0.05827;-7.49670;;
 
 37;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;6,7,0,8;,
 4;9,3,10,11;,
 4;3,2,12,10;,
 4;2,5,13,12;,
 4;5,4,14,13;,
 4;4,8,15,14;,
 4;8,0,11,15;,
 4;11,10,16,17;,
 4;10,12,18,16;,
 4;13,14,19,20;,
 4;14,15,21,19;,
 4;15,11,17,21;,
 4;17,16,22,23;,
 4;18,20,24,22;,
 4;20,19,25,24;,
 4;19,21,26,25;,
 4;21,17,23,26;,
 4;22,24,27,28;,
 4;25,26,29,27;,
 3;27,29,28;,
 3;30,1,0;,
 3;30,4,1;,
 3;30,6,4;,
 3;6,30,7;,
 3;30,0,7;,
 3;0,3,9;,
 3;4,6,8;,
 3;0,9,11;,
 3;12,20,18;,
 3;12,13,20;,
 3;16,18,22;,
 3;23,22,28;,
 3;24,25,27;,
 3;26,23,29;,
 3;23,28,29;;
 
 MeshMaterialList {
  1;
  37;
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
