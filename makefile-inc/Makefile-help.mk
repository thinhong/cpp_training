################################################################################
##  This file should only contains the text that will be shown when users type:
##      $> make help
################################################################################

## PRJ_SHOW_MACRO_USAGE is set in  "makefile-inc/Project-variables.mk"
ifeq (${PRJ_SHOW_MACRO_USAGE},0)
    _TXT_MACRO_USAGE :=
else
    _TXT_MACRO_USAGE := [C++_MACROS]
endif


## help
.help-impl: .help-pre
	@#    "--------------this-is-a-hidden-60-char-ruller---------------"
	@echo ""
	@echo "This makefile supports the following configurations:"
	@echo "    ${ALLCONFS}"
	@echo ""
	@echo "and the following targets:"
	@echo "    build  [default]  :  build a specific configuration."
	@echo "    clean             :  clean a specific configuration."
	@echo "    all               :  build all configurations."
	@echo "    clobber           :  remove all built files from all"
	@echo "                         configurations."
	@echo "    help              :  prints this message."
	@echo ""
	@echo "Make Command:"
	@echo "    make build    [CONF=<CONFIGURATION>]  ${_TXT_MACRO_USAGE}"
	@echo "    make clean    [CONF=<CONFIGURATION>]"
	@echo "    make all      ${_TXT_MACRO_USAGE}"
	@echo "    make clobber"
	@echo "    make help"
	@echo ""
	@#    "--------------this-is-a-hidden-60-char-ruller---------------"
