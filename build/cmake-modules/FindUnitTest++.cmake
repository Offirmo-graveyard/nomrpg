# - Try to find LibUnitTest++
# Once done this will define
#  LIBUNITTESTPP_FOUND - System has lib
#  LIBUNITTESTPP_INCLUDE_DIRS - The lib include directories
#  LIBUNITTESTPP_LIBRARIES - The libraries needed to use the lib
#  LIBUNITTESTPP_DEFINITIONS - Compiler switches required for using the lib
#
# XXX This script is made to work with UnitTest++
#     installed with the C++VM tool https://github.com/Offirmo/cvm
#     It may NOT find a manual UnitTest++ install
#
# To direct the script to a particular Wt installation, use the
# standard cmake variables CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH
#
# inspired by Pau Garcia i Quiles's FindWt.cmake script

# simply look in cmake pathes
FIND_PATH( UnitTestPP_INCLUDE_DIR NAMES UnitTest++.h PATHS ENV PATH PATH_SUFFIXES Posix tests )

IF( UnitTestPP_INCLUDE_DIR )
	FIND_LIBRARY( UnitTestPP_LIBRARIES NAMES UnitTest++ libUnitTest++ )

	IF( UnitTestPP_INCLUDE_DIR AND UnitTestPP_LIBRARIES)
		SET( UnitTestPP_FOUND TRUE )
	ENDIF( )
ENDIF( UnitTestPP_INCLUDE_DIR )

 IF( UnitTestPP_FOUND )
	IF( NOT UnitTestPP_FIND_QUIETLY )
		MESSAGE(STATUS "Found the UnitTest++ libraries at ${UnitTestPP_LIBRARIES}")
		MESSAGE(STATUS "Found the UnitTest++ headers at ${UnitTestPP_INCLUDE_DIR}")
	ENDIF( NOT UnitTestPP_FIND_QUIETLY )
ELSE( UnitTestPP_FOUND )
	IF( UnitTestPP_FIND_REQUIRED )
		MESSAGE(FATAL_ERROR "Could NOT find UnitTest++")
	ENDIF( UnitTestPP_FIND_REQUIRED )
ENDIF(UnitTestPP_FOUND)

MARK_AS_ADVANCED( UnitTestPP_INCLUDE_DIR UnitTestPP_LIBRARIES )


