#pragma once
#if 0
#include <stdint.h>  // uint*_t

#include "group.h"

#define BHP_CHUNK_SIZE (3)

//#define BHP_512_NUM_WINDOWS (6)
//#define BHP_512_WINDOW_SIZE (43)

#define BHP_1024_NUM_WINDOWS (8)
#define BHP_1024_WINDOW_SIZE (54)

//extern const group_t bhp_512_base[BHP_512_NUM_WINDOWS*BHP_512_WINDOW_SIZE*8];
//extern const bn_group_t bhp_512_base_bn[BHP_512_NUM_WINDOWS*BHP_512_WINDOW_SIZE*8];
//extern const compressed_group_t bhp_512_base_compressed[BHP_512_NUM_WINDOWS*BHP_512_WINDOW_SIZE*8];

extern const group_t bhp_1024_base[BHP_1024_NUM_WINDOWS*BHP_1024_WINDOW_SIZE*8];
extern const bn_group_t bhp_1024_base_bn[BHP_1024_NUM_WINDOWS*BHP_1024_WINDOW_SIZE*8];
extern const compressed_group_t bhp_1024_base_compressed[BHP_1024_NUM_WINDOWS*BHP_1024_WINDOW_SIZE*8];
#endif
