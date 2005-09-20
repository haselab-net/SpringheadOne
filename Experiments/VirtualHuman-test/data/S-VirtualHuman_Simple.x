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
		Solid soBody{
			0.28;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBody}
		}
	}#~SolidContainer

############################################################
###############  　 関節エンジン　　 #######################
############################################################

JointEngine{
	{frFloor}
	Joint joWaist1{ 	#屈曲・伸展 -Ｘ方向
		0; #関節の種類	0:蝶番, 1:スライド
		0.0; 0.5; 0.0;; #腰座標での関節の位置
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; -0.3; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.1; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
#		0; 0.0; 0.0; 0.0; 0.0; #位置入力の例
		3; 0.0; 10.0; 0.0; 0.0; #位置入力の例
		{soBody}
	}
}#~JointEngine

############################################################
###############  　 重力エンジン　　 #######################
############################################################

	GravityEngine{
		0.0; -9.8; 0.0;;
		{soBody}
	}
	
############################################################
###############  　 接触エンジン　　 #######################
############################################################

#	ContactEngine{
#		{soBody}
#		{frFloor}
#	}

############################################################
###############  　 カメラ設定　　　 #######################
############################################################
	Camera  {
		1.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.958496, 0.285103, 0.000000,
		0.000000, 0.285104, 0.958497, 0.000000,
		0.0, -1.0, 4.2, 1.0;;
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
		3;
		0.900000;0.900000;0.900000;0.900000;;
		0.900000;0.900000;0.900000;0.900000;;
		0.500000;0.500000;0.500000;0.500000;;
		0.000000;0.000000;0.000000;;
		-0.3 ;-0.3 ; -0.3;;
		20.000000;
		1.000000;
		0.800000;
		0.800000;
		0.800000;
		0.300000;
		0.330000;
		{ frLight }
	}


############################################################
###############   フレームインポート #######################
############################################################

	#基準となるフレーム
	#体（胸）
	Frame frBody {
		Import{"S-VH-Model/Body.x";}
	}
	#フロアー
	Frame frFloor {
		FrameTransformMatrix {
		  1.000000, 0.000000, 0.000000, 0.000000,
		  0.000000, 1.000000, 0.000000, 0.000000,
		  0.000000, 0.000000, 1.000000, 0.000000,
		  0.000000, 0.000000, 0.000000, 1.000000;;
		}
		Import{"S-VH-Model/floor.x";}
	}

}#~Scene
