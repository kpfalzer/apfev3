
RDIR ?= ../..
SRCS ?= src/apfev3/*.cxx src/main.cxx
INCL ?= src
OS   ?= $(shell uname -s)

ifeq ("Darwin","${OS}")
	CCOPTS ?= -std=c++11 -lstdc++
else
	CCOPTS ?= -std=c++11 -static-libstdc++
endif

.PHONY: debug
debug:
	-rm -rf debug/${OS}
	mkdir -p debug/${OS}
	(cd debug/${OS}; g++ -o apfev3 -g ${INCL:%=-I ${RDIR}/%} ${CCOPTS} ${SRCS:%=${RDIR}/%} )

