################################################################################
## You rarely need to change this file.
################################################################################
## Include prject-specific variables
include makefile-inc/Project-variables.mk


################################################################################
## Set basic flags for the compiler (you rarely need to change this file).
################################################################################
## Compiler standard
CXXSTANDARD := c++11

## CC Compiler Flags
CCFLAGS          :=
CXXFLAGS         :=
CXXFLAGS_COMMON  := -MMD -MP -MF "$@.depend"
CXXFLAGS_RELEASE := -O3 -w -DNDEBUG
CXXFLAGS_DEBUG   := -Wall -g -ggdb3
CXXFLAGS_PROFILE := -Wall -g -ggdb3
CXXFLAGS_SHLIB   := -O3 -w -DNDEBUG -fPIC -pedantic

## C Compiler Flags
CFLAGS :=

## Fortran Compiler Flags
FFLAGS :=

## Assembler Flags
ASFLAGS :=

## Link Libraries and Options
LDLIBSOPTIONS :=
LDFLAGS       := 
LDFLAGS_SHLIB := -shared

## All Configurations
ALLCONFS=Release  Debug  Profile  ShLib

## Default build configuration
DEFAULTCONF=Release
CONF=${DEFAULTCONF}


################################################################################
## Compatibility settings for different OSes
################################################################################
## Linux
CXXFLAGS_LINUX   := -D LINUX

## Mac OSX -- For OSX version 10.7 and 10.8
#CXXFLAGS_MAC     := -D OSX -mmacosx-version-min=10.7 -stdlib=libc++

## Mac OSX -- For OSX version 10.9 or higher
CXXFLAGS_MAC     := -D OSX

## Windows
CXXFLAGS_WINDOWS := -D WIN32


################################################################################
## Add OS-specific compiler flags.
################################################################################
CXXFLAGS_OS_SPEC :=

BUILD_OS   :=
BUILD_ARCH :=
BUILD_SYS  :=

ifeq ($(OS),Windows_NT)
	## Windows system
    CXXFLAGS_OS_SPEC += ${CXXFLAGS_WINDOWS}
    BUILD_OS = Windows

    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CXXFLAGS_OS_SPEC += -D AMD64
        BUILD_ARCH = 64-bit
    else ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        CXXFLAGS_OS_SPEC += -D AMD64
        BUILD_ARCH = 64-bit
    else ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        CXXFLAGS_OS_SPEC += -D IA32
        BUILD_ARCH = 32-bit
    endif

else
	## Unix/Linux system
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CXXFLAGS_OS_SPEC += ${CXXFLAGS_LINUX}
        BUILD_OS = Linux
    else ifeq ($(UNAME_S),Darwin)
        CXXFLAGS_OS_SPEC += ${CXXFLAGS_MAC}
        BUILD_OS = Mac OS
    endif

    UNAME_M := $(shell uname -m)
    ifeq ($(UNAME_M),x86_64)
        CXXFLAGS_OS_SPEC += -D AMD64
        BUILD_ARCH = 64-bit
    else ifneq ($(filter %86,$(UNAME_M)),)
        CXXFLAGS_OS_SPEC += -D IA32
        BUILD_ARCH = 32-bit
    else ifneq ($(filter arm%,$(UNAME_M)),)
        CXXFLAGS_OS_SPEC += -D ARM
        BUILD_ARCH = ARM
    endif
endif

BUILD_SYS = ${BUILD_OS} ${BUILD_ARCH}
BUILD_SYS_UPPERCASE := $(shell echo ${BUILD_SYS} | tr '[:lower:]' '[:upper:]')


################################################################################
## Generate lower-cased and upper-cased strings of the configuration name.
## This make configurations case-insensitive.
################################################################################
CONF_LOWERCASE := $(shell echo ${CONF} | tr '[:upper:]' '[:lower:]')
CONF_UPPERCASE := $(shell echo ${CONF} | tr '[:lower:]' '[:upper:]')


################################################################################
## Validate the selected configuration and
## add configuration-specific compiler flags.
################################################################################
CONF_VALID := 1
ifeq (${CONF_LOWERCASE}, release)
    CXXFLAGS += ${CXXFLAGS_COMMON} ${CXXFLAGS_RELEASE} 
else ifeq (${CONF_LOWERCASE}, debug)
    CXXFLAGS += ${CXXFLAGS_COMMON} ${CXXFLAGS_DEBUG} 
else ifeq (${CONF_LOWERCASE}, profile)
    CXXFLAGS += ${CXXFLAGS_COMMON} ${CXXFLAGS_PROFILE}
else ifeq (${CONF_LOWERCASE}, shlib)
    CXXFLAGS += ${CXXFLAGS_COMMON} ${CXXFLAGS_SHLIB}
    LDFLAGS  += ${LDFLAGS} ${LDFLAGS_SHLIB}
else
	# Reaching here means invalid configuration.
    CONF_VALID := 0
endif
################################################################################
