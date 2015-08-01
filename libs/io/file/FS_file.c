#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "FS_file.h"
#include "FS_buffer.h"

int FS_openfile( CONSTCHAR *path ){
    if ( path == NULL ){
        return -1;
    }
return open(path, O_APPEND|O_CREAT, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR|S_IRWXG|S_IRGRP);
}

int FS_write(int fd, FS_BUFFER *buffer){
    int r = 0;
    int remain = 0;
    r = (int)write(fd, buffer->raw, *buffer->len);
    if( r < 0){
        return -1;
    }

    remain = (int)buffer->len - r;
    if(remain > 0){
        (void)memmove(buffer->raw, buffer->raw[r], remain);
        buffer->len = (UINT)remain; 
    }
   return remain;
}

int FS_close(int fd){
    if ( fd < 0 ) return -1;
    return close(fd);
}
