#pragma once

#ifdef PRINTF
#undef PRINTF
#endif
#define PRINTF(...) \
    do {            \
    } while (0)