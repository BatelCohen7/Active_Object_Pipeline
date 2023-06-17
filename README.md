# Active Object Pipeline
The Active Object Pipeline is an implementation of a pipeline using the Active Object pattern. It is designed to process random numbers through several stages with thread-safe queues.

## Overview

The pipeline consists of four stages:

1. Generation of random numbers.
2. Printing the number, checking if it is prime, and passing it to the next stage.
3. Printing the number, checking if it is prime again after some modification, and passing it to the next stage.
4. Printing the received number and performing a final modification.

Each stage is implemented as an Active Object, which is essentially a thread that processes tasks from its own queue.

## API

The Active Object Pipeline provides the following API functions:

- `pointerActiveObject createActiveObject(pointerQueueFunc func)`: Creates a new Active Object instance and binds a thread to it. It associates a queue and a function pointer that is called for each element in the queue.
- `pointerQueue getQueue(pointerActiveObject activeObject)`: Returns a pointer to the queue of the specified Active Object.
- `void stopActiveObject(pointerActiveObject activeObject)`: Stops the Active Object and releases associated memory.
- Various task functions to generate random numbers, check for primality, and perform arithmetic operations.

## Files and Their Functions

##### main.c

This file contains the main function of the program. It initializes the pipeline, generates random numbers, and passes them through the active object stages.

##### ActiveObject.c

This file contains the implementation of the Active Object pattern.

- `createActiveObject(pointerQueueFunc func)`: Creates an active object, binds a thread to it, and associates it with a queue and a function pointer to be called for each element in the queue.
- `getQueue(pointerActiveObject activeObject)`: Returns a pointer to the queue of the active object.
- `stopActiveObject(pointerActiveObject activeObject)`: Stops the active object and releases all memory structures associated with it.

##### ActiveObject.h

This header file contains the declarations for the functions implemented in `ActiveObject.c`.

##### Queue.c

This file contains the implementation of a thread-safe queue with mutex protections and condition variables.

- `createQueue()`: Creates a new queue.
- `enqueue(pointerQueue queue, void *data)`: Adds an element to the queue.
- `dequeue(pointerQueue queue)`: Removes and returns an element from the queue.
- `destroyQueue(pointerQueue queue)`: Destroys the queue and releases associated memory.

##### Queue.h

This header file contains the declarations for the functions implemented in `Queue.c`.

##### Task.c

This file contains the implementation of tasks that are processed by the active objects. This includes the logic for checking if a number is prime.

##### Task.h

This header file contains the declarations for the functions implemented in `Task.c`.

##### Tasks.c

This file contains the implementations of different tasks that can be executed by the active objects, such as generating random numbers, modifying numbers, and checking for primes.

##### Tasks.h

This header file contains the declarations for the functions implemented in `Tasks.c`.

## Assignment Stages

###### Level A

The implementation includes a function that checks if a given unsigned integer is prime.

###### Level B

A real queue is implemented with mutex protections and condition variables, which holds `void*`.

###### Level C

An active object is implemented with the necessary functionalities including creating an active object, getting its queue, and stopping it.

###### Level D

This level involves utilizing the functions built in the previous stages to build the pipeline described in the overview section.

## Dependencies

The Active Object Pipeline has the following dependencies:

- C compiler (GCC)
- Linux machine (Ubuntu 22.04 or similar)
- POSIX Threads Library

## Installation

To install the Active Object Pipeline, follow the steps below:

1. Clone the repository or copy the provided source files.
2. Navigate to the project directory.
3. Compile the project using a C compiler such as GCC: `gcc -o pipeline main.c ActiveObject.c Queue.c Task.c Tasks.c -lpthread` or `maka all` .

## Configuration

The Active Object Pipeline can be configured by modifying the source code (`main.c`). Configuration options include:

- Number of tasks: Can be set by providing a command-line argument.
- Random seed: Can be set by providing a second command-line argument.

## Running

Run the Active Object Pipeline by executing the compiled binary with the number of tasks as the argument: (./pipeline <number_of_tasks> [<random_seed>]) </div>
./st_pipeline  </div>
</div>
## Author: </div>
[Batel Cohen Yerushalmi](https://github.com/BatelCohen7 "Batel Cohen Yerushalmi") 

