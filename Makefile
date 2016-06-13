# Makefile for FVlite
#
# Author: Liam Pattinson
# Date: 01/04/16

COMP = g++-4.9
CFLAGS = -Wall -Wextra -pedantic -O3 -std=c++11 -fmax-errors=3
DEBUG  = ${CFLAGS} -DDEBUG -g -pg
#DEBUG  = ${CFLAGS} -DDEBUG 
LIBLOCS = -L/home/cdt1507/lib/libconfig/lib/ -L/usr/lib/ 
INCLOCS = -I. -I/home/cdt1507/lib/libconfig/include/ -I/usr/include/ 
FILES = FVlite.cpp
EXEC = FVlite.exe
LIBS = -lfftw3 -lm -lconfig++ 
TESTFILES = test.cpp
TESTEXEC  = test.exe

euler: ${FILES}
	${COMP} ${CFLAGS} -DEULER  ${INCLOCS} -o ${EXEC} ${FILES} ${LIBLOCS} ${LIBS}

maxwell: ${FILES}
	${COMP} ${CFLAGS} -DMAXWELL ${INCLOCS} -o ${EXEC} ${FILES} ${LIBLOCS} ${LIBS}

debug: ${FILES}
	${COMP} ${DEBUG} -DEULER ${INCLOCS} -o ${EXEC} ${FILES} ${LIBLOCS} ${LIBS}

test: ${TESTFILES}
	${COMP} ${CFLAGS} ${TYPE} ${LIBLOCS} ${INCLOCS} -o ${TESTEXEC} ${TESTFILES} ${LIBS}
	rm -f ${TESTEXEC}

clean:
	rm -f ${EXEC}

rebuild:
	clean
	build
