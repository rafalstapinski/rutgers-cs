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

#define port 18563

struct Row *global_head;
pthread_mutex_t global_head_mutex;

struct worker_args {
  int socket;
};

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

      // sort_csv(filename, column);

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

  // if (client_socket == -1)
  // {
  //         fprintf(stderr, "Error on accept --> %s", strerror(errno));
  //
  //         exit(EXIT_FAILURE);
  // }
  // fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(client_address.sin_addr));
  //
  // sprintf(file_size, "%zd", file_stat.st_size);
  //
  // /* Sending file size */
  // len = send(client_socket, file_size, sizeof(file_size), 0);
  // if (len < 0)
  // {
  //       fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
  //
  //       exit(EXIT_FAILURE);
  // }
  //
  // fprintf(stdout, "Server sent %zd bytes for the size\n", len);
  //
  // offset = 0;
  // remain_data = file_stat.st_size;
  // /* Sending file data */
  // while (((sent_bytes = sendfile(client_socket, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
  // {
  //         fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %zd and remaining data = %d\n", sent_bytes, offset, remain_data);
  //         remain_data -= sent_bytes;
  //         fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %zd and remaining data = %d\n", sent_bytes, offset, remain_data);
  // }
  //
  // close(client_socket);
  // close(server_socket);

  return 0;
}
