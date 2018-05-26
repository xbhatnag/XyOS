#ifndef _MMAP_IO_H_
#define _MMAP_IO_H_
enum {
  MMAP_IO_BASE = 0x3F000000,
  MMAP_IO_END = 0x3FFFFFFF
};

void set(int addr, int value);
int get(int addr);
#endif
