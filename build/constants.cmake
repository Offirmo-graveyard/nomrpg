### Constants used in the build

PROJECT(TestApp)

ENABLE_LANGUAGE(CXX)

## The version number.
SET(VERSION_MAJOR  0)
SET(VERSION_MINOR  1)

## detect computer name
site_name(COMPUTER_NAME)

## We will generate a list of the sources in a text file
SET(SOURCE_LIST_FILE_NAME ${CMAKE_BINARY_DIR}/source_list.txt)


## Debug
message("\n")
message("* Starting build of ${PROJECT_NAME} ${VERSION_MAJOR}.${VERSION_MINOR}...")
message("  - Building on '${CMAKE_SYSTEM}' for '${CMAKE_GENERATOR}'")
message("  - source dir = CMAKE_HOME_DIRECTORY = ${CMAKE_HOME_DIRECTORY}")
message("  - build dir  = CMAKE_BINARY_DIR     = ${CMAKE_BINARY_DIR}")
message("* COMPUTER_NAME = '${COMPUTER_NAME}'")
