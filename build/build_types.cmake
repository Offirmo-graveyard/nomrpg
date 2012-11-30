# customize existing build types
message("* Ajusting default build types...")

# build for code coverage
# cf. http://gcc.gnu.org/onlinedocs/gcc/Gcov-Intro.html#Gcov-Intro
# "You should compile your code without optimization if you plan to use gcov"
# message("CMAKE_CXX_FLAGS_DEBUG           = ${CMAKE_CXX_FLAGS_DEBUG}")
# message("CMAKE_C_FLAGS_DEBUG             = ${CMAKE_C_FLAGS_DEBUG}")
# message("CMAKE_EXE_LINKER_FLAGS_DEBUG    = ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
# message("CMAKE_SHARED_LINKER_FLAGS_DEBUG = ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
SET(GCC_DEBUG_FLAGS "-g -Wall")
SET(CMAKE_CXX_FLAGS_DEBUG
	"${GCC_DEBUG_FLAGS}"
	CACHE STRING "Flags used by the C++ compiler during coverage builds."
	FORCE )
SET(CMAKE_C_FLAGS_DEBUG
	"${GCC_DEBUG_FLAGS}"
	CACHE STRING "Flags used by the C compiler during coverage builds."
	FORCE )

# Add new build types
message("* Adding build types...")
SET(CMAKE_CXX_FLAGS_COVERAGE
	"${GCC_DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
	CACHE STRING "Flags used by the C++ compiler during coverage builds."
	FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
	"${GCC_DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
	CACHE STRING "Flags used by the C compiler during coverage builds."
	FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
	"-lgcov"
	CACHE STRING "Flags used for linking binaries during coverage builds."
	FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
	""
	CACHE STRING "Flags used by the shared libraries linker during coverage builds."
	FORCE )
MARK_AS_ADVANCED(
	CMAKE_CXX_FLAGS_COVERAGE
	CMAKE_C_FLAGS_COVERAGE
	CMAKE_EXE_LINKER_FLAGS_COVERAGE
	CMAKE_SHARED_LINKER_FLAGS_COVERAGE )



