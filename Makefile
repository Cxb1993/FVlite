# Makefile for FVlite

COMP = g++
CFLAGS = -Wall -Wextra -pedantic -O3 -std=c++11 -fmax-errors=3
DEBUG  = ${CFLAGS} -DDEBUG -g -pg
LIBLOCS = -L/usr/lib/ 
INCLOCS = -I./src -I/usr/include/ 
FILES = src/FVlite.cpp
EXEC = FVlite.exe
LIBS = -lm -lconfig++ 

euler: ${FILES}
	${COMP} ${CFLAGS} -DEULER  ${INCLOCS} -o ${EXEC} ${FILES} ${LIBLOCS} ${LIBS}

maxwell: ${FILES}
	${COMP} ${CFLAGS} -DMAXWELL ${INCLOCS} -o ${EXEC} ${FILES} ${LIBLOCS} ${LIBS}

debug: ${FILES}
	${COMP} ${DEBUG} -DEULER ${INCLOCS} -o ${EXEC} ${FILES} ${LIBLOCS} ${LIBS}

test: ${TESTFILES}
	${COMP} ${CFLAGS} ${TYPE} ${LIBLOCS} ${INCLOCS} -o ${TESTEXEC} ${TESTFILES} ${LIBS}
	rm -f ${TESTEXEC}

install:
	mkdir -p data
	mkdir -p plots
	mkdir -p saved_data
	mkdir -p saved_plots

clean:
	rm -f ${EXEC}

rebuild:
	clean
	build
