#ifndef _FS_FILE_H
 #define _FS_FILE_H
#include "FS_type.h"
#include "FS_buffer.h"

int FS_openfile( CONSTCHAR *path );
int FS_write(int fd, FS_BUFFER *buffer);
int FS_writef();
int FS_close(int fd);
extern char *path;
#endif
