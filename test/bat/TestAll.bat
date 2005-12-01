rem **** テストを実行する ****

rem **** ラベル付けに使う環境変数の初期化 **** 
set DATESL=%DATE:~2,10%
set DATE=%DATESL:/=.%
set LABEL=ビルド失敗
set COMMENT=ビルド成功（

echo --- ビルドのログ ---  > log\Build.log
echo. >> log\Build.log
echo 日付 : %DATE% > log\BuildError.log
echo. >> log\BuildError.log
echo --- ビルドエラーのログ --- >> log\BuildError.log
echo. >> log\BuildError.log
echo --- 更新履歴のログ --- > log\History.log
echo. >> log\History.log

rem **** ビルドテストを行う(テストを行うファイルは引数で指定) **** 
rem call bat\BuildMFC(BuildBCB).bat (引数1) (引数2) 
rem (引数1) : プロジェクトファイルの場所
rem (引数2) : プロジェクトファイル名(BuildMFC-.dspの前の部分, BuildBCB-.bpgの前の部分)

rem ----- この下にビルドを行うファイルを追加する -----
call bat\BuildMFC.bat Springhead\src Springhead
call bat\BuildMFC.bat Springhead\src\Samples\DynaHaptic DynaHaptic
call bat\BuildMFC.bat Springhead\src\Samples\MFCD3DDyna MFCD3DDyna
call bat\BuildVC.bat  Springhead\src\Samples\SDKD3DDyna SDKD3DDyna
rem call bat\BuildBCB.bat Springhead\src\BCBGui\TreeViewTest TreeTestView
rem ----- ここまで -----

rem **** ラベル付け **** 
rem 上で行ったビルドが1つでも成功し、exeファイルができてたら$/Project/Springheadに成功ラベルを付ける
rem また、ビルドが成功したファイル名をコメントに羅列する 
rem call bat\Label.bat (引数1) (引数2)
rem (引数1) : exeファイルの場所
rem (引数2) : exeファイル名

rem ----- この下にビルドを行ったファイルを追加する -----
call bat\Label.bat Springhead\src\Springhead\Springhead
call bat\Label.bat Springhead\src\Samples\DynaHaptic\MFCDebug DynaHaptic 
call bat\Label.bat Springhead\src\Samples\DynaHaptic3DOF\MFCDebug DynaHaptic3DOF
call bat\Label.bat Springhead\src\Samples\MFCD3DDyna\MFCDebug MFCD3DDyna 
call bat\Label.bat Springhead\src\Samples\SDKD3DDyna\Debug SDKD3DDyna 
rem call bat\Label.bat Springhead\src\BCBGui\TreeViewTest TreeViewTest
rem ----- ここまで -----

set COMMENT=%COMMENT:~0,-1%）
rem if "%LABEL%" == "ビルド成功" ss Label $/Project/Springhead/test/LabelTest.txt -I- -L%LABEL%%date% -C%COMMENT%
rem if "%LABEL%" == "ビルド成功" ss Label $/Project/Springhead -I- -L%LABEL%%date% -C%COMMENT%
if "%LABEL%" == "ビルド成功" svn copy svn+ssh://sprsvn/export/spr/svn/repository/Springhead/trunk svn+ssh://sprsvn/export/spr/svn/repository/Springhead/tags/BuildSucceed%date% -m %COMMENT%

rem **** Springheadの更新履歴をHistory.logに出力 ****
rem ss History $/Project/Springhead -R -I- -#100 > log/History.log
svn log svn+ssh://sprsvn/export/spr/svn/repository/Springhead/trunk > log/History.log
rem **** ログをSambaにコピーする ****
set SMBBASE=\\samba\VSS\Web\springhead
if exist %SMBBASE%\result.log del %SMBBASE%\result.log
if "%LABEL%" == "ビルド成功" echo %COMMENT%>%SMBBASE%\result.log
copy log\BuildError.log %SMBBASE%

rem **** ログをSVNにコミットする ****
cd log 
svn commit -m "Autobuild done."
cd ..

rem **** 使用した環境変数のクリア **** 
set DATE=
set LABEL=
set COMMENT=
