#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H

#include "Queue.h"
#include <pthread.h>

// Type definition for a pointer to a function that takes a void pointer as an argument and returns an integer.
// This is intended to be used as a callback function for processing tasks from a queue.
typedef int (*pointerQueueFunc)(void *);

// Structure representing an active object.
// It encapsulates a thread that processes tasks from a queue.
typedef struct ActiveObject {
    unsigned int id;               // A unique identifier for the active object.
    pthread_t thread;              // The thread associated with the active object.
    pointerQueue queue;            // The queue that holds tasks to be processed by the active object.
    pointerQueueFunc func;         // The function pointer to the task processing function.
} ActiveObject, *pointerActiveObject; // ActiveObject is the structure, pointerActiveObject is a pointer to this structure.

/**
 * Creates a new active object with an associated thread that processes tasks from a queue.
 *
 * @param func A pointer to the function that will be used to process tasks from the queue.
 *             The function should return 0 if it has finished processing, and non-zero if it should continue processing tasks.
 * @return A pointer to the created active object, or NULL if creation failed.
 */
pointerActiveObject createActiveObject(pointerQueueFunc func);

/**
 * Retrieves the queue associated with the given active object.
 *
 * @param activeObject A pointer to the active object.
 * @return A pointer to the queue of the active object, or NULL if the active object is NULL.
 */
pointerQueue getQueue(pointerActiveObject activeObject);

/**
 * Stops the active object and releases its resources.
 *
 * @param activeObject A pointer to the active object to be stopped.
 */
void stopActiveObject(pointerActiveObject activeObject);

/**
 * The function that will be executed by the thread associated with each active object.
 * It processes tasks from the queue using the active object's callback function.
 *
 * @param activeObject A void pointer to the active object. It should be cast back to a pointerActiveObject type inside the function.
 * @return A void pointer to the active object.
 */
void *activeObjectRunFunction(void *activeObject);

#endif // ACTIVEOBJECT_H
