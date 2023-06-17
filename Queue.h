#ifndef _QUEUE_H
#define _QUEUE_H

#include "Task.h"
#include <pthread.h>

// Define the Node structure
typedef struct Node {
    void *data;           // Generic pointer to data of any type
    struct Node *next;    // Pointer to the next node in the queue
} Node, *pointerNode;     // Define typedefs for struct Node and pointer to struct Node

// Define the Queue structure
typedef struct Queue {
    pointerNode head;         // Pointer to the head of the queue
    pointerNode tail;         // Pointer to the tail of the queue
    unsigned int size;        // Size of the queue
    pthread_mutex_t lock;     // Mutex for thread synchronization
    pthread_cond_t cond;      // Condition variable for thread synchronization
} Queue, *pointerQueue;      // Define typedefs for struct Queue and pointer to struct Queue

// Function prototypes for queue operations
pointerQueue createQueue();                       // Create a new queue
void destroyQueue(pointerQueue queue);            // Destroy a queue and free its resources
void enqueueQueue(pointerQueue queue, void *data); // Enqueue data into the queue
void *dequeueQueue(pointerQueue queue);           // Dequeue data from the queue
int queueIsEmpty(pointerQueue queue);             // Check if the queue is empty
int queueSize(pointerQueue queue);                // Get the size of the queue

#if CONSUL_MESSAGES == 1
    // Additional function prototypes for debugging
    void *queuePeek(PQueue queue);
    void *queuePeekTail(PQueue queue);
    void queuePrint(PQueue queue);
#endif

// Macros for simplified enqueue and dequeue operations
#define ENQUEUE(queue, data) enqueueQueue(queue, (void *)data)
#define DEQUEUE(queue, type) ((type)dequeueQueue(queue))

#if CONSUL_MESSAGES == 1
    // Macros for debugging
    #define PEEK(queue, type) ((type)queuePeek(queue))
    #define PEEK_TAIL(queue, type) ((type)queuePeekTail(queue))
#endif

#endif // _QUEUE_H
