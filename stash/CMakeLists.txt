CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

### Top cmake file ###

# This cmake forwards to another makefile.
# We just put here what cannot be put into the other makefile.
# (nothing so far)

# Redirect to another cmakefile.
# Since this other cmakefile is not in a subdirectory of ours,
# we must tell cmake in which subdirectory of "build" to put the result files.
# trick taken from http://...
ADD_SUBDIRECTORY(.. "${CMAKE_CURRENT_BINARY_DIR}/bin")
