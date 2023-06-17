#include "Tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "ActiveObject.h"

// An array of pointers to Active Objects, used by the active objects functions.
pointerActiveObject *ActiveObjects_Array = NULL;

// A pointer to the task_init, which is the first task that is being sent to the queue of the first Active Object, which starts the chain of tasks.
pointerTask task_data = NULL;

int main(int argc, char **args) {
	// Initialize the Functions_Array which contains the functions pointers for each Active Object.
	pointerQueueFunc Functions_Array[4] =
	{
		AOTask1,
		AOTask2,
		AOTask3,
		AOTask4
	};

	// Variables for the main function.
	unsigned int n = 0, seed = 0;

	// Check the number of arguments, and validate them.
	switch(argc)
	{
		// If there are no arguments, print the usage message and exit.
		case 1:
		{
			printf("Usage: %s <n> [<seed>]\n", *args);
			return 1;
		}

		// If there is one argument, validate it.
		case 2:
		{
			if (atoi(*(args + 1)) < 0)
			{
				printf("Error: n must be a positive integer.\n");
				return 1;
			}

			n = atoi(*(args + 1));

			break;
		}

		// If there are two arguments, validate them.
		case 3:
		{
			if (atoi(*(args + 1)) <= 0)
			{
				printf("Error: n must be a positive integer.\n");
				return 1;
			}

			else if (atoi(*(args + 2)) <= 0)
			{
				printf("Error: seed must be a positive integer.\n");
				return 1;
			}

			n = atoi(*(args + 1));
			seed = atoi(*(args + 2));

			break;
		}

		// If there are more than two arguments, print the usage message and exit.
		default:
		{
			printf("Usage: %s <n> [<seed>]\n", *args);
			return 1;
		}
	}

	// Set the signal handler for SIGINT.
	signal(SIGINT, sigint_handler);

	// Memory allocations for the ActiveObjects_Array, Functions_Array and task_init.
	ActiveObjects_Array = (pointerActiveObject *) malloc(sizeof(pointerActiveObject) * 4);
	task_data = createNewTask(n, seed);

	if (ActiveObjects_Array == NULL || task_data == NULL)
	{
		perror("malloc() has been falied\n");

		if (ActiveObjects_Array != NULL)
			free(ActiveObjects_Array);

		if (task_data != NULL)
			destroyTask(task_data);

		return 1;
	}

	// Create the Active Objects.
	for (int i = 0; i < 4; i++)
	{
		*(ActiveObjects_Array + i) = createActiveObject(*(Functions_Array + i));

		if (*(ActiveObjects_Array + i) == NULL)
		{
			printf("The function- createActiveObject() has been failed\n");

			for (int j = 0; j < i; j++)
				stopActiveObject(*(ActiveObjects_Array + j));
			
			free(task_data);
			free(ActiveObjects_Array);

			return 1;
		}
	}

	// Queue the first task to the queue of the first Active Object.
	ENQUEUE(getQueue(*(ActiveObjects_Array)), task_data);

	// Join the threads.
	for (int i = 0; i < 4; i++)
	{
		int ret = pthread_join((*(ActiveObjects_Array + i))->thread, NULL);

		if (ret != 0)
		{
			printf("THe function- pthread_join() has been failed: %s\n", strerror(ret));

			for (int j = 0; j < 4; j++)
				stopActiveObject(*(ActiveObjects_Array + j));
			
			destroyTask(task_data);
			free(ActiveObjects_Array);

			return 1;
		}
	}

	// Clean up the active objects after the threads are done.
	for (int i = 0; i < 4; i++)
		stopActiveObject(*(ActiveObjects_Array + i));

	// Free all remaining memory allocations.
	destroyTask(task_data);
	free(ActiveObjects_Array);

	return 0;
}

void sigint_handler(int sig) {
	// Check if the signal is not SIGINT, as this function is only for SIGINT.
	if (sig != SIGINT)
		return;

	// Check if the ActiveObjects_Array and task_init are not NULL, as free for NULL is a no-op.
	if (ActiveObjects_Array == NULL || task_data == NULL)
		return;

	if (CONSUL_MESSAGES)
		printf("\nSIGINT was received, starting memory cleanup...\n");
	
	// Clean up the active objects if the user pressed CTRL+C.
	for (int i = 0; i < 4; i++)
		stopActiveObject(*(ActiveObjects_Array + i));

	// Free all remaining memory allocations.
	destroyTask(task_data);
	free(ActiveObjects_Array);

	exit(0);
}

