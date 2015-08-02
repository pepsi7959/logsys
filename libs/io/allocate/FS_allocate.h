#ifndef _FS_ALLOCATE_H
 #define _FS_ALLOCATE_H
#include <stdio.h>
#define FS_MALLOC(_sz) malloc(_sz) 
#define FS_CALLOC(_tsize, _sz) calloc(_tsize, _sz)
#endif
