#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create and initialize a new queue
pointerQueue createQueue() {
    // Allocate memory for the queue
    pointerQueue queue = (pointerQueue)malloc(sizeof(Queue));

    // Check if memory allocation succeeded
    if (queue == NULL) {
        perror("malloc()has been failed");
        return NULL;
    }

    // Initialize queue's attributes
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    // Initialize the mutex for thread-safety
    pthread_mutex_init(&queue->lock, NULL);
    // Initialize the condition variable for signaling
    pthread_cond_init(&queue->cond, NULL);

    return queue;
}

// Function to destroy a queue and free all associated resources
void destroyQueue(pointerQueue queue) {
    if (queue == NULL) {
        printf("The function- queueDestroy()has been failed: queue is NULL\n");
        return;
    }

    // Lock the mutex to ensure thread-safety during destruction
    pthread_mutex_lock(&queue->lock);

    // Free all nodes in the queue
    pointerNode node = queue->head;
    while (node != NULL) {
        pointerNode next = node->next;
        free(node->data);
        free(node);
        node = next;
    }

    // Unlock the mutex and destroy the mutex and condition variable
    pthread_mutex_unlock(&queue->lock);
    pthread_cond_destroy(&queue->cond);
    pthread_mutex_destroy(&queue->lock);

    // Finally, free the queue itself
    free(queue);
}

// Function to enqueue an item to the queue
void enqueueQueue(pointerQueue queue, void *data) {
    if (queue == NULL) {
        printf("The function- queueEnqueue() has been failed: queue is NULL\n");
        return;
    }

    // Allocate memory for the new node
    pointerNode node = (pointerNode)malloc(sizeof(Node));

    // Check if memory allocation succeeded
    if (node == NULL) {
        perror("malloc()has been failed");
        return;
    }

    // Initialize the new node
    node->data = data;
    node->next = NULL;

    // Lock the mutex for thread-safety
    pthread_mutex_lock(&queue->lock);

    // Add the new node to the queue
    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;

        // Signal any threads waiting on the condition variable
        pthread_cond_signal(&queue->cond);
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    // Increment the size of the queue
    queue->size++;

    // Unlock the mutex
    pthread_mutex_unlock(&queue->lock);
}

// Function to dequeue an item from the queue
void *dequeueQueue(pointerQueue queue) {
    if (queue == NULL) {
        printf("The function- queueDequeue() has been failed: queue is NULL\n");
        return NULL;
    }

    // Lock the mutex for thread-safety
    pthread_mutex_lock(&queue->lock);

    // Wait for the queue to be non-empty
    while (queue->head == NULL) {
        pthread_cond_wait(&queue->cond, &queue->lock);
    }

    // Dequeue the item
    pointerNode node = queue->head;
    void *data = node->data;
    queue->head = node->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    // Free the dequeued node and decrement the size of the queue
    free(node);
    queue->size--;

    // Unlock the mutex
    pthread_mutex_unlock(&queue->lock);

    return data;
}

// Function to check if the queue is empty
int queueIsEmpty(pointerQueue queue) {
    if (queue == NULL) {
        printf("THe function- queueIsEmpty() has been failed: queue is NULL\n");
        return -1;
    }

    // Lock the mutex for thread-safety
    pthread_mutex_lock(&queue->lock);

    // Check if the size of the queue is 0
    int isEmpty = (queue->size == 0);

    // Unlock the mutex
    pthread_mutex_unlock(&queue->lock);

    return isEmpty;
}

#if CONSUL_MESSAGES == 1
	int queueSize(pointerQueue queue) {
		if (queue == NULL)
		{
			printf("The function- queueSize() has been failed: queue is NULL\n");
			return -1;
		}

		pthread_mutex_lock(&queue->lock);
		int size = queue->size;
		pthread_mutex_unlock(&queue->lock);

		return size;
	}

	void *queuePeek(pointerQueue queue) {
		if (queue == NULL)
		{
			printf("The function- queuePeek() has been failed: queue is NULL\n");
			return NULL;
		}

		pthread_mutex_lock(&queue->lock);

		if (queue->head == NULL)
		{
			pthread_mutex_unlock(&queue->lock);
			return NULL;
		}

		void *data = queue->head->data;

		pthread_mutex_unlock(&queue->lock);

		return data;
	}

	void *queuePeekTail(pointerQueue queue) {
		if (queue == NULL)
		{
			printf("The function- queuePeekTail() has been failed: queue is NULL\n");
			return NULL;
		}

		pthread_mutex_lock(&queue->lock);

		if (queue->tail == NULL)
		{
			pthread_mutex_unlock(&queue->lock);
			return NULL;
		}

		void *data = queue->tail->data;

		pthread_mutex_unlock(&queue->lock);

		return data;
	}

	void queuePrint(pointerQueue queue) {
    if (queue == NULL) {
        printf("Error: The function- queuePrint() has been failed - The queue is NULL.\n");
        return;
    }

    pthread_mutex_lock(&queue->lock);

    printf("Queue Information:\n");
    printf("+-------------------------------------------------------+\n");
    printf("| %-15s | %-20p |\n", "Queue", (void *)queue);
    printf("| %-15s | %-20d |\n", "Queue Size", queue->size);
    printf("| %-15s | %-20p |\n", "Queue Head", (void *)queue->head);
    printf("| %-15s | %-20p |\n", "Queue Tail", (void *)queue->tail);
    printf("| %-15s | %-20p |\n", "Queue Lock", (void *)&queue->lock);
    printf("| %-15s | %-20p |\n", "Queue Condition", (void *)&queue->cond);
    printf("| %-15s | %-20s |\n", "Queue Nodes", "");

    if (queue->head == NULL) {
        printf("| %-15s | %-20s |\n", "", "The Queue is empty");
        printf("+-------------------------------------------------------+\n");
        pthread_mutex_unlock(&queue->lock);
        return;
    }

    pointerNode node = queue->head;

    while (node != NULL) {
        printf("| %-15s | %-20p |\n", "", node->data);
        node = node->next;
    }

    printf("+-------------------------------------------------------+\n");

    pthread_mutex_unlock(&queue->lock);
}
#endif 