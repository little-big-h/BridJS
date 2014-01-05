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
CND_PLATFORM=MinGW64-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1445274692/bridjs_module.o \
	${OBJECTDIR}/_ext/1445274692/dyncall_v8.o \
	${OBJECTDIR}/_ext/1445274692/dyncall_v8_utils.o \
	${OBJECTDIR}/_ext/1445274692/dyncallback_v8.o \
	${OBJECTDIR}/_ext/1445274692/dynload_v8.o \
	${OBJECTDIR}/_ext/1445274692/native_function_v8.o \
	${OBJECTDIR}/_ext/1445274692/pointer_v8.o \
	${OBJECTDIR}/_ext/1445274692/struct_v8.o \
	${OBJECTDIR}/_ext/1445274692/test.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared

${OBJECTDIR}/_ext/1445274692/bridjs_module.o: ../../src/bridjs_module.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/bridjs_module.o ../../src/bridjs_module.cc

${OBJECTDIR}/_ext/1445274692/dyncall_v8.o: ../../src/dyncall_v8.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/dyncall_v8.o ../../src/dyncall_v8.cc

${OBJECTDIR}/_ext/1445274692/dyncall_v8_utils.o: ../../src/dyncall_v8_utils.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/dyncall_v8_utils.o ../../src/dyncall_v8_utils.cc

${OBJECTDIR}/_ext/1445274692/dyncallback_v8.o: ../../src/dyncallback_v8.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/dyncallback_v8.o ../../src/dyncallback_v8.cc

${OBJECTDIR}/_ext/1445274692/dynload_v8.o: ../../src/dynload_v8.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/dynload_v8.o ../../src/dynload_v8.cc

${OBJECTDIR}/_ext/1445274692/native_function_v8.o: ../../src/native_function_v8.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/native_function_v8.o ../../src/native_function_v8.cc

${OBJECTDIR}/_ext/1445274692/pointer_v8.o: ../../src/pointer_v8.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/pointer_v8.o ../../src/pointer_v8.cc

${OBJECTDIR}/_ext/1445274692/struct_v8.o: ../../src/struct_v8.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/struct_v8.o ../../src/struct_v8.cc

${OBJECTDIR}/_ext/1445274692/test.o: ../../src/test.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} "$@.d"
	$(COMPILE.cc) -g  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1445274692/test.o ../../src/test.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
