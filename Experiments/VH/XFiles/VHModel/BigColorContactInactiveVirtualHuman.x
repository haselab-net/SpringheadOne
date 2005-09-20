xof 0303txt 0032

//回転行列は、
//xx xy xz
//yx yy yz
//zx zy zz
//回転方向は、軸に向かって時計周り

Scene {

	Simulator{ 0.001; }

############################################################
###############  　 ソリッド設定　　 #######################
############################################################

	SolidContainer{
		#腰
		Solid soWaist{
			0.17;                # 質量比
			0.1, 0.0, 0.0,       # 慣性テンソル(3×3)
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;     # 初期速度
			0.0; 0.0; 0.0;;     # 初期角速度
			0.0; 0.0; 0.0;;     # 重心（メッシュの原点からの相対位置）
			{frWaist}            # フレームへの参照
		}
		
		#腹
		Solid soAbdomen{
			0.028;                # 質量比
			0.1, 0.0, 0.0,       # 慣性テンソル(3×3)
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;     # 初期速度
			0.0; 0.0; 0.0;;     # 初期角速度
			0.0; 0.0; 0.0;;     # 重心（メッシュの原点からの相対位置）
			{frAbdomen}            # フレームへの参照
		}

		#胸
		Solid soChest{
			0.252;                # 質量比
			0.1, 0.0, 0.0,       # 慣性テンソル(3×3)
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;     # 初期速度
			0.0; 0.0; 0.0;;     # 初期角速度
			0.0; 0.0; 0.0;;     # 重心（メッシュの原点からの相対位置）
			{frChest}            # フレームへの参照
		}
		#頭
		Solid soHead{
			0.07;                # 質量比
			0.1, 0.0, 0.0,       # 慣性テンソル(3×3)
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;     # 初期速度
			0.0; 0.0; 0.0;;     # 初期角速度
			0.0; 0.0; 0.0;;     # 重心（メッシュの原点からの相対位置）
			{frHead}            # フレームへの参照
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
#***************** 下半身 *****************************
		#右大腿
		Solid soRThigh{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRThigh}
		}
		#右下腿
		Solid soRLThigh{
			0.05;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRLThigh}
		}
		#右踵
		Solid soRHeel{
			0.015;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRHeel}
		}
		#右爪先
		Solid soRToe{
			0.005;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frRToe}
		}
		#左大腿
		Solid soLThigh{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLThigh}
		}
		#左下腿
		Solid soLLThigh{
			0.05;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLLThigh}
		}
		#左踵
		Solid soLHeel{
			0.015;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLHeel}
		}
		#左爪先
		Solid soLToe{
			0.005;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLToe}
		}

		#テスト
		Solid soTest{
			0.17;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frTest}
		}
		Solid Pointer1{
			0.25;
		   	1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
		  	0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
		    {frPointer1}
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
	#腰-腹
	Joint joWaist1{ 	#屈曲・伸展 Ｘ方向
		0; #関節の種類	0:蝶番, 1:スライド
		0.0; 0.0384; 0.0;; #腰座標での関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.0; 0.0; 0.0; 0.0;	#位置入力の例
		Joint joWaist2{ #外・内転 +Ｚ方向
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
			3; 0.0; 0.0; 0.0; 0.0;
			Joint joWaist3{ #内・外旋 +Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; -0.0101; 0.0;; #腹座標での関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soAbdomen}

#************　胸関節 ***************
	#腹-胸
	Joint joChest1{ 	#屈曲・伸展 Ｘ方向
		0; #関節の種類	0:蝶番, 1:スライド
		0.0; 0.0101; 0.0;; #腹座標での関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.0; 0.0; 0.0; 0.0;	#位置入力の例
		Joint joChest2{ #外・内転 Ｚ方向
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
			3; 0.0; 0.0; 0.0; 0.0;
			Joint joChest3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; -0.1010; 0.0;; #胸座標での関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soChest}

#************　首関節 ***************
	#胸-頭
	Joint joNeck1{ #屈曲・伸展 Ｘ方向
		0;
		0.0; 0.1278; 0.0;; #胸座標での関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
		Joint joNeck2{ #外・内転 Ｚ方向
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
			3; 0.0; 0.0; 0.0; 0.0;
			Joint joNeck3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; -0.0968; 0.0;; #頭座標での関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soHead}
			} #End joNeck3
		} #End joNeck2
	} #End joNeck1

#************　右肩関節 ***************

	#胸-右上腕
	Joint joRShoulder1{#屈曲・伸展 -Ｘ方向
		0;
		0.116; 0.1010; 0.0;; #胸座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
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
			3; 0.0; 0.0; 0.0; 0.0;
			Joint joRShoulder3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0905; 0.0;; #右上腕座標での関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soRUArm}


#************　右肘関節 ***************
	#右上腕-右前腕
	Joint joRElbow1{#屈曲・伸展 -Ｘ方向
		0;
		0.0; -0.0905; 0.0;; #右上腕座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0, 0.0, 0.0;; 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 00.0; 0.0; 0.0;

		Joint joRElbow2{ #内・外旋-Ｙ方向
				0;
				0.0,0.0,0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0705; 0.0;; #右前腕座標での関節の位置
				0.0, 0.0, 1.0,
				-1.0, 0.0, 0.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soRFArm}

#************　右手首関節 ***************

	#右前腕-右手
	Joint joRWrist1{#屈曲・伸展 -Z方向
		0;
		0.0; -0.0705; 0.0;; #右前腕座標での関節の位置
		-1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, -1.0;;
		0.0, 0.0, 0.0;; 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;

		Joint joRWrist2{ #内・外旋-X方向
				0;
				0.0,0.0,0.0;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0530; 0.0;; #右手座標での関節の位置
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				-1.0, 0.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soRHand}
		}#End joRWrist2
	}#End joRWrist1


		}#End joRElbow2
	}#End joRElbow1
	
			}#End joRShoulder3
		}#End joRShoulder2
	}#End joRShoulder1

#************　左肩関節 ***************
	#胸ー左上腕
	Joint joLShoulder1{#屈曲・伸展 -Ｘ方向
		0;
		-0.116; 0.1010; 0.0;; #胸座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0; 0.0; 0.0;;
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
		
		Joint joLShoulder2{ #外・内転 -Ｚ方向
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
			3; 0.0; 0.0; 0.0; 0.0;
			
			Joint joLShoulder3{ #内・外旋Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0905; 0.0;; #右上腕座標での関節の位置
				-1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soLUArm}

#************　左肘関節 ***************

	#左上腕-左前腕
	Joint joLElbow1{#屈曲・伸展 -Ｘ方向
		0;
		0.0; -0.0905; 0.0;; #右上腕座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0, 0.0, 0.0;; 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;

		Joint joLElbow2{ #内・外旋-Ｙ方向
				0;
				0.0,0.0,0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0705; 0.0;; #右前腕座標での関節の位置
				0.0, 0.0, 1.0,
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soLFArm}

#************　左手首関節 ***************

	#左前腕ー左手
	Joint joLWrist1{#屈曲・伸展 Z方向
		0;
		0.0; -0.0705; 0.0;; #左前腕座標での関節の位置
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0, 0.0, 0.0;; 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;

		Joint joLWrist2{ #内・外旋-X方向
				0;
				0.0,0.0,0.0;;
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				-1.0, 0.0, 0.0;;
				0.0; 0.0530; 0.0;; #左手座標での関節の位置
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				-1.0, 0.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soLHand}

		}#End joLWrist2
	}#End joLWrist1

		}#End joLElbow-Y
	}#End joLElbow-X

			}#End joLShoulder-Y
		}#End joLShoulderZ
	}#End joLShoulder-X

			}#End joChest-Y
		}#End joChestZ
	}#End joChest-X

			}#End joWaist-Y
		}#End joWaistZ
	}#End joWaist-X

##################### 　 下半身    ########################

#************　右股関節 ***************

	#腰-右大腿
	Joint joRHip1{#屈曲・伸展 -X方向
		0;
		0.057;-0.0384;0.0;; #腰座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0, 0.0, 0.0;; 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
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
			3; 0.0; 0.0; 0.0; 0.0;
			Joint joRHip3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.1165; 0.0;;	#右大腿座標における関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soRThigh}

#************　右膝関節 ***************
	
	#右大腿-右下腿
	Joint joRKnee{
		0;
		0.0;-0.1165;0.0;;
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0, 0.1165, 0.0;; 
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
		{soRLThigh}
	

#************　右足首関節 ***************

	#右下腿-右踵
	Joint joRAnkle1{#屈曲・伸展 -X方向
		0;
		0.0;-0.1165;0.0;; #右下腿座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0, 0.0, 0.0;; 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
		Joint joRAnkle2{ #外・内転 Ｚ方向
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
			3; 0.0; 0.0; 0.0; 0.0;
			Joint joRAnkle3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0204; -0.0034;;	#右踵座標における関節の位置
				#0.0; 0.0204; -0.0234;;	#右踵座標における関節の位置
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, -1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soRHeel}

#************　右土踏まず関節 ***************
	#右踵-右爪先
	Joint joRToe{ #X方向
		0;
		0.0;0.0;0.0471;;
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0, 0.006, -0.0271;; #右爪先座標における関節の位置
		#0.0, 0.0, -0.0271;; 
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
		{soRToe}
	} #End joRToe

			} #End joRAnkle-Y
		} #End joRAnkleZ
	} #End joRAnkle-X

	} #End joKnee

			} #End joRHip-Y
		} #End joRHipZ
	} #End joRHip-X

#************　左股関節 ***************
	#腰-左大腿
	Joint joLHip1{#屈曲・伸展 -X方向
		0;
		-0.057;-0.0384;0.0;; #腰座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0, 0.0, 0.0;; 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
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
			3; 0.0; 0.0; 0.0; 0.0;
			Joint joLHip3{ #内・外旋-Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.1165; 0.0;;	#右大腿座標における関節の位置
				-1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soLThigh}

#************　左膝関節 ***************
	#左大腿-左下腿
	Joint joLKnee{
		0;
		0.0;-0.1165;0.0;;
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0, 0.1165, 0.0;; 
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
		{soLLThigh}

#************　左足首関節 ***************
	#左下腿-左踵
	Joint joLAnkle1{#屈曲・伸展 -X方向
		0;
		0.0;-0.1165;0.0;; #左下腿座標での関節の位置
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		-1.0, 0.0, 0.0;;
		0.0, 0.0, 0.0;; 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
		Joint joLAnkle2{ #外・内転 Ｚ方向
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
			3; 0.0; 0.0; 0.0; 0.0;
			Joint joLAnkle3{ #内・外旋Ｙ方向
				0;
				0.0, 0.0, 0.0;;
				1.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0204; -0.0034;;	#左踵座標における関節の位置
				#0.0; 0.0204; -0.0234;;	#左踵座標における関節の位置
				-1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0;;
				0.0; 0.0; 20000000.0; 0.0; 0.0;	
				3; 0.0; 0.0; 0.0; 0.0;
				{soLHeel}

#************　左土踏まず関節 ***************
	#左踵-左爪先
	Joint joLToe{ #X方向
		0;
		0.0;0.0;0.0471;; #左踵座標における関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0, 0.006, -0.0271;; #左爪先座標における関節の位置
		#0.0, 0.0, -0.0271;; #左爪先座標における関節の位置
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0;;
		0.0; 0.0; 20000000.0; 0.0; 0.0;	
		3; 0.0; 0.0; 0.0; 0.0;
		{soLToe}
	} #End joLToe

			} #End joLAnkle-Y
		} #End joLAnkleZ
	} #End joLAnkle-X


	} #End joLKnee
			} #End joLHip-Y
		} #End joLHipZ
	} #End joLHip-X

}#End JointEngine
############################################################
###############  　 重力エンジン　　 #######################
############################################################
	GravityEngine{
		0.0; -9.8; 0.0;;	# 重力をはたらかせる方向
		#0.0;-3.0;0.0;;
		{soWaist}	# 重力をはたらかせる剛体
		{soAbdomen}
		{soChest}
		{soHead}

		{soRUArm}
		{soRFArm}
		{soRHand}

		{soLUArm}
		{soLFArm}
		{soLHand}

		{soRThigh}
		{soRLThigh}
		{soRHeel}
		{soRToe}

		{soLThigh}
		{soLLThigh}
		{soLHeel}
		{soLToe}

	}

############################################################
###############  　 接触エンジン　　 #######################
############################################################

	ContactEngine{
		{soWaist}	#0
		{soAbdomen}	#1
		{soChest}	#2
		{soHead}	#3

		{soRUArm}	#4
		{soRFArm}	#5
		{soRHand}	#6

		{soLUArm}	#7
		{soLFArm}	#8
		{soLHand}	#9

		{soRThigh}	#10
		{soRLThigh}	#11
		{soRHeel}	#12
		{soRToe}	#13

		{soLThigh}	#14
		{soLLThigh}	#15
		{soLHeel}	#16
		{soLToe}	#17

		{frFloor}
		{Pointer1}
		
		ContactInactive{	#床としか接触しない
			18;
			0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17;
		}
	}



############################################################
###############  　 カメラ設定　　　 #######################
############################################################
	Camera  {
		1.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.958496, 0.285103, 0.000000,
		0.000000, 0.285104, 0.958497, 0.000000,
		0.0, -1.0, 4.2, 1.0;;
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


############################################################
###############   フレームインポート #######################
############################################################

	#腰（基準となるフレーム）
	Frame frWaist {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,1.0,0.0,1.0;; #腰の初期位置（基準の位置）
		}
		Import{"ColorParts/CWaist.x";}
	}

	#腹
	Frame frAbdomen {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CAbdomen.x";}
	}

	#胸
	Frame frChest {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CChest.x";}
	}

	#頭
	Frame frHead {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CHead.x";}
	}

	#右上腕
	Frame frRUArm {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CUpperArm.x";}
	}
	#右前腕
	Frame frRFArm {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CFrontArm.x";}
	}
	#右手
	Frame frRHand {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CHand.x";}
	}

	#左上腕
	Frame frLUArm {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CUpperArm.x";}
	}
	#左前腕
	Frame frLFArm {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CFrontArm.x";}
	}
	#左手
	Frame frLHand {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CHand.x";}
	}

#*******************　下半身　*************************

	#*************　右下半身　***************
	#右大腿
	Frame frRThigh {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CThigh.x";}
	}
	#右下腿
	Frame frRLThigh {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CLowerThigh.x";}
	}
	#右踵
	Frame frRHeel {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		#Import{"ColorParts/CHeel.x";}
		Import{"ColorParts/BigHeel.x";}
	}
	#右爪先
	Frame frRToe {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		#Import{"ColorParts/CToe.x";}
		Import{"ColorParts/BigToe.x";}
	}

	#*************　左下半身　***************
	#左大腿
	Frame frLThigh {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CThigh.x";}
	}
	#左下腿
	Frame frLLThigh {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		Import{"ColorParts/CLowerThigh.x";}
	}
	#左踵
	Frame frLHeel {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		#Import{"ColorParts/CHeel.x";}
		Import{"ColorParts/BigHeel.x";}
	}

	#左爪先
	Frame frLToe {
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0;;
		}
		#Import{"ColorParts/CToe.x";}
		Import{"ColorParts/BigToe.x";}
	}

	#フロアー
	Frame frFloor {
		FrameTransformMatrix {
		  1.000000, 0.000000, 0.000000, 0.000000,
		  0.000000, 1.000000, 0.000000, 0.000000,
		  0.000000, 0.000000, 1.000000, 0.000000,
		  0.000000, 0.000000, 0.000000, 1.000000;;
		}
		Import{"ColorParts/floor.x";}
	}

	#位置テスト
	Frame frTest {
		FrameTransformMatrix {
		  1.000000, 0.000000, 0.000000, 0.000000,
		  0.000000, 1.000000, 0.000000, 0.000000,
		  0.000000, 0.000000, 1.000000, 0.000000,
		  0.000000, 0.000000, 0.000000, 1.000000;;
		}
		#Import{"ball.x";}
	}
	Frame frPointer1{
		FrameTransformMatrix  {
			1.000000,0.000000,0.000000,0.000000,
			0.000000,1.000000,0.000000,0.000000,
			0.000000,0.000000,1.000000,0.000000,
			0.300000,0.200000,0.000000,1.000000;;
		}
		Sphere {
			0.05; 8; 8;
			Material {
				0.019608;1.000000;0.721569;1.000000;;
				22.000000;
				0.430000;0.430000;0.430000;;
				0.004902;0.250000;0.180392;;
			}
		}
	}

}#~Scene
