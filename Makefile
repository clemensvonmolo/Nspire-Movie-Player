GCC = nspire-gcc
LD = nspire-ld
GENZEHN = genzehn

LIBS = -lm
GCCFLAGS = -Wall -W -marm
GCCFLAGS += -O3 -g0
LDFLAGS =
ZEHNFLAGS = --name "nspiremovieplayer"

EXE = nspiremovieplayer
OBJS = config.o timer.o file.o jpeg.o frame.o main.o
DISTDIR = ./dst
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

HOSTCC = gcc
HOSTCFLAGS = -O2 -g0
HELPER = helper.c
HELPEREXE = moviecompile

vpath %.tns $(DISTDIR)

all: $(EXE).tns

jpeg.o: jpeg.c jpeg.h
	$(GCC) -Ofast -g0 -c $< -o $@

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@


$(EXE).elf: $(OBJS) jpeg.o
	mkdir -p $(DISTDIR)
	#$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(@:.tns=.elf)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $^ --output $@.zehn $(ZEHNFLAGS)
	make-prg $@.zehn $(DISTDIR)/$@
	rm $@.zehn

helper:
	$(HOSTCC) $(HOSTCFLAGS) $(HELPER) -o $(DISTDIR)/$(HELPEREXE)

clean:
	rm -f *.o *.elf *.zehn
	rm -f $(DISTDIR)/$(EXE).tns

clean_helper:
	rm -f $(DISTDIR)/$(HELPEREXE)
