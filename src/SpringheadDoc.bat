set PATH=%SPRINGHEAD%\BIN;%SPRINGHEAD%\BIN\Graphviz\bin
mkdir html\%1
del /Q html\%1\*.*
doxygen springhead%1.doxy
start html\%1\index.html
:END
