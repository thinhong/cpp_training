#
#  There exist several targets which are by default empty and which can be 
#  used for execution of your targets. These targets are usually executed 
#  before and after some main targets. They are: 
#
#     .build-pre:              called before 'build' target
#     .build-post:             called after 'build' target
#     .clean-pre:              called before 'clean' target
#     .clean-post:             called after 'clean' target
#     .clobber-pre:            called before 'clobber' target
#     .clobber-post:           called after 'clobber' target
#     .all-pre:                called before 'all' target
#     .all-post:               called after 'all' target
#     .help-pre:               called before 'help' target
#     .help-post:              called after 'help' target
#
#  Targets beginning with '.' are not intended to be called on their own.
#
#  Main targets can be executed directly, and they are:
#  
#     build                    build a specific configuration
#     clean                    remove built files from a configuration
#     clobber                  remove all built files
#     all                      build all configurations
#     help                     print help mesage
#  
#  Targets .build-impl, .clean-impl, .clobber-impl, .all-impl, and
#  .help-impl are implemented in makefile-inc/makefile-impl.mk.


# Environment 
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin



# Include make variables and the implementation makefile.
# (This order must not be changed)
include makefile-inc/Makefile-variables.mk
include makefile-inc/Makefile-impl.mk



PRJ_NAME_UPPERCASE := \
		$(shell echo ${PRJ_NAME} | tr '[:lower:]' '[:upper:]')

# build
build: .build-post

.build-pre:
	@echo ""
	@echo "------------------------------------------------------------"
	@echo "BUILDING ${CONF_UPPERCASE} CONFIGURATION ..."

.build-post: .build-impl
	@echo "------------------------------------------------------------"
	@echo "FINISHED BUILDING ${CONF_UPPERCASE} CONFIGURATION."
	@echo "------------------------------------------------------------"
	@echo ""


# clean
clean: .clean-post

.clean-pre:
	@echo ""
	@echo "------------------------------------------------------------"

.clean-post: .clean-impl
	@echo "------------------------------------------------------------"
	@echo ""


# clobber
clobber: .clobber-post

.clobber-pre:
	@echo ""
	@echo "------------------------------------------------------------"
	@echo "CLEANING ALL CONFIGURATIONS..."
	@echo "------------------------------------------------------------"

.clobber-post: .clobber-impl
	@echo "------------------------------------------------------------"
	@echo "FINISHED CLEANING ALL CONFIGURATIONS."
	@echo "------------------------------------------------------------"
	@echo ""


# all
all: .all-post

.all-pre:
	@echo ""
	@echo "------------------------------------------------------------"
	@echo "BUILDING ALL CONFIGURATIONS ..."

.all-post: .all-impl
	@echo "------------------------------------------------------------"
	@echo "FINISHED BUILDING ALL CONFIGURATIONS."
	@echo "------------------------------------------------------------"
	@echo ""


# build tests
build-tests: .build-tests-post

.build-tests-pre:
# Add your pre 'build-tests' code here...

.build-tests-post: .build-tests-impl
# Add your post 'build-tests' code here...


# run tests
test: .test-post

.test-pre:
# Add your pre 'test' code here...

.test-post: .test-impl
# Add your post 'test' code here...


# help
help: .help-post

.help-pre:
	@echo ""
	@echo "------------------------------------------------------------"
	@echo "${PRJ_NAME_UPPERCASE}  --  MAKEFILE USAGE"
	@echo "------------------------------------------------------------"

.help-post: .help-impl .help-proj-preprocessor
	@echo "------------------------------------------------------------"
	@echo ""
