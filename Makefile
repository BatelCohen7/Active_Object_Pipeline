# Flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -pedantic
PFLAGS = -pthread
RM = rm -f

# Phony targets
.PHONY: all default clean

# Default target
all: st_pipeline

# Alias 
default: all

# Programs
st_pipeline: Main.o Tasks.o ActiveObject.o Queue.o Task.o
	$(CC) $(CFLAGS) -o $@ $^ -lm $(PFLAGS)


# Object
%.o: %.c Tasks.h ActiveObject.hQueue.h Task.h
	$(CC) $(CFLAGS) -c $<
	

# Cleanup files 
clean:
	$(RM) *.o *.so st_pipeline