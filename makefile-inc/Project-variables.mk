## Project Name
PRJ_NAME := DiscreteTimeModel

## The folder that contains all the source files
PRJ_SOURCEROOT := src

## The folder that will contain the final built files
PRJ_EXEDIR := .

## Name of the executable file
PRJ_EXENAME :=${PRJ_NAME}

## Name of the shared library file (when the SHLIB configuration is used)
PRJ_SHLIBNAME :=lib${PRJ_NAME}.so

## Include directories
## Example:  PRJ_INCLUDE:= -I./include -I/usr/share/include
PRJ_INCLUDE := -I./src

## Link library folders
## Example:  PRJ_LIBDIR:= -L./lib
PRJ_LIBDIR :=

## Link library names (without the "lib" prefix and ".so" suffix)
## Example:  PRJ_LIBNAME:= -lpthread
PRJ_LIBNAME :=


## Set the following variable to 1 if you want to show the usage of
## project-specific preprocessors.  Setting it to 0 will empty the
## ".help-proj-preprocessor" target.
## This variable is recalled in 2 places:
##     "makefile-inc/Makefile-help.mk"
##     "makefile-inc/Project-preprocessors.mk"
PRJ_SHOW_MACRO_USAGE := 1


## Include project-specific preprocessors
include makefile-inc/Project-preprocessors.mk
