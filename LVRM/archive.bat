if exist LVRM_BIN.lzh del LVRM_BIN.lzh
lha32 u -rx LVRM_BIN.lzh bin\*.*
if exist LVRM.lzh del LVRM.lzh
lha32 u -rx LVRM.lzh *.h *.c *.cpp *.asm *.pas *.dfm *.dcr *.dat *.cnf *.def *.bat *.doc *.dsp *.rc* *.ico *.bmp *.cur *.wav *.x *.jpg *.gif *.htm *.html *.class *.txt *.tex *.lw? *.m? *.sap *.win *.iwz vector *.sts *.res *.fig *.shp *.wld *.dxx  makefile *.mak *.dsw *.mdp *.bpr *.bpg *.bpk *.vrt *.pdf LVRM_BIN.lzh
