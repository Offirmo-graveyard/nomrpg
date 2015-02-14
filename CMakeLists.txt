### global cmake file ###
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# enable more verbosity :
# cf. http://www.cmake.org/Wiki/CMake_FAQ#Is_there_an_option_to_produce_more_.27verbose.27_compiling.3F
set(CMAKE_VERBOSE_MAKEFILE ON)

include(build/constants.cmake)
include(build/special_build_options.cmake)
include(build/build_options.cmake)
include(build/libs.cmake)
include(build/sources.cmake)
include(build/exec.cmake)
include(build/targets.cmake)
include(build/includes.cmake)
include(build/flags.cmake)


#Sends the C++ 2011 enabling flag to the gcc compiler
#add_definitions(-std=c++11)

#http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt03ch17s02.html
#add_definitions(-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC)

### misc
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

if(${COMPUTER_NAME} STREQUAL "special-x86-32computer")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32") # generate code for a 32 bits environment, cf. http://gcc.gnu.org/onlinedocs/gcc-4.6.2/gcc/i386-and-x86_002d64-Options.html#i386-and-x86_002d64-Options
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()
