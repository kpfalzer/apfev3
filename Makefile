
RDIR ?= ../..
SRCS ?= src/apfev3/*.cxx src/xyzzy/*.cxx
MAIN ?= src/main.cxx
INCL ?= src
OS   ?= $(shell uname -s)
LIB  ?= apfev3
EXE  ?= apfev3

ifeq ("Darwin","${OS}")
	CCOPTS ?= -std=c++11 -lstdc++
else
	CCOPTS ?= -std=c++11 -static-libstdc++
endif

.PHONY: debug debug.a
debug: debug.a
	(cd debug/${OS}; g++ -o ${EXE} -g ${INCL:%=-I ${RDIR}/%} ${CCOPTS} ${MAIN:%=${RDIR}/%} ${LIB}.a )

debug.a:
	-rm -rf debug/${OS}
	mkdir -p debug/${OS}
	(cd debug/${OS}; g++ -c -g ${INCL:%=-I ${RDIR}/%} ${CCOPTS} ${SRCS:%=${RDIR}/%} )
	(cd debug/${OS}; ar cr ${LIB}.a *.o )

