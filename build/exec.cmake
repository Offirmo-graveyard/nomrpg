### generated executable


# reset
SET( EXEC_CMD )
SET( EXEC_PARAMS )

if(${EXEC_TYPE} STREQUAL "Full")
	# name of the generated executable
	SET( EXEC_TARGET wt_debug )
	# invocation is complicated
	SET( EXEC_PARAMS "--http-port" "8080" "--http-addr" "0.0.0.0" "--docroot" "../nomrpg/public" "-c" "../nomrpg/config/wt_config.xml" )
else()
	# name of the generated executable
	SET( EXEC_TARGET ut_model )
	# simple invocation, no params
endif()

ADD_EXECUTABLE( ${EXEC_TARGET}  ${ALL_SOURCE} )
TARGET_LINK_LIBRARIES( ${EXEC_TARGET}  ${ALL_LIBS} )

