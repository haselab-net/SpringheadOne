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
 49;
 -8.88660;5.33050;0.05000;,
 -8.88660;4.59270;-1.87800;,
 -8.88660;2.36030;-1.87800;,
 -8.88660;2.36030;0.05000;,
 -7.87530;2.94910;-6.17230;,
 -7.87530;2.36030;-6.17230;,
 -8.88660;6.10630;2.07730;,
 -8.88660;2.36030;2.07730;,
 -7.87530;2.36030;6.37160;,
 -7.87530;7.74980;6.37160;,
 -7.06590;2.36030;7.71510;,
 -7.06840;8.22490;7.71110;,
 -4.40070;2.38370;6.42920;,
 8.82000;2.50000;0.05000;,
 -4.41590;7.26920;6.43220;,
 -5.07330;6.86870;5.31060;,
 -5.06040;2.38390;5.30590;,
 -5.07330;6.86870;5.31060;,
 -5.82260;5.48230;1.72650;,
 -5.80800;2.38460;1.72490;,
 -5.06040;2.38390;5.30590;,
 -4.96770;2.38390;5.46380;,
 -5.82260;5.48230;1.72650;,
 -5.79260;4.83590;0.05000;,
 -5.76920;4.22410;-1.53850;,
 -5.75550;2.38510;0.05000;,
 -5.80800;2.38460;1.72490;,
 -5.74140;2.38530;-1.53550;,
 -4.90530;2.38530;-5.06540;,
 -5.74140;2.38530;-1.53550;,
 -5.76920;4.22410;-1.53850;,
 -4.90530;2.86910;-5.06530;,
 -4.90530;2.86910;-5.06530;,
 -4.21620;2.38540;-6.15870;,
 -7.06590;2.36030;-7.51580;,
 -4.90530;2.38530;-5.06540;,
 -7.87530;2.94910;-6.17230;,
 -7.06590;2.36030;-7.51580;,
 -5.82260;5.48230;1.72650;,
 -5.07330;6.86870;5.31060;,
 -5.74140;2.38530;-1.53550;,
 8.82000;2.50000;0.05000;,
 8.82000;2.50000;0.05000;,
 -4.90530;2.86910;-5.06530;,
 -5.76920;4.22410;-1.53850;,
 -5.07330;6.86870;5.31060;,
 -4.41590;7.26920;6.43220;,
 -5.80800;2.38460;1.72490;,
 8.82000;2.50000;0.05000;;
 
 32;
 4;0,1,2,3;,
 4;4,5,2,1;,
 4;6,0,3,7;,
 4;6,7,8,9;,
 4;9,8,10,11;,
 3;12,13,14;,
 4;12,14,11,10;,
 4;14,15,9,11;,
 4;16,12,10,8;,
 4;17,18,6,9;,
 4;19,20,8,7;,
 4;21,20,13,12;,
 3;22,13,23;,
 4;22,23,0,6;,
 3;23,13,24;,
 4;23,24,1,0;,
 3;25,13,26;,
 4;25,26,7,3;,
 4;27,25,3,2;,
 4;28,29,2,5;,
 4;30,31,4,1;,
 3;32,13,33;,
 4;32,33,34,4;,
 3;28,33,13;,
 4;35,5,34,33;,
 3;36,37,5;,
 3;38,39,13;,
 3;40,41,25;,
 3;28,42,29;,
 3;43,44,13;,
 3;45,46,13;,
 3;47,48,20;;
 
 MeshMaterialList {
  3;
  32;
  1,
  1,
  1,
  1,
  1,
  0,
  2,
  2,
  2,
  2,
  2,
  0,
  0,
  2,
  0,
  2,
  0,
  2,
  2,
  2,
  2,
  0,
  2,
  0,
  2,
  1,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250000;0.250000;0.250000;;
   TextureFilename {
    "suika_pulp.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250000;0.250000;0.250000;;
   TextureFilename {
    "suika_surface.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250000;0.250000;0.250000;;
   TextureFilename {
    "suika_side.jpg";
   }
  }
 }
 MeshNormals {
  41;
  0.434566;0.000799;0.900640;,
  -1.000000;0.000000;0.000000;,
  0.145302;0.935411;-0.322324;,
  -0.926138;0.000000;-0.377184;,
  -0.993321;0.000000;-0.115384;,
  -0.856576;-0.000007;0.516021;,
  -0.926143;-0.000003;0.377173;,
  -0.993321;0.000000;0.115384;,
  0.434552;0.000799;0.900647;,
  0.137513;0.909557;-0.392169;,
  0.007247;-0.999969;-0.002961;,
  0.006810;-0.999968;-0.004102;,
  0.144890;0.918387;-0.368202;,
  0.007791;-0.999969;-0.000921;,
  0.147665;0.923696;-0.353525;,
  0.146726;0.929430;-0.338572;,
  0.007902;-0.999969;-0.000040;,
  0.007810;-0.999969;0.000891;,
  0.007265;-0.999969;0.003003;,
  0.145511;0.935180;-0.322902;,
  0.145282;0.935424;-0.322297;,
  0.434538;0.000799;0.900653;,
  0.132923;0.906170;-0.401482;,
  0.137511;0.909557;-0.392170;,
  0.132923;0.906170;-0.401480;,
  0.006806;-0.999968;-0.004119;,
  0.006803;-0.999968;-0.004136;,
  0.007248;-0.999969;-0.002971;,
  0.144887;0.918388;-0.368200;,
  0.007799;-0.999969;-0.000916;,
  0.007455;-0.999972;0.000000;,
  0.143620;0.923243;-0.356365;,
  0.147673;0.923701;-0.353511;,
  0.146749;0.929432;-0.338558;,
  0.007920;-0.999969;-0.000026;,
  0.007839;-0.999969;0.000900;,
  0.007294;-0.999969;0.003001;,
  0.145535;0.935172;-0.322913;,
  0.006819;-0.999968;0.004182;,
  0.006844;-0.999968;0.004173;,
  -0.856563;0.000000;-0.516042;;
  32;
  4;1,4,4,1;,
  4;3,3,4,4;,
  4;7,1,1,7;,
  4;7,7,6,6;,
  4;6,6,5,5;,
  3;8,0,8;,
  4;8,8,21,21;,
  4;22,9,23,24;,
  4;10,25,26,27;,
  4;9,12,28,23;,
  4;13,10,27,29;,
  4;11,10,30,25;,
  3;12,31,14;,
  4;12,14,32,28;,
  3;14,31,15;,
  4;14,15,33,32;,
  3;16,30,13;,
  4;16,13,29,34;,
  4;17,16,34,35;,
  4;18,17,35,36;,
  4;15,19,37,33;,
  3;19,31,20;,
  4;19,20,2,37;,
  3;18,38,30;,
  4;18,36,39,38;,
  3;3,40,3;,
  3;12,9,31;,
  3;17,30,16;,
  3;18,30,17;,
  3;19,15,31;,
  3;9,22,31;,
  3;13,30,10;;
 }
 MeshTextureCoords {
  49;
  0.755880;0.736250;
  0.858480;0.758420;
  1.000000;0.993270;
  1.000000;0.993910;
  0.938720;0.881100;
  0.942890;0.964490;
  0.762500;0.599160;
  1.000000;0.993270;
  0.942890;0.964490;
  0.904700;0.201030;
  0.897180;0.942150;
  1.000000;0.058080;
  0.750650;0.870360;
  0.023810;0.514270;
  0.837030;0.134240;
  0.756850;0.253610;
  0.787930;0.888580;
  0.754450;0.254460;
  0.634680;0.584470;
  0.830270;0.909990;
  0.785030;0.887160;
  0.782690;0.886020;
  0.634460;0.584440;
  0.627960;0.697460;
  0.711520;0.715430;
  0.827380;0.909100;
  0.828210;0.908970;
  0.826600;0.908190;
  0.779390;0.884400;
  0.828510;0.909120;
  0.711830;0.715520;
  0.775960;0.815840;
  0.775890;0.815810;
  0.740510;0.865390;
  0.897180;0.942150;
  0.779370;0.884390;
  0.918600;0.906010;
  0.897830;0.936530;
  0.634940;0.583440;
  0.694930;0.418450;
  0.826320;0.908060;
  0.424910;0.711080;
  0.401590;0.699330;
  0.775890;0.815770;
  0.743860;0.765630;
  0.760520;0.246790;
  0.801810;0.186680;
  0.828220;0.908970;
  0.426010;0.711620;;
 }
}
