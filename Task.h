#ifndef _TASK_H
#define _TASK_H
// The above lines are used to prevent double inclusion of this header file.
// If _TASK_H is already defined, the preprocessor will not process the file beyond this point.

// Check if _XOPEN_SOURCE or _POSIX_C_SOURCE are not already defined
#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE)
	// If the C Standard version is C99 or later
	#if __STDC_VERSION__ >= 199901L
		#define _XOPEN_SOURCE 600
		// Set _XOPEN_SOURCE to 600, indicating the program is being compiled against POSIX 2004.
	#else
		#define _XOPEN_SOURCE 500
		// Otherwise, set it to 500, indicating the program is being compiled against POSIX 1995.
	#endif 
#endif 

// Define CONSUL_MESSAGES for conditional compilation.
// If set to 1, printTask function will be available.
#define CONSUL_MESSAGES 0

// Define the structure 'Task' with two unsigned integer fields: numberOfTasks and data.
// Also, define 'pointerTask' as a pointer to Task.
typedef struct Task {
	unsigned int numberOfTasks; // The number of tasks
	unsigned int data;          // Some data associated with the task
} Task, *pointerTask;

// Function prototype for checkPrime
// This function takes an unsigned integer and returns an integer indicating whether the input is prime (1) or not (0).
int checkPrime(unsigned int num);

// Function prototype for createNewTask
// This function takes two unsigned integers and returns a pointer to a dynamically allocated Task structure.
pointerTask createNewTask(unsigned int numberOfTasks, unsigned int data);

// Function prototype for destroyTask
// This function takes a pointer to a Task structure and deallocates the memory associated with it.
void destroyTask(pointerTask task);

// Conditional compilation of printTask function.
// The function will be available if CONSUL_MESSAGES is set to 1.
#if CONSUL_MESSAGES == 1
    // Function prototype for printTask
    // This function takes a pointer to a Task structure and prints its contents.
    void printTask(pointerTask task); // Note: It should be pointerTask instead of PTask
#endif 

#endif // End of the header guard _TASK_H
