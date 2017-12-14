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

#define PORT_NUMBER     5000
#define SERVER_ADDRESS  "127.0.0.1"
#define FILE_TO_SEND    "hello.txt"

struct worker_args {
  int socket;
};

void *handle_connection(void *args)
{

  struct worker_args *worker;
  worker = (struct worker_args *) args;
  //
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

      file_length = strtoll(file_length_str);
    }
  }

  // recv(sock, buffer, BUFSIZ, 0);
  // printf("%d - %s\n", sock, buffer);
  //
  // char filename[] = "/tmp/raf-aaaaaa";
  // const char alphabet[] = "qwertyuiopasdfghjklzxcvbnm";
  // int i = 0;
  // for (i = 9; i < 15; i++)
  // {
  //   filename[i] = alphabet[rand() % strlen(alphabet)];
  // }
  //
  //
  // printf("%s\n", filename);
  //
  //
  // /* Receiving file size */
  // recv(sock, buffer, BUFSIZ, 0);
  // int file_size = atoi(buffer);
  // // fprintf(stdout, "\nFile size : %d\n", file_size);
  //
  // FILE *received_file;
  //
  // received_file = fopen(filename, "w");
  // if (received_file == NULL)
  // {
  //         fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
  //
  //         exit(EXIT_FAILURE);
  // }
  //
  //
  // int received = 0;
  // int remaining = file_size;
  //
  // while (((received = recv(sock, buffer, BUFSIZ, 0)) > 0) && (remaining > 0))
  // {
  //   fwrite(buffer, sizeof(char), received, received_file);
  //
  //   // printf("%s\n\n", buffer);
  //
  //   remaining -= received;
  // }

  // fclose(received_file);
  close(sock);
  free(worker);

  return (void *) 0;

}

int main(int argc, char *argv[])
{
        int server_socket;
        int client_socket;
        socklen_t       sock_len;
        // ssize_t len;

        struct sockaddr_in      server_addr;
        struct sockaddr_in      client_address;
        int fd;
        // int sent_bytes = 0;
        // char file_size[256];
        struct stat file_stat;
        // off_t offset;
        // int remain_data;

        struct worker_args *worker;

        /* Create server socket */
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1)
        {
                fprintf(stderr, "Error creating socket --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Zeroing server_addr struct */
        memset(&server_addr, 0, sizeof(server_addr));
        /* Construct server_addr struct */
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET, SERVER_ADDRESS, &(server_addr.sin_addr));
        server_addr.sin_port = htons(PORT_NUMBER);

        /* Bind */
        if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
        {
                fprintf(stderr, "Error on bind --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Listening to incoming connections */
        if ((listen(server_socket, 5)) == -1)
        {
                fprintf(stderr, "Error on listen --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        // fd = open(FILE_TO_SEND, O_RDONLY);
        // if (fd == -1)
        // {
        //         fprintf(stderr, "Error opening file --> %s", strerror(errno));
        //
        //         exit(EXIT_FAILURE);
        // }
        //
        // /* Get file stats */
        // if (fstat(fd, &file_stat) < 0)
        // {
        //         fprintf(stderr, "Error fstat --> %s", strerror(errno));
        //
        //         exit(EXIT_FAILURE);
        // }
        //
        // fprintf(stdout, "File Size: \n%zd bytes\n", file_stat.st_size);

        sock_len = sizeof(struct sockaddr_in);
        /* Accepting incoming peers */
        // client_socket = accept(server_socket, (struct sockaddr *)&client_address, &sock_len);

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

            printf("Error creating thread.\n");
            exit(1);

          }

          // if (pthread_join(handle_thread, NULL))
          // {
          //
          //   printf("Error joining thread.\n");
          //   exit(2);
          //
          // }

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
