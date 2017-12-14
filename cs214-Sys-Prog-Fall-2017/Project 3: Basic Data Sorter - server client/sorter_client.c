// Author: Rafal Stapinski

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/sendfile.h>
#include <fcntl.h>

char *column;
char *host;
char *port;

int valid_file(const char *path)
{

  int fp = fopen(path, "r");

  if ((read = getline(&line, &len, fp)) != -1)
  {
    fclose(fp);
    if (strcmp(line, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n") != 0)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

}

void *send_file(void *var_path)
{

  const char *path = (const char *) var_path;

  if (!valid_file(path))
  {
    return (void *) 0;
  }

  int sock;
  struct sockaddr_in server_address;
  char buffer[BUFSIZ];
  char action[BUFSIZ];
  int fd;
  struct stat file_stat;

  memset(&server_address, 0, sizeof(server_address));

  server_address.sin_family = AF_INET;
  inet_pton(AF_INET, host, &(server_address.sin_addr));
  server_address.sin_port = htons(atoi(port));

  fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (fstat(fd, &file_stat) < 0)
  {
      fprintf(stderr, "Error getting file stats: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
  }

  memset(action, 0, sizeof(action));
  sprintf(action, "%s %zd", column, file_stat.st_size);

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1)
  {
    fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&server_address, sizeof(struct sockaddr)) == -1)
  {
          fprintf(stderr, "Error connecting: %s\n", strerror(errno));

          exit(EXIT_FAILURE);
  }

  if (send(sock, action, sizeof(action), 0) < 0)
  {
    fprintf(stderr, "Error sending action: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  int rd;

  while (1)
  {
    rd = read(fd, buffer, BUFSIZ);

    if (rd == 0)
    {
      break;
    }
    else if (rd == -1)
    {
      fprintf(stderr, "Unable to read file: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    if(write(sock, buffer, rd) == -1)
    {
      fprintf(stderr, "Unable to write to socket: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

  }

  char response[BUFSIZ];
  recv(sock, response, sizeof(response), 0);

  if (strcmp(response, "sorted") != 0)
  {
    printf("Server could not sort. \n");
    exit(EXIT_FAILURE);
  }

  close(sock);
  close(fd);

  return (void *) 0;

}

void traverse(const char *path)
{

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

      traverse(full_path);
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

      pthread_t send_file_pthread;

      if (pthread_create(&send_file_pthread, NULL, send_file, full_path))
      {

        printf("Error creating thread.\n");
        exit(1);

      }

      if (pthread_join(send_file_pthread, NULL))
      {

        printf("Error joining thread.\n");
        exit(2);

      }
    }
  }

  closedir(dir);
}

int main(int argc, char *argv[])
{

  if (argc < 7)
  {
    printf("Invalid number of arguments. \n");
    exit(-1);
  }

  if (strcmp(argv[1], "-c") == 0)
  {
    column = (char *) malloc(strlen(argv[2]) + 1);
    strcpy(column, argv[2]);
  }
  else
  {
    printf("Invalid argument order. \n");
    exit(-1);
  }

  if (strcmp(argv[3], "-h") == 0)
  {
    host = (char *) malloc(strlen(argv[4]) + 1);
    strcpy(host, argv[4]);
  }
  else
  {
    printf("Invalid argument order. \n");
    exit(-1);
  }

  if (strcmp(argv[5], "-p") == 0)
  {
    port = (char *) malloc(strlen(argv[6]) + 1);
    strcpy(port, argv[6]);
  }
  else
  {
    printf("Invalid argument order. \n");
    exit(-1);
  }

  char *input_dir = NULL;
  char *output_dir = NULL;

  if (argc > 7)
  {

    if (strcmp(argv[7], "-d") == 0)
    {
      input_dir = (char *) malloc(strlen(argv[8]) + 1);
      strcpy(input_dir, argv[8]);
    }

    else if (strcmp(argv[7], "-o") == 0)
    {
      output_dir = (char *) malloc(strlen(argv[8]) + 1);
      strcpy(output_dir, argv[8]);
    }

  }

  if (argc > 9)
  {

    if (strcmp(argv[9], "-d") == 0)
    {
      input_dir = (char *) malloc(strlen(argv[9]) + 1);
      strcpy(input_dir, argv[10]);
    }

    else if (strcmp(argv[9], "-o") == 0)
    {
      output_dir = (char *) malloc(strlen(argv[9]) + 1);
      strcpy(output_dir, argv[10]);
    }

  }

  if (!input_dir)
  {
    input_dir = (char *) malloc(2);
    strcpy(input_dir, ".");
  }

  if (!output_dir)
  {
    output_dir = (char *) malloc(2);
    strcpy(output_dir, ".");
  }

  traverse(input_dir);


  // free(column);
  // free(src_dir);


  return 0;


}
