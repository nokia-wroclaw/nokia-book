MINIKC=../src/minik
RT=../src/minikrt0.o
SOURCES=factorial.mik gcd.mik gcd_recursive.mik
ASMS=$(SOURCES:.mik=.s)
PROGRAMS=$(SOURCES:.mik=)


all: $(ASMS) programs

%.s: %.mik
	cat $< | $(MINIKC) 1> $@ 2>/dev/null

programs: $(PROGRAMS)

% : %.s
	gcc -m32 $(RT) $< -o $@

clean:
	rm -f $(ASMS)
	rm -f $(PROGRAMS)

