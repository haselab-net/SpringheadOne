Springhead					2004年6月 長谷川晶一

1.背景と目標
  東工大佐藤研で作られるVRシステムのソフトウエアをリバイスして，ライブ
  ラリ化可能な部分をライブラリにしたものです．VRのライブラリには VR Ju
  ggler や HIP などいろいろありますが，多くのライブラリは，そのライブラ
  リ全部を使ってVRアプリケーションを作ることを前提としており，つまみ食
  いは許されません．
  
  このライブラリでは，できる限りつまみ食いがしやすいように，小さくて機
  能が少ないパッケージの集合として実現するようにしています．
  
  Unixの考え方に，less is more というものがあります．これは，機能が少な
  く小さなシステムの方が，多機能な巨大システムよりも理解しやすく，再利
  用できるので，優れているということをあらわしています．
  Springheadも小さなモジュールでライブラリを構成することで，理解しやす
  く利用しやすいライブラリを目指しています．

2.ライブラリの概要
  ライブラリはいくつかのパッケージから構成されています．
2.1

3.インストール＆実行手順
  以下の手順に従ってください．
  1. Springheadのツリーを用意してください．
  2. 環境変数に SPRINGHEAD に ツリーのトップディレクトリを設定して下さ
     い．例えば次のような設定をすることになります．
       Win98:  次の1行をAutoexec.batに加える
               set SPRINGHEAD=C:\Home\C\Project\Springhead
       Win2000:マイコンピュータ－プロパティ－詳細－環境変数－新規を選
       び環境変数名 SPRINGHEAD に 値 C:\Home\C\Project\Springhead
       を設定する．
  3. DirectX8か9をインストールする．

  Visual C++ 6.0を使う場合．
  4. Visual C++ Service Pack 6 をインストールする．
  5. Visual C++ の [ツール]-[オプション]-[ディレクトリ]の
      インクルードディレクトリに，C:\DXSDK\include
      ライブラリディレクトリに，C:\DXSDK\lib
     を追加する．
  6. src/samples/DynaHaptic/DynaHaptic.dsw を開き，MFC Release構成をビ
     ルド，実行する．

  C++ Builder 6 を使う場合．
  4. Borland C++Builder 6 アップデートパック #4 をインストール
  5. src/SpringheadBCB.bpg を開いて，すべてのプロジェクトをコンパイル
     する．
  6. src/BCBGui/TreeViewTest/TreeTestView.bpg を開いて実行してみる．
  
  Visual C++ 7.1を使う場合
  VC6と同様だが，開発に使っていないため， .sln .vcproj ファイルが更新
  されていないことが多い．プロジェクトのファイルを追加／削除すると動く
  ことが多い．

4.ドキュメント生成手順
  サンプルソフトとしては，
  ・SPIDARのサンプル
    Springhead/src/HIS/Samples/Spidar/spidar.dsw
  ・物理シミュレータのサンプル
    Springhead/src/Samples/MFCD3DDyna/MFCD3DDyna.dsw
  があります．

  ドキュメントはまだできていません（２００４．６．１０）
/*
  リファレンスマニュアルは，ドキュメント生成ツール doxygen で生成でき
  ます．

 - リファレンスマニュアルの生成方法
   1. Springheadのインストールをする．
   2. Springhead/src/SpringheadDoc.bat JA を実行する．
   3. Springhead/src/html/index.html にドキュメントのトップページが生成される．
*/

◆配布・使用条件
  ・Springhead/src/Collision/CDGJK.cpp には， GPLで配布されている SOLID
    という衝突判定ルーチンを参考にしたコードが混ざっています．このファ
    イルはGPLになってしまう可能性があります．
  ・Springhead/bin 以下の実行形式は，コンパイルに必要なツールとハード
    ウェアのドライバです．GIVEIO.SYS の配布・使用条件は不明です．その
    他のツールはGPLです．
  ・そのほかは，GPL，Apacheライセンス，以下のライセンスのうち1つを選択
    できるものとします．
    再配布する場合は，著作者の署名・連絡先を改変・削除しないでください．
    また，本ライブラリを使用した成果を公表した場合，その成果に本ライブラリ
    が使用されていることを公表することを妨げないでください．
  ・このソフトウエアに同封の loadBmp.dll は paintLibを使用しています．
    loadBmp.dll contains paintlib code. paintlib is copyright (c) 1996-2002
    Ulrich von Zadow and other contributors

◆配布・使用の際のお願い
  ・本ライブラリを使用して何かを作った場合，よろしければ著者にメールで
    知らせて下さい．
  ・本ライブラリを使用して何かを作った場合，よろしければ使用したことを
    何らかの方法で公表してください．
  ・本ライブラリを使用して作ったものを用いて研究などを行い，成果を発表
    する場合は，使用したことを明記してください．

◆連絡先
長谷川 晶一 <hase@pi.titech.ac.jp>
東京工業大学 精密工学研究所          〒226-8503 横浜市緑区長津田町 4259
佐藤・小池研究室
