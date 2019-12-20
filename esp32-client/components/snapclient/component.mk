#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

SNAP_CLIENT := ../../../client
SNAP_COMMON := ../../../common
COMPONENT_DEPENDS := libflac
COMPONENT_PRIV_INCLUDEDIRS  := ../../../common/ ../../../client/ ../libflac/ 
COMPONENT_OBJS := ../../../client/player/player.o  \
					../../../client/controller.o \
					../../../client/client_connection.o \
					$(SNAP_CLIENT)/player/esp32_player.o \
					$(SNAP_CLIENT)/time_provider.o \
					$(SNAP_CLIENT)/stream.o \
					$(SNAP_COMMON)/sample_format.o \
					./esp32-workaround.o \
					$(SNAP_CLIENT)/decoder/pcm_decoder.o \
					$(SNAP_CLIENT)/decoder/flac_decoder.o
COMPONENT_SRCDIRS := . ../../../client/player ../../../client $(SNAP_COMMON) $(SNAP_CLIENT)/decoder
CXXFLAGS += -D NO_CPP11_STRING -fexceptions -D HAS_FLAC -DVERSION=\"v0.0.1\"
