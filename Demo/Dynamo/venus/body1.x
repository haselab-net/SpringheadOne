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
 58;
 -1.80521;0.37057;-2.59809;,
 -1.80986;0.86739;-2.83482;,
 -1.65985;0.63893;-3.16960;,
 -1.80986;0.86739;-2.36136;,
 -1.65985;0.63893;-2.02658;,
 -1.44770;0.31584;-2.02658;,
 -1.29769;0.08738;-2.36136;,
 -1.29769;0.08738;-2.83481;,
 -1.44770;0.31584;-3.16960;,
 -1.31093;1.50221;-3.03550;,
 -1.03375;1.08007;-3.65409;,
 -1.31093;1.50221;-2.16068;,
 -1.03375;1.08007;-1.54208;,
 -0.64175;0.48308;-1.54208;,
 -0.36456;0.06094;-2.16068;,
 -0.36456;0.06094;-3.03550;,
 -0.64175;0.48308;-3.65410;,
 0.04798;2.18201;-3.17229;,
 -0.02222;1.62683;-3.97782;,
 -0.01760;2.31203;-2.04947;,
 -0.02222;1.62683;-1.21835;,
 0.48995;0.84682;-1.21835;,
 0.79663;0.59748;-2.02352;,
 0.76906;0.59847;-3.16662;,
 0.48995;0.84682;-3.97783;,
 1.08419;3.64331;-3.57291;,
 1.30841;2.70214;-4.31349;,
 1.03223;3.62425;-1.47548;,
 1.28078;2.63019;-0.74653;,
 1.77511;1.35169;-1.10467;,
 1.99696;1.21704;-2.00425;,
 1.96019;1.17824;-3.21286;,
 1.77511;1.35169;-4.09151;,
 2.12792;3.39797;-3.21050;,
 2.50590;2.70083;-3.97782;,
 2.12364;3.35403;-1.97808;,
 2.50589;2.70083;-1.21835;,
 3.01807;1.92082;-1.21835;,
 3.23469;1.70978;-2.02360;,
 3.32511;1.71911;-3.16608;,
 3.01807;1.92082;-3.97783;,
 3.36041;3.48671;-3.03550;,
 3.77643;3.17936;-4.20096;,
 3.36041;3.48671;-2.16068;,
 3.71679;3.06458;-0.97274;,
 3.97691;2.59919;-0.97497;,
 4.13560;2.32917;-2.15833;,
 4.25166;2.39528;-3.03198;,
 3.94690;2.57769;-4.21714;,
 4.30201;3.46029;-2.74935;,
 4.29593;3.23183;-3.03240;,
 4.29354;3.46027;-2.36136;,
 4.25662;3.23183;-2.08087;,
 4.65570;2.90872;-2.02658;,
 4.80571;2.68026;-2.36136;,
 4.80571;2.68026;-2.83482;,
 4.65570;2.90872;-3.16960;,
 4.80106;3.17708;-2.59809;;
 
 64;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,7,6;,
 3;0,8,7;,
 3;0,2,8;,
 4;2,1,9,10;,
 4;1,3,11,9;,
 4;3,4,12,11;,
 4;4,5,13,12;,
 4;5,6,14,13;,
 4;6,7,15,14;,
 4;7,8,16,15;,
 4;8,2,10,16;,
 4;10,9,17,18;,
 4;9,11,19,17;,
 4;11,12,20,19;,
 4;12,13,21,20;,
 4;13,14,22,21;,
 4;14,15,23,22;,
 4;15,16,24,23;,
 4;16,10,18,24;,
 4;18,17,25,26;,
 4;17,19,27,25;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;22,23,31,30;,
 4;23,24,32,31;,
 4;24,18,26,32;,
 4;26,25,33,34;,
 4;25,27,35,33;,
 4;27,28,36,35;,
 4;28,29,37,36;,
 4;29,30,38,37;,
 4;30,31,39,38;,
 4;31,32,40,39;,
 4;32,26,34,40;,
 4;34,33,41,42;,
 4;33,35,43,41;,
 4;35,36,44,43;,
 4;36,37,45,44;,
 4;37,38,46,45;,
 4;38,39,47,46;,
 4;39,40,48,47;,
 4;40,34,42,48;,
 4;42,41,49,50;,
 4;41,43,51,49;,
 4;43,44,52,51;,
 4;44,45,53,52;,
 4;45,46,54,53;,
 4;46,47,55,54;,
 4;47,48,56,55;,
 4;48,42,50,56;,
 3;50,49,57;,
 3;49,51,57;,
 3;51,52,57;,
 3;52,53,57;,
 3;53,54,57;,
 3;54,55,57;,
 3;55,56,57;,
 3;56,50,57;;
 
 MeshMaterialList {
  1;
  64;
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
   1.000000;0.529412;0.219608;0.500000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250000;0.132353;0.054902;;
  }
 }
}
