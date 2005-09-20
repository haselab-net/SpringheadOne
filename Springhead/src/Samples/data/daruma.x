xof 0302txt 0064

Scene {
	Simulator{
		0.002;
	}
  SolidContainer{
    Solid Pointer{
      0.25;             # 質量
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,    # 慣性テンソル
      0.0, 0.0, 1.0;;
      0.0; 0.0; 0.0;;   # 初期速度
      0.0; 0.0; 0.0;;   # 初期角速度
      0.0; 0.0; 0.0;;   # 重心（メッシュの原点からの相対位置）
      {frPointer}       # フレームへの参照
                        # (複数のフレームをツリーとして持つ場合は
                        # トップフレームを指す）
    }
    Solid soBody1{
      1.0;
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      {frBody1}
    }
    Solid soBody2{
      1.0;
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      {frBody2}
    }
    Solid soBody3{
      1.0;
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      {frBody3}
    }
    Solid soHeadNormal{
      2.0;
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      {frHeadNormal}
    }
    Solid soHeadSmile{
      2.0;
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      {frHeadSmile}
    }
    Solid soHeadSad{
      2.0;
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.0; 0.0;;
      {frHeadSad}
    }
  }
  GravityEngine{
    0.0; -49.0; 0.0;;    # 加える加速度(x,y,z方向)
    {soBody1}           # 重力をはたらかせる剛体(solidの名前で指定する)
    {soBody2}
    {soBody3}
    {soHeadNormal}
    {soHeadSmile}
    {soHeadSad}
  }
  ContactEngine{
    {frFloor}           # 衝突判定をする物体を指定する
    {Pointer}           # 床のような動かない物体はFrameの名前で
    {soBody1}           # 衝突したときに反力を受ける物体はSolidの名前で指定する
    {soBody2}           # ここで指定された物体達は、すべての組み合わせで衝突判定するようになる
    {soBody3}
    {soHeadNormal}
    {soHeadSmile}
    {soHeadSad}
  }
  ChangeObjectContainer{
    ChangeObjectOrientation{
      0.0; 1.0; 0.0;;   #軸選択(とる値は1.0or0.0)　この場合y軸
      1.0, 0.0, 0.0,    #　　　　　　　　←今はy軸が選択されているのでこの行は処理には関係なし
      0.0, 1.0, 0.0,    #比較用姿勢行列　←この場合y軸(上の軸選択より)がベクトル(0,1,0)とどういう関係になっているか比較する
      0.0, 0.0, 1.0;;   #　　　　　　　　←今はy軸が選択されているのでこの行は処理には関係なし
      0.0; 0.0; 0.0;;   #目標内積値(x,y,z)．軸選択で選んだ軸と比較用姿勢行列で設定した軸とどういう関係になっているかを指定する．1.0:一致，0.0:垂直，-1.0:向きが正反対　
      0.0; 0.3; 0.0;;   #精度(x,y,z)．目標内積値からいくつ離れたところまで許容するかを指定
      {soHeadNormal}    #入れ替え前のSolid
      {soHeadSad}       #入れ替え後のSolid
      {frFloor}         #比較する物体(これはなくてもよく，ない場合は比較用姿勢行列と比較する)
    }
    ChangeObjectOrientation{
      0.0; 1.0; 0.0;;
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0;;
      0.0; 0.0; 0.0;;
      0.0; 0.3; 0.0;;
      {soHeadSmile}
      {soHeadSad}
      {frFloor}
    }
    ChangeObjectCollision{
      {frFloorSmall}    #検出用フレーム(Solidでも可)
      {soHeadNormal}    #入れ替え前のSolid
      {soHeadSmile}     #入れ替え後のSolid
    }
    ChangeObjectCollision{
      {frFloor}
      {soHeadNormal}
      {soHeadSad}
    }
  }
}

Frame frTop {

  FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,  # フレーム(子フレームを含む)の
    0.000000,1.000000,0.000000,0.000000,  # 初期位置(ローカル座標系)
    0.000000,0.000000,1.000000,0.000000,
    0, 0, 0, 1.000000;;
  }

  Frame frLight {
    FrameTransformMatrix  {
      0.707107,0.000000,-0.707107,0.000000,
      0.500000,0.707107,0.500000,0.000000,
      0.500000,-0.707107,0.500000,0.000000,
      0.000000,0.000000,0.000000,1.00000;;
    }
  }
  Light8{
    3;                                    # Type of light source
    1.0;1.0;1.0;1.0;;                     # Diffuse color of light(RGBA)
    0.900000;0.900000;0.900000;0.900000;; # Specular color of light(RGBA)
    0.000000;0.000000;0.000000;0.000000;; # Ambient color of light(RGBA)
    0.000000;0.000000;0.000000;;          # Position in world space
    -0.3 ;-0.3 ; -0.3;;                   # Direction in world space
    20.000000;                            # Cutoff range
    1.000000;                             # Falloff
    0.800000;                             # Constant attenuation
    0.800000;                             # Linear attenuation
    0.800000;                             # Quadratic attenuation
    0.300000;                             # Inner angle of spotlight cone
    0.330000;                             # Outer angle of spotlight cone
    { frLight }                           # フレームへの参照
  }

  Frame frPointer{                        # とんかち
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 0.0, 0.0, 1.000000;;
    }
    Import{"./parts/daruma/hammer.xi";}
  }
  Frame frBody1{                          # 胴体１
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 0.5, 3.01, 1.000000;;
    }
    Import{"./parts/daruma/body1.xi";}
  }
  Frame frBody2{                          # 胴体２
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 1.5, 3.0, 1.000000;;
    }
    Import{"./parts/daruma/body2.xi";}
  }
  Frame frBody3{                          # 胴体３
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 2.5, 3.01, 1.000000;;
    }
    Import{"./parts/daruma/body3.xi";}
  }
  Frame frHeadNormal{                     # 普通の顔
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 3.8, 3.0, 1.000000;;
    }
    Import{"./parts/daruma/head_normal.xi";}
  }
  Frame frHeadSmile{                      # 笑顔
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      60.0, 4.2, 10.0, 1.000000;;
    }
    Import{"./parts/daruma/head_smile.xi";}
  }
  Frame frHeadSad{                        # 泣き顔
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      50.0, 3.2, 10.01, 1.000000;;
    }
    Import{"./parts/daruma/head_sad.xi";}
  }
  Frame frFloor{                          # 床
    FrameTransformMatrix {
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, -1.0, 0.0, 1.000000;;
    }
    Import{"./parts/daruma/floor.xi";}
  }
  Frame frFloorSmall{                     # 入れ替え処理に使う小さな床
    FrameTransformMatrix {
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, -0.99, 3.0, 1.000000;;
    }
    Import{"./parts/daruma/floorSmall.xi";}
  }

  Camera  {
    1.000000,0.000000,0.000000,0.000000,    #
    0.000000,0.958496,-0.285103,0.000000,   # 視点行列
    0.000000,0.285104,0.958496,0.000000,    #
    0.000000,-1.000000,10.416403,1.000000;; #
    0.400000;0.400000;                      # スクリーンの幅，スクリーンの高さ
    0.000000;0.000000;                      # カメラからのスクリーンのずれ(X,Y) 
    0.400000;4000.000000;                   # カメラとクリッピング平面の距離(front,back)
  }

}
