# Makefile for FVTD
#
# Author: Liam Pattinson
# Date: 01/04/16

COMP = g++-4.9
CFLAGS = -Wall -Wextra -pedantic -g -O3 -std=c++11
LIBLOCS = -L/usr/lib/
INCLOCS = -I/usr/include/ -I.
FILES = FVTD.cpp
EXEC = FVTD.exe
TESTFILES = test.cpp
TESTEXEC  = test.exe

build: ${FILES}
	${COMP} ${CFLAGS} ${LIBLOCS} ${INCLOCS} -o ${EXEC} ${FILES} ${LIBS}

test: ${TESTFILES}
	${COMP} ${CFLAGS} ${LIBLOCS} ${INCLOCS} -o ${TESTEXEC} ${TESTFILES} ${LIBS}
	rm -f ${TESTEXEC}

clean:
	rm -f ${EXEC}

rebuild:
	clean
	build
