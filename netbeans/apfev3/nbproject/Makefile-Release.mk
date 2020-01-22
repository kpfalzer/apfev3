#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/a7f6c56/acceptor.o \
	${OBJECTDIR}/_ext/a7f6c56/charbuf.o \
	${OBJECTDIR}/_ext/a7f6c56/consumer.o \
	${OBJECTDIR}/_ext/a7f6c56/location.o \
	${OBJECTDIR}/_ext/a7f6c56/node.o \
	${OBJECTDIR}/_ext/a7f6c56/util.o \
	${OBJECTDIR}/_ext/2ae9850d/exception.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11
CXXFLAGS=-std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libapfev3.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libapfev3.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libapfev3.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libapfev3.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libapfev3.a

${OBJECTDIR}/_ext/a7f6c56/acceptor.o: ../../src/apfev3/acceptor.cxx
	${MKDIR} -p ${OBJECTDIR}/_ext/a7f6c56
	${RM} "$@.d"
	$(COMPILE.cc) -I../../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a7f6c56/acceptor.o ../../src/apfev3/acceptor.cxx

${OBJECTDIR}/_ext/a7f6c56/charbuf.o: ../../src/apfev3/charbuf.cxx
	${MKDIR} -p ${OBJECTDIR}/_ext/a7f6c56
	${RM} "$@.d"
	$(COMPILE.cc) -I../../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a7f6c56/charbuf.o ../../src/apfev3/charbuf.cxx

${OBJECTDIR}/_ext/a7f6c56/consumer.o: ../../src/apfev3/consumer.cxx
	${MKDIR} -p ${OBJECTDIR}/_ext/a7f6c56
	${RM} "$@.d"
	$(COMPILE.cc) -I../../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a7f6c56/consumer.o ../../src/apfev3/consumer.cxx

${OBJECTDIR}/_ext/a7f6c56/location.o: ../../src/apfev3/location.cxx
	${MKDIR} -p ${OBJECTDIR}/_ext/a7f6c56
	${RM} "$@.d"
	$(COMPILE.cc) -I../../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a7f6c56/location.o ../../src/apfev3/location.cxx

${OBJECTDIR}/_ext/a7f6c56/node.o: ../../src/apfev3/node.cxx
	${MKDIR} -p ${OBJECTDIR}/_ext/a7f6c56
	${RM} "$@.d"
	$(COMPILE.cc) -I../../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a7f6c56/node.o ../../src/apfev3/node.cxx

${OBJECTDIR}/_ext/a7f6c56/util.o: ../../src/apfev3/util.cxx
	${MKDIR} -p ${OBJECTDIR}/_ext/a7f6c56
	${RM} "$@.d"
	$(COMPILE.cc) -I../../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a7f6c56/util.o ../../src/apfev3/util.cxx

${OBJECTDIR}/_ext/2ae9850d/exception.o: ../../src/xyzzy/exception.cxx
	${MKDIR} -p ${OBJECTDIR}/_ext/2ae9850d
	${RM} "$@.d"
	$(COMPILE.cc) -I../../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/2ae9850d/exception.o ../../src/xyzzy/exception.cxx

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
