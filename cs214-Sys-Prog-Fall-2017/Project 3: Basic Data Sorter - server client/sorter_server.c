// Author: Rafal Stapinski

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <pthread.h>
#include <ctype.h>
#include "sorter_server.h"

struct Row *global_head;
pthread_mutex_t global_head_mutex;

struct worker_args {
  int socket;
};

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

void write_list()
{

  char output_path[] = "/tmp/raf-sorted";

  FILE *fp;

  fp = fopen(output_path, "w");

  fprintf(fp, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
  struct Row *ptr = global_head;

  while (ptr != NULL)
  {

    write_row(ptr, fp);
    ptr = ptr->next;

  }

  fclose(fp);
}

void sort_csv(const char *name, const char *column)
{

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
        fclose(fp);
        return;
      }
    }

    else
    {
      head = add_row(line, head);
    }
  }

  head = msort(head, column);

  pthread_mutex_lock(&global_head_mutex);

  if (global_head == NULL)
  {

    global_head = head;

  }
  else {

    global_head = merge_list(global_head, head, column);

  }

  pthread_mutex_unlock(&global_head_mutex);

  fclose(fp);
  free(line);
  return;

}

void *handle_connection(void *args)
{

  struct worker_args *worker;
  worker = (struct worker_args *) args;

  char buffer[BUFSIZ];
  char action[BUFSIZ];
  int sock = worker->socket;

  printf("Thread: %ld\n", pthread_self());
  printf("Socket: %d\n", sock);

  char *file_length_str;
  long long file_length;

  while (recv(sock, action, sizeof(action), 0))
  {

    printf("%s\n", action);

    if (strcmp(action, "get me all of the files!") == 0)
    {
      write_list();
      return (void *) 0;

    }
    else
    {

      for (file_length_str = action; file_length_str != '\0'; file_length_str++)
      {

        if (*file_length_str == ' ')
        {
          *file_length_str = '\0';
          file_length_str++;
          break;
        }
      }

      char filename[] = "/tmp/raf-      ";
      const char alphabet[] = "qwertyuiopasdfghjklzxcvbnm";
      int i = 0;
      for (i = 9; i < 15; i++)
      {
        filename[i] = alphabet[rand() % strlen(alphabet)];
      }

      file_length = strtoll(file_length_str, NULL, 10);

      int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
      if (fd == -1)
      {
        fprintf(stderr, "Unable to create temp file: %s", strerror(errno));
        exit(EXIT_FAILURE);
      }

      ssize_t rd;
      for (; file_length > 0; file_length -= rd)
      {

        rd = BUFSIZ;

        if (rd > file_length)
        {
          rd = file_length;
        }

        rd = read(sock, buffer, rd);

        if (rd == -1)
        {
          fprintf(stderr, "Unable to read file: %s", strerror(errno));
          exit(EXIT_FAILURE);
        }
        else if (rd == 0)
        {
          break;
        }

        if (write(fd, buffer, rd) == -1)
        {
          fprintf(stderr, "Unable to write to file: %s", strerror(errno));
          exit(EXIT_FAILURE);
        }
      }

      printf("\n\n");

      close(fd);

      sort_csv(filename, action);

      send(sock, "sorted", 6, 0);

    }
  }

  close(sock);
  free(worker);

  return (void *) 0;

}

int main(int argc, char *argv[])
{

  if (argc < 3)
  {
    printf("Not enough arguments. \n");
    exit(1);
  }

  int port = atoi(argv[2]);

  int server_socket;
  int client_socket;
  socklen_t sock_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  struct worker_args *worker;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == -1)
  {
    fprintf(stderr, "Error creating socket %s", strerror(errno));
    exit(EXIT_FAILURE);
  }


  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));
  server_address.sin_port = htons(port);


  if ((bind(server_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr))) == -1)
  {
    fprintf(stderr, "Error binding: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((listen(server_socket, 5)) == -1)
  {
    fprintf(stderr, "Error listening: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  sock_len = sizeof(struct sockaddr_in);

  char ip[INET_ADDRSTRLEN];

  while ((client_socket = accept(server_socket, (struct sockaddr *) &client_address, &sock_len)) != -1)
  {

    // inet_ntop(AF_INET, &client_address, ip, INET_ADDRSTRLEN);
    // printf("%s,", ip);

    pthread_t handle_thread;

    worker = malloc(sizeof(struct worker_args));
    worker->socket = client_socket;

    if (pthread_create(&handle_thread, NULL, handle_connection, worker))
    {
      fprintf(stderr, "Error creating thread: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

  }

  printf("\n");
  pthread_exit(NULL);

  return 0;
}
