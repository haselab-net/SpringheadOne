rem exeファイルができてたらビルドが成功したとみなす
rem %1 exeファイルの場所
rem %2 exeファイル名

rem ビルドが1つでも成功し、exeファイルができてたら$/Project/%3に成功ラベルを付ける
rem また、ビルドが成功したファイル名をコメントに羅列する 
rem そのためのLABEL,COMMENT変数の設定を行う

IF EXIST ..\..\%1\%2.exe set LABEL=ビルド成功
IF EXIST ..\..\%1\%2.exe set COMMENT=%COMMENT%%2,

IF EXIST ..\..\%1\%2.lib set LABEL=ビルド成功
IF EXIST ..\..\%1\%2.lib set COMMENT=%COMMENT%%2,
