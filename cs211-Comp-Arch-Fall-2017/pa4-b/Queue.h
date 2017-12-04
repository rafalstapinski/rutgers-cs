//
//  Queue.h
//  pa4
//
//  Created by Ryan D'souza on 4/18/17.
//  Copyright © 2017 Ryan D'souza. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

/** Represent a Node in the Queue */
struct Node {
    char enabled;
    unsigned long value;
    struct Node *next;
};

/** Represents a typical FIFO Data Structure */
struct Queue {
    struct Node *front;
    struct Node *rear;
    
    int MAX_SIZE;
    int size;
    char enabled;
};

/** Allocates memory for a Queue */
struct Queue* createQueue(int maxSize);

/** Returns 1 if the Queue contains the value. Returns -1 if the Queue does not contain the value */
int queueContainsValue(struct Queue *queue, unsigned long value);

/** Enqueues the value. If the Queue is greater than the MAX_SIZE, automatically dequeues */
void enqueueValue(struct Queue *queue, unsigned long value);

/** Dequeues the first item in the Queue */
void dequeueValue(struct Queue *queue);
