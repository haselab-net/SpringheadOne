rem **** �e�X�g�����s���� ****

rem **** ���x���t���Ɏg�����ϐ��̏����� **** 
set DATESL=%DATE:~2,10%
set DATE=%DATESL:/=.%
set LABEL=�r���h���s
set COMMENT=�r���h�����i

echo --- �r���h�̃��O ---  > log\Build.log
echo. >> log\Build.log
echo ���t : %DATE% > log\BuildError.log
echo. >> log\BuildError.log
echo --- �r���h�G���[�̃��O --- >> log\BuildError.log
echo. >> log\BuildError.log
echo --- �X�V�����̃��O --- > log\History.log
echo. >> log\History.log

rem **** �r���h�e�X�g���s��(�e�X�g���s���t�@�C���͈����Ŏw��) **** 
rem call bat\BuildMFC(BuildBCB).bat (����1) (����2) 
rem (����1) : �v���W�F�N�g�t�@�C���̏ꏊ
rem (����2) : �v���W�F�N�g�t�@�C����(BuildMFC-.dsp�̑O�̕���, BuildBCB-.bpg�̑O�̕���)

rem ----- ���̉��Ƀr���h���s���t�@�C����ǉ����� -----
call bat\BuildMFC.bat Springhead\src Springhead
call bat\BuildMFC.bat Springhead\src\Samples\DynaHaptic DynaHaptic
call bat\BuildMFC.bat Springhead\src\Samples\MFCD3DDyna MFCD3DDyna
call bat\BuildVC.bat  Springhead\src\Samples\SDKD3DDyna SDKD3DDyna
rem call bat\BuildBCB.bat Springhead\src\BCBGui\TreeViewTest TreeTestView
rem ----- �����܂� -----

rem **** ���x���t�� **** 
rem ��ōs�����r���h��1�ł��������Aexe�t�@�C�����ł��Ă���$/Project/Springhead�ɐ������x����t����
rem �܂��A�r���h�����������t�@�C�������R�����g�ɗ��񂷂� 
rem call bat\Label.bat (����1) (����2)
rem (����1) : exe�t�@�C���̏ꏊ
rem (����2) : exe�t�@�C����

rem ----- ���̉��Ƀr���h���s�����t�@�C����ǉ����� -----
call bat\Label.bat Springhead\src\Springhead\Springhead
call bat\Label.bat Springhead\src\Samples\DynaHaptic\MFCDebug DynaHaptic 
call bat\Label.bat Springhead\src\Samples\DynaHaptic3DOF\MFCDebug DynaHaptic3DOF
call bat\Label.bat Springhead\src\Samples\MFCD3DDyna\MFCDebug MFCD3DDyna 
call bat\Label.bat Springhead\src\Samples\SDKD3DDyna\Debug SDKD3DDyna 
rem call bat\Label.bat Springhead\src\BCBGui\TreeViewTest TreeViewTest
rem ----- �����܂� -----

set COMMENT=%COMMENT:~0,-1%�j
rem if "%LABEL%" == "�r���h����" ss Label $/Project/Springhead/test/LabelTest.txt -I- -L%LABEL%%date% -C%COMMENT%
rem if "%LABEL%" == "�r���h����" ss Label $/Project/Springhead -I- -L%LABEL%%date% -C%COMMENT%
if "%LABEL%" == "�r���h����" svn copy svn+ssh://sprsvn/export/spr/svn/repository/Springhead/trunk svn+ssh://sprsvn/export/spr/svn/repository/Springhead/tags/BuildSucceed%date% -m %COMMENT%

rem **** Springhead�̍X�V������History.log�ɏo�� ****
rem ss History $/Project/Springhead -R -I- -#100 > log/History.log
svn log svn+ssh://sprsvn/export/spr/svn/repository/Springhead/trunk > log/History.log
rem **** ���O��Samba�ɃR�s�[���� ****
set SMBBASE=\\samba\VSS\Web\springhead
if exist %SMBBASE%\result.log del %SMBBASE%\result.log
if "%LABEL%" == "�r���h����" echo %COMMENT%>%SMBBASE%\result.log
copy log\BuildError.log %SMBBASE%

rem **** ���O��SVN�ɃR�~�b�g���� ****
cd log 
svn commit -m "Autobuild done."
cd ..

rem **** �g�p�������ϐ��̃N���A **** 
set DATE=
set LABEL=
set COMMENT=
