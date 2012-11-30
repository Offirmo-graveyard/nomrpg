### Add new build options
### Those "regular" build options can be change with ccmake

## ON/OFF coverage option
option(COVERAGE  "Activate gathering of coverage informations. Available : ON, OFF."  "ON" )

## ON/OFF maximal warning option
option(EXTENSIVE_WARNINGS  "Activate all available warnings. Available : ON, OFF."  "ON" )

## type of exec generated
set(EXEC_TYPE "UT" CACHE STRING "Select the generated executable type. Available : Full, UT.")

## set the default build type if none set
## http://www.cmake.org/Wiki/CMake_FAQ#How_can_I_change_the_default_build_mode_and_see_it_reflected_in_the_GUI.3F
IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug
	    CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel."
	    FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
message("* Current build type is : ${CMAKE_BUILD_TYPE}")



########### check for options coherency
if(${COVERAGE})
	message("* Activating coverage...")
	if(NOT ${CMAKE_COMPILER_IS_GNUCC})
		message(WARNING "Coverage is not supported for this compiler")
	else()
		if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			message(WARNING "Coverage requires debug mode")
		endif()
	endif()
endif()