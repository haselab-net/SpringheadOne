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
		Solid soPole01{
			1.0;
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			0.0; 0.0; 0.0;;
			{frPole01}
		}
	}

############################################################
###############  　 重力エンジン　　 #######################
############################################################

	GravityEngine{
		0.0; -9.8; 0.0;;
		{soPole01}
	}


############################################################
###############  　 関節エンジン　　 #######################
############################################################

	JointEngine{
		{frBlock01}
		 Joint joPole01{
		  0.0;
		  0.0; 0.1; 0.0;;
		  1.0, 0.0, 0.0,
		  0.0, 1.0, 0.0,
		  0.0, 0.0, 1.0;;
		  0.0; -0.5; 0.0;;
		  1.0, 0.0, 0.0,
		  0.0, 1.0, 0.0,
		  0.0, 0.0, 1.0;;
		#変位, 速度, 最大トルク, 可動範囲-, 可動範囲+
		3.141592; 0.0; 20000000.0; 0.0; 0.0;	
		#入力の種類(0:トルク, 1:加速度, 2:速度, 3:変位), 入力値，P,I,D
		3; 0.0; 250.0; 0.01; 10.0;		#位置入力の例
		  {soPole01}
		}
	}


############################################################
###############  　 接触エンジン　　 #######################
############################################################

	ContactEngine{
		{soBlock01}
		{frFloor}
	}


	Frame frTop{
		FrameTransformMatrix  {
			1.000000,0.000000,0.000000,0.000000,
			0.000000,1.000000,0.000000,0.000000,
			0.000000,0.000000,1.000000,0.000000,
			0.000000,0.000000,0.000000,1.000000;;
		}		}

	
############################################################
###############  　 ライト設定　　　 #######################
############################################################

		Frame frLight {
			FrameTransformMatrix  {
				#1.000000,0.000000,0.000000,0.000000,
		  		#0.000000,1.000000,0.000000,0.000000,
				#0.000000,0.000000,1.000000,0.000000,
			  	#0.000000,0.000000,-10.000000,1.000000;;
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
			0.000000;0.000000;0.000000;0.000000;;
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



		Frame frPole01 {
			FrameTransformMatrix  {
			 1.000000,0.000000,0.000000,0.000000,
			 0.000000,1.000000,0.000000,0.000000,
			 0.000000,0.000000,1.000000,0.000000,
			 0.000000,0.000000,0.000000,1.000000;;
			}
			Mesh {
			   8;
			   -0.15000;0.50000;0.15000;,
			   0.15000;0.50000;0.15000;,
			   0.15000;0.50000;-0.15000;,
			   -0.15000;0.50000;-0.15000;,
			   0.15000;-0.50000;-0.15000;,
			   -0.15000;-0.50000;-0.15000;,
			   0.15000;-0.50000;0.15000;,
			   -0.15000;-0.500000;0.15000;;

			   6;
			   4;0,1,2,3;,
			   4;3,2,4,5;,
			   4;5,4,6,7;,
			   4;7,6,1,0;,
			   4;1,6,4,2;,
			   4;7,0,3,5;;
	 
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
			     1.0;0.0;0.0;1.000000;;
			     25.000000;
			     0.830000;0.830000;0.830000;;
			     0.280000;0.035137;0.021961;;
			    }
			  }
			}
	 	}

		 Frame frBlock01 {
		  FrameTransformMatrix  {
		   1.000000,0.000000,0.000000,0.000000,
		   0.000000,1.000000,0.000000,0.000000,
		   0.000000,0.000000,1.000000,0.000000,
		   0.000000,0.200000,0.000000,1.000000;;
		  }

		Mesh {
		 8;
		 -0.2500000;0.10000;0.2500000;,
		 0.2500000;0.10000;0.2500000;,
		 0.250000;0.10000;-0.2500000;,
		 -0.250000;0.10000;-0.2500000;,
		 0.2500000;-0.10000;-0.2500000;,
		 -0.2500000;-0.10000;-0.2500000;,
		 0.2500000;-0.10000;0.2500000;,
		 -0.2500000;-0.10000;0.2500000;;
		 
		 6;
		 4;0,1,2,3;,
		 4;3,2,4,5;,
		 4;5,4,6,7;,
		 4;7,6,1,0;,
		 4;1,6,4,2;,
		 4;7,0,3,5;;
 
		PhysicalMaterial {
			1.0; 1.0; 1.0; 1.0;
			#damper
			0.0;
			0.0;
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
		   1.000000;0.933333;0.003922;1.000000;;
		   25.000000;
		   0.670000;0.670000;0.670000;;
		   0.310000;0.289333;0.001216;;
		  }
		 }
		}
	 }


	 Frame frFloor{
	  FrameTransformMatrix  {
	   1.000000,0.000000,0.000000,0.000000,
	   0.000000,1.000000,0.000000,0.000000,
	   0.000000,0.000000,1.000000,0.000000,
	   0.000000,-0.1000000,0.000000,1.000000;;
	  }
		Mesh {
		 8;
		 -150.00000;0.00000;150.00000;,
		 150.00000;0.00000;150.00000;,
		 150.00000;0.00000;-150.00000;,
		 -150.00000;0.00000;-150.00000;,
		 150.00000;-1.00000;-150.00000;,
		 -150.00000;-1.00000;-150.00000;,
		 150.00000;-1.00000;150.00000;,
		 -150.00000;-1.00000;150.00000;;
		 
		 6;
		 4;0,1,2,3;,
		 4;3,2,4,5;,
		 4;5,4,6,7;,
		 4;7,6,1,0;,
		 4;1,6,4,2;,
		 4;7,0,3,5;;
		 
		PhysicalMaterial {
			1.0; 1.0; 1.0; 1.0;
			0.0;
			0.0;
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
		   1.000000;1.000000;1.000000;1.000000;;
		   0.000000;
		   0.000000;0.000000;0.000000;;
		   0.000000;0.000000;0.000000;;
		  }
		 }
		}
	 }
	}

############################################################
###############  　 カメラ設定　　　 #######################
############################################################
	Camera  {
		#15度傾けてます。
		1.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.965325, -0.258819, 0.000000,
		0.000000, 0.258819, 0.965325, 0.000000,
		0.000000, -1.000000, 10.000000, 1.000000;;
		0.400000; 0.400000;           
		0.000000; 0.000000;           
		0.400000; 4000.000000;        
	 }                                    


}#~frTop
}#~SCENE


