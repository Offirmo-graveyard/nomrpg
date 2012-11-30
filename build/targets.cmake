####### custom targets


### explicitely refresh cmake
## REM : -Wdev = Enable developer warnings.
ADD_CUSTOM_TARGET(cmake
		COMMENT "* Refreshing cmake..."
		COMMAND ${CMAKE_COMMAND} -Wdev ${CMAKE_HOME_DIRECTORY}
		VERBATIM
	)

### do nothing (test)
ADD_CUSTOM_TARGET(nothing
		COMMENT "* Doing nothing..."
		COMMAND echo -n ""
		VERBATIM
	)


### build & run the current exec
if(NOT DEFINED EXEC_CMD)
	SET( EXEC_CMD  ${EXEC_TARGET} )
endif()
# when using coverage, some cmds need to be added
SET( COVERAGE_PRE_EXEC_CMD_IF_NEEDED ) # default to nothing
SET( COVERAGE_POST_EXEC_CMD_IF_NEEDED ) # default to nothing
if(${COVERAGE} STREQUAL "ON")
	SET( COVERAGE_PRE_EXEC_CMD_IF_NEEDED ${CMAKE_CURRENT_SOURCE_DIR}/build/clean_temp_coverage_files.sh )
	SET( COVERAGE_POST_EXEC_CMD_IF_NEEDED echo "* Since coverage is activated, you may now type 'make cov' to see coverage results." )
endif()

# add the new target
ADD_CUSTOM_TARGET(run
		COMMENT "* running current executable..."
		COMMAND ${COVERAGE_PRE_EXEC_CMD_IF_NEEDED}
		COMMAND echo "* Starting the program..."
		COMMAND ${EXEC_CMD} ${EXEC_PARAMS}
		COMMAND ${COVERAGE_POST_EXEC_CMD_IF_NEEDED}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		VERBATIM
	)

# and a variant for valgrind
ADD_CUSTOM_TARGET(valrun
		COMMENT "* running current executable with valgrind..."
		COMMAND ${COVERAGE_PRE_EXEC_CMD_IF_NEEDED}
		COMMAND echo "* Starting the program..."
		COMMAND valgrind --trace-children=yes --track-fds=yes --gen-suppressions=all --leak-check=full ${EXEC_CMD} ${EXEC_PARAMS}
		COMMAND ${COVERAGE_POST_EXEC_CMD_IF_NEEDED}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		VERBATIM
	)

### special target for coverage
ADD_CUSTOM_TARGET(cov
		COMMENT "* Starting coverage analysis..."
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/run_coverage_analysis.sh ${SOURCE_LIST_FILE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${EXEC_TARGET}.dir/
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		VERBATIM
	)

