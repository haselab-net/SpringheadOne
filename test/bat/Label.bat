rem exe�t�@�C�����ł��Ă���r���h�����������Ƃ݂Ȃ�
rem %1 exe�t�@�C���̏ꏊ
rem %2 exe�t�@�C����

rem �r���h��1�ł��������Aexe�t�@�C�����ł��Ă���$/Project/%3�ɐ������x����t����
rem �܂��A�r���h�����������t�@�C�������R�����g�ɗ��񂷂� 
rem ���̂��߂�LABEL,COMMENT�ϐ��̐ݒ���s��

IF EXIST ..\..\%1\%2.exe set LABEL=�r���h����
IF EXIST ..\..\%1\%2.exe set COMMENT=%COMMENT%%2,

IF EXIST ..\..\%1\%2.lib set LABEL=�r���h����
IF EXIST ..\..\%1\%2.lib set COMMENT=%COMMENT%%2,
