# RULES

comma:=,
empty:=
space:= $(empty) $(empty)
#LIBS_FLAG=-Wl$(comma)-l $(subst $(space), -Wl$(comma)-l ,$(LIBS))


.PHONY: all clean

all:	a.mot

a.mot:	a.out
	sh-elf-objcopy --srec-len 248 -O srec a.out a.mot
	ihc a.mot 0
	sh-elf-objdump -t a.out > a.txt

clean:
	-rm a.out $(OBJS) a.mot a.txt

a.out: $(OBJS)	Makefile
	echo $(LIBS)
#	$(XLD) $(LDFLAGS) $(ECOS_GLOBAL_LDFLAGS) $(LIBS_FLAG) -o $@ $(OBJS)
	$(XLD) $(LDFLAGS) $(ECOS_GLOBAL_LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(XCC) -c -o $*.o $(CFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.cxx
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.C
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.cc
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.cpp
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<
