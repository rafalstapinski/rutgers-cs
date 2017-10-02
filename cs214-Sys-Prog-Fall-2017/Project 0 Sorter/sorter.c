#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "sorter.h"

struct Row *head = NULL;

void strip(char *entry) {

  size_t i = strlen(entry);
  while (i > 0 && isspace(entry[i - 1])) {
    --i;
  }
  entry[i] = '\0';

  i = strspn(entry, "\t\v\n ");
  memmove(entry, entry + i, strlen(entry + i) + 1);

}

char *get_entry(char *str) {
  static char *copy = NULL;
  char *ptr, *res;

  if (str != NULL) {
    copy = str;
  }
  if (copy == NULL) {
    return NULL;
  }

  if ((ptr = strpbrk(copy, ",")) != NULL) {
    *ptr = 0;
    res = copy;
    copy = ++ptr;
  } else if (*copy) {
    res = copy;
    copy = NULL;
  }

  strip(res);
  return res;
}

void add_row(char *line) {

  int i = 0;
  char *token = get_entry(line);
  struct Row *row = (struct Row*) malloc(sizeof(struct Row));

  while (token) {

    switch(i) {
      case 0:
        row->color = malloc(strlen(token) + 1);
        strncpy(row->color, token, strlen(token));
      case 1:
        row->director_name = malloc(strlen(token) + 1);
        strncpy(row->director_name, token, strlen(token));
      case 2:
        row->num_critic_for_reviews = malloc(sizeof(int));
        *(row->num_critic_for_reviews) = atoi(token);
      case 3:
        row->duration = malloc(sizeof(int));
        *(row->duration) = atoi(token);
      case 4:
        row->director_facebook_likes = malloc(sizeof(int));
        *(row->director_facebook_likes) = atoi(token);
      case 5:
        row->actor_3_facebook_likes = malloc(sizeof(int));
        *(row->actor_3_facebook_likes) = atoi(token);
      case 6:
        row->actor_2_name = malloc(strlen(token) + 1);
        strncpy(row->actor_2_name, token, strlen(token));
      case 7:
        row->actor_1_facebook_likes = malloc(sizeof(int));
        *(row->actor_1_facebook_likes) = atoi(token);
    }

    token = get_entry(NULL);
    i++;

  }

  row->next = head;
  head = row;

}

void printList() {
   struct Row *ptr = head;
   printf("\n[ ");

   //start from the beginning
   while(ptr != NULL) {
      printf("%s - %d \n",ptr->director_name, *(ptr->director_facebook_likes));
      ptr = ptr->next;
   }

   printf(" ]");
}

int main(int argc, char *argv[]) {

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  size_t read;

  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("File does not exist. ");
    return 1;
  }

  int i = 0;

  while ((read = getline(&line, &len, fp)) != -1) {

    // printf("-  %s  - \n", line);

    if (i == 0) {
      i++;
    } else {
      add_row(line);
    }
  }

  printList();
  head = mergesort(head);
  printList();

  if (line) {
    free(line);
  }

  fclose(fp);

  return 0;

}
