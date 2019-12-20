#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_DEPENDS := snapclient
COMPONENT_PRIV_INCLUDEDIRS  := ../../common/ ../../client/
COMPONENT_SRC_DIRS := . ../../client/
CXXFLAGS += -D NO_CPP11_STRING -fexceptions -D HAS_FLAC