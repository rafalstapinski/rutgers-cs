// #include <stdlib.h>
// #include <stdio.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <sys/wait.h>
// #include <sys/socket.h>
// #include <signal.h>
// #include <ctype.h>
// #include <arpa/inet.h>
// #include <netdb.h>
//
// #define PORT 20000
// #define LENGTH 512
//
//
// void error(const char *msg)
// {
// 	perror(msg);
// 	exit(1);
// }
//
// int main(int argc, char *argv[])
// {
// 	/* Variable Definition */
// 	int sockfd;
// 	int nsockfd;
// 	char revbuf[LENGTH];
// 	struct sockaddr_in remote_addr;
//
// 	/* Get the Socket file descriptor */
// 	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
// 	{
// 		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
// 		exit(1);
// 	}
//
// 	/* Fill the socket address struct */
// 	remote_addr.sin_family = AF_INET;
// 	remote_addr.sin_port = htons(PORT);
// 	inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr);
// 	bzero(&(remote_addr.sin_zero), 8);
//
// 	/* Try to connect the remote */
// 	if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
// 	{
// 		fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
// 		exit(1);
// 	}
// 	else
// 		printf("[Client] Connected to server at port %d...ok!\n", PORT);
//
// 	/* Send File to Server */
// 	//if(!fork())
// 	//{
// 		char* fs_name = "/home/aryan/Desktop/quotidiani.txt";
// 		char sdbuf[LENGTH];
// 		printf("[Client] Sending %s to the Server... ", fs_name);
// 		FILE *fs = fopen(fs_name, "r");
// 		if(fs == NULL)
// 		{
// 			printf("ERROR: File %s not found.\n", fs_name);
// 			exit(1);
// 		}
//
// 		bzero(sdbuf, LENGTH);
// 		int fs_block_sz;
// 		while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
// 		{
// 		    if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
// 		    {
// 		        fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
// 		        break;
// 		    }
// 		    bzero(sdbuf, LENGTH);
// 		}
// 		printf("Ok File %s from Client was Sent!\n", fs_name);
// 	//}
//
// 	/* Receive File from Server */
// 	printf("[Client] Receiveing file from Server and saving it as final.txt...");
// 	char* fr_name = "../medium2.csv";
// 	FILE *fr = fopen(fr_name, "a");
// 	if(fr == NULL)
// 		printf("File %s Cannot be opened.\n", fr_name);
// 	else
// 	{
// 		bzero(revbuf, LENGTH);
// 		int fr_block_sz = 0;
// 	    while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0)
// 	    {
// 			int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
// 	        if(write_sz < fr_block_sz)
// 			{
// 	            error("File write failed.\n");
// 	        }
// 			bzero(revbuf, LENGTH);
// 			if (fr_block_sz == 0 || fr_block_sz != 512)
// 			{
// 				break;
// 			}
// 		}
// 		if(fr_block_sz < 0)
//         {
// 			if (errno == EAGAIN)
// 			{
// 				printf("recv() timed out.\n");
// 			}
// 			else
// 			{
// 				fprintf(stderr, "recv() failed due to errno = %d\n", errno);
// 			}
// 		}
// 	    printf("Ok received from server!\n");
// 	    fclose(fr);
// 	}
// 	close (sockfd);
// 	printf("[Client] Connection lost.\n");
// 	return (0);
// }

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

char *column;
char *host;
char *port;

void *send_file(void *var_path)
{
  const char *path = (const char *) var_path;

  printf("%s\n", path);

  return NULL;
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
