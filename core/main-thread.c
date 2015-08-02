#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "FS_type.h"
#include "FS_buffer.h"
#include "FS_allocate.h"
#include "FS_common.h"
#include "FS_file.h"
#include "FS_log.h"

struct FS_log_t{
    int slot_size;
    FS_BUFFER **buffer;
    int occupied;
    int idx_p;
    int idx_c;
    int finished;
    pthread_mutex_t lock;
    pthread_cond_t less;
    pthread_cond_t more;
    pthread_cond_t is_finished;
};

typedef struct FS_log_t LOG; LOG logger;
const char data[]= "abbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb0z";
//global variable

void producer();
void *customer( void *args );
int main(int argc, char *argv[]){
    int i = 0; 
    int r = 0; 
    int slot_size = atoi(argv[1]);
    int max_thread = atoi(argv[2]);

    FS_DEBUG("start...\n");
    FS_DEBUG("init buffer\n");
    FS_init_LOG(&logger, slot_size); 
    pthread_t pth[max_thread];
    for (i = 0; i < max_thread ; i++){
        pthread_create(&pth[i], NULL, customer, (void *)&logger );
    }
//    pthread_create(&pth[1], NULL, customer, (void *)&logger );
//    pthread_create(&pth[2], NULL, customer, (void *)&logger );
//    pthread_create(&pth[3], NULL, customer, (void *)&logger );
    time_t begin = time(NULL);
    for(i = 0; i < 500000; i++)
   {
       producer(&logger, r);
   }
    logger.finished = 1;
    if( logger.occupied > 0  )
    pthread_cond_wait( &logger.is_finished, &logger.lock );
    FS_DEBUG("elap time : %lu second\n", (time(NULL) - begin));
    return 0;
}

int FS_init_LOG(LOG *log, int slot_size){
    int i = 0;
    if( log == NULL ) return -1;
    log->slot_size = slot_size;
    log->idx_p = 0;
    log->idx_c = 0;
    log->occupied = 0;
    log->finished = 0;
    log->buffer = FS_CALLOC( sizeof( FS_BUFFER* ), slot_size );
    for(; i < slot_size; i++){
        log->buffer[i] = FS_CALLOC( sizeof( FS_BUFFER ) + BUFFER_SIZE, 1 );
        log->buffer[i]->size = BUFFER_SIZE ;
        log->buffer[i]->len = 0;
    }
return 0;
}


//1-----------------------------------------
void producer(LOG *log, int i){
    pthread_mutex_lock(&log->lock);
    while ( log->occupied >= log->slot_size && log->buffer[log->idx_p]->len > 0 ){
        write(2, "buffer full\n", 12);
        pthread_cond_wait( &log->less, &log->lock );
    }
    log->buffer[log->idx_p]->len = sprintf( logger.buffer[log->idx_p]->raw, "data %d :%s\n", log->idx_p, data);
    FS_DEBUG("[%d] writing \n",log->idx_p);
    log->idx_p = (log->idx_p + 1)%log->slot_size;
    log->occupied = log->occupied + 1;
    pthread_cond_signal( &log->more );
    pthread_mutex_unlock( &log->lock );
}

void * customer(void *args){
    LOG *log = (LOG*)args;
    for ( ;; ) {
       pthread_mutex_lock( &log->lock );
       if( log->finished == 1 && log->occupied <= 0 ){
            pthread_cond_signal( &log->is_finished );
       } 
       while ( log->occupied <= 0 ){
            write(2, "buffer empty\n", 13);
            pthread_cond_wait( &log->more, &log->lock );
        }
        if( FS_write("logger.log",log->buffer[log->idx_c]) < 0 ){
            FS_DEBUG("customer: write failed\n");
            exit(0);
        }
        log->buffer[log->idx_c]->len = 0;
        log->idx_c = (log->idx_c + 1)%log->slot_size;
        log->occupied = (log->occupied - 1);
        pthread_cond_signal( &log->less );
        pthread_mutex_unlock( &log->lock );
    } 
    return (void*)0;
}
