# README #

Minimalistic compiler with register allocation for the purpose of article
to Nokia Book 3


Building and Usage
==================
In order to build MiNiK compiler (program called minik) and examples written in
MiNiK language it is sufficient to invoke ```make``` in the top-level directory.

minik program accepts MiNiK source code on standard input and produces resulting
assembly on standard output. Some compiler's debugging information (generated
abstract syntax tree and scope information) is produced to standard error. To
compile MiNiK source code from contained in ```prog.mik``` file and write the
generated assembly to ```prog.s``` the following command needs to be invoked:
```bash
$ src/minik < prog.mik 1> prog.s
```
To form the executable, MiNiK runtime environment has to be linked toghether
with generated assembly:
```bash
$ gcc -m32 src/runtime/minikrt0.o prog.s -o prog
```

