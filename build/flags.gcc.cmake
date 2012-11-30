### define sets of flags for GCC

## hat tip to cmaker : http://stackoverflow.com/a/6570687/587407
exec_program(${CMAKE_CXX_COMPILER}
             ARGS              --version
             OUTPUT_VARIABLE   _compiler_output)
string(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1" GCC_VERSION ${_compiler_output})
message(STATUS "GNU C++ compiler version: ${GCC_VERSION} [${CMAKE_CXX_COMPILER}]")
#if(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
#...
#if(gcc_compiler_version VERSION_GREATER "4.5.99")

### common flags
## -fmessage-length=0 : do not wrap output lines (good for big consoles)
SET( GCC_COMMON_FLAGS   "-fmessage-length=0" )

### debug flags
## http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
## Note : note sure if -g options cancel out each other or not...
## I believe -ggdb3 imply -g, -g3 and -ggdb
SET( GCC_DEBUG_FLAGS   "-O0 -ggdb3" )

### coverage flags
## cf. http://gcc.gnu.org/onlinedocs/gcc/Gcov-Intro.html
##     http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
## "You should compile your code without optimization if you plan to use gcov"
#SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
#SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
## Note : I just found --coverage, a shortcut easier to use
SET(GCC_COVERAGE_COMPILE_FLAGS "--coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "--coverage")

### Warning flags
## see http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html

#######
if(GCC_VERSION VERSION_GREATER "4.8")
# not available in my current gcc :
#  -Wdouble-promotion -Wnoexcept -Wuseless-cast -Wzero-as-null-pointer-constant

#######
elsif(GCC_VERSION VERSION_GREATER "4.1.2")
## note : -Wpedantic works only on recent gcc (>4.1 ?)
## -Wconversion : Warn for implicit conversions that may alter a value.
## -Wfloat-equal : Warn if floating-point values are used in equality comparisons.
## -Wformat : Check calls to printf and scanf, etc., to make sure that the arguments supplied...
##            Note : included in Wall but not with level 2
## -Winit-self : Warn about uninitialized variables that are initialized with themselves
## -Winline : Warn if a function that is declared as inline cannot be inlined.
## ...
SET( GCC_COMMON_WARNING_FLAGS   "-pedantic -Wall -Wextra -Wconversion -Wfloat-equal -Wformat=2 -Winit-self -Winline -Winvalid-pch -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstack-protector -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum" )
## selectively disable some warnings with additional flags
SET( GCC_COMMON_WARNING_FLAGS   "${GCC_COMMON_WARNING_FLAGS} -Wno-unused-parameter" )
## -Wctor-dtor-privacy: Warn when a class seems unusable because all the constructors or destructors in that class are private, and it has neither friends nor public static member functions.
SET( GCC_CXX_WARNING_FLAGS      "-Wctor-dtor-privacy" )
## other flags
# not useful eventually :
# -Weffc++ -Waggregate-return

#######
else()
## gcc 4.1.2 : -Wmissing-declarations" is valid for C/ObjC but not for C++
##             unrecognized command line option "-Wlogical-op"

## -Wconversion : Warn for implicit conversions that may alter a value.
## -Wfloat-equal : Warn if floating-point values are used in equality comparisons.
## -Wformat : Check calls to printf and scanf, etc., to make sure that the arguments supplied...
##            Note : included in Wall but not with level 2
## -Winit-self : Warn about uninitialized variables that are initialized with themselves
## -Winline : Warn if a function that is declared as inline cannot be inlined.
## ...
SET( GCC_COMMON_WARNING_FLAGS   "-pedantic -Wall -Wextra -Wconversion -Wfloat-equal -Wformat=2 -Winit-self -Winline -Winvalid-pch -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstack-protector -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum" )
## selectively disable some warnings with additional flags
SET( GCC_COMMON_WARNING_FLAGS   "${GCC_COMMON_WARNING_FLAGS} -Wno-unused-parameter" )
## -Wctor-dtor-privacy: Warn when a class seems unusable because all the constructors or destructors in that class are private, and it has neither friends nor public static member functions.
SET( GCC_CXX_WARNING_FLAGS      "-Wctor-dtor-privacy" )
## other flags
# not useful eventually :
# -Weffc++ -Waggregate-return
endif()
#######

#MARK_AS_ADVANCED(
#		GCC_COMMON_FLAGS
#		GCC_DEBUG_FLAGS
#		GCC_COVERAGE_COMPILE_FLAGS
#		GCC_COVERAGE_LINK_FLAGS
#		GCC_COMMON_WARNING_FLAGS
#		GCC_CXX_WARNING_FLAGS
#	)
