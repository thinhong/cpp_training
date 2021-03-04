################################################################################
##  This file should contain only project-specific preprocessor macros.       ##
##  (PRJ_SHOW_MACRO_USAGE is set in "makefile-inc/Project-variables.mk")      ##
################################################################################
## The following three global macros will automatically be passed to the
## compiler (see  "makefile-inc/Makefile.mk")
CXXMACRO_STRINGS :=
CXXMACRO_NUMBERS :=
CXXMACRO_FLAGS   :=



################################################################################
##             Add project-specific macros into the global macros             ##
################################################################################
## The fallback P-table file of 3Seq
# FALLBACK_PVT=/home/lamhm/PVT_100
#FALLBACK_PVT=
#ifneq ($(FALLBACK_PVT),)
#	CXXMACRO_STRINGS += -DFALLBACK_PVT=${FALLBACK_PVT}
#endif


## Set this to 1 (one) to enable RCpp support. This flag should only be enable
## when building the R package (make sure you have the RCpp library installed).
#HAS_RCPP=0
#ifeq ($(HAS_RCPP),1)
#	CXXMACRO_FLAGS += -DHAS_RCPP=1
#endif



################################################################################
##          Show usage for the macros that are defined in this file           ##
################################################################################
.help-proj-preprocessor:
#ifneq (${PRJ_SHOW_MACRO_USAGE},0)
#	@#    "--------------this-is-a-hidden-60-char-ruller---------------"
#	@echo "C++ Macros:"
#	@echo "    FALLBACK_PVT=<PATH_TO_PTABLE>"
#	@echo "            Specify the path to a fallback P-table file."
#	@echo "            This file will only be used if the software is"
#	@echo "            not associated with any P-table file."
#	@echo "            Default value:  none."
#	@echo "    HAS_RCPP=<0|1>"
#	@echo "            Set this flag to 1 (one) to enable RCpp support."
#	@echo "            This flag should only be enable when building"
#	@echo "            the R package. The RCpp library is required for"
#	@echo "            this to work."
#	@echo "            Default value:  0 (disabled)."
#	@echo ""
#	@#    "--------------this-is-a-hidden-60-char-ruller---------------"
#endif
#	@# Do nothing

################################################################################
