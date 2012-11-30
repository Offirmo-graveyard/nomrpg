### Adjust compiler flags

## customize existing build types
message("* Adjusting flags...")

### define sets of flags for GCC
include(build/flags.gcc.cmake)

### Apply our gcc flags
if(${CMAKE_COMPILER_IS_GNUCC})

	## available only in 2.8.8+ ?
	#message("CMAKE_C_COMPILER_VERSION        = ${CMAKE_C_COMPILER_VERSION}")
	#message("CMAKE_CXX_COMPILER_VERSION      = ${CMAKE_CXX_COMPILER_VERSION}")

	## common flags
	SET( CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS} ${GCC_COMMON_FLAGS}" )
	SET( CMAKE_EXE_LINKER_FLAGS   "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COMMON_FLAGS}" )
	# ...

	## warnings (if enabled)
	if(${EXTENSIVE_WARNINGS})
		SET( CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS} ${GCC_COMMON_WARNING_FLAGS} ${GCC_CXX_WARNING_FLAGS}" )
	endif()
	
endif()


if(${COVERAGE})
	if(NOT ${CMAKE_COMPILER_IS_GNUCC})
		# a message was already displayed to complain about that
	else()
		SET( CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
		SET( CMAKE_EXE_LINKER_FLAGS   "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )
	endif()
endif()


# message("CMAKE_COMPILE_FLAGS             = ${CMAKE_COMPILE_FLAGS}")
# message("CMAKE_CXX_FLAGS                 = ${CMAKE_CXX_FLAGS}")
# message("CMAKE_CXX_FLAGS_INIT            = ${CMAKE_CXX_FLAGS_INIT}")
# message("CMAKE_CXX_FLAGS_DEBUG           = ${CMAKE_CXX_FLAGS_DEBUG}")
# get_target_property(TEMP ${EXEC_TARGET} COMPILE_FLAGS)
# message("target ${EXEC_TARGET} compile flags      = ${TEMP}")
# message("CMAKE_C_FLAGS_DEBUG             = ${CMAKE_C_FLAGS_DEBUG}")
# message("CMAKE_EXE_LINKER_FLAGS          = ${CMAKE_EXE_LINKER_FLAGS}")
# message("CMAKE_EXE_LINKER_FLAGS_INIT     = ${CMAKE_EXE_LINKER_FLAGS_INIT}")
# message("CMAKE_EXE_LINKER_FLAGS_DEBUG    = ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
# message("CMAKE_SHARED_LINKER_FLAGS_DEBUG = ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")

