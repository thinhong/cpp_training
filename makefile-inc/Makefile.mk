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

ERROR_OFF=2>/dev/null || :


# Include project Makefile
include Makefile


# Add project-specific flags to the compiler flags
CXXFLAGS      += ${PRJ_INCLUDE}
LDLIBSOPTIONS += ${PRJ_LIBDIR} ${PRJ_LIBNAME}


# Process preprocessor macros
CXXMACRO_STRINGS_ := $(foreach string,$(CXXMACRO_STRINGS),$(string)\")
CXXMACRO_STRINGS_ := $(subst =,=\",$(CXXMACRO_STRINGS_))
CXXMACRO_ := $(CXXMACRO_STRINGS_)
CXXMACRO_ += $(CXXMACRO_NUMBERS)
CXXMACRO_ += $(CXXMACRO_FLAGS)


# Build directory
PRJ_BUILDDIR=build-${CONF_LOWERCASE}

# Object directory
PRJ_OBJECTDIR=${PRJ_BUILDDIR}

# Determine the output directory
PRJ_EXEDIR := ${PRJ_EXEDIR}/
PRJ_EXEDIR := $(subst ./,,$(PRJ_EXEDIR))
ifeq (${PRJ_EXEDIR},/)
	PRJ_EXEDIR :=
endif
PRJ_EXEDIR := $(subst //,/,$(PRJ_EXEDIR))

ifneq (${PRJ_EXEDIR},)
	PRJ_EXEDIR_ROOT := $(subst /, ,${PRJ_EXEDIR})
	PRJ_EXEDIR_ROOT := $(firstword ${PRJ_EXEDIR_ROOT})
else
	PRJ_EXEDIR_ROOT :=
endif

# Determine the full path to the final built file
ifeq (${CONF_LOWERCASE}, release)
	PRJ_EXEFILE=${PRJ_EXEDIR}${PRJ_EXENAME}
else ifeq (${CONF_LOWERCASE}, debug)
	PRJ_EXEFILE=${PRJ_EXEDIR}${PRJ_EXENAME}-${CONF_LOWERCASE}
else ifeq (${CONF_LOWERCASE}, profile)
	PRJ_EXEFILE=${PRJ_EXEDIR}${PRJ_EXENAME}-${CONF_LOWERCASE}
else ifeq (${CONF_LOWERCASE}, shlib)
	PRJ_EXEFILE=${PRJ_EXEDIR}${PRJ_SHLIBNAME}
endif


# Source directories
PRJ_SOURCEDIRS := $(shell find ${PRJ_SOURCEROOT} -type d)

# Source files
PRJ_SOURCEFILES := $(foreach dir,$(PRJ_SOURCEDIRS),$(wildcard $(dir)/*.cpp))
PRJ_SOURCEFILES := $(patsubst $(PRJ_SOURCEROOT)/%,%,$(PRJ_SOURCEFILES))

# Object files
PRJ_OBJECTFILES := $(patsubst %.cpp,%.o,$(PRJ_SOURCEFILES))
PRJ_OBJECTFILES := $(foreach file,$(PRJ_OBJECTFILES),$(PRJ_OBJECTDIR)/$(file))


_TEMPFILENAME=.temp${PRJ_NAME}${CONF_LOWERCASE}.000


# Build Targets	
.build-conf: ${BUILD_SUBPROJECTS}
	@echo "------------------------------------------------------------"
	@echo ""
	@echo "COMPILER STANDARD :  $(CXXSTANDARD)"
	@echo ""
	@echo "COMPILER FLAGS FOR $(BUILD_SYS_UPPERCASE) :"
	@echo "    $(CXXFLAGS_OS_SPEC)"
	@echo ""
	@echo "COMPILER FLAGS FOR $(CONF_UPPERCASE) :"
	@echo "    $(CXXFLAGS)"
	@echo ""
	@echo "C++ PREPROCESSOR MACROS :"
	@echo "    Strings :  $(CXXMACRO_STRINGS_)"
	@echo "    Numbers :  $(CXXMACRO_NUMBERS)"
	@echo "    Flags   :  $(CXXMACRO_FLAGS)"
	@echo ""
	@echo "BUILDING OBJECT FILES FOR $(CONF_UPPERCASE) :"
	@"${MAKE}" -f makefile-inc/Makefile.mk ${PRJ_EXEFILE}
	@echo ""


${PRJ_EXEFILE}: ${PRJ_OBJECTFILES}
	@echo ""
ifeq (${CONF_LOWERCASE}, shlib)
	@echo "BUILDING SHARED LIBRARY   [ ${PRJ_EXEFILE} ]"
else
	@echo "BUILDING $(CONF_UPPERCASE) EXECUTABLE   [ ${PRJ_EXEFILE} ]"
endif
	@${MKDIR} -p "$@${_TEMPFILENAME}"
	@${RM} -r "$@${_TEMPFILENAME}"
	@${LINK.cc} $(CXXFLAGS_OS_SPEC) -std=${CXXSTANDARD} -o ${PRJ_EXEFILE} ${PRJ_OBJECTFILES} ${LDLIBSOPTIONS}
	@${RM} ".depend"


${PRJ_OBJECTDIR}/%.o : ${PRJ_SOURCEROOT}/%.cpp
	@echo "    $@"
	@${MKDIR} -p "$@${_TEMPFILENAME}"
	@${RM} -r "$@${_TEMPFILENAME}"
	@${RM} "$@.o.depend"
	@$(CXX) $(CXXFLAGS) $(CXXFLAGS_OS_SPEC) -std=${CXXSTANDARD} $(CXXMACRO_) -c $< -o $@


# Subprojects
.build-subprojects:


# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	@${RM} ".depend"
	@${RM} -r ${PRJ_OBJECTDIR}
	@${RM} ${PRJ_EXEFILE}
ifneq (${PRJ_EXEDIR_ROOT},)
	@find ${PRJ_EXEDIR_ROOT} -type d -empty -delete  ${ERROR_OFF}
endif
	@echo "Cleaned all built files of the configuration:  $(CONF_UPPERCASE)"


# Subprojects
.clean-subprojects:


# Validation
.validate:
ifeq (${CONF_VALID},0)
	@echo "ERROR: Invalid configuration '${CONF_UPPERCASE}' in project ${PRJ_NAME}"
	@echo "       See 'make help' for details."
	@echo "------------------------------------------------------------"
	@echo ""
	@exit 1
endif
	@# Do nothing
