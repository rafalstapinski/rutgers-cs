#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

int *p_count;

void traverse(const char *path) {

   DIR *dir;
   struct dirent *ent;

   if ((dir = opendir(path)) == NULL)
   {
      fprintf(stderr, "Could not open directory\n");
      return;
   }

   if ((ent = readdir(dir)) == NULL)
   {
      fprintf(stderr, "Could not read directory\n");
      return;
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

        //  fprintf(stdout, "%s [%s]\n", path, entry->);

         traverse(full_path);
      }
      else { // files
         fprintf(stdout, "%s\n", full_path);

         //changeFile(fullPath);
      }
   }

   closedir(dir);
}

// void listdir(const char *name)
// {
//
//     DIR *dir;
//     struct dirent *entry;
//
//     if (!(dir = opendir(name)))
//     {
//         exit(-1);
//     }
//
//     if (!(entry = readdir(dir)))
//     {
//         exit(-1);
//     }
//
//     do
//     {
//
//         if (entry->d_type == DT_DIR)
//         {
//
//             char path[1024];
//
//             if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
//             {
//                 continue;
//             }
//
//             // snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
//             fprintf(stdout, "%s [%s] (%d)\n", name, entry->d_name, level);
//             // printf("%s\n", path);
//             // printf("[%s]\n", entry->d_name);
//
//             (*p_count)++;
//             pid_t child = fork();
//
//             if (child < 0)
//             {
//                 exit(-1);
//             }
//             else if (child == 0)
//             {
//                 listdir(path);
//                 // printf("%d,", getpid());
//                 exit(0);
//             }
//         }
//         else
//         {
//             // printf("- %s\n", entry->d_name);
//             (*p_count)++;
//             pid_t child = fork();
//
//
//             if (child < 0)
//             {
//                 exit(-1);
//             }
//             else if (child == 0)
//             {
//                 printf("i am child, will now sort %s/%s\n", entry->d_name);
//                 // printf("%d,", getpid());
//                 exit(0);
//             }
//
//         }
//         wait(NULL);
//     }
//     while ((entry = readdir(dir)) != NULL);
//     closedir(dir);
//     // printf("%d\n");
// }

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
