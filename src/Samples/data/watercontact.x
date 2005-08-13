xof 0303txt 0032	#DirectXファイルのバージョン表記(なくても動きます)
#	#か//で始まる行がコメントです．
#	Xファイルは左手系，Springheadは右手系なので，ロード・セーブ時に右手系，左手系を入れ替えています．

Scene {
	Simulator{
		0.01;	#	シミュレーションの時間刻み	dt
		0.99;	#	速度の減衰率．1秒間に減衰する割合
	}
	SolidContainer{				#	剛体を入れておくコンテナ
		Solid soBlock1{			#	1つ目の剛体
			1000000000.0;				#	質量
			10000000000.0, 0.0, 0.0,		
			0.0, 500000000000.0, 0.0,		
			0.0, 0.0, 500000000000.0;;	#	慣性モーメント (,=配列の区切り，;=フィールドの区切り)
			0.0; 0.0; 0.0;;		#	初期速度
			0.0; 0.0; 0.0;;		#	初期角速度
			0.0; 0.0; 0.0;;		#	重心の位置
			{frBlock1}			#	位置と形を持つフレームの名前
		}
	}
	GravityEngine{				#	重力
		0.0; -9.8; 0.0;;		#	重力の値
#		{soBlock1}				#	適応する剛体の名前
	}
	WaterEngine{				#	水エンジン．水面を動かす
		{water1}				#	水オブジェクトの名前
	}
	WaterContactEngine{			#	水の接触エンジン
		{water1}				#	水オブジェクトのの名前
		{soBlock1}				#	接触する剛体の名前

#		WaterRegistanceMap wrm1{
#			"stick.ffm";
#			40.0, 0.0, 0.0, 0.0,
#			0.0, 50.0, 0.0, 0.0,
#			0.0, 0.0, 25.0, 0.0,
#			-0.85, -0.5, 0, 1.0;;
#			100.0;			#	圧力ゲイン
#			{frCube}		#	適用するメッシュの名前．
#		}
	}

	Light8{						#	光源
		3;										#	1:点 2:スポット 3:方向 光源
		0.900000;0.900000;0.900000;0.900000;;	#	Diffuse		光の色
		0.900000;0.900000;0.900000;0.900000;;	#	Specular	光の色
		0.500000;0.500000;0.500000;0.500000;;	#	Ambient		光の色
		0.000000;0.000000;0.000000;;			#	Position	光源の位置(点光源・スポットのみ使用)
		0.3; 0.3; -0.3;;						#	Direction	光源の向き
		20.000000;								#	Range		光が届く範囲(点光源・スポットのみ使用)
		1.000000;								#	Falloff		
		0.800000;								#	Attenuation0
		0.800000;								#	Attenuation1
		0.800000;								#	Attenuation2
		0.300000;								#	Theta
		0.330000;								#	Phi
	}
	Frame frWater{								#	水を入れておくフレーム(座標系)
		FrameTransformMatrix  {					#	水はxy平面なので，DirectXの座標系にあわせるために回転
			1.0, 0.0, 0.0, 0.0,					#	位置・姿勢は左手系で記述
			0.0, 0.0, 1.0, 0.0,
			0.0, -1.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Water water1{							#	水オブジェクト
			120;									#	mx：X方向のセルの数
			120;									#	my：Y方向のセルの数
			0.05;								#	dh：セルの大きさ
			0.3;								#	depth：水の深さ
			6.8;								#	gravity：重力
			1000.0;								#	density：密度
			0.99;								#	loss：水の減衰率(1ステップあたりの速度の減衰)
			2.0; 0.0;	//	vx, vy				#	水の定常流．
			WaterTrackTarget{
				{frBlock1}						#	水はこのフレームが中心なるようにセルを展開する．
			}
			Material { 							#	水の材質
				1.0; 1.0; 1.0; 1.0;;			#	Diffuse
				5.000000;
				0.30; 0.3; 0.30;;
				0.30;0.30;0.30;;
				TextureFilename { "sky.jpg"; }
			}
		}
	}
	Frame frBlock1 {							#	剛体の形と位置を持つフレーム
		FrameTransformMatrix  {
			0.0, 0.0, 1.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			-1.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 1.0;;
		}
		Frame frKayak{
			FrameTransformMatrix  {
				0.0, 0.0, 1.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				-1.0, 0.0, 0.0, 0.0,
				0.0, 0.0, 0.0, 1.0;;
			}
#			Import{ "kayak_test.x"; }				#	他のXファイルをここに展開する インポート関数
		}
		Frame frWheel{
			FrameTransformMatrix  {	
				1.0, 0.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, -1.0, 0.0, 0.0,
				0.0, 0.0, 0.0, 1.0;;
			}
#			Import{ "wheel.xi"; }
		}
		Frame frCube{
			FrameTransformMatrix  {
				1.0, 0.0, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.0, -2.2, 0.0, 0.0,
				0.0, 0.0, 0.0, 1.0;;
			}
			Import{ "cube.xi"; }
		}
	}
}

