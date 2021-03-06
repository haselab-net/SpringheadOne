xof 0302txt 0064

Header {
  1;
  0;
  1;
}


Scene {
  SolidContainer{
    Solid soSpinTop{
      2.5;                    # 質量
      3.0, 0.0, 0.0,              
      0.0, 1.0, 0.0,          # 慣性テンソル
      0.0, 0.0, 3.0;;
      0.0; 0.0; 0.0;;         # 初期速度
      0.01; 16.0; 0.0;;       # 初期角速度
      0.0; 0.1; 0.0;;         # 重心（メッシュの原点からの相対位置）
      {frSpinTop}             # フレームへの参照
                              # (複数のフレームをツリーとして持つ場合は
                              # トップフレームを指す）
    }
    Solid Pointer1{
      0.3;                    # 質量
      0.1, 0.0, 0.0,
      0.0, 0.1, 0.0,          # 慣性テンソル
      0.0, 0.0, 0.1;;
      0.0; 0.0; 0.0;;         # 初期速度
      0.0; 0.0; 0.0;;         # 初期角速度
      0.0; 0.0; 0.0;;         # 重心（メッシュの原点からの相対位置）
      {frPointer1}             # フレームへの参照
                              # (複数のフレームをツリーとして持つ場合は
                              # トップフレームを指す）

    }
  }
  
  GravityEngine{
    0.0; -9.8; 0.0;;          # 重力をはたらかせる方向
    {soSpinTop}               # 重力をはたらかせる剛体
  }
  
  PenaltyEngine{
    Penalty{
      {frSpinTop}             # 衝突判定対象フレーム1
      {frFloor}               # 衝突判定対象フレーム2
      {soSpinTop}             # 反力を加える剛体1
    }
    Penalty{
      {frPointer1}             # 衝突判定対象フレーム1
      {frFloor}               # 衝突判定対象フレーム2
      {Pointer1}               # 反力を加える剛体1
    }
    Penalty{
      {frSpinTop}             # 衝突判定対象フレーム1
      {frPointer1}             # 衝突判定対象フレーム2
      {soSpinTop}             # 反力を加える剛体1
      {Pointer1}               # 反力を加える剛体2
    }
  }

  WarpEngine{
    Warp{
      0.000000; 3.700000; 0.000000;;  # ワープ先の位置
      0.0; 0.0; 0.0;;                 # ワープ直後の速度
      0.01; 16.0; 0.0;;               # ワープ直後の角速度
      {frSpinTop}                     # ワープするフレーム
      {frWarpDetector}                # ワープゾーンフレーム
      {soSpinTop}                     # ワープする剛体
    }
  }

}

Frame frRoot {

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,       # フレーム(子フレームを含む)の
   0.000000,1.000000,0.000000,0.000000,       # 初期位置(ローカル座標系)
   0.000000,0.000000,1.000000,0.000000,
   0, 0, 0, 1.000000;;
  }

  Frame frLight {
    FrameTransformMatrix  {
      0.707107,0.000000,-0.707107,0.000000,   # フレーム(子フレームを含む)の
      0.500000,0.707107,0.500000,0.000000,    # 初期位置(ローカル座標系)
      0.500000,-0.707107,0.500000,0.000000,
      0.000000,0.000000,0.000000,1.00000;;
    }
  }

  Light8{
    3;                                        # Type of light source
    0.900000;0.900000;0.900000;0.900000;;     # Diffuse color of light(RGBA)
    0.900000;0.900000;0.900000;0.900000;;     # Specular color of light(RGBA)
    0.000000;0.000000;0.000000;0.000000;;     # Ambient color of light(RGBA)
    0.000000;0.000000;0.000000;;              # Position in world space
    -0.3 ;-0.3 ; -0.3;;                       # Direction in world space
    20.000000;                                # Cutoff range
    1.000000;                                 # Falloff
    0.800000;                                 # Constant attenuation
    0.800000;                                 # Linear attenuation
    0.800000;                                 # Quadratic attenuation
    0.300000;                                 # Inner angle of spotlight cone
    0.330000;                                 # Outer angle of spotlight cone
    { frLight }                               # フレームへの参照
  }

  Frame frPointer1{
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,    # フレーム(子フレームを含む)の
      0.000000,1.000000,0.000000,0.000000,    # 初期位置(ローカル座標系)
      0.000000,0.000000,1.000000,0.000000,
      10.0, 10.0, 20.0, 1.000000;;
    }
    Mesh {
      24;                                       # 頂点数
      -0.35;0.35;0.35;,    0.35;0.35;0.35;,     # 各頂点の位置
       0.35;0.35;-0.35;,  -0.35;0.35;-0.35;,
      -0.35;0.35;-0.35;,   0.35;0.35;-0.35;,
       0.35;-0.35;-0.35;, -0.35;-0.35;-0.35;,
       0.35;0.35;0.35;,    0.35;-0.35;0.35;,  
       0.35;-0.35;-0.35;,  0.35;0.35;-0.35;,
      -0.35;-0.35;0.35;,   0.35;-0.35;0.35;,  
       0.35;0.35;0.35;,   -0.35;0.35;0.35;,
      -0.35;-0.35;0.35;,  -0.35;0.35;0.35;, 
      -0.35;0.35;-0.35;,  -0.35;-0.35;-0.35;,
      -0.35;-0.35;-0.35;,  0.35;-0.35;-0.35;, 
       0.35;-0.35;0.35;,  -0.35;-0.35;0.35;;
      6;                                        # 面の数
      4;0,1,2,3;,                               # 各面を構成する頂点
      4;4,5,6,7;,
      4;8,9,10,11;,
      4;12,13,14,15;,
      4;16,17,18,19;,
      4;20,21,22,23;;
      MeshMaterialList {
        1;                        # マテリアルの数
        6;                        # 面の数
        0,  0,  0,  0,  0,  0;;   # 各面のマテリアル(←の場合0番目のマテリアル)
        Material {                              # マテリアル
          0.313726;0.945098;0.992157;1.000000;; # 面の色(RGBA)
          5.000000;                             # power
          0.000000;0.000000;0.000000;;          # specular
          0.078431;0.236275;0.248039;;          # emissive
        }
      }
      MeshNormals {
        6;                                  # 法線の数
        0.000000;1.000000;0.000000;,        # 各法線の方向
        0.000000;0.000000;-1.000000;,
        1.000000;0.000000;0.000000;,
        0.000000;0.000000;1.000000;,
        -1.000000;0.000000;0.000000;,
        0.000000;-1.000000;-0.000000;;
        6;                                  # 面の数
        4;0,0,0,0;,                         # 各面の法線のインデックス
        4;1,1,1,1;,
        4;2,2,2,2;,
        4;3,3,3,3;,
        4;4,4,4,4;,
        4;5,5,5,5;;
      }
    }
  }

  Frame frSpinTop {
    FrameTransformMatrix {
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 3.7, 0.0, 1.0;;
    }
    Mesh {
      82;
      0.00000;0.30000;-1.00000;,
      0.30902;0.30000;-0.95106;,
      0.30902;0.20000;-0.95106;,
      0.00000;0.20000;-1.00000;,
      0.58779;0.30000;-0.80902;,
      0.58779;0.20000;-0.80902;,
      0.80902;0.30000;-0.58779;,
      0.80902;0.20000;-0.58779;,
      0.95106;0.30000;-0.30902;,
      0.95106;0.20000;-0.30902;,
      1.00000;0.30000;0.00000;,
      1.00000;0.20000;0.00000;,
      0.95106;0.30000;0.30902;,
      0.95106;0.20000;0.30902;,
      0.80902;0.30000;0.58779;,
      0.80902;0.20000;0.58779;,
      0.58779;0.30000;0.80902;,
      0.58779;0.20000;0.80902;,
      0.30902;0.30000;0.95106;,
      0.30902;0.20000;0.95106;,
      -0.00000;0.30000;1.00000;,
      -0.00000;0.20000;1.00000;,
      -0.30902;0.30000;0.95106;,
      -0.30902;0.20000;0.95106;,
      -0.58779;0.30000;0.80902;,
      -0.58779;0.20000;0.80902;,
      -0.80902;0.30000;0.58778;,
      -0.80902;0.20000;0.58778;,
      -0.95106;0.30000;0.30902;,
      -0.95106;0.20000;0.30902;,
      -1.00000;0.30000;-0.00000;,
      -1.00000;0.20000;-0.00000;,
      -0.95106;0.30000;-0.30902;,
      -0.95106;0.20000;-0.30902;,
      -0.80902;0.30000;-0.58779;,
      -0.80902;0.20000;-0.58779;,
      -0.58778;0.30000;-0.80902;,
      -0.58778;0.20000;-0.80902;,
      -0.30902;0.30000;-0.95106;,
      -0.30902;0.20000;-0.95106;,
      0.00000;0.30000;-1.00000;,
      0.00000;0.20000;-1.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;0.30000;0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;,
      0.00000;-0.61;-0.00000;;
     
      60;
      4;0,1,2,3;,
      4;1,4,5,2;,
      4;4,6,7,5;,
      4;6,8,9,7;,
      4;8,10,11,9;,
      4;10,12,13,11;,
      4;12,14,15,13;,
      4;14,16,17,15;,
      4;16,18,19,17;,
      4;18,20,21,19;,
      4;20,22,23,21;,
      4;22,24,25,23;,
      4;24,26,27,25;,
      4;26,28,29,27;,
      4;28,30,31,29;,
      4;30,32,33,31;,
      4;32,34,35,33;,
      4;34,36,37,35;,
      4;36,38,39,37;,
      4;38,40,41,39;,
      3;42,1,0;,
      3;43,4,1;,
      3;44,6,4;,
      3;45,8,6;,
      3;46,10,8;,
      3;47,12,10;,
      3;48,14,12;,
      3;49,16,14;,
      3;50,18,16;,
      3;51,20,18;,
      3;52,22,20;,
      3;53,24,22;,
      3;54,26,24;,
      3;55,28,26;,
      3;56,30,28;,
      3;57,32,30;,
      3;58,34,32;,
      3;59,36,34;,
      3;60,38,36;,
      3;61,40,38;,
      3;62,3,2;,
      3;63,2,5;,
      3;64,5,7;,
      3;65,7,9;,
      3;66,9,11;,
      3;67,11,13;,
      3;68,13,15;,
      3;69,15,17;,
      3;70,17,19;,
      3;71,19,21;,
      3;72,21,23;,
      3;73,23,25;,
      3;74,25,27;,
      3;75,27,29;,
      3;76,29,31;,
      3;77,31,33;,
      3;78,33,35;,
      3;79,35,37;,
      3;80,37,39;,
      3;81,39,41;;
 
      MeshMaterialList {
        2;
        60;
        0,
        0,
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
        1,
        1,
        1,
        1,
        1,
        0,
        0,
        0;;
        Material {
          1.0; 0.3; 0.3; 1.000000;;
          5.000000;
          0.000000;0.000000;0.000000;;
          0.25;0.000000;0.0;;
        }
        Material {
          0.80;0.80;0.80;1.000000;;
          5.000000;
          0.000000;0.000000;0.000000;;
          0.250000;0.250000;0.250000;;
        }
      }
      MeshNormals {
        42;
        0.000000;1.000000;0.000000;,
        0.000000;0.000000;-1.000000;,
        0.309017;0.000000;-0.951056;,
        0.587785;0.000000;-0.809017;,
        0.809017;0.000000;-0.587785;,
        0.951057;0.000000;-0.309017;,
        1.000000;0.000000;0.000000;,
        0.951056;0.000000;0.309017;,
        0.809017;0.000000;0.587785;,
        0.587785;0.000000;0.809017;,
        0.309017;0.000000;0.951057;,
        -0.000000;0.000000;1.000000;,
        -0.309017;0.000000;0.951056;,
        -0.587786;0.000000;0.809017;,
        -0.809017;0.000000;0.587785;,
        -0.951057;0.000000;0.309016;,
        -1.000000;0.000000;-0.000001;,
        -0.951056;0.000000;-0.309018;,
        -0.809016;0.000000;-0.587786;,
        -0.587784;0.000000;-0.809018;,
        -0.309016;0.000000;-0.951057;,
        0.000000;-0.434816;-0.900519;,
        0.278276;-0.434816;-0.856445;,
        0.529312;-0.434816;-0.728535;,
        0.728535;-0.434816;-0.529312;,
        0.856445;-0.434816;-0.278276;,
        0.900519;-0.434816;-0.000000;,
        0.856445;-0.434816;0.278276;,
        0.728535;-0.434816;0.529312;,
        0.529312;-0.434816;0.728535;,
        0.278276;-0.434816;0.856445;,
        -0.000000;-0.434816;0.900519;,
        -0.278276;-0.434816;0.856445;,
        -0.529312;-0.434816;0.728535;,
        -0.728536;-0.434816;0.529311;,
        -0.856445;-0.434816;0.278275;,
        -0.900519;-0.434816;-0.000001;,
        -0.856444;-0.434816;-0.278277;,
        -0.728535;-0.434816;-0.529313;,
        -0.529311;-0.434816;-0.728536;,
        -0.278275;-0.434816;-0.856445;,
        0.000000;-1.000000;-0.000000;;
        
        60;
        4;1,2,22,21;,
        4;2,3,23,22;,
        4;3,4,24,23;,
        4;4,5,25,24;,
        4;5,6,26,25;,
        4;6,7,27,26;,
        4;7,8,28,27;,
        4;8,9,29,28;,
        4;9,10,30,29;,
        4;10,11,31,30;,
        4;11,12,32,31;,
        4;12,13,33,32;,
        4;13,14,34,33;,
        4;14,15,35,34;,
        4;15,16,36,35;,
        4;16,17,37,36;,
        4;17,18,38,37;,
        4;18,19,39,38;,
        4;19,20,40,39;,
        4;20,1,21,40;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;0,0,0;,
        3;41,21,22;,
        3;41,22,23;,
        3;41,23,24;,
        3;41,24,25;,
        3;41,25,26;,
        3;41,26,27;,
        3;41,27,28;,
        3;41,28,29;,
        3;41,29,30;,
        3;41,30,31;,
        3;41,31,32;,
        3;41,32,33;,
        3;41,33,34;,
        3;41,34,35;,
        3;41,35,36;,
        3;41,36,37;,
        3;41,37,38;,
        3;41,38,39;,
        3;41,39,40;,
        3;41,40,21;;
      }
      MeshTextureCoords {
        82;
        0.000000;0.000000;
        0.050000;0.000000;
        0.050000;1.000000;
        0.000000;1.000000;
        0.100000;0.000000;
        0.100000;1.000000;
        0.150000;0.000000;
        0.150000;1.000000;
        0.200000;0.000000;
        0.200000;1.000000;
        0.250000;0.000000;
        0.250000;1.000000;
        0.300000;0.000000;
        0.300000;1.000000;
        0.350000;0.000000;
        0.350000;1.000000;
        0.400000;0.000000;
        0.400000;1.000000;
        0.450000;0.000000;
        0.450000;1.000000;
        0.500000;0.000000;
        0.500000;1.000000;
        0.550000;0.000000;
        0.550000;1.000000;
        2.200;0.000000;
        2.200;1.000000;
        0.650000;0.000000;
        0.650000;1.000000;
        0.700000;0.000000;
        0.700000;1.000000;
        0.750000;0.000000;
        0.750000;1.000000;
        0.800000;0.000000;
        0.800000;1.000000;
        0.850000;0.000000;
        0.850000;1.000000;
        0.900000;0.000000;
        0.900000;1.000000;
        0.950000;0.000000;
        0.950000;1.000000;
        1.000000;0.000000;
        1.000000;1.000000;
        0.025000;0.000000;
        0.075000;0.000000;
        0.125000;0.000000;
        0.175000;0.000000;
        0.225000;0.000000;
        0.275000;0.000000;
        0.325000;0.000000;
        0.375000;0.000000;
        0.425000;0.000000;
        0.475000;0.000000;
        0.525000;0.000000;
        0.575000;0.000000;
        0.625000;0.000000;
        0.675000;0.000000;
        0.725000;0.000000;
        0.775000;0.000000;
        0.825000;0.000000;
        0.875000;0.000000;
        0.925000;0.000000;
        0.975000;0.000000;
        0.025000;1.000000;
        0.075000;1.000000;
        0.125000;1.000000;
        0.175000;1.000000;
        0.225000;1.000000;
        0.275000;1.000000;
        0.325000;1.000000;
        0.375000;1.000000;
        0.425000;1.000000;
        0.475000;1.000000;
        0.525000;1.000000;
        0.575000;1.000000;
        0.625000;1.000000;
        0.675000;1.000000;
        0.725000;1.000000;
        0.775000;1.000000;
        0.825000;1.000000;
        0.875000;1.000000;
        0.925000;1.000000;
        0.975000;1.000000;;
      }
    }
  }
  
  Frame frFloor{
    FrameTransformMatrix {
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, -1.0, 0.0, 1.000000;;
    }
    Mesh {
      24;
      -5.0; 0.0; 5.0;,   5.0; 0.0; 5.0;,   5.0; 0.0;-5.0;,  -5.0; 0.0;-5.0;,
      -5.0; 0.0;-5.0;,   5.0; 0.0;-5.0;,   5.0;-1.0;-5.0;,  -5.0;-1.0;-5.0;,
       5.0; 0.0; 5.0;,   5.0;-1.0; 5.0;,   5.0;-1.0;-5.0;,   5.0; 0.0;-5.0;,
      -5.0;-1.0; 5.0;,   5.0;-1.0; 5.0;,   5.0; 0.0; 5.0;,  -5.0; 0.0; 5.0;,
      -5.0;-1.0; 5.0;,  -5.0; 0.0; 5.0;,  -5.0; 0.0;-5.0;,  -5.0;-1.0;-5.0;,
      -5.0;-1.0;-5.0;,   5.0;-1.0;-5.0;,   5.0;-1.0; 5.0;,  -5.0;-1.0; 5.0;;
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
          0.70;0.70;0.70;1.00;;
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
  
  Frame frWarpDetector{
    FrameTransformMatrix {
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, -4.0, 0.0, 1.000000;;
    }
    Mesh {
      24;
      -10.0; 0.0; 10.0;,   10.0; 0.0; 10.0;,
       10.0; 0.0;-10.0;,  -10.0; 0.0;-10.0;,
      -10.0; 0.0;-10.0;,   10.0; 0.0;-10.0;,   
       10.0;-1.0;-10.0;,  -10.0;-1.0;-10.0;,
       10.0; 0.0; 10.0;,   10.0;-1.0; 10.0;,   
       10.0;-1.0;-10.0;,   10.0; 0.0;-10.0;,
      -10.0;-1.0; 10.0;,   10.0;-1.0; 10.0;,  
       10.0; 0.0; 10.0;,  -10.0; 0.0; 10.0;,
      -10.0;-1.0; 10.0;,  -10.0; 0.0; 10.0;,  
      -10.0; 0.0;-10.0;,  -10.0;-1.0;-10.0;,
      -10.0;-1.0;-10.0;,   10.0;-1.0;-10.0;,  
       10.0;-1.0; 10.0;,  -10.0;-1.0; 10.0;;
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
          0.70;0.70;0.70;0.00;;
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

  Camera  {
    1.000000,0.000000,0.000000,0.000000,    # 視点行列
    0.000000,0.852045,-0.523465,0.000000,   #
    0.000000,0.523466,0.852046,0.000000,    #
    0.000000,-1.000000,8.416400,1.000000;; #
    0.400000;0.400000;        # スクリーンの幅，スクリーンの高さ
    0.000000;0.000000;        # カメラからのスクリーンのずれ(X,Y) 
    0.400000;4000.000000;     # カメラとクリッピング平面の距離(front,back)
  }
}
