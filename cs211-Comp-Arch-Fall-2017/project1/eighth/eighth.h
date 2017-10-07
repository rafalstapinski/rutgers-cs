#include <stdio.h>

struct Node
{
    int value;
    int level;
    struct Node *leftChild;
    struct Node *rightChild;
};
