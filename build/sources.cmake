### source files
# Note :
# - we use the globbing technique to detect source files.
#   While some people consider it bad, we found it fine.
# - we generate groups of sources that may or may not be used
#   in the build

message("* Looking for sources...")

# configure a header file to pass some of the CMake settings
# to the source code
CONFIGURE_FILE (
	"app/app_config.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/generated/app_config.h"
)

# App source
# first the model, usually plain C++
FILE(GLOB_RECURSE  APP_MODEL_SOURCE
		app/models/*
		app/helpers/*
	)
# then the view, usually tied to complex external libs
FILE(GLOB_RECURSE  APP_VIEW_SOURCE
		app/views/*
	)
# shared (third party ?) sources
FILE(GLOB_RECURSE  SHARED_SOURCE    shared/* )
# generated (if any) sources
# note : we explicitely reference some files
# in case they are not yet generated
FILE(GLOB_RECURSE  GENERATED_SOURCE
		generated/*
		generated/app_config.h
	)
# unit tests sources
FILE(GLOB_RECURSE  SPEC_SOURCE      spec/* )
#	spec/spec_check_launcher.cpp

IF(NOT ${USE_WT})
	# remove Wt-related sources
	message("* Disabling Wt framework...")
	# subpart of app sources that are tied to Wt framework

	###
	FILE(GLOB_RECURSE  APP_MODEL_SOURCE_WT
		app/helpers/wt_related/*
		app/models/wt_related/*
	)
	LIST(REMOVE_ITEM  APP_MODEL_SOURCE  ${APP_MODEL_SOURCE_WT} )
	###
	FILE(GLOB_RECURSE  APP_VIEW_SOURCE_WT
		app/views/wt/*
	)
	LIST(REMOVE_ITEM  APP_VIEW_SOURCE  ${APP_VIEW_SOURCE_WT} )
	###
	FILE(GLOB_RECURSE  SHARED_SOURCE_WT
		shared/off_spec/off_spec_wt_model.*
	)
	LIST(REMOVE_ITEM  SHARED_SOURCE  ${SHARED_SOURCE_WT} )
	###
	FILE(GLOB_RECURSE  SPEC_SOURCE_WT
		spec/helpers/wt_related/*
		spec/models/wt_related/*
		spec/spec_utils/wt_related/*
		spec/views/wt/*
	)
	LIST(REMOVE_ITEM  SPEC_SOURCE  ${SPEC_SOURCE_WT} )
endif()


##########################################################
## put them all together, according to the build options
## common
SET(ALL_SOURCE  ${APP_MODEL_SOURCE} ${SHARED_SOURCE} ${GENERATED_SOURCE} )
## specific
if(${EXEC_TYPE} STREQUAL "Full")
	# we add wt sources and entry point
	LIST(APPEND  ALL_SOURCE  ${APP_VIEW_SOURCE} app/main.cpp )
else()
	# we add specs sources and entry point
	LIST(APPEND  ALL_SOURCE  ${APP_VIEW_SOURCE} ${SPEC_SOURCE} )
endif()
#message("---\nAll source = ${ALL_SOURCE}")


### write the list of sources in a file for coverage and other hacks
## first copy the list of source
SET(ALL_SOURCE_FOR_COVERAGE ${ALL_SOURCE})
## then remove a few files that are not relevant in the coverage analysis
FILE(GLOB_RECURSE  IRRELEVANT_SOURCE_FOR_COVERAGE
		spec/spec_check_launcher.cpp
	)
LIST(REMOVE_ITEM  ALL_SOURCE_FOR_COVERAGE ${IRRELEVANT_SOURCE_FOR_COVERAGE} )
## now format the list of sources
string(REPLACE ";" "\n" ALL_SOURCE_FOR_COVERAGE "${ALL_SOURCE_FOR_COVERAGE}")
## message("All source = ${ALL_SOURCE_FOR_COVERAGE}")
## eventually, write this list into a file
file(WRITE ${SOURCE_LIST_FILE_NAME} "${ALL_SOURCE_FOR_COVERAGE}" )

