xof 0302txt 0064

Frame frTop {

  FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,
    0.000000,1.000000,0.000000,0.000000,
    0.000000,0.000000,1.000000,0.000000,
    0, 0, 0, 1.000000;;
  }

  Frame frPointer{                          # とんかちフレーム
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 0.0, 0.0, 1.000000;;             # とんかちの位置
    }                                       
    Import{"./hammer.xi";}                  # とんかちのメッシュの読み込み
  }
  Frame frBody1{
    FrameTransformMatrix{                   # 一番下の胴体
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 0.5, 3.01, 1.000000;;            # 位置
    }
    Import{"./body1.xi";}                   # メッシュの読み込み
  }
  Frame frBody2{                            # 二番目の胴体
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 1.5, 3.0, 1.000000;;             # 位置
    }
    Import{"./body2.xi";}                   # メッシュの読み込み
  }
  Frame frBody3{                            # 三番目の胴体
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 2.5, 3.01, 1.000000;;            # 位置
    }
    Import{"./body3.xi";}                   # メッシュの読み込み
  }
  Frame frHeadNormal{                       # 普通の顔
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, 3.8, 3.0, 1.000000;;             # 位置
    }
    Import{"./head_normal.xi";}             # メッシュの読み込み
  }
  Frame frHeadSmile{                        # 笑顔
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      60.0, 4.2, 10.0, 1.000000;;           # 位置(遠くに配置)
    }
    Import{"./head_smile.xi";}
  }
  Frame frHeadSad{                          # 泣き顔
    FrameTransformMatrix{
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      50.0, 3.2, 10.01, 1.000000;;          # 位置(遠くに配置)
    }
    Import{"./head_sad.xi";}                # メッシュの読み込み
  }
  Frame frFloor{                            # 床
    FrameTransformMatrix {
      1.000000,0.000000,0.000000,0.000000,
      0.000000,1.000000,0.000000,0.000000,
      0.000000,0.000000,1.000000,0.000000,
      0.0, -1.0, 0.0, 1.000000;;            # 位置
    }
    Import{"./floor.xi";}                   # メッシュの読み込み
  }
}
