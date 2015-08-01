#ifndef _FS_BUFFER_H
#define _FS_BUFFER_H
#include "FS_type.h"

struct {
    UINT size;
    UINT len;
    CHAR raw[1]; //must be last
}buffer_t;
typedef struct buffer_t FS_BUFFER;
#endif
