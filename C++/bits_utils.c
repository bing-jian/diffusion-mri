#include "bits_utils.h"

/*
 * IsThisMachineLittleEndian
 *
 * Function attempts to determine if this machine stores
 * its values in little endian notation or not.  This is
 * used to determine whether the MIPS data must be
 * reversed or not.
 *
 * Function determines endianness by storing a known
 * sequence of bits and then observing what value is at
 * the "left" end.
 */
int is_this_machine_little_endian() {
  int x = 0x87654321;
  char* t =(char*) &x;
  return (t[0] ==(char)0x21);
}

void swap_bytes(unsigned char*x, unsigned size) {
  unsigned char c;
  unsigned short s;
  unsigned int l;
  switch (size) {
    case 2: /* swap two bytes */
      c = *x;
      *x = *(x + 1);
      *(x + 1) = c;
      break;
    case 4: /* swap two shorts(2 - byte words) */
      s = *(unsigned short *)x;
      *(unsigned short *)x = *((unsigned short *)x + 1);
      *((unsigned short *)x + 1) = s;
      swap_bytes((unsigned char *)x, 2);
      swap_bytes((unsigned char *)((unsigned short *)x + 1), 2);
      break;
    case 8: /* swap two ints(4 - bytes words) */
      l = *(unsigned int *)x;
      *(unsigned int *)x = *((unsigned int *)x + 1);
      *((unsigned int *)x + 1) = l;
      swap_bytes((unsigned char *)x, 4);
      swap_bytes((unsigned char *)((unsigned int *)x + 1), 4);
      break;
  }
}
