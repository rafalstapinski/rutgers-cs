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
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/sendfile.h>
#include <fcntl.h>


char *column;
char *host;
char *port;

#define FILENAME "a.txt"

void *send_file(void *var_path)
{

  const char *path = (const char *) var_path;

  int sock;
  ssize_t len;
  struct sockaddr_in remote_addr;
  char buffer[BUFSIZ];
  char action[BUFSIZ];
  // int res_size;
  int file_size;
  // FILE *received_file;
  int fd;
  struct stat file_stat;
  int remain_data = 0;
  off_t offset;

  /* Zeroing remote_addr struct */
  memset(&remote_addr, 0, sizeof(remote_addr));

  /* Construct remote_addr struct */
  remote_addr.sin_family = AF_INET;
  inet_pton(AF_INET, host, &(remote_addr.sin_addr));
  remote_addr.sin_port = htons(atoi(port));

  fd = open(path, O_RDONLY);
  if (fd == -1)
  {
          fprintf(stderr, "Error opening file --> %s", strerror(errno));
          exit(EXIT_FAILURE);
  }

  /* Get file stats */
  if (fstat(fd, &file_stat) < 0)
  {
          fprintf(stderr, "Error fstat --> %s", strerror(errno));
          exit(EXIT_FAILURE);
  }

  memset(action, 0, sizeof(action));
  sprintf(action, "%s %zd", column, file_stat.st_size);

  /* Create client socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
          fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));

          exit(EXIT_FAILURE);
  }

  /* Connect to the server */
  if (connect(sock, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
  {
          fprintf(stderr, "Error on connect --> %s\n", strerror(errno));

          exit(EXIT_FAILURE);
  }

  // fprintf(stdout, "%zd\n", file_stat.st_size);

  //
  /* Sending file size */
  // len = send(client_socket, file_stat.st_size, sizeof(file_stat.st_size), 0);

  if (send(sock, action, sizeof(action), 0) < 0)
  {
    fprintf(stderr, "%s", strerror(errno));
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
      fprintf(stderr, "Unable to read file: %s", strerror(errno));
      exit(EXIT_FAILURE);
    }

    if(write(sock, buffer, rd) == -1)
    {
      fprintf(stderr, "Unable to write to socket: %s", strerror(errno));
      exit(EXIT_FAILURE);
    }

  }

  // int sent;
  // int remaining = file_stat.st_size;
  //
  // printf("%s\n\n", path);

  // while (((sent = sendfile(sock, fd, &offset, BUFSIZ)) > 0) && (remaining > 0))
  // {
  //   printf("okko");
  //   fprintf(stdout, "1. Serversent sent %d bytes from file's data, offset is now : %zd and remaining data = %d\n", sent, offset, remaining);
  //   remaining -= sent;
  //   fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %zd and remaining data = %d\n", sent, offset, remaining);
  // }

  // if (send(sock, " ", 1, 0) < 0)
  // {
  //   fprintf(stderr, "%s", strerror(errno));
  //   exit(EXIT_FAILURE);
  // }
  //
  // if (send(sock, column, strlen(column), 0) < 0)
  // {
  //   fprintf(stderr, "%s", strerror(errno));
  //   exit(EXIT_FAILURE);
  // }
  //
  // if (send(sock, " ", 1, 0) < 0)
  // {
  //   fprintf(stderr, "%s", strerror(errno));
  //   exit(EXIT_FAILURE);
  // }
  //
  // if (send(sock, path, strlen(path), 0) < 0)
  // {
  //   fprintf(stderr, "%s", strerror(errno));
  //   exit(EXIT_FAILURE);
  // }
  //
  // int remaining = file_stat.st_size;
  // /* Sending file data */
  //
  // int sent;
  //
  // while (((sent = sendfile(socket, fd, &offset, BUFSIZ)) > 0) && (remaining > 0))
  // {
  //   fprintf(stdout, "1. Serversent sent %d bytes from file's data, offset is now : %zd and remaining data = %d\n", sent, offset, remaining);
  //   remaining -= sent;
  //   fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %zd and remaining data = %d\n", sent, offset, remaining);
  // }

  // fprintf(stdout, "Server sent %zd bytes for the size\n", len);
  //
  // offset = 0;
  // remain_data = file_stat.st_size;
  // /* Sending file data */
  // while (((sent_bytes = sendfile(server_socket, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
  // {
  //         fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %zd and remaining data = %d\n", sent_bytes, offset, remain_data);
  //         remain_data -= sent_bytes;
  //         fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %zd and remaining data = %d\n", sent_bytes, offset, remain_data);
  // }

  /* Receiving file size */
  // recv(client_socket, buffer, BUFSIZ, 0);
  // file_size = atoi(buffer);
  // //fprintf(stdout, "\nFile size : %d\n", file_size);
  //
  // received_file = fopen(FILENAME, "w");
  // if (received_file == NULL)
  // {
  //         fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
  //
  //         exit(EXIT_FAILURE);
  // }
  //
  // remain_data = file_size;
  //
  // while (((len = recv(client_socket, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0))
  // {
  //         fwrite(buffer, sizeof(char), len, received_file);
  //         remain_data -= len;
  //         fprintf(stdout, "Receive %zd bytes and we hope :- %d bytes\n", len, remain_data);
  // }
  // fclose(received_file);

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
