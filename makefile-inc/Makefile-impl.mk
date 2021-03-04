## Implement project-specific help message
include makefile-inc/Makefile-help.mk


# Building and Cleaning subprojects are done by default, but can be controlled with the SUB
# macro. If SUB=no, subprojects will not be built or cleaned. The following macro
# statements set BUILD_SUB-CONF and CLEAN_SUB-CONF to .build-reqprojects-conf
# and .clean-reqprojects-conf unless SUB has the value 'no'
SUB_no=NO
SUBPROJECTS=${SUB_${SUB}}
BUILD_SUBPROJECTS_=.build-subprojects
BUILD_SUBPROJECTS_NO=
BUILD_SUBPROJECTS=${BUILD_SUBPROJECTS_${SUBPROJECTS}}
CLEAN_SUBPROJECTS_=.clean-subprojects
CLEAN_SUBPROJECTS_NO=
CLEAN_SUBPROJECTS=${CLEAN_SUBPROJECTS_${SUBPROJECTS}}


# Suppress printing directory if not in verbose mode
ifndef VERBOSE
	MAKEFLAGS += --no-print-directory
endif


# build
.build-impl: .build-pre .validate-impl 
	@#echo "=> Running $@... Configuration=$(CONF)"
	@"${MAKE}" -f makefile-inc/Makefile.mk QMAKE=${QMAKE} SUBPROJECTS=${SUBPROJECTS} .build-conf


# clean
.clean-impl: .clean-pre .validate-impl 
	@#echo "=> Running $@... Configuration=$(CONF)"
	@"${MAKE}" -f makefile-inc/Makefile.mk QMAKE=${QMAKE} SUBPROJECTS=${SUBPROJECTS} .clean-conf


# clobber 
.clobber-impl: .clobber-pre 
	@#echo "=> Running $@..."
	@for CONF in ${ALLCONFS}; \
	do \
	    "${MAKE}" -f makefile-inc/Makefile.mk CONF=$${CONF} QMAKE=${QMAKE} SUBPROJECTS=${SUBPROJECTS} .clean-conf; \
	done

# all 
.all-impl: .all-pre 
	@#echo "=> Running $@..."
	@for CONF in ${ALLCONFS}; \
	do \
	    "${MAKE}" -f makefile-inc/Makefile.mk CONF=$${CONF} QMAKE=${QMAKE} SUBPROJECTS=${SUBPROJECTS} .build-conf; \
	done

# build tests
.build-tests-impl: .build-impl .build-tests-pre
	@#echo "=> Running $@... Configuration=$(CONF)"
	@"${MAKE}" -f makefile-inc/Makefile.mk SUBPROJECTS=${SUBPROJECTS} .build-tests-conf

# run tests
.test-impl: .build-tests-impl .test-pre
	@#echo "=> Running $@... Configuration=$(CONF)"
	@"${MAKE}" -f makefile-inc/Makefile.mk SUBPROJECTS=${SUBPROJECTS} .test-conf

# configuration validation
.validate-impl:
	@#echo "=> Running $@... Configuration=$(CONF)"
	@"${MAKE}" -f makefile-inc/Makefile.mk SUBPROJECTS=${SUBPROJECTS} .validate
