rem @echo off

rem **** src�ȉ��̃t�H���_�����ׂď������� ****
rem �������܂��� cd ..
rem �������܂��� rmdir /S /Q src
rem �������܂��� cd test

rem **** SVN����Springhead�ȉ����擾���� **** 
call bat\GetSpringheadSVN.bat

rem **** �e�X�g���s�� ****
call bat\TestAll.bat

rem **** �h�L�������g(doxygen)����� ****
call bat\MakeDoc.bat

rem **** �\�[�X�c���[(Springhead\src�ȉ�)��Web�̉��ɃR�s�[ ****
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
