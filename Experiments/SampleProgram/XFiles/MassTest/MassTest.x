xof 0303txt 0032

//回転行列は、
//xx xy xz
//yx yy yz
//zx zy zz
//回転方向は、軸に向かって時計周り

Scene {

############################################################
###############  　 ソリッド設定　　 #######################
############################################################

	SolidContainer{
		//ボディ
		Solid soBox{
			0.28;                # 質量
			0.1, 0.0, 0.0,       # 慣性テンソル(3×3)
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;     # 初期速度
			0.0; 0.0; 0.0;;     # 初期角速度
			0.0; 0.0; 0.0;;     # 重心（メッシュの原点からの相対位置）
			{frBox}            # フレームへの参照
		}
	}#~SolidContainer


############################################################
###############  　 重力エンジン　　 #######################
############################################################

	GravityEngine{
		0.0; -9.8; 0.0;;      # 重力をはたらかせる方向
		{soBox}              # 重力をはたらかせる剛体
	}
	
############################################################
###############  　 接触エンジン　　 #######################
############################################################

	ContactEngine{
		{soBox}
		{frFloor}
	}

############################################################
###############  　 カメラ設定　　　 #######################
############################################################
	Camera  {
		1.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.958496, 0.285103, 0.000000,
		0.000000, 0.285104, 0.958497, 0.000000,
		#0.0,0.0,-1.0,0.0,
		#0.0,-1.0,0.0,0.0,
		0.0, -10.0, 40.2, 1.0;;
		#1.000000,0.000000,0.000000,0.000000,
		#0.000000,0.397708,-0.917512,0.000000,
		#0.000000,0.917512,0.397708,0.000000,
		#0.000000,0.000125,42.416588,1.000000;;
		0.400000;
		0.400000;
		0.000000;
		0.000000;
		0.400000;
		4000.000000;
	}
	
############################################################
###############  　 ライト設定　　　 #######################
############################################################
	Frame frLight {
		FrameTransformMatrix  {
			0.707107,0.000000,-0.707107,0.000000,
			0.500000,0.707107,0.500000,0.000000,
			0.500000,-0.707107,0.500000,0.000000,
			0.000000,0.000000,0.000000,1.00000;;
		}
	}
	Light8{
		3;                                            # Type of light source
		0.900000;0.900000;0.900000;0.900000;;         # Diffuse color of light(RGBA)
		0.900000;0.900000;0.900000;0.900000;;         # Specular color of light(RGBA)
		0.500000;0.500000;0.500000;0.500000;;         # Ambient color of light(RGBA)
		0.000000;0.000000;0.000000;;                  # Position in world space
		-0.3 ;-0.3 ; -0.3;;                           # Direction in world space
		20.000000;                                    # Cutoff range
		1.000000;                                     # Falloff
		0.800000;                                     # Constant attenuation
		0.800000;                                     # Linear attenuation
		0.800000;                                     # Quadratic attenuation
		0.300000;                                     # Inner angle of spotlight cone
		0.330000;                                     # Outer angle of spotlight cone
		{ frLight }                                   # フレームへの参照
	}

	Frame frBox {
		FrameTransformMatrix  {
		  1.000000, 0.000000, 0.000000, 0.000000,
		  0.000000, 1.000000, 0.000000, 0.000000,
		  0.000000, 0.000000, 1.000000, 0.000000,
		  0.000000, 0.000000, 0.000000, 1.000000;;
		}
		#Import{"test2.x";}
		#Import{"MassCenterMove.x";}
		Import{"arr.x";}
	}
	#フロアー
	Frame frFloor {
		FrameTransformMatrix {
		  1.000000, 0.000000, 0.000000, 0.000000,
		  0.000000, 1.000000, 0.000000, 0.000000,
		  0.000000, 0.000000, 1.000000, 0.000000,
		  0.000000, 0.000000, 0.000000, 1.000000;;
		}
		Import{"floor.x";}
	}

}#~Scene
