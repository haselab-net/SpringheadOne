:@echo off
@echo ライブラリをマージします．VisualC++のライブラリ管理ツール LIB.EXE にパスを通しておいてください．

: ビルド構成にあわせたライブラリ名のサフィックスを設定
if "%1"=="RELEASE" set EXT=
if "%1"=="DEBUG" set EXT=D
if "%1"=="MFCRELEASE" set EXT=M
if "%1"=="MFCDEBUG" set EXT=MD
if "%2"=="7" set EXT=7%EXT%

: 出力ファイルと入力ファイル
set OUTPUT=WinDX%EXT%.lib
set INPUT=%1%2\WinDXS.lib

: DirectXのライブラリの在り処．
if exist c:\mssdk set DXROOT=c:\mssdk
if exist d:\mssdk set DXROOT=d:\mssdk
if exist c:\dxsdk set DXROOT=c:\dxsdk
if exist d:\dxsdk set DXROOT=d:\dxsdk
if exist c:\dx90sdk set DXROOT=c:\dx90sdk
if exist d:\dx90sdk set DXROOT=d:\dx90sdk

if not "%DXROOT%"=="" set DXLIB=%DXROOT%\lib
if exist %DXLIB%\x86\nul set DXLIB=%DXLIB%\x86

: DirectXのバージョン
set DXVER=9


if DXVER==9 set DINPUT=dinput.lib
if DXVER==8 set DINPUT=dinput8.lib
set LIBS=ddraw.lib d3d%DXVER%.lib d3dx%DXVER%.lib dsound.lib strmiids.lib Quartz.lib d3dxof.lib %DINPUT% setupapi.lib dxguid.lib

: DirectShow の BaseClassライブラリがあればそれも追加
if "%EXT%"=="D" if exist %DXROOT%\samples\Multimedia\DirectShow\BaseClasses\Debug\strmbasd.lib set DSHOW=%DXROOT%\samples\Multimedia\DirectShow\BaseClasses\Debug\strmbasd.lib
if "%EXT%"=="" if exist %DXROOT%\samples\Multimedia\DirectShow\BaseClasses\Release\strmbase.lib set DSHOW=%DXROOT%\samples\Multimedia\DirectShow\BaseClasses\Release\strmbase.lib
set LIBS=%LIBS% %DSHOW%

if "%LIBS%"=="" echo 環境変数用メモリの空きが不足しています．
if "%LIBS%"=="" echo 空きを増やしてもう一度実行します．
if "%LIBS%"=="" echo このメッセージが続けて表示される場合は，
if "%LIBS%"=="" echo コマンドプロンプトの設定を修正してください．
if "%LIBS%"=="" command /e:4096 /c%0 %1
if not "%LIBS%"=="" if not "%DXLIB%"=="" LIB /LIBPATH:"%DXLIB%" /OUT:%OUTPUT% %INPUT% %LIBS%
if not "%LIBS%"=="" if "%DXLIB%"=="" LIB /OUT:%OUTPUT% %INPUT% %LIBS%
