//
//  Queue.c
//  pa4
//
//  Created by Ryan D'souza on 4/18/17.
//  Copyright © 2017 Ryan D'souza. All rights reserved.
//

#include "Queue.h"

/** Allocates memory for a Queue */
struct Queue* createQueue(int maxSize)
{
    struct Queue *queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    queue->MAX_SIZE = maxSize;
    queue->enabled = '1';
    
    return queue;
}

/** Returns 1 if the Queue contains the value. Returns -1 if the Queue does not contain the value */
int queueContainsValue(struct Queue *queue, unsigned long value)
{
    if(queue == NULL || queue->enabled != '1') {
        return -1;
    }
    
    struct Node *iterator = queue->front;
    
    while(iterator != NULL) {
        if(iterator->enabled == '1' && value == iterator->value) {
            return 1;
        }
        iterator = iterator->next;
    }
    
    return -1;
}

/** Dequeues the first item in the Queue */
void dequeueValue(struct Queue *queue)
{
    if(queue->front == NULL || queue->front->enabled != '1') {
        return;
    }
    
    struct Node *oldFront = queue->front;
    
    queue->front = queue->front->next;

    free(oldFront);
    
    queue->size--;
}

/** Enqueues the value. If the Queue is greater than the MAX_SIZE, automatically dequeues */
void enqueueValue(struct Queue *queue, unsigned long value)
{
    struct Node *node = (struct Node*) malloc(sizeof(struct Node));
    node->value = value;
    node->next = NULL;
    node->enabled = '1';
    
    if(queue->front == NULL || queue->front->enabled != '1') {
        queue->front = node;
        queue->rear = queue->front;
    }
    
    else {
        queue->rear->next = node;
        queue->rear = queue->rear->next;
    }
    
    queue->size++;
    
    if(queue->size > queue->MAX_SIZE) {
        dequeueValue(queue);
    }
}
