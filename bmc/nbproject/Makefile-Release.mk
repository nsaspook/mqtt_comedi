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
	${OBJECTDIR}/bmc.o \
	${OBJECTDIR}/bmc_mqtt.o \
	${OBJECTDIR}/daq.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs comedilib` `pkg-config --libs libcjson` `pkg-config --libs libcurl` -lm  -lpaho-mqtt3c  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bmc

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bmc: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bmc ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/bmc.o: bmc.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O3 -Wall `pkg-config --cflags comedilib` `pkg-config --cflags libcjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bmc.o bmc.c

${OBJECTDIR}/bmc_mqtt.o: bmc_mqtt.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O3 -Wall `pkg-config --cflags comedilib` `pkg-config --cflags libcjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bmc_mqtt.o bmc_mqtt.c

${OBJECTDIR}/daq.o: daq.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O3 -Wall `pkg-config --cflags comedilib` `pkg-config --cflags libcjson` `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/daq.o daq.c

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
