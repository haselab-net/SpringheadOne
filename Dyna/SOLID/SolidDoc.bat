set PATH=%LVRM_ROOT%\BIN
mkdir html
del /Q html\*.*
doxygen solid.doxy
start html\index.html
