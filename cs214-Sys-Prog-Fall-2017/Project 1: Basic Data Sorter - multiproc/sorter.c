// Author: Rafal Stapinski

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stddef.h>
#include <ctype.h>
#include "sorter.h"

int *p_count;

void strip(char *entry) {

  //remove whitespace from a string

  int i = strlen(entry);

  while (i > 0 && isspace(entry[i - 1]))
  {
    --i;
  }

  entry[i] = '\0';

  i = strspn(entry, "\t\v\n ");
  memmove(entry, entry + i, strlen(entry + i) + 1);

}

char *get_entry(char *str)
{

  // tokenize, but also return an empty string if empty column

  static char *copy = NULL;
  char *ptr = NULL;
  char *res = NULL;

  if (str != NULL)
  {
    copy = str;
  }

  if (copy == NULL) {
    return NULL;
  }

  if ((ptr = strpbrk(copy, ",")) != NULL)
  {
    *ptr = 0;
    res = copy;
    copy = ++ptr;
  }

  else if (*copy)
  {
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

void traverse(const char *path)
{

  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(path)) == NULL)
  {
    exit(-1)
  }

   if ((ent = readdir(dir)) == NULL)
   {
      exit(-1)
   }

   while ((ent = readdir(dir)) != NULL)
   {

    char full_path[1024];

    snprintf(full_path, 1023, "%s/%s", path, ent->d_name);

    if (ent->d_type == DT_DIR)
    {

      if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
      {
        continue;
      }

      traverse(full_path);
    }

    else
    {
    fprintf(stdout, "%s\n", full_path);
    }

  }

  closedir(dir);
}


int main(int argc, char *argv[])
{

    // int segmentId = shmget(IPC_PRIVATE, sizeof(int), S_IRUSR | S_IWUSR);
    // p_count = (int *) shmat(segmentId, NULL, 0);
    //
    // *p_count = 0;
    //
    // printf("%d\n", getpid());

  traverse("./files");

    // printf("\n%d\n", *p_count);


    return 0;


}
