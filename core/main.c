#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "FS_type.h"
#include "FS_buffer.h"

struct{
    FS_BUFFER *buffer[1024];
    pthread_mutex_t lock;
}log_t;

typedef struct log_t LOG;

LOG data;

//global variable

int main(){
    for(;;) {
        //producer();
        //customer();
        sleep(1);
    }
    
    return 0;
}

void producer(){
    
}

void customer(){
}
