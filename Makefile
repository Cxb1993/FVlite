# Makefile for FVTD
#
# Author: Liam Pattinson
# Date: 01/04/16

COMP = g++-4.9
CFLAGS = -Wall -Wextra -pedantic -O3 -std=c++11
TYPE = -DMAXWELL
DEBUG  = ${CFLAGS} -pedantic -g -pg
LIBLOCS = -L/usr/lib/
INCLOCS = -I/usr/include/ -I.
FILES = FVTD.cpp
EXEC = FVTD.exe
LIBS = -lfftw3 -lm
TESTFILES = test.cpp
TESTEXEC  = test.exe

build: ${FILES}
	${COMP} ${CFLAGS} ${TYPE}  ${LIBLOCS} ${INCLOCS} -o ${EXEC} ${FILES} ${LIBS}

euler: ${FILES}
	${COMP} ${CFLAGS} -DEULER  ${LIBLOCS} ${INCLOCS} -o ${EXEC} ${FILES} ${LIBS}

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
