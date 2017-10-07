#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "first.h"

struct Node *head = NULL;

void insert(int value) {

    if (head == NULL) {

        head = malloc(sizeof(struct Node));
        head->value = value;
        head->next = NULL;

    } else if (value < head->value) {

        struct Node *newNode = malloc(sizeof(struct Node));
        newNode->value = value;
        newNode->next = head;
        head = newNode;

    } else if (head->next == NULL && value > head->value) {

        struct Node *newNode = malloc(sizeof(struct Node));
        newNode->value = value;
        newNode->next = NULL;
        head->next = newNode;

    } else {

        struct Node *ptr = NULL;

        for (ptr = head; ptr->next != NULL; ptr = ptr->next) {

            if (value < ptr->next->value && value > ptr->value) {
                struct Node *newNode = malloc(sizeof(struct Node));

                newNode->value = value;
                newNode->next = ptr->next;
                ptr->next = newNode;
            }
        }

        if (value > ptr->value) {

            struct Node *newNode = malloc(sizeof(struct Node));
            newNode->value = value;
            newNode->next = NULL;
            ptr->next = newNode;
        }
    }
}

void delete(int value) {

    struct Node *ptr = NULL;

    if (value == head->value) {
        head = head->next;

    } else {

        struct Node *prev = head;

        for (ptr = head->next; ptr != NULL; ptr = ptr->next) {

            if (ptr->value == value) {

                prev->next = ptr->next;
            }

            prev = ptr;
        }
    }
}

int main(int argc, char *argv[]) {

    FILE *f;

    if (access(argv[1], F_OK) == -1) {

        printf("error\n");
        exit(0);
    }

    f = fopen(argv[1], "r");
    size_t size;
    char *line = NULL;
    int read;
    char *token;

    while ((read = getline(&line, &size, f)) != -1) {
        token = strtok(line, "\t");
        if (strcmp(token, "i") == 0) {

            token = strtok(NULL, "\t");
            insert(atoi(token));

        } else {

            token = strtok(NULL, "\t");
            delete(atoi(token));
        }
    }

    struct Node *ptr = NULL;

    if (head == NULL) {

        printf("0\n");

    } else {

        for (ptr = head; ptr != NULL; ptr = ptr->next) {
            printf("%d\n", ptr->value);
        }
    }

    exit(0);
}
