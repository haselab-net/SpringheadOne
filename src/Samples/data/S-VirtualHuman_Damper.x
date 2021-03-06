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
		#頭
		Solid soHead{
			0.07;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frHead}
		}
		#右上腕
		Solid soRUArm{
			0.04;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRUArm}
		}
		#左上腕
		Solid soLUArm{
			0.04;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLUArm}
		}
		#右前腕
		Solid soRFArm{
			0.02;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRFArm}
		}
		#左前腕
		Solid soLFArm{
			0.02;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLFArm}
		}
		#右手
		Solid soRHand{
			0.01;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRHand}
		}
		#左手
		Solid soLHand{
			0.01;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLHand}
		}
		#腰
		Solid soWaist{
			0.17;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frWaist}
		}
		#右大腿
		Solid soRULeg{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRULeg}
		}
		#左大腿
		Solid soLULeg{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLULeg}
		}
		#右下腿
		Solid soRLLeg{
			0.05;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRLLeg}
		}
		#左下腿
		Solid soLLLeg{
			0.05;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLLLeg}
		}
		#右足
		Solid soRFoot{
			0.02;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRFoot}
		}
		#左足
		Solid soLFoot{
			0.02;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLFoot}
		}
	}#~SolidContainer

############################################################
###############  　 関節エンジン　　 #######################
############################################################

JointEngine{
	#腰を原点とする。
	{soWaist}
##################### 　 上半身    ########################

#************　腰関節 ***************
	#腰-胸
	Joint joWaist1{ 	#屈曲・伸展 -Ｘ方向
		0; #関節の種類	0:蝶番, 1:スライド
		0.0; 0.0; 0.0;; #腰座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.0; 10.0; 0.0; 1.0;	#位置入力の例
		Joint joWaist2{ #外・内転 Ｚ方向
			0;
			0.0, 0.0, 0.0;;
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0,
			1.0, 0.0, 0.0;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			Joint joWaist3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; -0.3; 0.0;; #体座標での関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 10.0; 0.0; 1.0;
				{soBody}

#************　首関節 ***************
	Joint joNeck1{ #屈曲・伸展 -Ｘ方向
		0;
		0.0; 0.3; 0.0;; #体座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;

		Joint joNeck2{ #外・内転 Ｚ方向
			0;
			0.0, 0.0, 0.0;;
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0,
			1.0, 0.0, 0.0;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			Joint joNeck3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; -0.15; 0.0;; #頭座標での関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 10.0; 0.0; 1.0;
				{soHead}
			} #~joNeck3
		} #~joNeck2
	} #~joNeck1


#************　右肩関節 ***************
	#体ー右上腕
	Joint joRShoulder1{#屈曲・伸展 -Ｘ方向
		0;
		0.2; 0.3; 0.0;; #体座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		Joint joRShoulder2{ #外・内転 Ｚ方向
			0;
			0.0, 0.0, 0.0;;
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0,
			1.0, 0.0, 0.0;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			Joint joRShoulder3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.15; 0.0;; #右上腕座標での関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 10.0; 0.0; 1.0;
				{soRUArm}

#************　右肘関節 ***************
	#右上腕-右前腕
	Joint joRElbow1{#屈曲・伸展 -Ｘ方向
		0;
		0.0; -0.15; 0.0;; #右上腕座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.125; 0.0;; #右前腕座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		{soRFArm}

#************　右手首関節 ***************
	#右前腕-右手
	Joint joRWrist1{#屈曲・伸展 -Ｘ方向
		0;
		0.0; -0.125; 0.0;; #右前腕座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		Joint joRWrist2{ #外・内転 Ｚ方向
			0;
			0.0, 0.0, 0.0;;
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0,
			1.0, 0.0, 0.0;;
			0.0; 0.05; 0.0;; #右手座標での関節の位置
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			{soRHand}
			}#~joRWrist2
			}#~joRWrist1
		}#~joRElbow1
	} #~joRShoulder3
	} #~joRShoulder2
	} #~joRShoulder1

#************　左肩関節 ***************
	#体ー左上腕
	Joint joLShoulder1{ #屈曲・伸展 -Ｘ方向
		0;
		-0.2; 0.3; 0.0;;
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		Joint joLShoulder2{ #外・内転 Ｚ方向
			0;
			0.0, 0.0, 0.0;;
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			Joint joLShoulder3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.15; 0.0;;
				-1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 10.0; 0.0; 1.0;
				{soLUArm}

#************　左肘関節 ***************
	#左上腕-左前腕
	Joint joLElbow1{ #屈曲・伸展 -Ｘ方向
		0;
		0.0; -0.15; 0.0;;
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.125; 0.0;;
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		{soLFArm}

#************　左手首関節 ***************
					#左前腕-右手
	Joint joLWrist1{ #屈曲・伸展 -Ｘ方向
		0;
		0.0; -0.125; 0.0;;	#
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		
		Joint joLWrist2{ #外・内転 Ｚ方向
			0;
			0.0, 0.0, 0.0;;
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.05; 0.0;;	#
			-1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, -1.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			{soLHand}
			}#~joLWrist2
			}#~joLWrist1
		}#~joLElbow1
	}#~joLShoulder3
	}#~joLShoulder2
	}#~joLShoulder1

} #~joWaist3
} #~joWaist2
} #~joWaist1


##################### 　 下半身    ########################

#************　右股関節 ***************
	#腰-右大腿
	Joint joRHip1{ #屈曲・伸展 -Ｘ方向
		0; #関節の種類	0:蝶番, 1:スライド
		0.15; 0.0; 0.0;; #腰座標における関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.0; 10.0; 0.0; 1.0;		#位置入力の例
		Joint joRHip2{ #外・内転 Ｚ方向
			0;
			0.0, 0.0, 0.0;;
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0,
			1.0, 0.0, 0.0;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			Joint joRHip3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.2; 0.0;; #右大腿座標における関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 10.0; 0.0; 1.0;
				{soRULeg}

#************　右膝関節 ***************
	#右大腿-右下腿
	Joint joRKnee1{ #屈曲・伸展 Ｘ方向
		0;
		0.0; -0.2; 0.0;; #右大腿座標における関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.2; 0.0;; #右下腿座標における関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		{soRLLeg}

#************　右足首関節 ***************
	#右下腿-右足
	Joint joRShin {
		1;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0,-1.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0,-1.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		Joint joRAnkle1{#背屈・底屈 -Ｘ方向
			0;
			0.0; -0.2; 0.0;; #右下腿座標における関節の位置
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.045; 0.0;; #右足座標における関節の位置
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			{soRFoot}
		} #joRAnkle1
	}
		}#joRKnee1
	}#joRHip3
	}#joRHip2
	}#joRHip1



#************　左股関節 ***************
	#腰-左大腿
	Joint joLHip1{ #屈曲・伸展 -Ｘ方向
		0;
		-0.15; 0.0; 0.0;; #腰座標における関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		Joint joLHip2{ #外・内転 Ｚ方向
			0;
			0.0, 0.0, 0.0;;
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			Joint joLHip3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.2; 0.0;; #左大腿座標における関節の位置
				-1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 10.0; 0.0; 1.0;
				{soLULeg}

#************　左膝関節 ***************
	#左大腿-左下腿
	Joint joLKnee1{ #屈曲・伸展 Ｘ方向
		0;
		0.0; -0.2; 0.0;; #左大腿座標における関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.2; 0.0;; #左下腿座標における関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		{soLLLeg}

#************　左足首関節 ***************
	Joint joLShin {
		1;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0,-1.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0,-1.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 10.0; 0.0; 1.0;
		#左下腿-左足
		Joint joLAnkle1{#背屈・底屈 -Ｘ方向
			0;
			0.0; -0.2; 0.0;; #左下腿座標における関節の位置
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.045; 0.0;; #左足座標における関節の位置
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.0; 20000000.0; 0.0; 0.0;	
			3; 0.0; 10.0; 0.0; 1.0;
			{soLFoot}
		} #joLAnkle1
	}
		}#joLKnee1
	}#joLHip3
	}#joLHip2
	}#joLHip1

}#~JointEngine

############################################################
###############  　 重力エンジン　　 #######################
############################################################

	GravityEngine{
		0.0; -9.8; 0.0;;
		{soBody}
		{soHead}
		{soWaist}
		
		{soRUArm}
		{soRFArm}
		{soRHand}

		{soLUArm}
		{soLFArm}
		{soLHand}

		{soRULeg}
		{soRLLeg}
		{soRFoot}

		{soLULeg}
		{soLLLeg}
		{soLFoot}

	}
	
############################################################
###############  　 接触エンジン　　 #######################
############################################################

	ContactEngine{
#		{soBody}
#		{soHead}
#		{soWaist}
		
#		{soRUArm}
#		{soRFArm}
#		{soRHand}

#		{soLUArm}
#		{soLFArm}
#		{soLHand}

#		{soRULeg}
#		{soRLLeg}
		{soRFoot}

#		{soLULeg}
#		{soLLLeg}
		{soLFoot}

		{frFloor}
#		ContactInactive{
#			15;
#			0,1,2,3,4,5,6,7,8,9,10,11,12,13,14;
#		}
	}

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
	#腰
	Frame frWaist {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,1.0,0.0,1.0;; #腰の初期位置
		}
		Import{"S-VH-Model/Waist.x";}
	}

	#体（胸）
	Frame frBody {
		Import{"S-VH-Model/Body.x";}
	}

	#頭
	Frame frHead {
		Import{"S-VH-Model/Head.x";}
	}

	#右上腕
	Frame frRUArm {
		Import{"S-VH-Model/UArm.x";}
	}
	#左上腕
	Frame frLUArm {
		Import{"S-VH-Model/UArm.x";}
	}
	#右前腕
	Frame frRFArm {
		Import{"S-VH-Model/FArm.x";}
	}
	#左前腕
	Frame frLFArm {
		Import{"S-VH-Model/FArm.x";}
	}
	#右手
	Frame frRHand {
		Import{"S-VH-Model/Hand.x";}
	}
	#左手
	Frame frLHand {
		Import{"S-VH-Model/Hand.x";}
	}

	#右大腿
	Frame frRULeg {
		Import{"S-VH-Model/ULeg.x";}
	}
	#左大腿
	Frame frLULeg {
		Import{"S-VH-Model/ULeg.x";}
	}
	#右下腿
	Frame frRLLeg {
		Import{"S-VH-Model/LLeg.x";}
	}
	#左下腿
	Frame frLLLeg {
		Import{"S-VH-Model/LLeg.x";}
	}
	#右足
	Frame frRFoot {
		Import{"S-VH-Model/Foot.x";}
	}
	#左足
	Frame frLFoot {
		Import{"S-VH-Model/Foot.x";}
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
