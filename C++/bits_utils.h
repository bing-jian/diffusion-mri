#ifndef BITS_UTILS_H_
#define BITS_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

int is_this_machine_little_endian();
void swap_bytes(unsigned char*, unsigned size);

#ifdef __cplusplus
}
#endif

#endif  /* BITS_UTILS_H_ */
