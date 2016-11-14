MINIKC=../src/minik
RT=../src/runtime/minikrt0.o
SOURCES=factorial.mik gcd.mik gcd_recursive.mik prime.mik need_spill.mik inference.mik
ASMS=$(SOURCES:.mik=.s)
LOGS=$(SOURCES:.mik=.log)
PROGRAMS=$(SOURCES:.mik=)


all: $(ASMS) programs

%.s: %.mik
	$(MINIKC) $< 1> $@ 2> ${<:.mik=.log}

programs: $(PROGRAMS)

% : %.s
	gcc -g -m32 $(RT) $< -o $@

clean:
	rm -f $(ASMS)
	rm -f $(PROGRAMS)
	rm -f $(LOGS)

