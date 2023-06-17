#include "Task.h" // Include the custom header file that has definitions related to Task structure
#include <stdio.h> // Include standard input/output functions for printing errors
#include <stdlib.h> // Include standard library for malloc and free functions
#include <errno.h> // Include error number for perror
#include <math.h> // Include math library for square root function

// Function: checkPrime
// Purpose: Check if the given number is a prime number
// Input: An unsigned integer num
// Output: Returns 1 if the number is prime, 0 otherwise
int checkPrime(unsigned int num) {
    // Handle special cases: 0 and 1 are not prime, 2 is prime
    if (num < 2 || num % 2 == 0) 
        return 0;
    if (num == 2)
        return 1;

    // Calculate the square root of the number for optimization
    unsigned int root = (unsigned int)sqrt(num);

    // Check if the number is divisible by any odd number up to its square root
    for (unsigned int i = 3; i <= root; i += 2) {
        if (num % i == 0) // If divisible, it's not prime
            return 0;
    }

    // If we haven't found any factors, then it is prime
    return 1;
}

// Function: createNewTask
// Purpose: Dynamically allocate and initialize a Task structure
// Input: Two unsigned integers - numberOfTasks and data
// Output: Returns a pointer to the newly allocated Task structure
pointerTask createNewTask(unsigned int numberOfTasks, unsigned int data) {
    // Allocate memory for new task
    pointerTask task = (pointerTask)malloc(sizeof(Task));

    // Check if memory allocation was successful
    if (task == NULL) {
        perror("malloc() has been failed"); // Print error message
        return NULL; // Return NULL pointer
    }

    // Initialize the task with the provided data
    task->numberOfTasks = numberOfTasks;
    task->data = data;

    // Return the pointer to the new task
    return task;
}

// Function: destroyTask
// Purpose: Free the memory allocated for a Task structure
// Input: A pointer to a Task structure
// Output: None
void destroyTask(pointerTask task) {
    // Check if the task pointer is NULL
    if (task == NULL) {
        printf("The function- destroyTask() has been failed: task is NULL\n");
        return;
    }
    // Free the memory
    free(task);
}

// Conditional compilation for the printTask function based on the DEBUG_MESSAGES flag
#if DEBUG_MESSAGES == 1
    // Function: printTask
    // Purpose: Prints the contents of a Task structure
    // Input: A pointer to a Task structure
    // Output: None
    void printTask(pointerTask task) {
        // Check if the task pointer is NULL
        if (task == NULL) {
            printf("The function- printTask()has been failed: task is NULL\n");
            return;
        }
        // Print the contents of the task
        printf("Task: %u %u\n", task->numberOfTasks, task->data);
    }
#endif
