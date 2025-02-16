CC = gcc

CSAPP_INC = .
CSAPP_SRC = .

CFLAGS = -Wall -g -Og -I $(CSAPP_INC) -I .
LDLIBS = -lpthread

PROGS =	statcheck\
	cpstdin\
	cpfile\
	hello\
	ffiles1\
	ffiles2\
	ffiles3\

all: $(CSAPP_SRC)/csapp.o $(PROGS) 
$(PROGS): $(CSAPP_SRC)/csapp.o
$(CSAPP_SRC)/csapp.o: $(CSAPP_SRC)/csapp.c $(CSAPP_INC)/csapp.h



clean:
	rm -f $(PROGS) *.o *~

#  make
# gcc -Wall -g -Og -I . -I .    statcheck.c csapp.o  -lpthread -o statcheck
# gcc -Wall -g -Og -I . -I .    cpstdin.c csapp.o  -lpthread -o cpstdin
# gcc -Wall -g -Og -I . -I .    cpfile.c csapp.o  -lpthread -o cpfile
# gcc -Wall -g -Og -I . -I .    hello.c csapp.o  -lpthread -o hello
# gcc -Wall -g -Og -I . -I .    ffiles1.c csapp.o  -lpthread -o ffiles1
# gcc -Wall -g -Og -I . -I .    ffiles2.c csapp.o  -lpthread -o ffiles2
# gcc -Wall -g -Og -I . -I .    ffiles3.c csapp.o  -lpthread -o ffiles3


# How It Works
# 1. Compiling csapp.c: The Makefile first compiles csapp.c into csapp.o using the rule:
# $(CSAPP_SRC)/csapp.o: $(CSAPP_SRC)/csapp.c $(CSAPP_INC)/csapp.h
# 2. Building Programs: Each program in PROGS is then compiled and linked with csapp.o. For example, to build ffiles1, the Makefile will run:
# gcc -Wall -g -Og -I . -I . ffiles1.c csapp.o -lpthread -o ffiles1
# Cleaning Up: The clean target removes all compiled programs and object files:

