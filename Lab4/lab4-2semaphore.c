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
sem_t semlock;

void insert(int item){
    sem_wait(&semlock);
    while (count == BUFFER_SIZE); //full, lets wait
    
    buffer[indexIn] = item; //ok, count is not buffer size, so add item
    count++; //increment count now since we added an item
    sleep(1);
    sem_post(&semlock);
}

int remove_item(){
    int item;
    sem_wait(&semlock);
    while (count == 0); //if count is 0 it means there's nothing to remove! - just wait
    item = buffer[indexOut]; //ok we broke out of the loop, retrieve item from buffer
    count--; //decrease count since we removed an item
    sleep(1);
    sem_post(&semlock);
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

    sem_init(&semlock, 0, 1); //initialize semaphore as binary and to use threads

    for (i = 0; i < producers; i++)
       pthread_create(&tid, NULL, producer, NULL);

    for (i = 0; i < consumers; i++)
       pthread_create(&tid, NULL, consumer, NULL); 

    pthread_join(tid, NULL);
    sem_destroy(&semlock);
}
