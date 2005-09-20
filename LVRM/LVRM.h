#ifndef LVRM_LVRM_H
#define LVRM_LVRM_H
/**	@mainpage Lesser VR Module library
@section background 背景と目的
LVRMライブラリは，東工大佐藤研で作られるVRシステムのソフトウエアをリバイスして，ライブラリ化可能な部分をライブラリにしたものです．VRのライブラリには VR Juggler やHIP などいろいろありますが，多くのライブラリは，そのライブラリ全部を使ってVRアプリケーションを作ることを前提としており，つまみ食いは許されません．

このライブラリでは，できる限りつまみ食いがしやすいように，小さくて機能が少ないパッケージの集合として実現するようにしています．

Unixの考え方に，less is more というものがあります．これは，機能が少なく小さなシステムの方が，多機能な巨大システムよりも理解しやすく，再利用できるので，優れているということをあらわしています．LVRMという名前は，Lesser Virtual Reality Module の頭文字です．機能的には劣っていて小さなモジュールでライブラリを構成することで，理解しやすく利用しやすいライブラリを目指しています．

@section lib ライブラリの構成
@subsection structure パッケージ
LVRMライブラリは以下のパッケージ(幾つかのクラスが集まったライブラリ)から構成されます．
 <table>
  <tr>
   <td> パッケージ名 </td>
   <td> 機能 </td>
   <td> ライブラリファイルの出来る場所 </td>
  </tr> <tr>
   <td> <a href = "../Base/html/index.html"> Base </a> </td>
   <td> 基本部分，行列・ベクトル，デバッグ支援，ユーティリティ </td>
   <td> LVRM/Base/Base.lib </td>
  </tr> <tr>
   <td> <a href = "../SceneGraph/html/index.html"> SceneGraph </a> </td>
   <td> VRML97をロードし，シーングラフを構築する </td>
   <td> LVRM/SceneGraph/SceneGraph.lib </td>
  </tr> <tr>
   <td> <a href = "../Render/html/index.html"> Render </a> </td>
   <td> OpenGLアプリ用のフレームワーク </td>
   <td> LVRM/Render/Render.lib </td>
  </tr> <tr>
   <td> <a href = "../Device/html/index.html"> Device </a> </td>
   <td> D/A・A/D・カウンタボードなどのデバイスの抽象化 </td>
   <td> LVRM/Device/Device.lib </td>
  </tr> <tr>
   <td> <a href = "../HIS/html/index.html"> Haptic </a> </td>
   <td> 力覚提示措置(SPIDAR)の制御 </td>
   <td> LVRM/HIS/HIS.lib </td>
  </tr> <tr>
   <td> <a href = "../WinBasis/html/index.html"> WinBasis </a> </td>
   <td> Windowsの基本機能のラッパー </td>
   <td> LVRM/WinBasis/WinBasis.lib </td>
  </tr> <tr>
   <td> <a href = "../WinDX/html/index.html"> WinDX </a> </td>
   <td> DirectXの機能のラッパー </td>
   <td> LVRM/WinDX/WinDX.lib </td>
  </tr> <tr>
   <td> LVRM </a> </td>
   <td> 全パッケージを一つにまとめたもの </td>
   <td> LVRM/LVRM.lib </td>
  </tr>
 </table>

@subsection depend パッケージの依存関係
あるクラスAが別のクラスBのポインタを持っている場合など，AはBがなければ使えません．このような状況をAがBに依存していると言います．パッケージにはいくつものクラスがありますが，あるパッケージに入っているクラスはそのパッケージの中の別のクラスに依存していることが多いです．

また，あるパッケージAのクラスが別のパッケージBのクラスに依存していることもあります．例えば，DeviceパッケージのクラスはBaseパッケージやWinBasisパッケージのクラスに依存しているので，BaseとWinBasisなしでは使えません．逆に，Baseのクラスは，Baseパッケージだけがあれば使うことはできます．

パッケージの依存関係を図にすると次のようになります．
 <ul>
  <li> Base
  <ul>
    <li> WinBasis
    <ul>
      <li> Device
      <ul>
        <li> Haptic
      </ul>
    </ul>
    <li> SceneGraph
    <li> Render
  </ul>
 </ul>

@section usage 使い方
ビルドできる環境を作ってください．ビルドにはVisualC++を使うことを想定しています．gccやbcc32やbcbでも構築できないことはないですが，makefileを自分で作る必要があります．bcbのプロジェクトファイルはメンテナンスしていません．

@subsection build 環境の作り方
<ol>
  <li> ソースツリーの入手 <br>
       VSSの $/Project/LVRM 以下を取得するなどして，ソースツリーを作ってください．
  <li> 環境変数の設定
    <li> 環境変数を設定します<br>
    環境変数 LVRM_ROOT に LVRMを取得したディレクトリを設定して下さ
    い．例えば次のような設定をすることになります．<br>
    <ul>
      <li>Win98: 次の1行をAutoexec.batに加える<br>
         set LMVR_ROOT=C:\Home\C\Project\LVRM
      <li>Win2000: 環境変数に次の設定をする<br>
         マイコンピュータ－プロパティ－詳細－環境変数－新規を選ぶ．<br>
         環境変数名 LVRM_ROOT に 値 C:\Home\C\Project\LVRM を設定する．
    </ul>
</ol>

@subsection app アプリケーションの作り方
<ol>
  <li> コンソールアプリのプロジェクトの作成<br>
       特にWindowsらしいアプリケーションを作りたい場合以外は，
       コンソールアプリを作成することをお勧めします．
       OpenGLをGLUTを介して使用する場合もコンソールアプリが最適です．
    <ol>
      <li> [ファイル]-[新規作成]-(プロジェクト)-(Win32 Console Application) を選択し，適当なプロジェクト
           を作成する．
      <li> (空のプロジェクト)を選び(終了)をクリックする．
      <li> [表示]-[ワークスペース]を選び，ワークスペース(Class View と File View)を表示する．
      <li> (FileView)を選び，プロジェクトにソースファイルを追加する．<br>
              プログラムは追加したソースファイルの main() 関数から始まる．
    </ol>
  <li> コンソールアプリのインクルードファイルとライブラリの設定<br>
          LVRMのクラスを使用するためには，インクルードファイルをインクルードし，
          ライブラリをプロジェクトに追加しなければなりません．
    <ol>
      <li> [プロジェクト]-[設定]-(設定の対象:全ての構成)-(C/C++)-(カテゴリ：プリプロセッサ)
           を選択し，インクルードファイルのパスに，<br>
           , \$(LVRM_ROOT)<br>
           という文字列をを追加する．
      <li> プロジェクトに使用するパッケージとそのパッケージが依存するパッケージのライブラリを
         追加する．このとき，Debug版とRelease版でライブラリが異なるので注意する．
         例えば，Baseパッケージは，Debug版がBaseD.lib，Release版がBase.libとなる．具体的には
         以下の設定をする．
        <ol>
          <li> プロジェクトに，Debug版，Release版両方のライブラリを追加する．
          <li> [プロジェクト]-[設定]-(設定の対象:Win32 Debug 版) を選択し，追加したライブラリを
             選択する．
          <li> (一般)-(このファイルをビルドしない)チェックボックスを使って，
             Debug版ではDebug版のライブラリ，Release版ではRelease版のライブラリがビルドされる
             ように設定する．
        </ol>
    </ol>
  <li> インクルードファイルのインクルードと名前空間の設定<br>
     自分の書くソースファイルでライブラリのクラスを使うためには，ライブラリのヘッダファイル
     をインクルードし，名前空間を導入しなければなりません．
    <ol>
      <li> ヘッダのインクルード
        使用したいクラスが含まれるヘッダファイルを LVRM からの相対パスでインクルードします．
        例えば<br>
        #include <Base/Affine.h><br>
        とします．
      <li> 名前空間の導入<br>
        LVRMのクラスは名前空間LVRMの中で宣言されています．
        そこで，インクルードの後で次のようにします．<br>
        using namespace LVRM;<br>
        こうすると，Affinefクラスが使えるようになります．
        こうする代りに，Affinef のところを名前空間付きで LVRM::Affinef と書い
        てもOKです．
    </ol>
</ol>

@subsection libfile libファイルの役割と使い方
CやC++のソースをコンパイルするとオブジェクトファイル(ファイル名の最後が.obj や .o のファイル)ができます．これをリンクすると(つなぐと)実行ファイル(ファイル名の最後が.exeのファイル)ができます．

ライブラリファイル(libファイル，ファイル名の最後が.libのファイル)は，実行ファイルを作る代りに，オブジェクトファイルをそのまま１つにまとめたものです．ライブラリファイルを作っておくと，オブジェクトファイルをひとつひとつリンクする必要がないので，プロジェクトの設定が楽になります．

LVRMは，パッケージごとにLibファイルを用意しています．各Libファイルは，デバッグ版/リリース版，MFCのあり/なし で違うファイルになります．これらのライブラリは Base.lib BaseD.lib BaseM.lib BaseMD.lib のように名前を変えて，各パッケージのルートノードにできます．(例えば，C:\Home\C\Project\LVRM\Base\Base.lib にできます．)

これらのパッケージ毎のライブラリを１つにまとめたものがLVRM.lib(D,M,MDもあります)です．これは，LVRMのディレクトリ(例えばC:\Home\C\Project\LVRM\LVRM.lib)にできます．同じオブジェクトが含まれるライブラリをリンクするとエラーになります．LVRM.libをリンクしたら，Base.libなどをリンクしてはいけません．

@subsection doc ドキュメント生成手順
マニュアルは，ドキュメント生成ツール doxygen で生成できます．<br>
マニュアルの生成方法:
  <ol>
    <li> LVRMのインストールをする．
    <li> LVRM/AllDoc.bat を実行する．
    <li> LVRM/html/index.html にドキュメントのトップページが生成される．
  </ol>

@section file ファイル・フォルダ構成
LVRMのソースツリーには，C++のソース以外にビルドに必要なツールやデバイスドライバのバイナリが含まれています．
<dl>
  <dt>LVRM/bin
  <dd>make flex bison などのビルドに必要なツールがあります．
     <table><tr><td>flex</td><td>
     SceneGraphパッケージのVRMLローダーは，lexという字句解析エンジン生成ツールを用いています．flexはGNUが提供するlexです．
     </td></tr><tr><td>bison</td><td>
     SceneGraphパッケージのVRMLローダーは，yaccという構文解析エンジン生成ツールを用いています．bisonはGNUが提供するyaccです．
     </td></tr><tr><td>make</td><td>
     make は ファイルの作り方と依存関係が書いてあるファイル makefile を読み，
     依存関係に従ってファイルを生成するツールです．
     flexとbisonを動かすために使用しています．
     </td></tr><tr><td>doxygen</td><td>
     doxygenはC++などのソースコードからリファレンスマニュアル
     を生成するツールです．このドキュメントもdoxygenを使用して生成しています．
      @ref doc をご参照ください．
     </td></tr></table>
  <dt>LVRM/bin/drivers
  <dd>デバイスドライバがあります．再配布ができないものもあります．
  研究室外部に公開する場合は，この部分を除く必要があります．
</dl>
  
@section license 配布・使用条件
  <ul>
    <li>LVRM/SceneGraph/Wrl2View 以下は GPLで配布されている Wrl2View というVRML2 Viewer を改変したものなので，GPLです．
    <li>LVRM/bin 以下のバイナリは，コンパイルに必要なツールとハードウェアのドライバです．GIVEIO.SYS の配布・使用条件は不明です．その他のツール
    はGPLです．
    <li>そのほかは以下のとおりとします．再配布する場合は，著作者の署名・連絡先を改変・削除しないでください．閉鎖的な会社でソフトウエアを書く人にも自由に使用できるようにしたいので，GPL・LGPLでないライセンスで配布します．もちろんGPL・LGPLに改変して再配布してくださっても結構です．このソースコードを用いた研究を発表する場合には，参考文献にURLを含めてください．
  </ul>

@section contanct 連絡先
長谷川 晶一 <hase@hi.pi.titech.ac.jp>
東京工業大学 精密工学研究所          〒226-8503 横浜市緑区長津田町 4259
佐藤・小池研究室

*/

#include "Base/Base.h"
#include "Device/Device.h"
#include "HIS/Haptic.h"
#include "Render/Render.h"
#include "Render/Render.h"

#endif
