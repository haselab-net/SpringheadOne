rem @echo off

rem **** SVN����Springhead�ȉ����擾���� **** 
cd ..
svn update
cd test

rem **** �e�X�g���s�� ****
call bat\TestAll.bat

rem **** �h�L�������g(doxygen)����� ****
call bat\MakeDoc.bat

rem **** �\�[�X�c���[(Springhead\src�ȉ�)��Web�̉��ɃR�s�[ ****
cd ..\src
call DelAll.bat
cd ..\test
rmdir /s /q \\samba\spr\www\src
xcopy /e/c/f/h/i/y ..\src \\samba\spr\www\src
