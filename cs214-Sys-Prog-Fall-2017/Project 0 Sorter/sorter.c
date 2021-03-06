#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "sorter.h"

struct Row *head = NULL;
char *column;

void strip(char *entry) {

  //remove whitespace from a string

  int i = strlen(entry);
  while (i > 0 && isspace(entry[i - 1])) {
    --i;
  }
  entry[i] = '\0';

  i = strspn(entry, "\t\v\n ");
  memmove(entry, entry + i, strlen(entry + i) + 1);

}

char *get_entry(char *str) {

  // tokenize, but also return an empty string if empty column

  static char *copy = NULL;
  char *ptr = NULL;
  char *res = NULL;

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

  // column count

  int i = 0;

  // deal with commas inside quotes (temporarily replace with semicolons)

  int flag = 0;
  char *temp = line;
  while (*temp) {
    if ((flag == 0) && (strchr("\"", *temp))) {
      flag = 1;
    } else if ((flag == 1) && (*temp == ',')) {
      *temp = ';';
    } else if ((flag == 1) && (*temp == '"')) {
      flag = 2;
    } else if ((flag == 2) && (*temp == ',')) {
      flag = 0;
    }
    temp++;
  }

  // tokenize each column, and assign to row

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
      case 8:
        row->gross = malloc(sizeof(int));
        *(row->gross) = atoi(token);
      case 9:
        row->genres = malloc(strlen(token) + 1);
        strncpy(row->genres, token, strlen(token));
      case 10:
        row->actor_1_name = malloc(strlen(token) + 1);
        strncpy(row->actor_1_name, token, strlen(token));
      case 11:
        row->movie_title = malloc(strlen(token) + 1);
        strncpy(row->movie_title, token, strlen(token));
      case 12:
        row->num_voted_users = malloc(sizeof(int));
        *(row->num_voted_users) = atoi(token);
      case 13:
        row->cast_total_facebook_likes = malloc(sizeof(int));
        *(row->cast_total_facebook_likes) = atoi(token);
      case 14:
        row->actor_3_name = malloc(strlen(token) + 1);
        strncpy(row->actor_3_name, token, strlen(token));
      case 15:
        row->facenumber_in_poster = malloc(sizeof(int));
        *(row->facenumber_in_poster) = atoi(token);
      case 16:
        row->plot_keywords = malloc(strlen(token) + 1);
        strncpy(row->plot_keywords, token, strlen(token));
      case 17:
        row->movie_imdb_link = malloc(strlen(token) + 1);
        strncpy(row->movie_imdb_link, token, strlen(token));
      case 18:
        row->num_user_for_reviews = malloc(sizeof(int));
        *(row->num_user_for_reviews) = atoi(token);
      case 19:
        row->language = malloc(strlen(token) + 1);
        strncpy(row->language, token, strlen(token));
      case 20:
        row->country = malloc(strlen(token) + 1);
        strncpy(row->country, token, strlen(token));
      case 21:
        row->content_rating = malloc(strlen(token) + 1);
        strncpy(row->content_rating, token, strlen(token));
      case 22:
        row->budget = malloc(sizeof(int));
        *(row->budget) = atoi(token);
      case 23:
        row->title_year = malloc(sizeof(int));
        *(row->title_year) = atoi(token);
      case 24:
        row->actor_2_facebook_likes = malloc(sizeof(int));
        *(row->actor_2_facebook_likes) = atoi(token);
      case 25:
        row->imdb_score = malloc(sizeof(int));
        *(row->imdb_score) = atof(token);
      case 26:
        row->aspect_ratio = malloc(sizeof(int));
        *(row->aspect_ratio) = atof(token);
      case 27:
        row->movie_facebook_likes = malloc(sizeof(int));
        *(row->movie_facebook_likes) = atoi(token);
    }

    token = get_entry(NULL);

    i++;

  }

  row->next = head;
  head = row;

}

void print_row(struct Row *row) {

  // print each column of a row

  printf("%s,%s,%d,%d,%d,%d,%s,%d,%f,%s,%s,%s,%d,%d,%s,%d,%s,%s,%d,%s,%s,%s,%d,%d,%d,%f,%f,%d\n",
            row->color,
            row->director_name,
            *row->num_critic_for_reviews,
            *row->duration,
            *row->director_facebook_likes,
            *row->actor_3_facebook_likes,
            row->actor_2_name,
            *row->actor_1_facebook_likes,
            *row->gross,
            row->genres,
            row->actor_1_name,
            row->movie_title,
            *row->num_voted_users,
            *row->cast_total_facebook_likes,
            row->actor_3_name,
            *row->facenumber_in_poster,
            row->plot_keywords,
            row->movie_imdb_link,
            *row->num_user_for_reviews,
            row->language,
            row->country,
            row->content_rating,
            *row->budget,
            *row->title_year,
            *row->actor_2_facebook_likes,
            *row->imdb_score,
            *row->aspect_ratio,
            *row->movie_facebook_likes);
}

void print_list() {

  // print the title row, then each successive row

  printf("color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
  struct Row *ptr = head;
  while(ptr != NULL) {
    print_row(ptr);
    ptr = ptr->next;
  }
}

int main(int argc, char *argv[]) {

  // deal with invalid arguments

  if ((strcmp(argv[1], "-c") != 0) || (argc != 3)) {
    printf("Invalid arguments.\n");
    return 1;
  } else if ((strcmp(argv[2], "color") == 0) ||
            (strcmp(argv[2], "director_name") == 0) ||
            (strcmp(argv[2], "num_critic_for_reviews") == 0) ||
            (strcmp(argv[2], "duration") == 0) ||
            (strcmp(argv[2], "director_facebook_likes") == 0) ||
            (strcmp(argv[2], "actor_3_facebook_likes") == 0) ||
            (strcmp(argv[2], "actor_2_name") == 0) ||
            (strcmp(argv[2], "actor_1_facebook_likes") == 0) ||
            (strcmp(argv[2], "gross") == 0) ||
            (strcmp(argv[2], "genres") == 0) ||
            (strcmp(argv[2], "actor_1_name") == 0) ||
            (strcmp(argv[2], "movie_title") == 0) ||
            (strcmp(argv[2], "num_voted_users") == 0) ||
            (strcmp(argv[2], "cast_total_facebook_likes") == 0) ||
            (strcmp(argv[2], "actor_3_name") == 0) ||
            (strcmp(argv[2], "facenumber_in_poster") == 0) ||
            (strcmp(argv[2], "plot_keywords") == 0) ||
            (strcmp(argv[2], "movie_imdb_link") == 0) ||
            (strcmp(argv[2], "num_user_for_reviews") == 0) ||
            (strcmp(argv[2], "language") == 0) ||
            (strcmp(argv[2], "country") == 0) ||
            (strcmp(argv[2], "content_rating") == 0) ||
            (strcmp(argv[2], "budget") == 0) ||
            (strcmp(argv[2], "title_year") == 0) ||
            (strcmp(argv[2], "actor_2_facebook_likes") == 0) ||
            (strcmp(argv[2], "imdb_score") == 0) ||
            (strcmp(argv[2], "aspect_ratio") == 0) ||
            (strcmp(argv[2], "movie_facebook_likes") == 0)) {
    column = (char *)malloc(strlen(argv[2] + 1));
    strncpy(column, argv[2], strlen(argv[2]));
  } else {

    printf("Invalid column.\n");
    return 1;

  }

  char *line = NULL;
  size_t size = 0;
  int read;

  int i = 0;

  // read through STDIN line by line, skip first then add_row for each successive

  while ((read = getline(&line, &size, stdin)) != -1) {

    if (i == 0) {
      i++;
    } else {
      add_row(line);
    }
  }

  // mergesort the linked list

  head = msort(head);
  print_list();

  return 0;
}
