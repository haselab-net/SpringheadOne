xof 0303txt 0032

Frame frCubeMesh{
Mesh meshCube{
	8;
	-0.27;0.27;0.27;,
	0.27;0.27;0.27;,
	0.27;0.27;-0.27;,
	-0.27;0.27;-0.27;,
	0.27;-0.27;-0.27;,
	-0.27;-0.27;-0.27;,
	0.27;-0.27;0.27;,
	-0.27;-0.27;0.27;;

	6;
	4;0,1,2,3;,
	4;3,2,4,5;,
	4;5,4,6,7;,
	4;7,6,1,0;,
	4;1,6,4,2;,
	4;7,0,3,5;;
	PhysicalMaterial pmPointer{
			0.4; 0.3; 0.4; 0.3;             # 抗力のバネ係数，ダンパ係数，摩擦力のバネ係数，ダンパ係数
	                                          # (デフォルト値の何倍かで設定する．つまりデフォルト値は1.0)
			1.6;                            # 静止摩擦係数(デフォルト値は0.6)
			1.3;                            # 動摩擦係数(デフォルト値は0.3)
	}

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
			1.000000; 0.00; 0.60; 1.00000;;
			5.000000;
			0.000000;0.000000;0.000000;;
			0.250000;0.075490;0.075490;;
		}
	}
}
}
