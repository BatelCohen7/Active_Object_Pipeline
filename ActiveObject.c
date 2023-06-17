#include "ActiveObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Creates and initializes a new active object.
 * @param func Function pointer to be executed by the active object.
 * @return A pointer to the newly created active object.
 */
pointerActiveObject createActiveObject(pointerQueueFunc func) {
    static unsigned int id = 0; // static ID counter for active objects

    // Allocate memory for the new active object.
    pointerActiveObject activeObject = (pointerActiveObject)malloc(sizeof(ActiveObject));

    // Check for memory allocation failure.
    if (activeObject == NULL) {
        perror("CreateActiveObject() has been failed: The function- malloc() failed");
        return NULL;
    }

    // Create a new queue for the active object.
    activeObject->queue = createQueue();

    // Check if queue creation was successful.
    if (activeObject->queue == NULL) {
        printf("CreateActiveObject() has been failed: The function- queueCreate() failed\n");
        free(activeObject);
        return NULL;
    }

    // Check if the callback function is not NULL.
    if (func == NULL) {
        printf("CreateActiveObject() has been failed: The function is NULL\n");
        destroyQueue(activeObject->queue);
        free(activeObject);
        return NULL;
    }

    // Set the callback function and ID.
    activeObject->func = func;
    activeObject->id = id++;

    // Create a new thread that runs activeObjectRunFunction.
    int ret = pthread_create(&activeObject->thread, NULL, activeObjectRunFunction, activeObject);

    // Check if thread creation was successful.
    if (ret != 0) {
        printf("CreateActiveObject() has been failed: The function- pthread_create() failed: %s\n", strerror(ret));
        destroyQueue(activeObject->queue);
        free(activeObject);
        return NULL;
    }

    // Optionally print success message.
    if (CONSUL_MESSAGES) {
        printf("CreateActiveObject() has been succeeded: activeObject created and thread started, id %d\n", activeObject->id);
    }

    return activeObject;
}

/**
 * Retrieves the queue associated with an active object.
 * @param activeObject Pointer to the active object.
 * @return Pointer to the queue associated with the active object.
 */
pointerQueue getQueue(pointerActiveObject activeObject) {
    if (activeObject == NULL) {
        if (CONSUL_MESSAGES)
            printf("getQueue() has been failed: activeObject is NULL\n");
        return NULL;
    }
    return activeObject->queue;
}

/**
 * Stops the active object and releases resources associated with it.
 * @param activeObject Pointer to the active object to be stopped.
 */
void stopActiveObject(pointerActiveObject activeObject) {
    if (activeObject == NULL) {
        printf("stopActiveObject() has been failed: activeObject is NULL\n");
        return;
    }

    unsigned int id = activeObject->id;

    // Cancel the active object's thread.
    pthread_cancel(activeObject->thread);
    activeObject->func = NULL;
    pthread_join(activeObject->thread, NULL);

    // Destroy queue and free allocated memory.
    destroyQueue(activeObject->queue);
    free(activeObject);

    // Optionally print success message.
    if (CONSUL_MESSAGES) {
        printf("stopActiveObject() has been succeeded: activeObject stopped and destroyed, id %d\n", id);
    }
}

/**
 * Function executed by the thread associated with each active object.
 * It processes tasks from the queue until there are no more tasks or until the function returns 0.
 * @param activeObject Pointer to the active object.
 * @return Pointer to the active object.
 */
void *activeObjectRunFunction(void *activeObject) {
    if (activeObject == NULL) {
        printf("activeObjectRunFunction() has been failed: activeObject is NULL\n");
        return NULL;
    }

    // Cast the input pointer to the active object structure.
    pointerActiveObject ao = (pointerActiveObject)activeObject;

    // Retrieve the associated queue.
    pointerQueue queue = ao->queue;

    void *task = NULL;

    if (queue == NULL) {
        printf("activeObjectRunFunction() has been failed: queue is NULL\n");
        return NULL;
    }

    // Optionally print status message.
    if (CONSUL_MESSAGES) {
        printf("ActiveObject thread started, id: %d\n", ao->id);
    }

    // Process tasks from the queue until the function returns 0 or there are no more tasks.
    while (ao->func && ((task = DEQUEUE(queue, void *)))) {
        int ret = ao->func(task);

        if (ret == 0) {
            if (CONSUL_MESSAGES) {
                printf("activeObjectRunFunction() has been succeeded: task completed, id %d\n", ao->id);
            }
            break;
        }
    }

    // Optionally print status message.
    if (CONSUL_MESSAGES) {
        if (queueIsEmpty(queue)) {
            printf("activeObjectRunFunction() has been succeeded: queue is empty, thread ended, id %d\n", ao->id);
        } else {
            printf("activeObjectRunFunction() has been failed: queue is not empty, id %d\n", ao->id);
        }
    }

    return activeObject;
}
