#include "Berde_threadpool.h"
#include <pthread.h> 
#include <semaphore.h>
#include <stdio.h>

#define THREAD_COUNT 3
#define QUEUE_SIZE 10

typedef struct { // from repo provided by Professor Chen in assignment
    void (*somefunction)(void *p);
    void *p;
} task;

void *worker(void *param);
int enqueue(task t);
task dequeue();
void execute(void (*somefunction)(void *p), void *p);

pthread_t thread_pool[THREAD_COUNT];
sem_t lock_pool; // mutex lock for thread pool
sem_t lock_queue; // mutex lock for waiting queue
sem_t queue_count; // counting semaphore for count of tasks in waiting queue for pool; prevents dequeue if queue is empty

task waiting_queue[QUEUE_SIZE]; 
int front_index = 0; // index of front of FIFO waiting queue
int rear_index = 0; // index of back of FIFO waiting queue
int queue_size = 0;

// initializes a thread pool object and all the necessary semaphores
void pool_init() {
    sem_init(&lock_pool, 0, 1); // semaphore should be for threads within this process
    sem_init(&lock_queue, 0, 1);
    sem_init(&queue_count, 0, 0);
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, worker, NULL);
        thread_pool[i] = thread;
    }
}

// calling thread submits request to thread pool
int pool_submit(void (*somefunction)(void *p), void *p) {
    task work;
    work.somefunction = somefunction;
    work.p = p;
    int status = enqueue(work);
    if (status == 1) {
        printf("QUEUE IS FULL, COULD NOT SUBMIT REQUEST.\n");
    }
    return status;
}

// adds a task to the back of the waiting queue of pool
int enqueue(task t) {
    sem_wait(&lock_queue);
    if (queue_size == QUEUE_SIZE) {
        sem_post(&lock_queue);
        return 1;
    }
    waiting_queue[rear_index] = t;
    rear_index = (rear_index + 1) % QUEUE_SIZE; // queue needs to be circular, since waiting_queue is an array of fixed size
    queue_size++;
    sem_post(&lock_queue);
    sem_post(&queue_count);
    return 0;
}

// removes task from the front of the waiting queue of the pool
task dequeue() {
    sem_wait(&queue_count);
    sem_wait(&lock_queue);
    task t = waiting_queue[front_index];
    front_index = (front_index + 1) % QUEUE_SIZE; // queue needs to be circular, since waiting_queue is an array of fixed size
    queue_size--;
    sem_post(&lock_queue);
    return t; 
}

// function called by all threads in the pool
void *worker(void *param) {
    while (1) {
        task work = dequeue();
        execute(work.somefunction, work.p);
    }
}

// simply calls the function the calling thread requested a working thread to run (project directions require this function, isn't fully necessary)
void execute(void (*somefunction)(void *p), void *p) {
    somefunction(p);
}

// pool is shutdown SAFELY (through deferred cancellations)
void pool_shutdown() {
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_cancel(thread_pool[i]); // send deferred cancellation request
        pthread_join(thread_pool[i], NULL);
    }
}

