xof 0303txt 0032

Scene SHScene1 {
 

 GravityEngine  {
  0.000000;-9.800000;0.000000;;
  { soBlock1 }
  { soBlock2 }
  { soBlock3 }
  { soBlock4 }
  { soBlock5 }
  { soBlock6 }
 }

 PenaltyEngine  {
  

  Penalty  {
   
   { frPointer }
   { frFloor }
   { Pointer }
  }

  Penalty  {
   
   { frPointer2 }
   { frFloor }
   { Pointer2 }
  }

  Penalty  {
   
   { frBlock1 }
   { frFloor }
   { soBlock1 }
  }

  Penalty  {
   
   { frBlock2 }
   { frFloor }
   { soBlock2 }
  }

  Penalty  {
   
   { frBlock3 }
   { frFloor }
   { soBlock3 }
  }

  Penalty  {
   
   { frBlock4 }
   { frFloor }
   { soBlock4 }
  }

  Penalty  {
   
   { frBlock5 }
   { frFloor }
   { soBlock5 }
  }

  Penalty  {
   
   { frBlock6 }
   { frFloor }
   { soBlock6 }
  }

  Penalty  {
   
   { frBlock1 }
   { frPointer }
   { soBlock1 }
   { Pointer }
  }

  Penalty  {
   
   { frBlock2 }
   { frPointer }
   { soBlock2 }
   { Pointer }
  }

  Penalty  {
   
   { frBlock3 }
   { frPointer }
   { soBlock3 }
   { Pointer }
  }

  Penalty  {
   
   { frBlock4 }
   { frPointer }
   { soBlock4 }
   { Pointer }
  }

  Penalty  {
   
   { frBlock5 }
   { frPointer }
   { soBlock5 }
   { Pointer }
  }

  Penalty  {
   
   { frBlock6 }
   { frPointer }
   { soBlock6 }
   { Pointer }
  }

  Penalty  {
   
   { frBlock1 }
   { frPointer2 }
   { soBlock1 }
   { Pointer2 }
  }

  Penalty  {
   
   { frBlock2 }
   { frPointer2 }
   { soBlock2 }
   { Pointer2 }
  }

  Penalty  {
   
   { frBlock3 }
   { frPointer2 }
   { soBlock3 }
   { Pointer2 }
  }

  Penalty  {
   
   { frBlock4 }
   { frPointer2 }
   { soBlock4 }
   { Pointer2 }
  }

  Penalty  {
   
   { frBlock5 }
   { frPointer2 }
   { soBlock5 }
   { Pointer2 }
  }

  Penalty  {
   
   { frBlock6 }
   { frPointer2 }
   { soBlock6 }
   { Pointer2 }
  }

  Penalty  {
   
   { frBlock1 }
   { frBlock2 }
   { soBlock1 }
   { soBlock2 }
  }

  Penalty  {
   
   { frBlock1 }
   { frBlock3 }
   { soBlock1 }
   { soBlock3 }
  }

  Penalty  {
   
   { frBlock1 }
   { frBlock4 }
   { soBlock1 }
   { soBlock4 }
  }

  Penalty  {
   
   { frBlock1 }
   { frBlock5 }
   { soBlock1 }
   { soBlock5 }
  }

  Penalty  {
   
   { frBlock1 }
   { frBlock6 }
   { soBlock1 }
   { soBlock6 }
  }

  Penalty  {
   
   { frBlock2 }
   { frBlock3 }
   { soBlock2 }
   { soBlock3 }
  }

  Penalty  {
   
   { frBlock2 }
   { frBlock4 }
   { soBlock2 }
   { soBlock4 }
  }

  Penalty  {
   
   { frBlock2 }
   { frBlock5 }
   { soBlock2 }
   { soBlock5 }
  }

  Penalty  {
   
   { frBlock2 }
   { frBlock6 }
   { soBlock2 }
   { soBlock6 }
  }

  Penalty  {
   
   { frBlock3 }
   { frBlock4 }
   { soBlock3 }
   { soBlock4 }
  }

  Penalty  {
   
   { frBlock3 }
   { frBlock5 }
   { soBlock3 }
   { soBlock5 }
  }

  Penalty  {
   
   { frBlock3 }
   { frBlock6 }
   { soBlock3 }
   { soBlock6 }
  }

  Penalty  {
   
   { frBlock4 }
   { frBlock5 }
   { soBlock4 }
   { soBlock5 }
  }

  Penalty  {
   
   { frBlock4 }
   { frBlock6 }
   { soBlock4 }
   { soBlock6 }
  }

  Penalty  {
   
   { frBlock5 }
   { frBlock6 }
   { soBlock5 }
   { soBlock6 }
  }
 }

 SolidContainer  {
  

  Solid Pointer {
   0.500000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frPointer }
  }

  Solid Pointer2 {
   0.500000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frPointer2 }
  }

  Solid soBlock1 {
   0.400000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frBlock1 }
  }

  Solid soBlock2 {
   0.400000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frBlock2 }
  }

  Solid soBlock3 {
   0.400000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frBlock3 }
  }

  Solid soBlock4 {
   0.400000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frBlock4 }
  }

  Solid soBlock5 {
   0.400000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frBlock5 }
  }

  Solid soBlock6 {
   0.400000;
   1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   { frBlock6 }
  }
 }

 Camera Cam1 {
  0.893372,0.140077,0.426926,0.000000,0.000000,0.950163,-0.311755,0.000000,-0.449320,0.278513,0.848848,0.000000,-1.199999,-1.200000,17.416431,1.000000;;
  0.400000;
  0.400000;
  0.000000;
  0.000000;
  0.400000;
  4000.000000;
 }

 Camera NewCamera {
  1.000000,0.000000,0.000000,0.000000,0.000000,0.894427,-0.447213,0.000000,0.000000,0.447214,0.894427,0.000000,0.000000,0.000000,13.416408,1.000000;;
  0.400000;
  0.400000;
  0.000000;
  0.000000;
  0.400000;
  4000.000000;
 }

 Frame frTop {
  

  Light8 light {
   3;
   0.900000;0.900000;0.900000;1.000000;;
   0.900000;0.900000;0.900000;1.000000;;
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   0.500000;-0.707107;-0.500000;;
   20.000000;
   1.000000;
   0.800000;
   0.800000;
   0.800000;
   0.300000;
   0.330000;
  	{ frLight }
  }

  Frame frLight {
   FrameTransformMatrix  {
    0.707107,0.000000,-0.707107,0.000000,0.500000,0.707107,0.500000,0.000000,0.500000,-0.707107,0.500000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }

  Frame frPointer2 {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,10.000000,0.000000,0.000000,1.000000;;
   }
  }

  Frame frPointer {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,10.000000,10.000000,0.000000,1.000000;;
   }
  }

  Frame frBlock1 {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.500000,0.500000,0.000000,1.000000;;
   }

   Mesh meshBlock {
    24;
    -0.500000;0.500000;1.000000;,
    0.500000;0.500000;1.000000;,
    0.500000;0.500000;-1.000000;,
    -0.500000;0.500000;-1.000000;,
    -0.500000;0.500000;-1.000000;,
    0.500000;0.500000;-1.000000;,
    0.500000;-0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    0.500000;0.500000;1.000000;,
    0.500000;-0.500000;1.000000;,
    0.500000;-0.500000;-1.000000;,
    0.500000;0.500000;-1.000000;,
    -0.500000;-0.500000;1.000000;,
    0.500000;-0.500000;1.000000;,
    0.500000;0.500000;1.000000;,
    -0.500000;0.500000;1.000000;,
    -0.500000;-0.500000;1.000000;,
    -0.500000;0.500000;1.000000;,
    -0.500000;0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    0.500000;-0.500000;-1.000000;,
    0.500000;-0.500000;1.000000;,
    -0.500000;-0.500000;1.000000;;
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
      1.000000;0.121569;0.043137;1.000000;;
      5.000000;
      0.000000;0.000000;0.000000;;
      0.250000;0.030392;0.010784;;

      TextureFilename  {
       "";
      }
     }
    }
   }
  }

  Frame frBlock2 {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-0.500000,0.500000,0.000000,1.000000;;
   }

   Mesh  {
    24;
    -0.500000;0.500000;1.000000;,
    0.500000;0.500000;1.000000;,
    0.500000;0.500000;-1.000000;,
    -0.500000;0.500000;-1.000000;,
    -0.500000;0.500000;-1.000000;,
    0.500000;0.500000;-1.000000;,
    0.500000;-0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    0.500000;0.500000;1.000000;,
    0.500000;-0.500000;1.000000;,
    0.500000;-0.500000;-1.000000;,
    0.500000;0.500000;-1.000000;,
    -0.500000;-0.500000;1.000000;,
    0.500000;-0.500000;1.000000;,
    0.500000;0.500000;1.000000;,
    -0.500000;0.500000;1.000000;,
    -0.500000;-0.500000;1.000000;,
    -0.500000;0.500000;1.000000;,
    -0.500000;0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    0.500000;-0.500000;-1.000000;,
    0.500000;-0.500000;1.000000;,
    -0.500000;-0.500000;1.000000;;
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
      1.000000;0.992157;0.062745;1.000000;;
      5.000000;
      0.000000;0.000000;0.000000;;
      0.250000;0.248039;0.015686;;

      TextureFilename  {
       "";
      }
     }
    }
   }
  }

  Frame frBlock3 {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,1.500000,0.500000,1.000000;;
   }

   Mesh  {
    24;
    -1.000000;0.500000;0.500000;,
    1.000000;0.500000;0.500000;,
    1.000000;0.500000;-0.500000;,
    -1.000000;0.500000;-0.500000;,
    -1.000000;0.500000;-0.500000;,
    1.000000;0.500000;-0.500000;,
    1.000000;-0.500000;-0.500000;,
    -1.000000;-0.500000;-0.500000;,
    1.000000;0.500000;0.500000;,
    1.000000;-0.500000;0.500000;,
    1.000000;-0.500000;-0.500000;,
    1.000000;0.500000;-0.500000;,
    -1.000000;-0.500000;0.500000;,
    1.000000;-0.500000;0.500000;,
    1.000000;0.500000;0.500000;,
    -1.000000;0.500000;0.500000;,
    -1.000000;-0.500000;0.500000;,
    -1.000000;0.500000;0.500000;,
    -1.000000;0.500000;-0.500000;,
    -1.000000;-0.500000;-0.500000;,
    -1.000000;-0.500000;-0.500000;,
    1.000000;-0.500000;-0.500000;,
    1.000000;-0.500000;0.500000;,
    -1.000000;-0.500000;0.500000;;
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
      0.058824;0.152941;1.000000;1.000000;;
      5.000000;
      0.000000;0.000000;0.000000;;
      0.014706;0.038235;0.250000;;

      TextureFilename  {
       "";
      }
     }
    }
   }
  }

  Frame frBlock4 {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,1.500000,-0.500000,1.000000;;
   }

   Mesh  {
    24;
    -1.000000;0.500000;0.500000;,
    1.000000;0.500000;0.500000;,
    1.000000;0.500000;-0.500000;,
    -1.000000;0.500000;-0.500000;,
    -1.000000;0.500000;-0.500000;,
    1.000000;0.500000;-0.500000;,
    1.000000;-0.500000;-0.500000;,
    -1.000000;-0.500000;-0.500000;,
    1.000000;0.500000;0.500000;,
    1.000000;-0.500000;0.500000;,
    1.000000;-0.500000;-0.500000;,
    1.000000;0.500000;-0.500000;,
    -1.000000;-0.500000;0.500000;,
    1.000000;-0.500000;0.500000;,
    1.000000;0.500000;0.500000;,
    -1.000000;0.500000;0.500000;,
    -1.000000;-0.500000;0.500000;,
    -1.000000;0.500000;0.500000;,
    -1.000000;0.500000;-0.500000;,
    -1.000000;-0.500000;-0.500000;,
    -1.000000;-0.500000;-0.500000;,
    1.000000;-0.500000;-0.500000;,
    1.000000;-0.500000;0.500000;,
    -1.000000;-0.500000;0.500000;;
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
      0.109804;1.000000;0.278431;1.000000;;
      5.000000;
      0.000000;0.000000;0.000000;;
      0.027451;0.250000;0.069608;;

      TextureFilename  {
       "";
      }
     }
    }
   }
  }

  Frame frBlock5 {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.500000,2.500000,0.000000,1.000000;;
   }

   Mesh  {
    24;
    -0.500000;0.500000;1.000000;,
    0.500000;0.500000;1.000000;,
    0.500000;0.500000;-1.000000;,
    -0.500000;0.500000;-1.000000;,
    -0.500000;0.500000;-1.000000;,
    0.500000;0.500000;-1.000000;,
    0.500000;-0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    0.500000;0.500000;1.000000;,
    0.500000;-0.500000;1.000000;,
    0.500000;-0.500000;-1.000000;,
    0.500000;0.500000;-1.000000;,
    -0.500000;-0.500000;1.000000;,
    0.500000;-0.500000;1.000000;,
    0.500000;0.500000;1.000000;,
    -0.500000;0.500000;1.000000;,
    -0.500000;-0.500000;1.000000;,
    -0.500000;0.500000;1.000000;,
    -0.500000;0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    0.500000;-0.500000;-1.000000;,
    0.500000;-0.500000;1.000000;,
    -0.500000;-0.500000;1.000000;;
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
      1.000000;0.121569;0.043137;1.000000;;
      5.000000;
      0.000000;0.000000;0.000000;;
      0.250000;0.030392;0.010784;;

      TextureFilename  {
       "";
      }
     }
    }
   }
  }

  Frame frBlock6 {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-0.500000,5.500000,0.000000,1.000000;;
   }

   Mesh  {
    24;
    -0.500000;0.500000;1.000000;,
    0.500000;0.500000;1.000000;,
    0.500000;0.500000;-1.000000;,
    -0.500000;0.500000;-1.000000;,
    -0.500000;0.500000;-1.000000;,
    0.500000;0.500000;-1.000000;,
    0.500000;-0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    0.500000;0.500000;1.000000;,
    0.500000;-0.500000;1.000000;,
    0.500000;-0.500000;-1.000000;,
    0.500000;0.500000;-1.000000;,
    -0.500000;-0.500000;1.000000;,
    0.500000;-0.500000;1.000000;,
    0.500000;0.500000;1.000000;,
    -0.500000;0.500000;1.000000;,
    -0.500000;-0.500000;1.000000;,
    -0.500000;0.500000;1.000000;,
    -0.500000;0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    -0.500000;-0.500000;-1.000000;,
    0.500000;-0.500000;-1.000000;,
    0.500000;-0.500000;1.000000;,
    -0.500000;-0.500000;1.000000;;
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
      1.000000;0.992157;0.062745;1.000000;;
      5.000000;
      0.000000;0.000000;0.000000;;
      0.250000;0.248039;0.015686;;

      TextureFilename  {
       "";
      }
     }
    }
   }
  }

  Frame frFloor {
   

   FrameTransformMatrix  {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,1.000000;;
   }

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
      0.600000;0.600000;0.600000;0.600000;;
      0.000000;
      0.000000;0.000000;0.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename  {
       "";
      }
     }
    }
   }
  }
 }
}