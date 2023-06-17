#include "Tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

// Assuming this is defined somewhere else, as it's an extern declaration.
extern pointerActiveObject *ActiveObjects_Array;

// Function: AOTask1
// Purpose: It initializes a task by enqueuing 'n' random numbers.
int AOTask1(void *task) {
    // Cast input void pointer to pointerTask type.
    pointerTask task_init = (pointerTask)task;

    // Extract the number of tasks and data from the task structure.
    unsigned int n = task_init->numberOfTasks;
    unsigned int data = task_init->data;

    // Seed the random number generator.
    srand(data != 0 ? data : time(NULL));

    // Generate 'n' random numbers and enqueue them.
    for (unsigned int i = 0; i < n; i++) {
        unsigned int num = (rand() % 900000) + 100000;

        // Create a new task with random number.
        pointerTask task_data = createNewTask(n, num);

        // If memory allocation fails, print error and exit.
        if (task_data == NULL) {
            perror("malloc() has been failed");
            exit(1);
        }

        // Enqueue the task into the queue associated with the second active object.
        ENQUEUE(getQueue(*(ActiveObjects_Array + 1)), task_data);

        // Sleep for 1ms.
        usleep(1000);
    }

    return 0;
}

// Function: AOTask2
// Purpose: Process data by checking if it's prime, modifies the data, and passes it to next active object in the chain.
int AOTask2(void *task) {
    static unsigned int counter = 0; // Counter for the number of times the function has been called.
    pointerTask task_data = (pointerTask)task;

    unsigned int iter = task_data->numberOfTasks;
    unsigned int num = task_data->data;

    // Print number and whether it's prime.
    printf("%u\n%s\n", num, checkPrime(num) ? "true" : "false");

    // Deallocate memory for current task.
    destroyTask(task_data);

    // Create new task with modified data.
    task_data = createNewTask(iter, (num + 11));

    // Handle memory allocation failure.
    if (task_data == NULL) {
        perror("malloc() has been failed");
        exit(1);
    }

    // Enqueue the task into the queue associated with the third active object.
    ENQUEUE(getQueue(*(ActiveObjects_Array + 2)), task_data);

    // Check termination condition.
    if (iter <= ++counter) 
        return 0;
    else 
        return 1;
}

// Function: AOTask3
// Purpose: Similar to AOTask2, but modifies data differently and prints to stdout.
int AOTask3(void *task) {
    static unsigned int counter = 0; // Counter for the number of times the function has been called.
    pointerTask task_data = (pointerTask)task;

    unsigned int iter = task_data->numberOfTasks;
    unsigned int num = task_data->data;

    // Print number and whether it's prime to stdout.
    fprintf(stdout, "%u\n%s\n", num, checkPrime(num) ? "true" : "false");

    // Deallocate memory for current task.
    destroyTask(task_data);

    // Create new task with modified data.
    task_data = createNewTask(iter, (num - 13));

    // Handle memory allocation failure.
    if (task_data == NULL) {
        perror("malloc() has been failed");
        exit(1);
    }

    // Enqueue the task into the queue associated with the fourth active object.
    ENQUEUE(getQueue(*(ActiveObjects_Array + 3)), task_data);

    // Check termination condition.
    if (iter <= ++counter) 
        return 0;
    else 
        return 1;
}

// Function: AOTask4
// Purpose: Similar to AOTask2 and AOTask3, but with different data modification.
int AOTask4(void *task) {
    static unsigned int counter = 0; // Counter for the number of times the function has been called.
    pointerTask task_data = (pointerTask)task;

    unsigned int iter = task_data->numberOfTasks;
    unsigned int num = task_data->data;

    // Print number, whether it's prime, and modified data to stdout.
    fprintf(stdout, "%u\n%s\n%u\n", num, checkPrime(num) ? "true" : "false", (num + 2));

    // Deallocate memory for current task.
    destroyTask(task_data);

    // Check termination condition.
    if (iter <= ++counter) 
        return 0;
    else 
        return 1;
}
