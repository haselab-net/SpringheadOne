set PATH=%LVRM_ROOT%\BIN;%LVRM_ROOT%\BIN\Graphviz\bin
mkdir html
del /Q html\*.*
if not "%1"=="" goto END
doxygen lvrm.doxy
start html\index.html
:END
