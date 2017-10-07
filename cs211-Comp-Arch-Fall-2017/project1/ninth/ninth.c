#include "eighth.h"

#include <stdio.h>
#include <stdlib.h>

/** Deletes a Node given the value of the node and the depth */
struct Node* deleteNode(struct Node *node, int value)
{
  if(node == NULL) {
    return node;
  }
}

/** Inserts a Node given the value of the node and the depth */
struct Node* insertNode(struct Node *node, int value, int height)
{
    //Need to create a new Node and insert it
    if(node == NULL) {
        node = (struct Node*) malloc(sizeof(struct Node));

        node->value = value;
        node->level = height;

        node->leftChild = NULL;
        node->rightChild = NULL;
        printf("inserted %d\n", node->level);

        return node;
    }

    else if(node->value == value) {
        printf("duplicate\n");
        return node;
    }

    //Recurse through the rest of the children
    if(node->value > value) {
        node->rightChild = insertNode(node->rightChild, value, ++height);
    }

    else if(node->value < value) {
        node->leftChild = insertNode(node->leftChild, value, ++height);
    }

    return node;
}


/** Recursively searches for a value.
         Returns -1 if not found otherwise returns the height of the tree */
int search(struct Node *node, int value)
{
    if(node == NULL) {
        return -1;
    }

    if(node->value == value) {
        return node->level;
    }

    else if(node->value < value) {
        return search(node->leftChild, value);
    }

    else {
        return search(node->rightChild, value);
    }
}


int main(int argc, char **argv)
{
    if(argc <= 1) {
        printf("error\n");
        exit(0);
    }

    FILE *file = fopen(argv[1], "r");

    if(!file) {
        printf("error\n");
        exit(0);
    }

    struct Node *head = NULL;

    char inputCommand;
    int inputValue;

    while(fscanf(file, "%c\t%d\n", &inputCommand, &inputValue) != EOF) {

        if(inputCommand == 's') {
            int searchTreeLevel = search(head, inputValue);
            if(searchTreeLevel == -1) {
                printf("absent\n");
            }
            else {
                printf("present %d\n", searchTreeLevel);
            }
        }

        else if(inputCommand == 'i') {
            head = insertNode(head, inputValue, 1);
        }

        else {
            printf("error\n");
        }
    }

    fclose(file);

    return 0;
}
