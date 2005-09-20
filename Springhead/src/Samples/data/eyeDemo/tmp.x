xof 0303txt 0032

Scene  {
 

 GravityEngine  {
  0.000000;-98.000000;0.000000;;
 }

 SolidContainer  {
  

  Solid Pointer1 {
   0.300000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frPointer1 }
  }

  Solid soTarget1 {
   0.300000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frTarget1 }
  }

  Solid soOcularL {
   0.300000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frOcularL }
  }

  Solid soOcularR {
   0.300000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frOcularR }
  }

  Solid soCube3 {
   0.300000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frCube3 }
  }
 }

 Simulator  {
  0.002000;
  0.980000;
 }

 Light8  {
  3;
  0.900000;0.900000;0.900000;0.900000;;
  0.900000;0.900000;0.900000;0.900000;;
  0.500000;0.500000;0.500000;0.500000;;
  0.000000;0.000000;0.000000;;
  0.500000;-0.707107;0.500000;;
  20.000000;
  1.000000;
  0.800000;
  0.800000;
  0.800000;
  0.300000;
  0.330000;
  { frLight }
 }

 Material matPointer1 {
  1.000000;0.380000;0.398000;1.000000;;
  5.000000;
  0.000000;0.000000;0.000000;;
  0.095000;0.174500;0.250000;;
 }

 Material matEye {
  1.000000;0.380000;0.398000;1.000000;;
  5.000000;
  0.000000;0.000000;0.000000;;
  0.095000;0.174500;0.250000;;
 }

 Material matTarget1 {
  0.080000;1.000000;0.098000;1.000000;;
  5.000000;
  0.000000;0.000000;0.000000;;
  0.095000;0.174500;0.250000;;
 }

 PhysicalMaterial pmPointer {
  0.200000;
  0.100000;
  0.200000;
  0.100000;
  0.600000;
  0.300000;
 }

 Frame frLight {
  

  FrameTransformMatrix  {
   0.707107,0.000000,-0.707107,0.000000,0.500000,0.707107,0.500000,0.000000,0.500000,-0.707107,0.500000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }
 }

 Frame frPointer1 {
  
 }

 Frame frTarget1 {
  

  FrameTransformMatrix  {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,-10.000000,1.000000;;
  }

  Sphere  {
   0.200000;
   20;
   20;
   { matTarget1 }
  }
 }

 Frame frOcularL {
  

  FrameTransformMatrix  {
   0.980615,0.000000,-0.195946,0.000000,0.000000,1.000000,0.000000,0.000000,0.195946,0.000000,0.980615,0.000000,20.000000,0.000000,0.000000,10.000000;;
  }

  Import  {
   "./eye/eye0910Bsight.x";
  }
 }

 Frame frOcularR {
  

  FrameTransformMatrix  {
   0.980615,0.000000,0.195946,0.000000,0.000000,1.000000,0.000000,0.000000,-0.195946,0.000000,0.980615,0.000000,-20.000000,0.000000,0.000000,10.000000;;
  }

  Import  {
   "./eye/eye0910Bsight.x";
  }
 }

 Frame frCube3 {
  

  FrameTransformMatrix  {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,5.000000,1.000000;;
  }
 }

 Frame frFloor {
  

  Mesh  {
   24;
   -25.000000;0.000000;25.000000;,
   25.000000;0.000000;25.000000;,
   25.000000;0.000000;-25.000000;,
   -25.000000;0.000000;-25.000000;,
   -25.000000;0.000000;-25.000000;,
   25.000000;0.000000;-25.000000;,
   25.000000;-1.000000;-25.000000;,
   -25.000000;-1.000000;-25.000000;,
   25.000000;0.000000;25.000000;,
   25.000000;-1.000000;25.000000;,
   25.000000;-1.000000;-25.000000;,
   25.000000;0.000000;-25.000000;,
   -25.000000;-1.000000;25.000000;,
   25.000000;-1.000000;25.000000;,
   25.000000;0.000000;25.000000;,
   -25.000000;0.000000;25.000000;,
   -25.000000;-1.000000;25.000000;,
   -25.000000;0.000000;25.000000;,
   -25.000000;0.000000;-25.000000;,
   -25.000000;-1.000000;-25.000000;,
   -25.000000;-1.000000;-25.000000;,
   25.000000;-1.000000;-25.000000;,
   25.000000;-1.000000;25.000000;,
   -25.000000;-1.000000;25.000000;;
   12;
   3;0,1,2;,
   3;0,2,3;,
   3;4,5,6;,
   3;4,6,7;,
   3;8,9,10;,
   3;8,10,11;,
   3;12,13,14;,
   3;12,14,15;,
   3;16,17,18;,
   3;16,18,19;,
   3;20,21,22;,
   3;20,22,23;;

   MeshMaterialList  {
    1;
    12;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;

    Material  {
     0.300000;0.300000;0.300000;0.600000;;
     0.000000;
     0.000000;0.000000;0.000000;;
     0.000000;0.000000;0.000000;;
    }
   }

   MeshNormals  {
    24;
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;;
    12;
    3;0,1,2;,
    3;0,2,3;,
    3;4,5,6;,
    3;4,6,7;,
    3;8,9,10;,
    3;8,10,11;,
    3;12,13,14;,
    3;12,14,15;,
    3;16,17,18;,
    3;16,18,19;,
    3;20,21,22;,
    3;20,22,23;;
   }
  }

  Camera  {
   0.999998,-0.000000,0.000001,0.000000,0.000000,0.958496,-0.285103,0.000000,0.000000,0.285104,0.958495,0.000000,-0.000013,-0.238736,53.182941,1.000000;;
   0.400000;
   0.400000;
   0.000000;
   0.000000;
   0.400000;
   4000.000000;
  }
 }
}