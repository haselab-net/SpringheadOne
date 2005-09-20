CPPFILE = Wrl2View/scanner.cpp Wrl2View/parser.cpp
CPPFILE2 = Wrl2View/parser.h

all : $(CPPFILE)

clean : 
	rm -f $(CPPFILE) $(CPPFILE2)
#
# suffix rules
#
.y.cpp:
	bison -d -b $@ $<
	mv -f $@.tab.c $@
	mv -f $@.tab.h Wrl2View/tokendef.h


.l.cpp:
	flex -st  $< > $@
