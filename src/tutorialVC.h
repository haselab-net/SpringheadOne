#ifndef Spr_TUTORIALVC_H
#define Spr_TUTORIALVC_H

/** @page TutorialVC チュートリアル(Visual C++)

	@section Content 目次
		- @ref Execution
		- @ref EditDataFile
			-# @ref MakeConvex
			-# @ref MakeConcave
			-# @ref SetTrianglePole
			-# @ref TiltFloor
			-# @ref SetSphere
			-# @ref SetLight
			-# @ref SetPhysics
			-# @ref SetCamera
			-# @ref BroadenFloor
			-# @ref SetVessel
			-# @ref AccelSphere
			-# @ref SetTexture
			-# @ref Adjustment
			-# @ref SetPointer
		- @ref SPIDAR
		- @ref NewProject

	@section Execution ビルドして実行してみる
	まずは，慣れるために既存のプロジェクト（今回は Springhead/src/Samples/DynaHaptic）を実行してみます．
		-# ビルドを行う準備として以下のことを行ってください．
			- 環境変数 Springhead に Springhead をインストールしたディレクトリを
			指定して下さい．<br>
			Windows2000ならば[マイコンピュータ]-[プロパティ]-[詳細]-[環境変数]-[新規]のダイアログに
			[変数名] Springhead [変数値] インストールしたディレクトリとしてください．
			- DirectX8.1か9.0のSDKをインストールしてください．
		-# Springhead/src/Samples/DynaHaptic/DynaHaptic.dsw を Visual C++ で開いてビルドしてください．<br>
		ビルドが成功しない方は以下のことを確認してください．
			- DirectX SDKのインクルードファイルとライブラリファイルのディレクトリの設定<br>
			[ツール]-[オプション]-[ディレクトリ]の「表示するディレクトリ(S)」を「インクルードファイル」に変え，
			DirectX SDKのインクルードファイルのディレクトリ(例:"C:\DXSDK\INCLUDE")が設定されているか確認してください．<br>
			設定されていなければ設定してください．<br>
			また，検索の優先順位を上げるために「↑」のボタンを何度かクリックして，
			一番上に来るようにしてください．<br>
			ライブラリファイルについてもインクルードファイルと同様にパスの設定を行ってください
			（「表示するディレクトリ(S)」を「ライブラリファイル」に変更する）．
			- アクティブプロジェクトの設定
			[ビルド]-[アクティブな構成の設定]の
			「プロジェクトの構成]を "Dynahaptic - Win32 MFC Debug (or Release)" 
			にして下さい．
			- <A href=http://sklab-www.pi.titech.ac.jp/~hase/springhead/>Springheadのページ</A>の「今朝のビルド」を見てください．
			ここにはその日の午前３時の時点でVSSにアップされているプロジェクトのビルド結果が表示されています．
			ここに載っていないプロジェクトのビルドは通らないということになりますので，過去のビルドが成功するバージョンのファイルを取得してきてください．
		-# 実行します．<br>
		実行したら，[ファイル]-[開く]で<A href=../../Samples/data/>Springhead/src/Samples/data/</A>にあるデータファイル(@ref XFile "Direct3D Xファイル")を適当にロードしてみてください．
		実行時の主なキー操作は以下のようになっています．
				- ↑：前進
				- ↓：後退
				- ←：左回転
				- →：右回転
				- S：上回転
				- X：下回転
				- Q：左平行移動
				- W：右平行移動
				- A：上平行移動
				- Z：下平行移動
				- R：視点リセット
				- F：フルスクリーン
				- G：画面をフルスクリーンから元に戻す
				- C：SPIDARのグリップのキャリブレーション
				- スペース：SPIDARに力を返すか返さないかのON-OFF
				- T：マルチメディアタイマーの起動
				（精度の高い力覚を提示するためには必要．Release版のみ）

	@section EditDataFile データファイルを編集する
	ボールを転がして箱に入れるという仮想世界の作成を通して，
	Xファイルの編集の方法を簡単に見ていきます．<br><br>
	- @ref XFile とは？
	- 完成例
		- rollingBall <A href=../../Samples/data/tutorial/rollingBall.x>Springhead/src/Samples/data/tutorial/rollingBall.x</A>
		- golf <A href=../../Samples/data/tutorial/golf.x>Springhead/src/Samples/data/turorial/golf.x</A>

	まずは，rollingBall から作っていきます．

		@subsection MakeConvex 凸物体を作る
		まず，仮想世界に必要なオブジェクトとして，
		床，ジャンプ台となる三角柱，ボールをモデリングします．
		.x形式で保存することのできるモデリングソフトを使用して作ってください．<br><br>
		－作成例－
		- <A href=../../Samples/data/tutorial/floor.x>floor.x</A>
		- <A href=../../Samples/data/tutorial/trianglePole.x>trianglePole.x</A>
		- <A href=../../Samples/data/tutorial/sphere.x>sphere.x</A>

		@subsection MakeConcave 凹物体を作る
		仮想世界に登場する物体にはさきほど作成したような直方体，球のような凸物体と，
		箱のような凹物体があります．
		Springheadでは衝突判定に用いているアルゴリズムの性質上，
		物体はすべて凸物体でなければなりません．
		そこで，箱を作るときには，底と四方の壁の５つの直方体を組み合わせて作成します．<br>
		作成例では，親フレーム Vessel の下に
		子フレームとして VesselFront,Back,Left,Right,Bottom を配置しています．
		各フレームにある FrameTransformMatrix は
		アフィン行列を転置した形で記述されており，
		親フレームからのローカル変換を表しています．
		（VesselFront,Back,left,right はこのローカル変換が異なるだけで，
		その下に記述されている Mesh の部分は同じものを使用しています）<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/vessel.x>vessel.x</A> 

		@subsection SetTrianglePole 床に三角柱を配置する
		では実際に世界を作っていきます．
		まず，世界の一番の親フレームとして frTop を作成し，
		その子フレームに床を，そして床の子フレームとして三角柱を配置します．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall1.x>rollingBall1.x</A>
		
		@subsection TiltFloor 床を傾ける
		次は，床を傾けます．<br>
		さきほど，三角柱を床の子フレームとしているので，
		床フレームの FrameTransformMatrix を編集するだけで，
		三角柱ごと傾けることができます．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall2.x>rollingBall2.x</A>

		@subsection SetSphere ボールを配置する
		ボールを frTop の子フレームとして配置します．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall3.x>rollingBall3.x</A>

		@subsection SetLight ライトを追加する 
		ここまでの状態で実行しても，
		暗くてあまり見えないのでライトを追加します．
		具体的には，ライトの位置・姿勢を決めるためのフレームと
		そのライトの性質を示す Light8 を frTop の下に追加します．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall4.x>rollingBall4.x</A>
		
		@subsection SetPhysics 物理法則を追加する
		ではいよいよ仮想世界に物理法則を追加します．
		具体的には，
		- SolidContainer : 仮想物体の物理的性質
		- GravityEngine : 重力の制御
		- PenaltyEngine : 衝突判定の制御

		を追加することになります．<br>
		現在作りたい世界の仕様を，

			- ボールが動く．
			- 重力がボールにはたらく．床にははたらかない．
			- ボールと床の間で衝突が起こり，衝突後ボールは跳ね返るが床は固定．

		とすると，SolidContainer および GravityEngine にはボールだけを登録し，
		PenaltyEngine には，ボールと床の衝突判定を行い衝突後はボールだけに反力を加えるという記述をします．<br>
		なお，PenaltyEngine では，
		親フレームとの衝突判定を行うように記述すれば，
		再帰的に子フレームとの衝突判定も行うので，
		ボールと床との衝突判定を記述しておけば，
		床の子フレームである三角柱との衝突判定も自動的に行います．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall5.x>rollingBall5.x</A>

		@subsection SetCamera カメラを追加
		このままでは画面に映る範囲が狭いので，
		少しひいた（ズームアウトした）位置に視点を動かします．<br>
		視点の位置を設定するためには，frTop の下に Camera を追加します．
		Camera がない場合はデフォルトのカメラ位置になります．
		（デフォルトのカメラ設定については <A href=../../src/Graphics/GRCamera.cpp>Springhead/src/Graphics/GRCamera.cpp</A> の GRCameraData::InitData() を参照）
		これまではカメラの設定をしていなかったので，
		デフォルトのカメラ位置になっていたということになります．<br>
		カメラの位置の設定には，保存の機能を使うと便利です．
		保存を行うと，そのときのカメラの設定をXファイルに書き出してくれます．
		よって，<br>

		実行 → キー操作により任意の視点位置に移動 → “名前を変えて”保存 
		→ 保存したXファイルのカメラ設定を元のファイルにコピー

		という手順で行うと楽です．
		保存する際，ボールの位置など仮想世界の状態も一緒に保存してしまうので，
		別の名前で保存する必要があります．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall6.x>rollingBall6.x</A>

		@subsection BroadenFloor 床を広くしてボールを移動
		次は，床を広くして，ボールももっと上から転がるように変更します．
		床を広くするためには物体の頂点座標を管理している Mesh を変更します．
		ボールの位置の変更は FrameTransformMatrix の平行移動成分を変更して行います．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall7.x>rollingBall7.x</A>

		@subsection SetVessel 箱を配置
		ボールをキャッチする箱を配置します．<br>
		@ref MakeConcave で作成した箱フレームを追加し，
		ボールとの衝突判定を行うように Penalty も追加します．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall8.x>rollingBall8.x</A>

		@subsection AccelSphere ボールの初期速度を大きくする
		箱にボールが入らなかったので，ボールがもっと遠くへ飛んでいくように，
		Solid を編集して初期速度を大きくします．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall9.x>rollingBall9.x</A>

		@subsection SetTexture  背景をつけ，テクスチャを貼る．
		箱にボールが入るところまでできたので，少し見た目を向上させるために，
		背景を付け，テクスチャも貼ります．<br>
		背景は，世界の後方に大きな平面オブジェクトを置いて作ることにします．
		テクスチャは，Material の中の TextureFilename で指定します．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/rollingBall.x>rollingBall.x</A>

		これでひとまず rollingBall は完成です．

		@subsection Adjustment 各オブジェクト位置の変更
		つづいて rollingBall を改良して golf を作っていきます．<br>
		まずは，世界をgolf用に変更します．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/golf1.x>golf1.x</A>

		行った作業は以下の通りです．
		- 床の変更
				- 傾きを水平にした
				- カップを作るために床を４分割した(そのうちの一つには傾斜をつけた)
				- ジャンプ台を低くした
		- 球の初期位置，初期速度の変更
		- 箱の位置の変更

		@subsection SetPointer 力覚ポインタを作る
		SPIDARのグリップ部分に対応するポインタを作ります．
		まずは，ポインタのフレームを作成します．
		そして，ポインタの物理(Solid,Penalty)を追加します．
		データファイル側の作業は以上で終わりです．<br><br>
		－作成例－　
		- <A href=../../Samples/data/tutorial/golf.x>golf.x</A>
		
		あとはSPIDARの設定等，プログラムを編集する作業になります．
		SPIDARの設定については @ref SPIDAR を参照．

	@section SPIDAR SPIDAR
	もうしばらくおまちください....

	@section NewProject 新しくプロジェクトを作る
	自分用に新しくプロジェクトを作ります．<br>
	プログラムに変更を加えていく場合は，
	新しいプロジェクトを作ることをお勧めします．<br>
	今回は，簡単な方法ということで既存のプロジェクト（Springhead/src/Samples/DynaHaptic）を
	コピーして名前だけ変更するという方法を取ることにします．
		-# VSSから $/Project/Springhead/src/Samples/DynaHaptic 以下を取得する．
		-# DynaHapticフォルダをコピーし，
		適当なところ（Samplesの下でいいと思います）に貼り付ける．
		-# 貼り付けたフォルダ内のすべてのファイルの読み取り専用のチェックをはずし，
		ファイル名及び各ファイルの中に登場する "DynaHaptic" を
		すべて新しいプロジェクト名に置換する．<br>
		.dsp, dsw, rc, sccファイルなんかもテキストエディタで開いて置換してください．		-# ビルドして実行する．
		ビルドがうまくいかない方は，名前がすべて変更されているか，
		または@ref Executionのビルドのところを確認してください．


*/

#endif