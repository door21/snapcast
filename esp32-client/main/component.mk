#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_PRIV_INCLUDEDIRS  := ../../common/ ../../client
CXXFLAGS += -D NO_CPP11_STRING -fexceptions