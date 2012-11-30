# - Try to find LibUnitTest++
# Once done this will define
#  LIBUNITTESTPP_FOUND - System has lib
#  LIBUNITTESTPP_INCLUDE_DIRS - The lib include directories
#  LIBUNITTESTPP_LIBRARIES - The libraries needed to use the lib
#  LIBUNITTESTPP_DEFINITIONS - Compiler switches required for using the lib

#set(LIPUNITTESTPP_WORK_PATH "/data/infinity/ut1kab/xp/build/UnitTest++-native")
set(LIPUNITTESTPP_WORK_PATH "/Home/ut1kab/xp/UnitTest++-32")
set(LIPUNITTESTPP_HOME_PATH "/srv/dev/tools/build44/UnitTest++")

find_package(PkgConfig)
pkg_check_modules(PC_LIBUNITTESTPP QUIET libUnitTest++)
set(LIBXML2_DEFINITIONS ${PC_LIBUNITTESTPP_CFLAGS_OTHER})

find_path(LIBUNITTESTPP_INCLUDE_DIR src/UnitTest++.h
          HINTS ${PC_LIBUNITTESTPP_INCLUDEDIR} ${PC_LIBUNITTESTPP_INCLUDE_DIRS} ${LIPUNITTESTPP_WORK_PATH} ${LIPUNITTESTPP_HOME_PATH}
          PATH_SUFFIXES src )
set(LIBUNITTESTPP_INCLUDE_DIR ${LIBUNITTESTPP_INCLUDE_DIR}/src)

find_library(LIBUNITTESTPP_LIBRARY NAMES UnitTest++ libUnitTest++
             HINTS ${PC_LIBUNITTESTPP_LIBDIR} ${PC_LIBUNITTESTPP_LIBRARY_DIRS} ${LIPUNITTESTPP_WORK_PATH} ${LIPUNITTESTPP_HOME_PATH} )

set(LIBUNITTESTPP_LIBRARIES ${LIBUNITTESTPP_LIBRARY} )
set(LIBUNITTESTPP_INCLUDE_DIRS ${LIBUNITTESTPP_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBxxx_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libUnitTest++  DEFAULT_MSG
                                  LIBUNITTESTPP_LIBRARY LIBUNITTESTPP_INCLUDE_DIR)

mark_as_advanced(LIBUNITTESTPP_INCLUDE_DIR LIBUNITTESTPP_LIBRARY )
