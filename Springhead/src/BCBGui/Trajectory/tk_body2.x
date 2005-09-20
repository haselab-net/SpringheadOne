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
 678;
 -1.61010;2.94490;2.78890;,
 -2.78890;2.94490;1.61020;,
 -3.67190;2.21020;2.12000;,
 -2.11990;2.21020;3.67190;,
 -4.25790;0.99240;2.45830;,
 -2.45830;0.99240;4.25790;,
 -4.33010;0.00000;2.50000;,
 -2.50000;0.00000;4.33010;,
 -3.67710;-0.97790;2.12290;,
 -2.12300;-0.97790;3.67700;,
 -2.82310;-1.67230;1.62990;,
 -1.62990;-1.67230;2.82310;,
 -1.73210;-2.10160;1.00000;,
 -1.00000;-2.10160;1.73210;,
 -3.22030;2.94490;0.00000;,
 -4.24000;2.21020;0.00000;,
 -4.91660;0.99240;0.00000;,
 -5.00000;0.00000;0.00000;,
 -4.24590;-0.97790;0.00000;,
 -3.25980;-1.67230;0.00000;,
 -2.00000;-2.10160;0.00000;,
 -2.78890;2.94490;-1.61010;,
 -3.67190;2.21020;-2.11990;,
 -4.25790;0.99240;-2.45830;,
 -4.33010;0.00000;-2.50000;,
 -3.67700;-0.97790;-2.12300;,
 -2.82310;-1.67230;-1.62990;,
 -1.73210;-2.10160;-1.00000;,
 -1.61020;2.94490;-2.78890;,
 -2.12000;2.21020;-3.67190;,
 -2.45830;0.99240;-4.25790;,
 -2.50000;0.00000;-4.33010;,
 -2.12290;-0.97790;-3.67710;,
 -1.62990;-1.67230;-2.82310;,
 -1.00000;-2.10160;-1.73210;,
 0.00000;2.94490;-3.22030;,
 0.00000;2.21020;-4.24000;,
 0.00000;0.99240;-4.91660;,
 0.00000;0.00000;-5.00000;,
 0.00000;-0.97790;-4.24590;,
 -0.00000;-1.67230;-3.25980;,
 -0.00000;-2.10160;-2.00000;,
 1.61010;2.94490;-2.78890;,
 2.11990;2.21020;-3.67190;,
 2.45830;0.99240;-4.25790;,
 2.50000;0.00000;-4.33010;,
 2.12300;-0.97790;-3.67700;,
 1.62990;-1.67230;-2.82310;,
 1.00000;-2.10160;-1.73210;,
 2.78890;2.94490;-1.61020;,
 3.67190;2.21020;-2.12000;,
 4.25790;0.99240;-2.45830;,
 4.33010;0.00000;-2.50000;,
 3.67710;-0.97790;-2.12290;,
 2.82310;-1.67230;-1.62990;,
 1.73210;-2.10160;-1.00000;,
 3.22030;2.94490;0.00000;,
 4.24000;2.21020;0.00000;,
 4.91660;0.99240;0.00000;,
 5.00000;0.00000;0.00000;,
 4.24590;-0.97790;0.00000;,
 3.25980;-1.67230;-0.00000;,
 2.00000;-2.10160;-0.00000;,
 2.78890;2.94490;1.61010;,
 3.67190;2.21020;2.11990;,
 4.25790;0.99240;2.45830;,
 4.33010;0.00000;2.50000;,
 3.67700;-0.97790;2.12300;,
 2.82310;-1.67230;1.62990;,
 1.73210;-2.10160;1.00000;,
 1.61020;2.94490;2.78890;,
 2.12000;2.21020;3.67190;,
 2.45830;0.99240;4.25790;,
 2.50000;0.00000;4.33010;,
 2.12290;-0.97790;3.67710;,
 1.62990;-1.67230;2.82310;,
 1.00000;-2.10160;1.73210;,
 0.00000;2.94490;3.22030;,
 0.00000;2.21020;4.24000;,
 0.00000;0.99240;4.91660;,
 0.00000;0.00000;5.00000;,
 0.00000;-0.97790;4.24590;,
 0.00000;-1.67230;3.25980;,
 0.00000;-2.10160;2.00000;,
 -1.60490;-2.88610;-0.92660;,
 -1.85320;-2.88610;0.00000;,
 -1.27310;-3.60330;-0.73500;,
 -1.47000;-3.60330;0.00000;,
 0.00000;-3.60330;0.00000;,
 0.00000;-3.60330;0.00000;,
 -0.92660;-2.88610;-1.60490;,
 -0.73500;-3.60330;-1.27310;,
 0.00000;-3.60330;0.00000;,
 0.00000;-2.88610;-1.85320;,
 0.00000;-3.60330;-1.47000;,
 0.00000;-3.60330;0.00000;,
 0.92660;-2.88610;-1.60490;,
 0.73500;-3.60330;-1.27310;,
 0.00000;-3.60330;0.00000;,
 1.60490;-2.88610;-0.92660;,
 1.27310;-3.60330;-0.73500;,
 0.00000;-3.60330;0.00000;,
 1.85320;-2.88610;0.00000;,
 1.47000;-3.60330;0.00000;,
 0.00000;-3.60330;0.00000;,
 1.60490;-2.88610;0.92660;,
 1.27310;-3.60330;0.73500;,
 0.00000;-3.60330;0.00000;,
 0.92660;-2.88610;1.60490;,
 0.73500;-3.60330;1.27310;,
 0.00000;-3.60330;0.00000;,
 0.00000;-2.88610;1.85320;,
 0.00000;-3.60330;1.47000;,
 0.00000;-3.60330;0.00000;,
 -0.92660;-2.88610;1.60490;,
 -0.73500;-3.60330;1.27310;,
 0.00000;-3.60330;0.00000;,
 -1.60490;-2.88610;0.92660;,
 -1.27310;-3.60330;0.73500;,
 0.00000;-3.60330;0.00000;,
 -1.36720;3.43090;0.78930;,
 0.00000;3.43090;1.57870;,
 -1.36710;3.43090;-0.78940;,
 0.00000;3.43090;-1.57870;,
 1.36720;3.43090;-0.78930;,
 1.36710;3.43090;0.78940;,
 -0.00000;-0.20400;-5.12610;,
 -0.00000;-0.60550;-3.88500;,
 0.72660;-0.90650;-3.88500;,
 1.01050;-0.62260;-5.12610;,
 1.02750;-1.63300;-3.88500;,
 1.42900;-1.63300;-5.12610;,
 0.72660;-2.35960;-3.88500;,
 1.01050;-2.64350;-5.12610;,
 -0.00000;-2.66050;-3.88500;,
 -0.00000;-3.06210;-5.12610;,
 -0.72660;-2.35960;-3.88500;,
 -1.01050;-2.64350;-5.12610;,
 -1.02750;-1.63300;-3.88500;,
 -1.42900;-1.63300;-5.12610;,
 -0.72660;-0.90650;-3.88500;,
 -1.01050;-0.62260;-5.12610;,
 -0.00000;-1.63300;-2.35190;,
 0.00000;6.93390;0.00000;,
 0.00000;6.93390;0.00000;,
 0.22740;6.93390;0.13130;,
 0.22740;6.93390;-0.13130;,
 0.31180;6.86190;0.18000;,
 0.31180;6.86190;-0.18000;,
 0.31180;5.13390;0.18000;,
 0.31180;5.13390;-0.18000;,
 0.38970;5.05950;0.22500;,
 0.38970;5.05950;-0.22500;,
 0.38970;3.61950;0.22500;,
 0.38970;3.61950;-0.22500;,
 0.00000;6.93390;0.00000;,
 0.00000;6.93390;0.26260;,
 0.00000;6.86190;0.36000;,
 0.00000;5.13390;0.36000;,
 0.00000;5.05950;0.45000;,
 0.00000;3.61950;0.45000;,
 0.00000;6.93390;0.00000;,
 -0.22740;6.93390;0.13130;,
 -0.31180;6.86190;0.18000;,
 -0.31180;5.13390;0.18000;,
 -0.38970;5.05950;0.22500;,
 -0.38970;3.61950;0.22500;,
 0.00000;6.93390;0.00000;,
 -0.22740;6.93390;-0.13130;,
 -0.31180;6.86190;-0.18000;,
 -0.31180;5.13390;-0.18000;,
 -0.38970;5.05950;-0.22500;,
 -0.38970;3.61950;-0.22500;,
 0.00000;6.93390;0.00000;,
 0.00000;6.93390;-0.26260;,
 0.00000;6.86190;-0.36000;,
 0.00000;5.13390;-0.36000;,
 0.00000;5.05950;-0.45000;,
 0.00000;3.61950;-0.45000;,
 1.36710;3.43090;0.78940;,
 1.36720;3.43090;-0.78930;,
 0.00000;3.43090;1.57870;,
 1.36710;3.43090;0.78940;,
 -1.36720;3.43090;0.78930;,
 0.00000;3.43090;1.57870;,
 -1.36710;3.43090;-0.78940;,
 -1.36720;3.43090;0.78930;,
 0.00000;3.43090;-1.57870;,
 -1.36710;3.43090;-0.78940;,
 1.36720;3.43090;-0.78930;,
 0.00000;3.43090;-1.57870;,
 -4.18890;-2.30000;2.15810;,
 -0.98520;-2.30000;0.17160;,
 -4.26010;-1.31010;1.75430;,
 -4.43200;-0.90000;0.77930;,
 -4.60390;-1.31010;-0.19550;,
 -4.67510;-2.30000;-0.59940;,
 -4.60390;-3.28990;-0.19550;,
 -4.43200;-3.70000;0.77930;,
 -4.26010;-3.28990;1.75430;,
 4.18890;-2.30000;2.15810;,
 4.26010;-1.31010;1.75430;,
 0.98520;-2.30000;0.17160;,
 4.43200;-0.90000;0.77930;,
 4.60390;-1.31010;-0.19550;,
 4.67510;-2.30000;-0.59940;,
 4.60390;-3.28990;-0.19550;,
 4.43200;-3.70000;0.77930;,
 4.26010;-3.28990;1.75430;,
 -4.34580;-2.30000;-1.82170;,
 -0.76470;-2.30000;-0.64440;,
 -4.08220;-1.31010;-2.13580;,
 -3.44590;-0.90000;-2.89420;,
 -2.80960;-1.31010;-3.65250;,
 -2.54590;-2.30000;-3.96660;,
 -2.80960;-3.28990;-3.65250;,
 -3.44590;-3.70000;-2.89420;,
 -4.08220;-3.28990;-2.13580;,
 4.34580;-2.30000;-1.82170;,
 4.08220;-1.31010;-2.13580;,
 0.76470;-2.30000;-0.64440;,
 3.44590;-0.90000;-2.89420;,
 2.80960;-1.31010;-3.65250;,
 2.54590;-2.30000;-3.96660;,
 2.80960;-3.28990;-3.65250;,
 3.44590;-3.70000;-2.89420;,
 4.08220;-3.28990;-2.13580;,
 -2.91830;7.31720;-6.43010;,
 -4.37340;7.31720;-8.01990;,
 -4.74190;4.01140;-8.01990;,
 -3.18440;4.01140;-6.20490;,
 -4.94660;1.00900;-8.01990;,
 -3.24590;0.14800;-6.20490;,
 -4.98750;-2.18060;-8.01990;,
 -3.24590;-2.18060;-6.52560;,
 -4.37340;7.31720;-13.79380;,
 -2.91830;7.31720;-15.33860;,
 -3.18440;4.01140;-16.71220;,
 -4.74190;4.01140;-14.89720;,
 -3.24590;1.00900;-17.09490;,
 -4.94660;1.00900;-15.27990;,
 -3.24590;-2.18060;-17.09490;,
 -4.98750;-2.18060;-15.27990;,
 -2.75450;8.85240;-8.01990;,
 -2.75450;8.85240;-12.84810;,
 -3.24590;-3.79760;-8.01990;,
 -3.24590;-3.79760;-15.29200;,
 0.00000;7.31230;-6.43090;,
 0.00000;4.00650;-6.20570;,
 0.00000;0.14310;-6.20570;,
 -0.00000;-2.18550;-6.52640;,
 -0.00000;-3.80250;-8.02070;,
 0.00000;8.84750;-8.02070;,
 0.00000;7.31230;-15.33940;,
 0.00000;8.84750;-12.84890;,
 0.00000;4.00650;-16.71300;,
 0.00000;1.00410;-17.09570;,
 -0.00000;-2.18550;-17.09570;,
 -0.00000;-3.80250;-15.29280;,
 -3.24590;-3.79760;-8.01990;,
 0.00000;8.84750;-8.02070;,
 0.00000;8.84750;-12.84890;,
 -0.00000;-3.78420;-10.14850;,
 -1.29900;-3.78420;-10.89850;,
 -1.12500;-4.53410;-10.99900;,
 -0.00000;-4.53410;-10.34950;,
 -1.29900;-3.78420;-12.39860;,
 -1.12500;-4.53410;-12.29810;,
 -0.00000;-3.78420;-13.14860;,
 -0.00000;-4.53410;-12.94760;,
 1.29900;-3.78420;-12.39850;,
 1.12500;-4.53410;-12.29810;,
 1.29900;-3.78420;-10.89850;,
 1.12500;-4.53410;-10.99900;,
 -0.64950;-5.08310;-11.27350;,
 -0.00000;-5.08310;-10.89850;,
 -0.64950;-5.08310;-12.02350;,
 -0.00000;-5.08310;-12.39860;,
 0.64950;-5.08310;-12.02350;,
 0.64950;-5.08310;-11.27350;,
 -0.00000;-5.28410;-11.64850;,
 -6.08270;4.90900;-11.50900;,
 -5.92210;4.51500;-10.85950;,
 -5.84370;5.26090;-10.85950;,
 -5.96130;4.14210;-11.50900;,
 -5.92210;4.51500;-12.15850;,
 -5.84370;5.26090;-12.15850;,
 -5.80450;5.63390;-11.50900;,
 -5.40470;4.18460;-10.38400;,
 -5.26900;5.47650;-10.38400;,
 -5.47260;3.53870;-11.50900;,
 -5.40470;4.18460;-12.63400;,
 -5.26900;5.47650;-12.63400;,
 -5.20110;6.12250;-11.50900;,
 -4.66930;4.00630;-10.21000;,
 -4.51260;5.49810;-10.21000;,
 -4.74770;3.26040;-11.50900;,
 -4.66930;4.00630;-12.80800;,
 -4.51260;5.49810;-12.80800;,
 -4.43420;6.24400;-11.50900;,
 2.91830;7.31720;-6.43010;,
 3.18440;4.01140;-6.20490;,
 4.74190;4.01140;-8.01990;,
 4.37340;7.31720;-8.01990;,
 3.24590;0.14800;-6.20490;,
 4.94660;1.00900;-8.01990;,
 3.24590;-2.18060;-6.52560;,
 4.98750;-2.18060;-8.01990;,
 4.37340;7.31720;-13.79380;,
 4.74190;4.01140;-14.89720;,
 3.18440;4.01140;-16.71220;,
 2.91830;7.31720;-15.33860;,
 4.94660;1.00900;-15.27990;,
 3.24590;1.00900;-17.09490;,
 4.98750;-2.18060;-15.27990;,
 3.24590;-2.18060;-17.09490;,
 2.75450;8.85240;-8.01990;,
 2.75450;8.85240;-12.84810;,
 3.24590;-3.79760;-8.01990;,
 3.24590;-3.79760;-15.29200;,
 0.00000;7.31230;-6.43090;,
 0.00000;4.00650;-6.20570;,
 0.00000;0.14310;-6.20570;,
 -0.00000;-2.18550;-6.52640;,
 -0.00000;-3.80250;-8.02070;,
 0.00000;8.84750;-8.02070;,
 0.00000;7.31230;-15.33940;,
 0.00000;8.84750;-12.84890;,
 0.00000;4.00650;-16.71300;,
 0.00000;1.00410;-17.09570;,
 -0.00000;-3.80250;-15.29280;,
 -0.00000;-2.18550;-17.09570;,
 3.24590;-3.79760;-8.01990;,
 0.00000;8.84750;-8.02070;,
 0.00000;8.84750;-12.84890;,
 6.08270;4.90900;-11.50900;,
 5.84370;5.26090;-10.85950;,
 5.92210;4.51500;-10.85950;,
 5.96130;4.14210;-11.50900;,
 5.92210;4.51500;-12.15850;,
 5.84370;5.26090;-12.15850;,
 5.80450;5.63390;-11.50900;,
 5.26900;5.47650;-10.38400;,
 5.40470;4.18460;-10.38400;,
 5.47260;3.53870;-11.50900;,
 5.40470;4.18460;-12.63400;,
 5.26900;5.47650;-12.63400;,
 5.20110;6.12250;-11.50900;,
 4.51260;5.49810;-10.21000;,
 4.66930;4.00630;-10.21000;,
 4.74770;3.26040;-11.50900;,
 4.66930;4.00630;-12.80800;,
 4.51260;5.49810;-12.80800;,
 4.43420;6.24400;-11.50900;,
 -0.00000;-0.52960;-5.88420;,
 -0.76550;-0.84660;-5.88420;,
 -0.88390;-0.72820;-5.25920;,
 -0.00000;-0.36210;-5.25920;,
 -1.08250;-1.61210;-5.88420;,
 -1.25000;-1.61210;-5.25920;,
 -0.76550;-2.37760;-5.88420;,
 -0.88390;-2.49600;-5.25920;,
 -0.00000;-2.69460;-5.88420;,
 -0.00000;-2.86210;-5.25920;,
 0.76550;-2.37760;-5.88420;,
 0.88390;-2.49600;-5.25920;,
 1.08250;-1.61210;-5.88420;,
 1.25000;-1.61210;-5.25920;,
 0.76550;-0.84660;-5.88420;,
 0.88390;-0.72820;-5.25920;,
 -0.76550;-0.84660;-4.63420;,
 -0.00000;-0.52960;-4.63420;,
 -1.08250;-1.61210;-4.63420;,
 -0.76550;-2.37760;-4.63420;,
 -0.00000;-2.69460;-4.63420;,
 0.76550;-2.37760;-4.63420;,
 1.08250;-1.61210;-4.63420;,
 0.76550;-0.84660;-4.63420;,
 1.18960;-1.60940;-5.87510;,
 0.84120;-2.45060;-5.87510;,
 0.69610;-2.30550;-6.98490;,
 0.98440;-1.60940;-6.98490;,
 -0.00000;-2.79910;-5.87510;,
 -0.00000;-2.59380;-6.98490;,
 -0.84120;-2.45060;-5.87510;,
 -0.69610;-2.30550;-6.98490;,
 -1.18960;-1.60940;-5.87510;,
 -0.98440;-1.60940;-6.98490;,
 -0.84120;-0.76820;-5.87510;,
 -0.69610;-0.91340;-6.98490;,
 -0.00000;-0.41980;-5.87510;,
 -0.00000;-0.62510;-6.98490;,
 0.00000;-0.79780;5.31100;,
 -0.63480;-1.00400;5.31100;,
 -0.63480;-1.00400;8.30690;,
 0.00000;-0.79780;8.30690;,
 -1.02710;-1.54400;5.31100;,
 -1.02710;-1.54400;8.30690;,
 -0.63480;-1.00400;8.30690;,
 -1.02710;-2.21150;5.31100;,
 -1.02710;-2.21150;8.30690;,
 -1.02710;-1.54400;8.30690;,
 -0.63480;-2.75150;5.31100;,
 -0.63480;-2.75150;8.30690;,
 -1.02710;-2.21150;8.30690;,
 0.00000;-2.95780;5.31100;,
 0.00000;-2.95780;8.30690;,
 -0.63480;-2.75150;8.30690;,
 0.63480;-2.75150;5.31100;,
 0.63480;-2.75150;8.30690;,
 0.00000;-2.95780;8.30690;,
 1.02710;-2.21150;5.31100;,
 1.02710;-2.21150;8.30690;,
 0.63480;-2.75150;8.30690;,
 1.02710;-1.54400;5.31100;,
 1.02710;-1.54400;8.30690;,
 1.02710;-2.21150;8.30690;,
 0.63480;-1.00400;5.31100;,
 0.63480;-1.00400;8.30690;,
 1.02710;-1.54400;8.30690;,
 0.00000;-0.79780;8.30690;,
 0.63480;-1.00400;8.30690;,
 0.00000;-1.87780;8.60690;,
 0.00000;-1.11320;8.60690;,
 -0.44940;-1.25920;8.60690;,
 -0.44940;-1.25920;8.60690;,
 -0.72710;-1.64150;8.60690;,
 -0.72710;-1.64150;8.60690;,
 -0.72710;-2.11400;8.60690;,
 -0.72710;-2.11400;8.60690;,
 -0.44940;-2.49630;8.60690;,
 -0.44940;-2.49630;8.60690;,
 0.00000;-2.64230;8.60690;,
 0.00000;-2.64230;8.60690;,
 0.44940;-2.49630;8.60690;,
 0.44940;-2.49630;8.60690;,
 0.72710;-2.11400;8.60690;,
 0.72710;-2.11400;8.60690;,
 0.72710;-1.64150;8.60690;,
 0.72710;-1.64150;8.60690;,
 0.44940;-1.25920;8.60690;,
 0.44940;-1.25920;8.60690;,
 0.00000;-1.11320;8.60690;,
 -0.57130;-1.09140;5.03980;,
 -0.57130;-1.09140;5.39410;,
 0.00000;-0.90580;5.39410;,
 0.00000;-0.90580;5.08350;,
 -0.92440;-1.57740;4.92530;,
 -0.92440;-1.57740;5.39410;,
 -0.92440;-2.17810;4.78380;,
 -0.92440;-2.17810;5.39410;,
 -0.57130;-2.66410;4.66940;,
 -0.57130;-2.66410;5.39410;,
 0.00000;-2.84980;4.62560;,
 0.00000;-2.84980;5.39410;,
 0.57130;-2.66410;4.66940;,
 0.57130;-2.66410;5.39410;,
 0.92440;-2.17810;4.78380;,
 0.92440;-2.17810;5.39410;,
 0.92440;-1.57740;4.92530;,
 0.92440;-1.57740;5.39410;,
 0.57130;-1.09140;5.03980;,
 0.57130;-1.09140;5.39410;,
 0.00000;-1.26270;2.78090;,
 -0.42430;-0.88990;3.03960;,
 0.00000;-0.71940;3.08210;,
 -0.60000;-1.30160;2.93690;,
 -0.42430;-1.71320;2.83430;,
 0.00000;-1.88380;2.79180;,
 0.42430;-1.71320;2.83430;,
 0.60000;-1.30160;2.93690;,
 0.42430;-0.88990;3.03960;,
 -0.73480;-0.69480;3.54090;,
 0.00000;-0.39940;3.61450;,
 -1.03920;-1.40780;3.36310;,
 -0.73480;-2.12080;3.18530;,
 0.00000;-2.41620;3.11170;,
 0.73480;-2.12080;3.18530;,
 1.03920;-1.40780;3.36310;,
 0.73480;-0.69480;3.54090;,
 -0.84850;-0.72960;4.15050;,
 0.00000;-0.38860;4.23560;,
 -1.20000;-1.55300;3.94530;,
 -0.84850;-2.37630;3.74000;,
 0.00000;-2.71730;3.65490;,
 0.84850;-2.37630;3.74000;,
 1.20000;-1.55300;3.94530;,
 0.84850;-0.72960;4.15050;,
 -0.73480;-0.98510;4.70520;,
 0.00000;-0.68970;4.77890;,
 -1.03920;-1.69810;4.52740;,
 -0.73480;-2.41110;4.34970;,
 0.00000;-2.70650;4.27600;,
 0.73480;-2.41110;4.34970;,
 1.03920;-1.69810;4.52740;,
 0.73480;-0.98510;4.70520;,
 -0.42430;-1.39270;5.05620;,
 0.00000;-1.22220;5.09870;,
 -0.60000;-1.80440;4.95360;,
 -0.42430;-2.21600;4.85100;,
 0.00000;-2.38660;4.80840;,
 0.42430;-2.21600;4.85100;,
 0.60000;-1.80440;4.95360;,
 0.42430;-1.39270;5.05620;,
 0.00000;3.54360;3.88040;,
 -0.40590;3.42940;4.28630;,
 0.00000;3.42940;4.45440;,
 -0.57400;3.42940;3.88040;,
 -0.40590;3.42940;3.47450;,
 0.00000;3.42940;3.30640;,
 0.40590;3.42940;3.47450;,
 0.57400;3.42940;3.88040;,
 0.40590;3.42940;4.28630;,
 -0.75000;3.10430;4.63040;,
 0.00000;3.10430;4.94110;,
 -1.06070;3.10430;3.88040;,
 -0.75000;3.10430;3.13040;,
 0.00000;3.10430;2.81970;,
 0.75000;3.10430;3.13040;,
 1.06070;3.10430;3.88040;,
 0.75000;3.10430;4.63040;,
 -0.97990;2.61760;4.86030;,
 0.00000;2.61760;5.26620;,
 -1.38580;2.61760;3.88040;,
 -0.97990;2.61760;2.90050;,
 0.00000;2.61760;2.49460;,
 0.97990;2.61760;2.90050;,
 1.38580;2.61760;3.88040;,
 0.97990;2.61760;4.86030;,
 -1.06070;2.04360;4.94110;,
 0.00000;2.04360;5.38040;,
 -1.50000;2.04360;3.88040;,
 -1.06070;2.04360;2.81970;,
 0.00000;2.04360;2.38040;,
 1.06070;2.04360;2.81970;,
 1.50000;2.04360;3.88040;,
 1.06070;2.04360;4.94110;,
 -0.97990;1.46960;4.86030;,
 0.00000;1.46960;5.26620;,
 -1.38580;1.46960;3.88040;,
 -0.97990;1.46960;2.90050;,
 0.00000;1.46960;2.49460;,
 0.97990;1.46960;2.90050;,
 1.38580;1.46960;3.88040;,
 0.97990;1.46960;4.86030;,
 -0.75000;0.98290;4.63040;,
 0.00000;0.98290;4.94110;,
 -1.06070;0.98290;3.88040;,
 -0.75000;0.98290;3.13040;,
 0.00000;0.98290;2.81970;,
 0.75000;0.98290;3.13040;,
 1.06070;0.98290;3.88040;,
 0.75000;0.98290;4.63040;,
 -0.40590;0.65780;4.28630;,
 0.00000;0.65780;4.45440;,
 -0.57400;0.65780;3.88040;,
 -0.40590;0.65780;3.47450;,
 0.00000;0.65780;3.30640;,
 0.40590;0.65780;3.47450;,
 0.57400;0.65780;3.88040;,
 0.40590;0.65780;4.28630;,
 0.00000;0.54360;3.88040;,
 3.88040;3.54360;0.00000;,
 4.28630;3.42940;0.40590;,
 4.45440;3.42940;0.00000;,
 3.88040;3.42940;0.57400;,
 3.47450;3.42940;0.40590;,
 3.30640;3.42940;0.00000;,
 3.47450;3.42940;-0.40590;,
 3.88040;3.42940;-0.57400;,
 4.28630;3.42940;-0.40590;,
 4.63040;3.10430;0.75000;,
 4.94110;3.10430;0.00000;,
 3.88040;3.10430;1.06070;,
 3.13040;3.10430;0.75000;,
 2.81970;3.10430;0.00000;,
 3.13040;3.10430;-0.75000;,
 3.88040;3.10430;-1.06070;,
 4.63040;3.10430;-0.75000;,
 4.86030;2.61760;0.97990;,
 5.26620;2.61760;0.00000;,
 3.88040;2.61760;1.38580;,
 2.90050;2.61760;0.97990;,
 2.49460;2.61760;0.00000;,
 2.90050;2.61760;-0.97990;,
 3.88040;2.61760;-1.38580;,
 4.86030;2.61760;-0.97990;,
 4.94110;2.04360;1.06070;,
 5.38040;2.04360;0.00000;,
 3.88040;2.04360;1.50000;,
 2.81970;2.04360;1.06070;,
 2.38040;2.04360;0.00000;,
 2.81970;2.04360;-1.06070;,
 3.88040;2.04360;-1.50000;,
 4.94110;2.04360;-1.06070;,
 4.86030;1.46960;0.97990;,
 5.26620;1.46960;0.00000;,
 3.88040;1.46960;1.38580;,
 2.90050;1.46960;0.97990;,
 2.49460;1.46960;0.00000;,
 2.90050;1.46960;-0.97990;,
 3.88040;1.46960;-1.38580;,
 4.86030;1.46960;-0.97990;,
 4.63040;0.98290;0.75000;,
 4.94110;0.98290;0.00000;,
 3.88040;0.98290;1.06070;,
 3.13040;0.98290;0.75000;,
 2.81970;0.98290;0.00000;,
 3.13040;0.98290;-0.75000;,
 3.88040;0.98290;-1.06070;,
 4.63040;0.98290;-0.75000;,
 4.28630;0.65780;0.40590;,
 4.45440;0.65780;0.00000;,
 3.88040;0.65780;0.57400;,
 3.47450;0.65780;0.40590;,
 3.30640;0.65780;0.00000;,
 3.47450;0.65780;-0.40590;,
 3.88040;0.65780;-0.57400;,
 4.28630;0.65780;-0.40590;,
 3.88040;0.54360;0.00000;,
 -3.88040;3.54360;0.00000;,
 -4.28630;3.42940;-0.40590;,
 -4.45440;3.42940;0.00000;,
 -3.88040;3.42940;-0.57400;,
 -3.47450;3.42940;-0.40590;,
 -3.30640;3.42940;0.00000;,
 -3.47450;3.42940;0.40590;,
 -3.88040;3.42940;0.57400;,
 -4.28630;3.42940;0.40590;,
 -4.63040;3.10430;-0.75000;,
 -4.94110;3.10430;0.00000;,
 -3.88040;3.10430;-1.06070;,
 -3.13040;3.10430;-0.75000;,
 -2.81970;3.10430;0.00000;,
 -3.13040;3.10430;0.75000;,
 -3.88040;3.10430;1.06070;,
 -4.63040;3.10430;0.75000;,
 -4.86030;2.61760;-0.97990;,
 -5.26620;2.61760;0.00000;,
 -3.88040;2.61760;-1.38580;,
 -2.90050;2.61760;-0.97990;,
 -2.49460;2.61760;0.00000;,
 -2.90050;2.61760;0.97990;,
 -3.88040;2.61760;1.38580;,
 -4.86030;2.61760;0.97990;,
 -4.94110;2.04360;-1.06070;,
 -5.38040;2.04360;0.00000;,
 -3.88040;2.04360;-1.50000;,
 -2.81970;2.04360;-1.06070;,
 -2.38040;2.04360;0.00000;,
 -2.81970;2.04360;1.06070;,
 -3.88040;2.04360;1.50000;,
 -4.94110;2.04360;1.06070;,
 -4.86030;1.46960;-0.97990;,
 -5.26620;1.46960;0.00000;,
 -3.88040;1.46960;-1.38580;,
 -2.90050;1.46960;-0.97990;,
 -2.49460;1.46960;0.00000;,
 -2.90050;1.46960;0.97990;,
 -3.88040;1.46960;1.38580;,
 -4.86030;1.46960;0.97990;,
 -4.63040;0.98290;-0.75000;,
 -4.94110;0.98290;0.00000;,
 -3.88040;0.98290;-1.06070;,
 -3.13040;0.98290;-0.75000;,
 -2.81970;0.98290;0.00000;,
 -3.13040;0.98290;0.75000;,
 -3.88040;0.98290;1.06070;,
 -4.63040;0.98290;0.75000;,
 -4.28630;0.65780;-0.40590;,
 -4.45440;0.65780;0.00000;,
 -3.88040;0.65780;-0.57400;,
 -3.47450;0.65780;-0.40590;,
 -3.30640;0.65780;0.00000;,
 -3.47450;0.65780;0.40590;,
 -3.88040;0.65780;0.57400;,
 -4.28630;0.65780;0.40590;,
 -3.88040;0.54360;0.00000;;
 
 610;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;1,14,15,2;,
 4;2,15,16,4;,
 4;4,16,17,6;,
 4;6,17,18,8;,
 4;8,18,19,10;,
 4;10,19,20,12;,
 4;14,21,22,15;,
 4;15,22,23,16;,
 4;16,23,24,17;,
 4;17,24,25,18;,
 4;18,25,26,19;,
 4;19,26,27,20;,
 4;21,28,29,22;,
 4;22,29,30,23;,
 4;23,30,31,24;,
 4;24,31,32,25;,
 4;25,32,33,26;,
 4;26,33,34,27;,
 4;28,35,36,29;,
 4;29,36,37,30;,
 4;30,37,38,31;,
 4;31,38,39,32;,
 4;32,39,40,33;,
 4;33,40,41,34;,
 4;35,42,43,36;,
 4;36,43,44,37;,
 4;37,44,45,38;,
 4;38,45,46,39;,
 4;39,46,47,40;,
 4;40,47,48,41;,
 4;42,49,50,43;,
 4;43,50,51,44;,
 4;44,51,52,45;,
 4;45,52,53,46;,
 4;46,53,54,47;,
 4;47,54,55,48;,
 4;49,56,57,50;,
 4;50,57,58,51;,
 4;51,58,59,52;,
 4;52,59,60,53;,
 4;53,60,61,54;,
 4;54,61,62,55;,
 4;56,63,64,57;,
 4;57,64,65,58;,
 4;58,65,66,59;,
 4;59,66,67,60;,
 4;60,67,68,61;,
 4;61,68,69,62;,
 4;63,70,71,64;,
 4;64,71,72,65;,
 4;65,72,73,66;,
 4;66,73,74,67;,
 4;67,74,75,68;,
 4;68,75,76,69;,
 4;70,77,78,71;,
 4;71,78,79,72;,
 4;72,79,80,73;,
 4;73,80,81,74;,
 4;74,81,82,75;,
 4;75,82,83,76;,
 4;77,0,3,78;,
 4;78,3,5,79;,
 4;79,5,7,80;,
 4;80,7,9,81;,
 4;81,9,11,82;,
 4;82,11,13,83;,
 4;20,27,84,85;,
 4;85,84,86,87;,
 4;87,86,88,89;,
 4;27,34,90,84;,
 4;84,90,91,86;,
 4;86,91,92,88;,
 4;34,41,93,90;,
 4;90,93,94,91;,
 4;91,94,95,92;,
 4;41,48,96,93;,
 4;93,96,97,94;,
 4;94,97,98,95;,
 4;48,55,99,96;,
 4;96,99,100,97;,
 4;97,100,101,98;,
 4;55,62,102,99;,
 4;99,102,103,100;,
 4;100,103,104,101;,
 4;62,69,105,102;,
 4;102,105,106,103;,
 4;103,106,107,104;,
 4;69,76,108,105;,
 4;105,108,109,106;,
 4;106,109,110,107;,
 4;76,83,111,108;,
 4;108,111,112,109;,
 4;109,112,113,110;,
 4;83,13,114,111;,
 4;111,114,115,112;,
 4;112,115,116,113;,
 4;13,12,117,114;,
 4;114,117,118,115;,
 4;115,118,119,116;,
 4;12,20,85,117;,
 4;117,85,87,118;,
 4;118,87,89,119;,
 3;120,0,121;,
 3;120,1,0;,
 3;14,1,120;,
 3;122,14,120;,
 3;21,14,122;,
 3;28,21,122;,
 3;122,123,28;,
 3;28,123,35;,
 3;35,123,42;,
 3;123,124,42;,
 3;49,42,124;,
 3;49,124,56;,
 3;56,124,125;,
 3;63,56,125;,
 3;70,63,125;,
 3;70,125,121;,
 3;121,77,70;,
 3;121,0,77;,
 4;126,127,128,129;,
 4;129,128,130,131;,
 4;131,130,132,133;,
 4;133,132,134,135;,
 4;135,134,136,137;,
 4;137,136,138,139;,
 4;139,138,140,141;,
 4;141,140,127,126;,
 3;142,130,128;,
 3;142,132,130;,
 3;142,134,132;,
 3;142,136,134;,
 3;142,138,136;,
 3;142,140,138;,
 4;143,144,145,146;,
 4;146,145,147,148;,
 4;148,147,149,150;,
 4;150,149,151,152;,
 4;152,151,153,154;,
 4;144,155,156,145;,
 4;145,156,157,147;,
 4;147,157,158,149;,
 4;149,158,159,151;,
 4;151,159,160,153;,
 4;155,161,162,156;,
 4;156,162,163,157;,
 4;157,163,164,158;,
 4;158,164,165,159;,
 4;159,165,166,160;,
 4;161,167,168,162;,
 4;162,168,169,163;,
 4;163,169,170,164;,
 4;164,170,171,165;,
 4;165,171,172,166;,
 4;167,173,174,168;,
 4;168,174,175,169;,
 4;169,175,176,170;,
 4;170,176,177,171;,
 4;171,177,178,172;,
 4;173,143,146,174;,
 4;174,146,148,175;,
 4;175,148,150,176;,
 4;176,150,152,177;,
 4;177,152,154,178;,
 4;153,179,180,154;,
 4;160,181,182,153;,
 4;166,183,184,160;,
 4;172,185,186,166;,
 4;178,187,188,172;,
 4;154,189,190,178;,
 3;191,192,193;,
 3;193,192,194;,
 3;194,192,195;,
 3;195,192,196;,
 3;196,192,197;,
 3;197,192,198;,
 3;198,192,199;,
 3;199,192,191;,
 3;200,201,202;,
 3;201,203,202;,
 3;203,204,202;,
 3;204,205,202;,
 3;205,206,202;,
 3;206,207,202;,
 3;207,208,202;,
 3;208,200,202;,
 3;209,210,211;,
 3;211,210,212;,
 3;212,210,213;,
 3;213,210,214;,
 3;214,210,215;,
 3;215,210,216;,
 3;216,210,217;,
 3;217,210,209;,
 3;218,219,220;,
 3;219,221,220;,
 3;221,222,220;,
 3;222,223,220;,
 3;223,224,220;,
 3;224,225,220;,
 3;225,226,220;,
 3;226,218,220;,
 4;227,228,229,230;,
 4;230,229,231,232;,
 4;232,231,233,234;,
 4;235,236,237,238;,
 4;238,237,239,240;,
 4;240,239,241,242;,
 3;228,227,243;,
 3;236,235,244;,
 3;234,233,245;,
 3;242,241,246;,
 4;247,227,230,248;,
 4;248,230,232,249;,
 4;249,232,234,250;,
 4;234,245,251,250;,
 4;247,252,243,227;,
 4;253,236,244,254;,
 4;253,255,237,236;,
 4;237,255,256,239;,
 4;241,257,258,246;,
 4;239,256,257,241;,
 4;246,258,251,245;,
 4;228,235,238,229;,
 4;229,238,240,231;,
 4;231,240,242,233;,
 4;233,242,246,259;,
 4;243,244,235,228;,
 4;260,261,244,243;,
 4;262,263,264,265;,
 4;263,266,267,264;,
 4;266,268,269,267;,
 4;268,270,271,269;,
 4;270,272,273,271;,
 4;272,262,265,273;,
 4;265,264,274,275;,
 4;264,267,276,274;,
 4;267,269,277,276;,
 4;269,271,278,277;,
 4;271,273,279,278;,
 4;273,265,275,279;,
 3;275,274,280;,
 3;274,276,280;,
 3;276,277,280;,
 3;277,278,280;,
 3;278,279,280;,
 3;279,275,280;,
 3;281,282,283;,
 3;281,284,282;,
 3;281,285,284;,
 3;281,286,285;,
 3;281,287,286;,
 3;281,283,287;,
 4;283,282,288,289;,
 4;282,284,290,288;,
 4;284,285,291,290;,
 4;285,286,292,291;,
 4;286,287,293,292;,
 4;287,283,289,293;,
 4;289,288,294,295;,
 4;288,290,296,294;,
 4;290,291,297,296;,
 4;291,292,298,297;,
 4;292,293,299,298;,
 4;293,289,295,299;,
 4;300,301,302,303;,
 4;301,304,305,302;,
 4;304,306,307,305;,
 4;308,309,310,311;,
 4;309,312,313,310;,
 4;312,314,315,313;,
 3;303,316,300;,
 3;311,317,308;,
 3;306,318,307;,
 3;314,319,315;,
 4;320,321,301,300;,
 4;321,322,304,301;,
 4;322,323,306,304;,
 4;306,323,324,318;,
 4;320,300,316,325;,
 4;326,327,317,311;,
 4;326,311,310,328;,
 4;310,313,329,328;,
 4;315,319,330,331;,
 4;313,315,331,329;,
 4;319,318,324,330;,
 4;303,302,309,308;,
 4;302,305,312,309;,
 4;305,307,314,312;,
 4;307,332,319,314;,
 4;316,303,308,317;,
 4;333,316,317,334;,
 3;335,336,337;,
 3;335,337,338;,
 3;335,338,339;,
 3;335,339,340;,
 3;335,340,341;,
 3;335,341,336;,
 4;336,342,343,337;,
 4;337,343,344,338;,
 4;338,344,345,339;,
 4;339,345,346,340;,
 4;340,346,347,341;,
 4;341,347,342,336;,
 4;342,348,349,343;,
 4;343,349,350,344;,
 4;344,350,351,345;,
 4;345,351,352,346;,
 4;346,352,353,347;,
 4;347,353,348,342;,
 4;354,355,356,357;,
 4;355,358,359,356;,
 4;358,360,361,359;,
 4;360,362,363,361;,
 4;362,364,365,363;,
 4;364,366,367,365;,
 4;366,368,369,367;,
 4;368,354,357,369;,
 4;357,356,370,371;,
 4;356,359,372,370;,
 4;359,361,373,372;,
 4;361,363,374,373;,
 4;363,365,375,374;,
 4;365,367,376,375;,
 4;367,369,377,376;,
 4;369,357,371,377;,
 4;378,379,380,381;,
 4;379,382,383,380;,
 4;382,384,385,383;,
 4;384,386,387,385;,
 4;386,388,389,387;,
 4;388,390,391,389;,
 4;392,393,394,395;,
 4;393,396,397,398;,
 4;396,399,400,401;,
 4;399,402,403,404;,
 4;402,405,406,407;,
 4;405,408,409,410;,
 4;408,411,412,413;,
 4;411,414,415,416;,
 4;414,417,418,419;,
 4;417,392,420,421;,
 3;422,423,424;,
 3;422,425,426;,
 3;422,427,428;,
 3;422,429,430;,
 3;422,431,432;,
 3;422,433,434;,
 3;422,435,436;,
 3;422,437,438;,
 3;422,439,440;,
 3;422,441,442;,
 4;394,424,423,395;,
 4;397,426,425,398;,
 4;400,428,427,401;,
 4;403,430,429,404;,
 4;406,432,431,407;,
 4;409,434,433,410;,
 4;412,436,435,413;,
 4;415,438,437,416;,
 4;418,440,439,419;,
 4;420,442,441,421;,
 4;443,444,445,446;,
 4;447,448,444,443;,
 4;449,450,448,447;,
 4;451,452,450,449;,
 4;453,454,452,451;,
 4;455,456,454,453;,
 4;457,458,456,455;,
 4;459,460,458,457;,
 4;461,462,460,459;,
 4;446,445,462,461;,
 3;463,464,465;,
 3;463,466,464;,
 3;463,467,466;,
 3;463,468,467;,
 3;463,469,468;,
 3;463,470,469;,
 3;463,471,470;,
 3;463,465,471;,
 4;465,464,472,473;,
 4;464,466,474,472;,
 4;466,467,475,474;,
 4;467,468,476,475;,
 4;468,469,477,476;,
 4;469,470,478,477;,
 4;470,471,479,478;,
 4;471,465,473,479;,
 4;473,472,480,481;,
 4;472,474,482,480;,
 4;474,475,483,482;,
 4;475,476,484,483;,
 4;476,477,485,484;,
 4;477,478,486,485;,
 4;478,479,487,486;,
 4;479,473,481,487;,
 4;481,480,488,489;,
 4;480,482,490,488;,
 4;482,483,491,490;,
 4;483,484,492,491;,
 4;484,485,493,492;,
 4;485,486,494,493;,
 4;486,487,495,494;,
 4;487,481,489,495;,
 4;489,488,496,497;,
 4;488,490,498,496;,
 4;490,491,499,498;,
 4;491,492,500,499;,
 4;492,493,501,500;,
 4;493,494,502,501;,
 4;494,495,503,502;,
 4;495,489,497,503;,
 3;504,505,506;,
 3;504,507,505;,
 3;504,508,507;,
 3;504,509,508;,
 3;504,510,509;,
 3;504,511,510;,
 3;504,512,511;,
 3;504,506,512;,
 4;506,505,513,514;,
 4;505,507,515,513;,
 4;507,508,516,515;,
 4;508,509,517,516;,
 4;509,510,518,517;,
 4;510,511,519,518;,
 4;511,512,520,519;,
 4;512,506,514,520;,
 4;514,513,521,522;,
 4;513,515,523,521;,
 4;515,516,524,523;,
 4;516,517,525,524;,
 4;517,518,526,525;,
 4;518,519,527,526;,
 4;519,520,528,527;,
 4;520,514,522,528;,
 4;522,521,529,530;,
 4;521,523,531,529;,
 4;523,524,532,531;,
 4;524,525,533,532;,
 4;525,526,534,533;,
 4;526,527,535,534;,
 4;527,528,536,535;,
 4;528,522,530,536;,
 4;530,529,537,538;,
 4;529,531,539,537;,
 4;531,532,540,539;,
 4;532,533,541,540;,
 4;533,534,542,541;,
 4;534,535,543,542;,
 4;535,536,544,543;,
 4;536,530,538,544;,
 4;538,537,545,546;,
 4;537,539,547,545;,
 4;539,540,548,547;,
 4;540,541,549,548;,
 4;541,542,550,549;,
 4;542,543,551,550;,
 4;543,544,552,551;,
 4;544,538,546,552;,
 4;546,545,553,554;,
 4;545,547,555,553;,
 4;547,548,556,555;,
 4;548,549,557,556;,
 4;549,550,558,557;,
 4;550,551,559,558;,
 4;551,552,560,559;,
 4;552,546,554,560;,
 3;554,553,561;,
 3;553,555,561;,
 3;555,556,561;,
 3;556,557,561;,
 3;557,558,561;,
 3;558,559,561;,
 3;559,560,561;,
 3;560,554,561;,
 3;562,563,564;,
 3;562,565,563;,
 3;562,566,565;,
 3;562,567,566;,
 3;562,568,567;,
 3;562,569,568;,
 3;562,570,569;,
 3;562,564,570;,
 4;564,563,571,572;,
 4;563,565,573,571;,
 4;565,566,574,573;,
 4;566,567,575,574;,
 4;567,568,576,575;,
 4;568,569,577,576;,
 4;569,570,578,577;,
 4;570,564,572,578;,
 4;572,571,579,580;,
 4;571,573,581,579;,
 4;573,574,582,581;,
 4;574,575,583,582;,
 4;575,576,584,583;,
 4;576,577,585,584;,
 4;577,578,586,585;,
 4;578,572,580,586;,
 4;580,579,587,588;,
 4;579,581,589,587;,
 4;581,582,590,589;,
 4;582,583,591,590;,
 4;583,584,592,591;,
 4;584,585,593,592;,
 4;585,586,594,593;,
 4;586,580,588,594;,
 4;588,587,595,596;,
 4;587,589,597,595;,
 4;589,590,598,597;,
 4;590,591,599,598;,
 4;591,592,600,599;,
 4;592,593,601,600;,
 4;593,594,602,601;,
 4;594,588,596,602;,
 4;596,595,603,604;,
 4;595,597,605,603;,
 4;597,598,606,605;,
 4;598,599,607,606;,
 4;599,600,608,607;,
 4;600,601,609,608;,
 4;601,602,610,609;,
 4;602,596,604,610;,
 4;604,603,611,612;,
 4;603,605,613,611;,
 4;605,606,614,613;,
 4;606,607,615,614;,
 4;607,608,616,615;,
 4;608,609,617,616;,
 4;609,610,618,617;,
 4;610,604,612,618;,
 3;612,611,619;,
 3;611,613,619;,
 3;613,614,619;,
 3;614,615,619;,
 3;615,616,619;,
 3;616,617,619;,
 3;617,618,619;,
 3;618,612,619;,
 3;620,621,622;,
 3;620,623,621;,
 3;620,624,623;,
 3;620,625,624;,
 3;620,626,625;,
 3;620,627,626;,
 3;620,628,627;,
 3;620,622,628;,
 4;622,621,629,630;,
 4;621,623,631,629;,
 4;623,624,632,631;,
 4;624,625,633,632;,
 4;625,626,634,633;,
 4;626,627,635,634;,
 4;627,628,636,635;,
 4;628,622,630,636;,
 4;630,629,637,638;,
 4;629,631,639,637;,
 4;631,632,640,639;,
 4;632,633,641,640;,
 4;633,634,642,641;,
 4;634,635,643,642;,
 4;635,636,644,643;,
 4;636,630,638,644;,
 4;638,637,645,646;,
 4;637,639,647,645;,
 4;639,640,648,647;,
 4;640,641,649,648;,
 4;641,642,650,649;,
 4;642,643,651,650;,
 4;643,644,652,651;,
 4;644,638,646,652;,
 4;646,645,653,654;,
 4;645,647,655,653;,
 4;647,648,656,655;,
 4;648,649,657,656;,
 4;649,650,658,657;,
 4;650,651,659,658;,
 4;651,652,660,659;,
 4;652,646,654,660;,
 4;654,653,661,662;,
 4;653,655,663,661;,
 4;655,656,664,663;,
 4;656,657,665,664;,
 4;657,658,666,665;,
 4;658,659,667,666;,
 4;659,660,668,667;,
 4;660,654,662,668;,
 4;662,661,669,670;,
 4;661,663,671,669;,
 4;663,664,672,671;,
 4;664,665,673,672;,
 4;665,666,674,673;,
 4;666,667,675,674;,
 4;667,668,676,675;,
 4;668,662,670,676;,
 3;670,669,677;,
 3;669,671,677;,
 3;671,672,677;,
 3;672,673,677;,
 3;673,674,677;,
 3;674,675,677;,
 3;675,676,677;,
 3;676,670,677;;
 
 MeshMaterialList {
  4;
  610;
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
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.180000;0.396000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.045000;0.099000;0.250000;;
  }
  Material {
   0.655000;0.655000;0.655000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.163750;0.163750;0.163750;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.250000;0.250000;0.250000;;
  }
  Material {
   0.259000;0.259000;0.259000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.064750;0.064750;0.064750;;
  }
 }
}
