#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 20

int count = 0;
int buffer [BUFFER_SIZE];

int indexIn = 0;
int indexOut = 0;

pthread_t tid;
pthread_mutex_t mutexLock;
pthread_cond_t condEmpty, condFull;

void insert(int item){
    pthread_mutex_lock(&mutexLock);
    while (count == BUFFER_SIZE){ //full, lets wait
        pthread_cond_wait(&condFull, &mutexLock);
    }
    buffer[indexIn] = item; //ok, count is not buffer size, so add item
    count++; //increment count now since we added an item
    if (count == 1) { //there's at least one element in the buffer, signal the empty cond so it can be retrieved
        pthread_cond_signal(&condEmpty);
    }
    sleep(1);
    pthread_mutex_unlock(&mutexLock);
}

int remove_item(){
    int item;
    pthread_mutex_lock(&mutexLock);
    while (count == 0){ //if count is 0 it means there's nothing to remove! - just wait
        pthread_cond_wait(&condEmpty, &mutexLock);
    } 
    item = buffer[indexOut]; //ok we broke out of the loop, retrieve item from buffer
    count--; //decrease count since we removed an item
    if (count == BUFFER_SIZE - 1) {
        pthread_cond_signal(&condFull);
    }
    sleep(1);
    pthread_mutex_unlock(&mutexLock);
    return item; //ok and we finish returning the item
}

void * producer(void * param){
   int item;
   while(1){
       item = rand() % BUFFER_SIZE ;
       insert(item);
       indexIn = (indexIn + 1) % BUFFER_SIZE;
       printf("in: %d inserted: %d\n", indexIn, item);
   }
}

void * consumer(void * param){
   int item;
   while(1){
    item = remove_item();
    indexOut = (indexOut + 1) % BUFFER_SIZE;
    printf("out: %d removed: %d\n", indexOut, item);
   }
}

int main(int argc, char * argv[])
{
    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    int i;

    if (pthread_mutex_init(&mutexLock, NULL) != 0){ //initialize mutex
        printf("Error!");
        exit(1);
    }
    pthread_cond_init(&condEmpty, NULL); //initialize cond for empty waiting
    pthread_cond_init(&condFull, NULL); //initialize cond for full waiting

    for (i = 0; i < producers; i++)
       pthread_create(&tid, NULL, producer, NULL);

    for (i = 0; i < consumers; i++)
       pthread_create(&tid, NULL, consumer, NULL); 

    pthread_join(tid, NULL);
    pthread_mutex_destroy(&mutexLock);
    pthread_cond_destroy(&condEmpty);
    pthread_cond_destroy(&condFull);
}
