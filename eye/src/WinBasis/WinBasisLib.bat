@echo off
@echo ライブラリをマージします．VisualC++のライブラリ管理ツール LIB.EXE にパスを通しておいてください．

if "%1"=="RELEASE" set EXT=
if "%1"=="DEBUG" set EXT=D
if "%1"=="MFCRELEASE" set EXT=M
if "%1"=="MFCDEBUG" set EXT=MD
if "%2"=="7" set EXT=7%EXT%

set OUTPUT=WinBasis%EXT%.lib
set INPUT=%1%2\WinBasisS.lib
set LIBS=winmm.lib ws2_32.lib opengl32.lib glu32.lib

if "%LIBS%"=="" echo 環境変数用メモリの空きが不足しています．
if "%LIBS%"=="" echo 空きを増やしてもう一度実行します．
if "%LIBS%"=="" echo このメッセージが続けて表示される場合は，
if "%LIBS%"=="" echo コマンドプロンプトの設定を修正してください．
if "%LIBS%"=="" command /e:4096 /c%0 %1
if not "%LIBS%"=="" LIB /OUT:%OUTPUT% %INPUT% %LIBS%
