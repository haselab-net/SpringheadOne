rem @echo off

rem **** SVNからSpringhead以下を取得する **** 
cd ..
svn update
cd test

rem **** テストを行う ****
call bat\TestAll.bat

rem **** ドキュメント(doxygen)を作る ****
call bat\MakeDoc.bat

rem **** ソースツリー(Springhead\src以下)をWebの下にコピー ****
cd ..\src
call DelAll.bat
cd ..\test
rmdir /s /q \\samba\VSS\Web\springhead\svntest\src
rmdir /s /q \\samba\VSS\Web\springhead\svntest\src
rmdir /s /q \\samba\VSS\Web\springhead\svntest\src
rmdir /s /q \\samba\VSS\Web\springhead\svntest\src
rmdir /s /q \\samba\VSS\Web\springhead\svntest\src
rmdir /s /q \\samba\VSS\Web\springhead\svntest\src
rmdir /s /q \\samba\VSS\Web\springhead\svntest\src
xcopy /e/c/f/h/i/y ..\src \\samba\VSS\Web\springhead\svntest\src
