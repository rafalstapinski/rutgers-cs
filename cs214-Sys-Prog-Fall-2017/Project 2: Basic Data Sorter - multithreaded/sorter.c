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
#include <sys/syscall.h>
#include <unistd.h>
#include <stddef.h>
#include <ctype.h>
#include <pthread.h>
#include "sorter.h"

int *p_count;

// struct Row *head;
struct Row *global_head;
char *column;
char *src_dir;

pthread_mutex_t global_head_mutex;

long parent_tid;

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

struct Row *add_row(char *line, struct Row *head) {

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

  while (token)
  {

    switch(i)
    {
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

  return head;

}

void write_row(struct Row *row, FILE *fp)
{

  // print each column of a row

  fprintf(fp, "%s,%s,%d,%d,%d,%d,%s,%d,%f,%s,%s,%s,%d,%d,%s,%d,%s,%s,%d,%s,%s,%s,%d,%d,%d,%f,%f,%d\n",
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

void write_list(const char *f_path)
{

  // print the title row, then each successive row

  // printf("%s\n", f_path);

  // printf("\n\n %s \n\n", f_path);
  // char *f_name = strrchr(f_path, '/');
  //
  // if (f_name != NULL)
  // {
  //   f_name++;
  // }
  //
  // f_name[strlen(f_name) - 4] = 0;
  //
  // // printf("%s\n\n", f_name);
  //
  // // char *output_path = (char *) malloc(strlen(output_dir) + 1 + strlen(f_name) + 1 + 6 + 1 + strlen(column) + 4 + 1);
  //
  char output_path[1000];
  //


  strcpy(output_path, f_path);
  strcat(output_path, "/");
  strcat(output_path, "AllFiles-sorted-");
  strcat(output_path, column);
  strcat(output_path, ".csv");

  printf("\n\n %s \n\n", output_path);

  //
  // printf("\n\n %s \n\n", output_path);
  //
  FILE *fp;
  //
  fp = fopen(output_path, "ab+");

  fprintf(fp, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
  struct Row *ptr = global_head;

  while (ptr != NULL)
  {

    write_row(ptr, fp);
    ptr = ptr->next;

  }

  fclose(fp);
}

void *sort_csv(void *var_name)
{

  printf("%ld,", syscall(__NR_gettid));

  const char *name = (const char *) var_name;

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(name, "r");

  if (fp == NULL)
  {
    exit(-1);
  }

  int i = 0;

  struct Row *head = NULL;

  while ((read = getline(&line, &len, fp)) != -1)
  {
    if (i == 0)
    {
      i++;
      if (strcmp(line, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n") != 0)
      {
        printf("invalid format");
        printf(" %s", name);
        exit(0);
      }
    }

    else
    {
      head = add_row(line, head);
    }
  }

  head = msort(head);

  pthread_mutex_lock(&global_head_mutex);

  if (global_head == NULL)
  {

    global_head = head;

  }
  else {

    global_head = merge_list(global_head, head);

  }

  pthread_mutex_unlock(&global_head_mutex);

  fclose(fp);
  free(line);
  return NULL;

}

void *traverse(void *var_path)
{

  if (syscall(__NR_gettid) != parent_tid)
  {
    printf("%ld,", syscall(__NR_gettid));
  }

  const char *path = (const char *) var_path;

  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(path)) == NULL)
  {
    exit(-1);
  }

   if ((ent = readdir(dir)) == NULL)
   {
      exit(-1);
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

      (*p_count)++;

      pthread_t traverse_thread;

      if (pthread_create(&traverse_thread, NULL, traverse, full_path))
      {
        printf("Error creating thread.\n");
        exit(1);
      }

      if (pthread_join(traverse_thread, NULL))
      {
        printf("Error joining thread.\n");
        exit(2);
      }
      // pid_t child = fork();

      // if (child < 0)
      // {
      //   exit(-1);
      // }

      // else if (child == 0)
      // {
      //   traverse(full_path);
      //   // printf("%d,", getpid());
      //   exit(0);
      // }
      // traverse(full_path);

    }

    else
    {

      if (strstr(ent->d_name, "-sorted-") != NULL)
      {
        continue;
      }

      int len = strlen(ent->d_name);

      if(len < 3 || strcmp(ent->d_name + len - 4, ".csv"))
      {
        continue;
      }

      (*p_count)++;

      pthread_t sort_csv_thread;

      if (pthread_create(&sort_csv_thread, NULL, sort_csv, full_path))
      {
        printf("Error creating thread.\n");
        exit(1);
      }

      if (pthread_join(sort_csv_thread, NULL))
      {
        printf("Error joining thread.\n");
        exit(2);
      }

      // printf("file %s\n", ent->d_name);

      // pid_t child = fork();

      // if (child < 0)
      // {
      //   exit(-1);
      // }

      // else if (child == 0)
      // {
      //
      //   if (output_dir == NULL)
      //   {
      //     output_dir = (char *) malloc(strlen(path) + 1);
      //     strcpy(output_dir, path);
      //   }

        // sort_csv(full_path);
        // printf("%d,", getpid());
        // free(output_dir);

        // exit(0);
      // }
    }

    // wait(NULL);
  }

  closedir(dir);
  return NULL;
}

int main(int argc, char *argv[])
{

  char *output_dir;

  if (argc < 3)
  {
    printf("Invalid number of arguments. \n");
    exit(-1);
  }

  if (argc > 3 )
  {

    if (strcmp(argv[3], "-o") == 0)
    {

      output_dir = (char *) malloc(strlen(argv[4]) + 1);
      strcpy(output_dir, argv[4]);

    }

    else if (strcmp(argv[3], "-d") == 0)
    {

      src_dir = (char *) malloc(strlen(argv[4]) + 1);
      strcpy(src_dir, argv[4]);

    }
  }

  if (argc > 5)
  {

    if (strcmp(argv[5], "-o") == 0)
    {

      output_dir = (char *) malloc(strlen(argv[6]) + 1);
      strcpy(output_dir, argv[6]);

    }

    else if (strcmp(argv[5], "-d") == 0)
    {

      src_dir = (char *) malloc(strlen(argv[6]) + 1);
      strcpy(src_dir, argv[6]);

    }
  }

  if (src_dir == NULL)
  {

    src_dir = malloc(2);
    strcpy(src_dir, ".");

  }

  if (output_dir != NULL)
  {

    DIR *test = opendir(output_dir);
    if (test == NULL)
    {
      exit(-1);
    }
  }

  column = malloc(sizeof(char) * strlen(argv[2]) + 1);
  strcpy(column, argv[2]);

  int segmentId = shmget(IPC_PRIVATE, sizeof(int), S_IRUSR | S_IWUSR);
  p_count = (int *) shmat(segmentId, NULL, 0);
  *p_count = 1;

  // printf("%d\n", getpid());

  // mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_init(&global_head_mutex, NULL);
  parent_tid = syscall(__NR_gettid);

  printf("Initial PID: %ld,", parent_tid);
  printf("\n\tTIDS of all child thread: ");

  traverse(src_dir);

  printf("\n\tTotal number of threads: %d\n", *p_count);

  shmdt(p_count);
  shmctl(segmentId, IPC_RMID, NULL);

  // write_list(output_dir);

  if (output_dir == NULL)
  {
    write_list(".");
  }
  else
  {
    write_list(output_dir);
  }

  free(column);
  free(src_dir);


  return 0;


}
