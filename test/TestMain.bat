rem @echo off

rem **** src以下のフォルダをすべて消去する ****
rem もうやりません cd ..
rem もうやりません rmdir /S /Q src
rem もうやりません cd test

rem **** SVNからSpringhead以下を取得する **** 
call bat\GetSpringheadSVN.bat

rem **** テストを行う ****
call bat\TestAll.bat

rem **** ドキュメント(doxygen)を作る ****
call bat\MakeDoc.bat

rem **** ソースツリー(Springhead\src以下)をWebの下にコピー ****
cd ..\src
call DelAll.bat
cd ..\test
rmdir /s /q \\samba\VSS\Web\springhead\src
rmdir /s /q \\samba\VSS\Web\springhead\src
rmdir /s /q \\samba\VSS\Web\springhead\src
rmdir /s /q \\samba\VSS\Web\springhead\src
rmdir /s /q \\samba\VSS\Web\springhead\src
rmdir /s /q \\samba\VSS\Web\springhead\src
rmdir /s /q \\samba\VSS\Web\springhead\src
xcopy /e/c/f/h/i/y ..\src \\samba\VSS\Web\springhead\src
