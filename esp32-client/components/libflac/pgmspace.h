// This is for libflac - it uses pgmspace from Arduino
// This file provides the bare-bones pgmspace utils, instead
// of pulling in the whole Arduino system

#ifndef PGMSPACE_H
#define PGMSPCACE_H
#define PROGMEM
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
#define pgm_read_word(addr) ({ \
  typeof(addr) _addr = (addr); \
  *(const unsigned short *)(_addr); \
})
#endif
