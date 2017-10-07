#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "second.h"


int main(int argc, char *argv[]) {

    FILE *f;
    f = fopen(argv[1], "r");
    size_t size;
    char *line = NULL;
    int read;
    char *token;

    int flag = 0;

    struct Node *head = NULL;
    struct Node *curr = NULL;

    while ((read = getline(&line, &size, f)) != -1) {

        if (flag == 0) {

            flag++;

        } else {

            token = strtok(line, " ");

            while (token != NULL) {

                curr = malloc(sizeof(struct Node));
                curr->value = atoi(token);
                curr->next = head;
                head = curr;
                token = strtok(NULL, " ");
            }
        }
    }

    struct Node *i = NULL;
    struct Node *j = NULL;
    struct Node *tmp = NULL;
    int min;

    for (i = head; i != NULL; i = i->next) {

        min = INT_MAX;
        tmp = i;

        for (j = i; j != NULL; j = j->next) {

            if (j->value < min) {

                min = j->value;
                tmp = j;
            }
        }

        tmp->value = i->value;
        i->value = min;
        tmp = NULL;
    }

    for (i = head; i != NULL; i = i->next) {

        if (i->next == NULL) {

            printf("%d\n", i->value);

        } else {

            printf("%d\t", i->value);
        }
    }

    exit(0);

}
