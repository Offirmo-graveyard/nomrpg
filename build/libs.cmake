### detect needed libs
message("* Looking for needed libs...")

# add new modules :
# cf. http://stackoverflow.com/questions/10765885/how-to-install-your-custom-cmake-find-module
# http://www.vtk.org/Wiki/CMake/Examples#Specifying_where_to_look_for_.cmake_files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/cmake-modules/")
# message(${CMAKE_MODULE_PATH})

# use boost
# cf. module cmake FindBoost (usually in /usr/share/cmake-2.8/Modules/FindBoost.cmake)
SET(Boost_DEBUG                 ON) # activate debug output for the FindBoost cmake module
SET(Boost_USE_MULTITHREADED     ON)
SET(Boost_DETAILED_FAILURE_MSG  ON)

if(${COMPUTER_NAME} STREQUAL "cmxd7095")
	SET(Boost_NO_SYSTEM_PATHS       TRUE)
	SET(BOOST_ROOT                  /Soft/infinityFO/boost)
elseif(${COMPUTER_NAME} STREQUAL "toto")
	SET(Boost_NO_SYSTEM_PATHS       TRUE)
	SET(BOOST_ROOT                  /srv/dev/tools/gcc44/boost)
endif()

#FIND_PACKAGE( Boost 1.47.0 )
FIND_PACKAGE( Boost 1.40.0 COMPONENTS system program_options filesystem)
message("Boost_FOUND = ${Boost_FOUND}")
message("Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
message("Boost_LIBRARIES = ${Boost_LIBRARIES}")
message("Boost_SYSTEM_LIBRARY = ${Boost_SYSTEM_LIBRARY}")

FIND_PACKAGE( UnitTest++ )
# message("\n=== UnitTest++ ===")
# message("utpp found = ${LIBUNITTESTPP_FOUND}")
# message("utpp inc   = ${LIBUNITTESTPP_INCLUDE_DIRS}")
# message("utpp lib   = ${LIBUNITTESTPP_LIBRARIES}")
# message("utpp defs  = ${LIBUNITTESTPP_DEFINITIONS}")

if(${USE_WT})
FIND_PACKAGE( Wt REQUIRED )
# option specific to Wt
SET (WT_CONNECTOR "wthttp" CACHE STRING "Connector used (wthttp or wtfcgi)")
# message("Wt_LIBRARY = ${Wt_LIBRARY}")
# message("Wt_DEBUG_LIBRARY = ${Wt_DEBUG_LIBRARY}")
# message("Wt_DBOSQLITE3_LIBRARY = ${Wt_DBOSQLITE3_LIBRARY}")
# message("Wt_DBOSQLITE3_DEBUG_LIBRARY = ${Wt_DBOSQLITE3_DEBUG_LIBRARY}")
add_definitions( -DUSE_WT=1 )
endif()

	#TARGET_LINK_LIBRARIES( ${MODEL_SPEC_TARGET}
		#${Wt_DEBUG_LIBRARY}
		#${Wt_DBOSQLITE3_DEBUG_LIBRARY}
		
		#${Boost_FILESYSTEM_LIBRARY}
	#)


# ${Boost_system_LIBRARY}
# ${Boost_filesystem_LIBRARY}
# ${Boost_program_options_LIBRARY}

# ${LIBUNITTESTPP_LIBRARIES}

# ${Wt_DEBUG_LIBRARY}        # or {Wt_LIBRARY}
# ${Wt_HTTP_DEBUG_LIBRARY}   # or {Wt_HTTP_LIBRARY}
# ${Wt_EXT_DEBUG_LIBRARY}    # or {Wt_EXT_LIBRARY}
# ${Wt_DBO_DEBUG_LIBRARY}
# ${Wt_DBOSQLITE3_LIBRARY}
#/srv/dev/tools/gcc44/wt/lib/libwtd.so
#/srv/dev/tools/gcc44/wt/lib/libwthttpd.so
#/srv/dev/tools/gcc44/wt/lib/libwtextd.so
#/srv/dev/tools/gcc44/wt/lib/libwtdbod.so
#/srv/dev/tools/gcc44/wt/lib/libwtdbosqlite3d.so


##########################################################
## put them all together, according to the build options
## common (none for now)
SET( ALL_LIBS )
## specific
if(${EXEC_TYPE} STREQUAL "Full")
	LIST(APPEND  ALL_LIBS  ${Wt_DEBUG_LIBRARY} ${Wt_DBOSQLITE3_DEBUG_LIBRARY} ${Wt_HTTP_DEBUG_LIBRARY} )
else()
	LIST(APPEND  ALL_LIBS  ${Wt_DEBUG_LIBRARY} ${Wt_DBOSQLITE3_DEBUG_LIBRARY} )
	LIST(APPEND  ALL_LIBS  ${LIBUNITTESTPP_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Wt_TEST_DEBUG_LIBRARY} )
endif()
#message("---\nAll libs = ${ALL_LIBS}")
