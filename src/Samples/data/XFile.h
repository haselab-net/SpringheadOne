#ifndef Spr_XFILE_H
#define Spr_XFILE_H

/** @page XFile Xファイル

	@section WhatXFile Xファイルとは
	「Xファイル」とは，ファイル名に .x 拡張子が付いたファイルを指し，
	Direct3Dにおけるモデルデータの標準フォーマットです．<br>
	3Dの形状データ，マテリアル，テクスチャなどがテンプレートとして定義され，
	さらにユーザによって自由にテンプレートの定義の拡張を行うことができます．
	
	@section XFileHowToEdit 編集方法
	- 3Dの形状データ，マテリアルなど<br>
	3ds max や Metasequoia などのXファイルを出力できる市販のソフトを使って編集します．

	- 物理，力覚<br>
	Xファイルをメモ帳などのテキストエディタで開いて，	
	データを直接入力して編集します．<br><br>

	@subsection template 各データ型の記述テンプレート
	- Xファイル標準のテンプレートについては<A href=http://www.microsoft.com/japan/msdn/library/default.asp?url=/japan/msdn/library/ja/jpdx8_c/hh/directx8_c/_dx_x_file_templates_graphicsxof.asp>Microsoft DirectX 8.0 Xファイル テンプレート</A>に書いてあります．

	- Springheadで拡張定義したテンプレートについては，
	<A href=../ImpD3D/SGD3D.cpp>SGD3D.cpp</A>の中のD3FileDoc::RegisterTemplates()に，<br>
	@verbatim

	"template 型名 {\n"
	"キー\n"
	"型 データの名前;\n"
	"      :           " 
	"      :           " 
	"[参照]\n"
	"}\n"@endverbatim<br>
	のような形で定義されています．
	例として，剛体オブジェクトのデータを持つテンプレート Solid では，
	以下のような定義に対し，<br>
	@verbatim

 <剛体データの定義部分>

	"template Solid {\n"
	" <A9C20358-EDEB-4c0b-97AB-37D330B6BFA4>\n"
	" FLOAT mass;\n"
	" Matrix3x3 inertia;\n"
	" Vector velocity;\n"
	" Vector angularVelocity;\n"
	" Vector center;\n"
	" [Frame]\n"
	"}\n"@endverbatim<br>
	Xファイルでは，<br>
	@verbatim

 <Xファイルでの剛体データの記述>

    Solid soSpinTop{
      2.5;                    # 質量
      3.0, 0.0, 0.0,              
      0.0, 1.0, 0.0,          # 慣性テンソル
      0.0, 0.0, 3.0;;
      0.0; 0.0; 0.0;;         # 初期速度
      0.01; 16.0; 0.0;;       # 初期角速度
      0.0; 0.1; 0.0;;         # 重心（メッシュの原点からの相対位置）
      {frSpinTop}             # フレームへの参照
                              # (複数のフレームをツリーとして持つ場合は
                              # トップフレームを指す）
    }@endverbatim<br>
	のように記述されます．

	- サンプルファイルとしてコマ(<A href=../../Samples/data/TopRotYwithComment.x>TopRotYwithComment.x</A>)にコメントを付けましたのでご参照ください．<br>

*/

#endif