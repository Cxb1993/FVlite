# Makefile for FVlite
#
# Author: Liam Pattinson
# Date: 01/04/16

COMP = g++-4.9
CFLAGS = -Wall -Wextra -pedantic -O3 -std=c++11
DEBUG  = ${CFLAGS} -pedantic -g -pg
LIBLOCS = -L/usr/lib/ -L/home/cdt1507/lib/libconfig/lib
INCLOCS = -I/usr/include/ -I. -I/home/cdt1507/lib/libconfig/include
FILES = FVlite.cpp
EXEC = FVlite.exe
LIBS = -lfftw3 -lm -lconfig++
TESTFILES = test.cpp
TESTEXEC  = test.exe

euler: ${FILES}
	${COMP} ${CFLAGS} -DEULER  ${LIBLOCS} ${INCLOCS} -o ${EXEC} ${FILES} ${LIBS}

maxwell: ${FILES}
	${COMP} ${CFLAGS} -DMAXWELL  ${LIBLOCS} ${INCLOCS} -o ${EXEC} ${FILES} ${LIBS}

debug: ${FILES}
	${COMP} ${DEBUG} ${TYPE} ${LIBLOCS} ${INCLOCS} -o ${EXEC} ${FILES} ${LIBS}

test: ${TESTFILES}
	${COMP} ${CFLAGS} ${TYPE} ${LIBLOCS} ${INCLOCS} -o ${TESTEXEC} ${TESTFILES} ${LIBS}
	rm -f ${TESTEXEC}

clean:
	rm -f ${EXEC}

rebuild:
	clean
	build
