xof 0303txt 0032

//回転行列は、
//xx xy xz
//yx yy yz
//zx zy zz
//回転方向は、軸に向かって時計周り

Scene {
	SolidContainer{
		Solid Pointer1{	//	spidar[0]
			0.07;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frPointer1}
		}
		Solid Pointer2{	//	spidar[1]
			0.07;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frPointer2}
		}
		Solid soBlock1{
			1.0;
			5.0, 0.0, 0.0,
			0.0, 5.0, 0.0,
			0.0, 0.0, 5.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock1}
		}
		Solid soBlock2{
			1.0;
			5.0, 0.0, 0.0,
			0.0, 5.0, 0.0,
			0.0, 0.0, 5.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock2}
		}
		Solid soBlock3{
			1.0;
			5.0, 0.0, 0.0,
			0.0, 5.0, 0.0,
			0.0, 0.0, 5.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBlock3}
		}
		//虫の頭
		Solid soHead{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frHead}
		}
		Solid soNeckY{
			0.01;
			0.01, 0.0, 0.0,
			0.0, 0.01, 0.0,
			0.0, 0.0, 0.01;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frNeckY}
		}
		//虫の胴体
		Solid soBody{
			100000000.0;
			100000000.1, 0.0, 0.0,
			0.0, 100000000.1, 0.0,
			0.0, 0.0, 100000000.1;;
			0.5; 0.0; -0.5;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frBody}
		}
		//虫の尾
		Solid soTail{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frTail}
		}
		//左前足第一節
		Solid soLegLFY{
			0.01;				//無質量
			0.01, 0.0, 0.0,
			0.0, 0.01, 0.0,
			0.0, 0.0, 0.01;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLFY}
		}
		//左前第二節
		Solid soLegLFP{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLFP}
		}
		//左前第三節
		Solid soLegLFK{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLFK}
		}
		//左前第四節
		Solid soLegLFH{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLFH}
		}
		
		//左中足
		Solid soLegLMY{
			0.01;
			0.01, 0.0, 0.0,
			0.0, 0.01, 0.0,
			0.0, 0.0, 0.01;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLMY}
		}
		Solid soLegLMP{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLMP}
		}
		Solid soLegLMK{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLMK}
		}
		Solid soLegLMH{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLMH}
		}
		
		//左後足
		Solid soLegLBY{
			0.01;
			0.01, 0.0, 0.0,
			0.0, 0.01, 0.0,
			0.0, 0.0, 0.01;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLBY}
		}
		Solid soLegLBP{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLBP}
		}
		Solid soLegLBK{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLBK}
		}
		Solid soLegLBH{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegLBH}
		}
		
		//右前足
		Solid soLegRFY{
			0.01;
			0.01, 0.0, 0.0,
			0.0, 0.01, 0.0,
			0.0, 0.0, 0.01;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRFY}
		}
		Solid soLegRFP{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRFP}
		}
		Solid soLegRFK{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRFK}
		}
		Solid soLegRFH{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRFH}
		}
		
		//右中足
		Solid soLegRMY{
			0.01;
			0.01, 0.0, 0.0,
			0.0, 0.01, 0.0,
			0.0, 0.0, 0.01;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRMY}
		}
		Solid soLegRMP{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRMP}
		}
		Solid soLegRMK{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRMK}
		}
		Solid soLegRMH{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRMH}
		}
		
		//右後足
		Solid soLegRBY{
			0.01;
			0.01, 0.0, 0.0,
			0.0, 0.01, 0.0,
			0.0, 0.0, 0.01;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRBY}
		}
		Solid soLegRBP{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRBP}
		}
		Solid soLegRBK{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRBK}
		}
		Solid soLegRBH{
			0.1;
			0.1, 0.0, 0.0,
			0.0, 0.1, 0.0,
			0.0, 0.0, 0.1;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frLegRBH}
		}		
	}
	GravityEngine{
#		0.0; -9.8; 0.0;;
		0.0; 0.0; 0.0;;
		{soHead}
		{soBody}
		{soTail}

		{soLegLFP}
		{soLegLFK}
		{soLegLFH}

		{soLegLMP}
		{soLegLMK}
		{soLegLMH}

		{soLegLBP}
		{soLegLBK}
		{soLegLBH}

		{soLegRFP}
		{soLegRFK}
		{soLegRFH}

		{soLegRMP}
		{soLegRMK}
		{soLegRMH}

		{soLegRBP}
		{soLegRBK}
		{soLegRBH}

		{soBlock1}
		{soBlock2}
		{soBlock3}
	}
	
	ContactEngine{
		{soHead}
		{soBody}
		{soTail}

		{soLegLFP}
		{soLegLFK}

		{soLegLMP}
		{soLegLMK}

		{soLegLBP}
		{soLegLBK}

		{soLegRFP}
		{soLegRFK}

		{soLegRMP}
		{soLegRMK}

		{soLegRBP}
		{soLegRBK}

		{Pointer1}	#15
		{Pointer2}	#16
		
		{frFloor}
		{soLegRBH}
		{soLegLFH}
		{soLegLMH}
		{soLegLBH}
		{soLegRFH}
		{soLegRMH}

		{soBlock1}	#24
		{soBlock2}
		{soBlock3}


		ContactInactive{
			15;
			 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10,11,12,13,14;
		}
		ContactInactive{
			2;
			15,16;
		}
		ContactInactive{
			7;
			17,18,19,20,21,22,23;
		}
	}
	JointEngine{
		//胴体がルート
		{soBody}
		//{frBody}
		//首関節
		Joint jntNeckY{
			0;
			0.0; 0.3; 1.5;;
			1.0, 0.0, 0.0,
			0.0, 0.0, 1.0,
			0.0, -1.0, 0.0;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.0, 1.0,
			0.0, -1.0, 0.0;;
			0.0; 0.0; -1.0; 0.5; -0.5;
			3; 0.0; 80.0; 0.0; 3.0;
			{soNeckY}
			Joint jntNeckP{
				0;
				0.0; 0.0; 0.0;;
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				-1.0, 0.0, 0.0;;
				0.0; 0.0; -0.5;;
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				-1.0, 0.0, 0.0;;
				0.0; 0.0; -1.0; 0.5; -0.5;
				3; 0.0; 80.0; 0.0; 3.0;
				{soHead}
			}
			
		}
		//胴体−尾関節
		Joint jntTail{
			0;
			-1.6; 0.3; -0.5;;
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.0; 0.5;;
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			-1.0, 0.0, 0.0;;
			0.0; 0.0; -1.0; 0.5; -0.5;
			3; 0.0; 80.0; 0.0; 3.0;
			{soTail}
		}

		//左前足
		Joint jntLegLFY{
			0;
			-0.4; -0.3; 0.9;;
			1.0, 0.0, 0.0,
			0.0, -0.707, -0.707,
			0.0, 0.707, -0.707;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0;;
			-1.3; 0.0; 20.01; -2.3; -0.3;
			0; 0.0;
			200.0; //P
			0.0;  //I
			5.0; //D
			{soLegLFY}
			Joint jntLegLFP{
				0;
				0.0; 0.0; 0.0;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; -0.5;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; 20.01; -1.0; 1.0;
				0; 0.0;
				0.6; //speed
				0.8; //stride
				0.3; //clearance
				{soLegLFP}
				Joint jntLegLFK{
					0;
					0.0; 0.0; 0.5;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					0.0; 0.1; -0.6;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					1.5; 0.0; 20.01; 0.5; 2.5;
					0; 0.0;
					1.3; //swingback rate
					0.4; //turn angle
					0.05;
					{soLegLFK}
					Joint jntLegLFH{
						0;
						0.0; 0.0; 0.5;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						0.0; 0.15; -0.4;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						-0.7; 0.0; 20.01; 0.5; -0.5;
						3; -0.7; 30.01; 0.0; 5.01;
						{soLegLFH}
					}
				}
			}
		}
		
		//左中足
		Joint jntLegLMY{
			0;
			-0.4; -0.3; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.5, -0.87,
			0.0, 0.87, 0.5;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0;;
			-1.5; 0.0; 20.01; -2.5; -0.5; 0; 0.0;
			-1.5; -1.0; 2.0; //前脚つま先位置
			{soLegLMY}
			Joint jntLegLMP{
				0;
				0.0; 0.0; 0.0;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; -0.6;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; 20.01; -1.0; 1.0; 0; 0.0;
				-2.2; -1.0; -0.5; //中脚つま先位置
				{soLegLMP}
				Joint jntLegLMK{
					0;
					0.0; 0.0; 0.5;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					0.0; 0.0; -0.6;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					1.4; 0.0; 20.01; 0.4; 2.4; 0; 0.0;
					-1.4; -1.0; -2.5; //後脚つま先位置
					{soLegLMK}
					Joint jntLegLMH{
						0;
						0.0; 0.0; 0.5;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						0.0; 0.0; -0.3;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						-0.7; 0.0; 20.01; 0.5; -0.5;
						3; -0.3; 30.01; 0.0; 5.01;
						{soLegLMH}
					}
				}
			}
		}
		
		//左後足
		Joint jntLegLBY{
			0;
			-0.1; -0.3; -0.9;;
			1.0, 0.0, 0.0,
			0.0, 0.95, -0.3,
			0.0, 0.3, 0.95;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0;;
			-2.0; 0.0; 20.01; -3.0; -1.0;
			0; 0.0; 100.0; 0.0; 0.05;
			{soLegLBY}
			Joint jntLegLBP{
				0;
				0.0; 0.0; 0.0;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; -0.6;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				-0.5; 0.0; 20.01; -1.5; 0.5;
				0; 0.0; 100.0; 0.0; 0.05;
				{soLegLBP}
				Joint jntLegLBK{
					0;
					0.0; 0.0; 0.5;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					0.0; 0.0; -0.8;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					1.3; 0.0; 20.01; 0.3; 2.0;
					0; 0.0; 100.0; 0.0; 0.05;
					{soLegLBK}
					Joint jntLegLBH{
						0;
						0.0; 0.0; 0.7;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						0.0; 0.0; -0.3;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						-0.7; 0.0; 20.01; -1.7; 0.3;
						3; -0.7; 30.01; 0.0; 5.01;
						{soLegLBH}
					}
				}
			}
		}
		
		//右前足
		Joint jntLegRFY{
			0;
			0.4; -0.3; 0.9;;
			1.0, 0.0, 0.0,
			0.0, -0.707, -0.707,
			0.0, 0.707, -0.707;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0;;
			1.3; 0.0; 20.01; 0.3; 2.3; 0; 0.0;
			40.0;
			0.4;
			20.0;
			{soLegRFY}
			Joint jntLegRFP{
				0;
				0.0; 0.0; 0.0;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; -0.5;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; 20.01; -1.0; 1.0; 0; 0.0;
				1.0;
				0.5;
				0.1;
				{soLegRFP}
				Joint jntLegRFK{
					0;
					0.0; 0.0; 0.5;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					0.0; 0.1; -0.6;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					1.5; 0.0; 20.01; 0.5; 2.5; 0; 0.0;
					1.3;
					0.5;
					0.05;
					{soLegRFK}
					Joint jntLegRFH{
						0;
						0.0; 0.0; 0.5;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						0.0; 0.05; -0.4;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						-0.7; 0.0; 20.01; -1.7; 0.3;
						3; -0.7; 30.01; 0.0; 5.01;
						{soLegRFH}
					}
				}
			}
		}
		
		//右中足
		Joint jntLegRMY{
			0;
			0.4; -0.3; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.5, -0.87,
			0.0, 0.87, 0.5;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0;;
			1.5; 0.0; 20.01; 0.5; 2.5; 0; 0.0;
			-2.0; -2.0; 2.0;
			{soLegRMY}
			Joint jntLegRMP{
				0;
				0.0; 0.0; 0.0;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; -0.6;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; 20.01; -1.0; 1.0; 0; 0.0;
				-2.0; -2.0; -1.0;
				{soLegRMP}
				Joint jntLegRMK{
					0;
					0.0; 0.0; 0.5;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					0.0; 0.0; -0.6;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					1.4; 0.0; 20.01; 0.4; 2.4; 0; 0.0;
					-1.5; -1.8; -3.0;
					{soLegRMK}
					Joint jntLegRMH{
						0;
						0.0; 0.0; 0.5;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						0.0; 0.0; -0.3;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						-0.3; 0.0; 20.01; -1.7; 0.3; 
						3; -0.7; 30.01; 0.0; 5.01;
						{soLegRMH}
					}
				}
			}
		}
		
		//右後足
		Joint jntLegRBY{
			0;
			0.1; -0.3; -0.9;;
			1.0, 0.0, 0.0,
			0.0, 0.95, -0.3,
			0.0, 0.3, 0.95;;
			0.0; 0.0; 0.0;;
			1.0, 0.0, 0.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0;;
			2.0; 0.0; 20.01; 1.0; 3.0; 0; 0.0; 100.0; 0.0; 0.05;
			{soLegRBY}
			Joint jntLegRBP{
				0;
				0.0; 0.0; 0.0;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				0.0; 0.0; -0.6;;
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0;;
				-0.5; 0.0; 20.01; -1.5; 0.5; 0; 0.0; 100.0; 0.0; 0.05;
				{soLegRBP}
				Joint jntLegRBK{
					0;
					0.0; 0.0; 0.5;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					0.0; 0.0; -0.8;;
					0.0, 0.0, -1.0,
					0.0, 1.0, 0.0,
					1.0, 0.0, 0.0;;
					1.3; 0.0; 20.01; 0.3; 2.3; 0; 0.0; 100.0; 0.0; 0.05;
					{soLegRBK}
					Joint jntLegRBH{
						0;
						0.0; 0.0; 0.7;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						0.0; 0.0; -0.3;;
						0.0, 0.0, -1.0,
						0.0, 1.0, 0.0,
						1.0, 0.0, 0.0;;
						-0.7; 0.0; 20.01; -1.7; 0.3;
						3; -0.7; 30.01; 0.0; 5.01;
						{soLegRBH}
					}
				}
			}
		}
			
	}
	
 Camera  {
  1.000000,0.000000,0.000000,0.000000,0.000000,0.930001,-0.367557,0.000000,0.000000,0.367557,0.930001,0.000000,0.000000,-0.000106,23.416607,1.000000;;
  0.400000;
  0.400000;
  0.000000;
  0.000000;
  0.400000;
  4000.000000;
 }
# Camera  {
# 1.000000,0.000000,0.000000,0.000000,0.000000,0.397708,-0.917512,0.000000,0.000000,0.917512,0.397708,0.000000,0.000000,-0.000122,21.416588,1.000000;;
#  0.400000;
#  0.400000;
#  0.000000;
#  0.000000;
#  0.400000;
#  4000.000000;
# }
	
	Frame frLight {
		FrameTransformMatrix  {
			0.707107,0.000000,0.707107,0.000000,
			0.500000,0.707107,-0.500000,0.000000,
			0.500000,-0.707107,-0.500000,0.000000,
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
	

	#フレームインポート
	#Frame frHead {}
	Frame frHead {Import{"./x-data/head.X";}}
	Frame frNeckY {}
	Frame frBody {
		FrameTransformMatrix  {
			0.707107,0.000000,-0.707107,0.000000,
			0.000000,1.00000,0.000000,0.000000,
			0.707107,0.000000,0.707107,0.000000,
			-3.0,2.0,-6.0,1.0;;
		}
		Import{"./x-data/joint.X";}
	}

	#体
	#Frame frTail {}
	Frame frTail {Import{"./x-data/body.X";}}

	#左前足
	Frame frLegLFY {}
	Frame frLegLFP {Import{"./x-data/leg41.X";}}
	Frame frLegLFK {Import{"./x-data/leg42.X";}}
	Frame frLegLFH {Import{"./x-data/leg43.X";}}

	#左中足
	Frame frLegLMY {}
	Frame frLegLMP {Import{"./x-data/leg51.X";}}
	Frame frLegLMK {Import{"./x-data/leg52.X";}}
	Frame frLegLMH {Import{"./x-data/leg53.X";}}

	#左後ろ足
	Frame frLegLBY {}
	Frame frLegLBP {Import{"./x-data/leg31.X";}}
#	Frame frLegLBP {Import{"./x-data/leg61.X";}}
	Frame frLegLBK {Import{"./x-data/leg62.X";}}
	Frame frLegLBH {Import{"./x-data/leg63.X";}}
	
	#右前足
	Frame frLegRFY {}
	Frame frLegRFP {Import{"./x-data/leg41.X";}}
	Frame frLegRFK {Import{"./x-data/leg42.X";}}
	Frame frLegRFH {Import{"./x-data/leg43.X";}}
#	Frame frLegRFP {Import{"./x-data/leg11.X";}}
#	Frame frLegRFK {Import{"./x-data/leg12.X";}}
#	Frame frLegRFH {Import{"./x-data/leg13.X";}}

	#右中足
	Frame frLegRMY {}
	Frame frLegRMP {Import{"./x-data/leg21.X";}}
	Frame frLegRMK {Import{"./x-data/leg22.X";}}
	Frame frLegRMH {Import{"./x-data/leg23.X";}}

	#右後ろ足
	Frame frLegRBY {}
	Frame frLegRBP {Import{"./x-data/leg31.X";}}
	Frame frLegRBK {Import{"./x-data/leg32.X";}}
	Frame frLegRBH {Import{"./x-data/leg33.X";}}
	
	
	Material matPointer1{
	   1.000000;0.380000;0.398000;1.000000;;
	   5.000000;
	   0.000000;0.000000;0.000000;;
	   0.095000;0.174500;0.250000;;
	}
	Material matPointer2{
	   0.080000;1.000000;0.098000;1.000000;;
	   5.000000;
	   0.000000;0.000000;0.000000;;
	   0.095000;0.174500;0.250000;;
	}
	Frame frPointer1{
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			8.0,2.0,0.0,1.0;;
		}
		Sphere{ 0.3; 10; 10; {matPointer1}}
	}
	Frame frPointer2{
		FrameTransformMatrix  {
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			-8.0,2.0,0.0,1.0;;
		}
		Sphere{ 0.3; 10; 10; {matPointer2}}
	}
	Frame frBlock1{
		FrameTransformMatrix  {
			1.000000,0.000000,0.000000,0.000000,
			0.000000,1.000000,0.000000,0.000000,
			0.000000,0.000000,1.000000,0.000000,
			1.000000,112.500000,2.000000,1.000000;;
		}
		Frame{
			FrameTransformMatrix  {
				3.000000,0.000000,0.000000,0.000000,
				0.000000,10.000000,0.000000,0.000000,
				0.000000,0.000000,3.000000,0.000000,
				0.000000,0.00000,0.000000,1.000000;;
			}
			Import{ "block1.x"; }
		}
	}
	Frame frBlock2{
		FrameTransformMatrix  {
			1.000000,0.000000,0.000000,0.000000,
			0.000000,1.000000,0.000000,0.000000,
			0.000000,0.000000,1.000000,0.000000,
			1.000000,115.00000, 1.000000,1.000000;;
		}
		Frame{
			FrameTransformMatrix  {
				3.000000,0.000000,0.000000,0.000000,
				0.000000,10.000000,0.000000,0.000000,
				0.000000,0.000000,3.000000,0.000000,
				0.000000,0.00000,0.000000,1.000000;;
			}
			Import{ "block2.x"; }
		}
	}
	Frame frBlock3{
		FrameTransformMatrix  {
			1.000000,0.000000,0.000000,0.000000,
			0.000000,1.000000,0.000000,0.000000,
			0.000000,0.000000,1.000000,0.000000,
			1.000000,110.00000,4.000000,1.000000;;
		}
		Frame{
			FrameTransformMatrix  {
				3.000000,0.000000,0.000000,0.000000,
				0.000000,10.000000,0.000000,0.000000,
				0.000000,0.000000,3.000000,0.000000,
				0.000000,0.00000,0.000000,1.000000;;
			}
			Import{ "block3.x"; }
		}
	}

	#フロアー
	Frame frFloor {
		FrameTransformMatrix  {
			0.500000,0.000000,0.000000,0.000000,
			0.000000,1.000000,0.000000,0.000000,
			0.000000,0.000000,0.500000,0.000000,
			0.000000,0.000000,0.000000,1.000000;;
		}
		Import{"floor.X";}
#		Import{"wall_back.x";}
#		Import{"wall_left.x";}
#		Import{"wall_right.x";}
#		Import{"wall_front.x";}
	}
}
Beetle{
	100.0; 0.0; 10.0;	//	PID係数
	0.8; 				//	stride
	0.4; 				//	turn angle
	0.3; 				//	clearance
	1.0; 				//	speed
	1.2; 				//	swingback rate
	-1.5; -1.0; 2.0;,	//前脚つま先位置
	-2.0; -1.0; -0.5;,	//中脚つま先位置
	-1.3; -1.0; -2.3;;	//後脚つま先位置
	0;					//	AIを使うなら1
}
