************************************************************************
*            Virtual Human デモ実行手順 ドキュメント                   *
*                                        since : 2003.10.28            *
*                                   lastupdate : 2003.10.29            *
************************************************************************

//全体プロクラム
以下のようにソースプログラムのpathとMPIで実行されるDEMO用のPathが違うので注意してください。

SOURCE:
　\home\VSS\project\HumanScale\DEMO_VH
Execute file and data：
　\home\demo\project\HumanScale\DEMO_VH

まず、最新実行ファイルをバッチ処理で更新してください。
\home\VSS\project\HumanScale\DEMO_VHにupexec.batというサンプルがありますので、利用してください。

1）VMSQL   SQLサーバ専用機
   D-vision裏のDPC03の場所に接続されています。

2) VMControl コントロール用PC
   DOMの代わりに使用します。

<toggleキー>
/////////////////////////////////////////////
Spidar関連制御:　
	b：最小張力
	c: Calibration, ボール初期化
Animation関連:
	Space:　animation起動・停止
	Tab: ボールを投げる　	
	f : full screen
	escape: Exit

////////////////////////////////////////////

//TODO BigSpidarの起動手順を追記


1.起動方法
　アンプとPCをUSB2.0で接続する．
　アンプのスイッチＯＮすると最小張力が発生する．
　左図の様に、片手ずつ組み立てる．
　糸が引っかかるので、片手が上になるようにする．

2．片つけ方法

big_spidar.htmを参考にしてください。
（OSがXPではない方は「XP Office run」をDownloadする必要がある）

*注意：BigSpidar専用でLVRMのDeviceの中を修正した部分があります。
LVRM\Device\DRUsb20Simple.cpp のReset()のエンコーダ相順設定、
標準のoutBuffer[2] = 0xFF;を 0x5A にしています。

=====================================================================
■起動手順

まず、MPI関連DLLがない方は，mpi folderに「mpich.nt.1.2.5.exe」がありますので、
Installしてください。DLLが /Sytem32に自動的COPYされます。


・Step0

 VPC00～VPC33のマシンの上でluncher.exeが実行されていないことを確認する。
 もし、luncher.exeが実行されているのならば終了させる。

・Step1

 VMCONTROLで、コマンドプロンプトから次を実行。

----------------------------------------------
c:
cd \home\demo\project\HumanScale\DEMO_VH
mpirun -logon config-full.txt
----------------------------------------------

mpirunを実行するとaccount、passwordが聞かれるので、
account  : mpich
password : mpich
を入力すると、デモが開始される。


=====================================================================
■終了手順

・Step0

 VMCONTROLでアプリケーションを終了させる。(VMCONTROLで終了させると、VPCの各PC上のアプリケーションも終了する)

・Step1

 VPC00～VPC33上で、"スタート"→"プログラム"→"スタートアップ"から"lunch.bat"を実行させる。
(他のD-visionのアプリケーションは今のところ、VPC上でluncher.exeが実行されていることが必要なので。ずいぶんメンドウだけど･･･)


=====================================================================
■キーリファレンス

//TODO ここにキーリファレンスを追記


