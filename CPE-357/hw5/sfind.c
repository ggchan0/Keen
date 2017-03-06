#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>
#include "find_utils.h"

void sfind(char *buf, int will_print, int will_exec, int will_name,
            char *sub, int substitute, char ** argv) {
   DIR *dir;
   struct dirent *entry;
   struct stat stat_buf;
   List *l = initializeList();
   if (doNotFilter(will_name, sub, buf)) {
      processFile(will_print, will_exec, substitute, buf, argv);
   }
   if (!(dir = opendir(buf))) {
      printf("Cannot access directory %s\n", buf);
      return;
   }
   while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
         continue;
      }
      if (entry->d_name != NULL) {
         Node *n = initializeNode(entry->d_name);
         insert(l, n);
      }
   }
   closedir(dir);
   Node *temp = l->head;
   while (temp != NULL) {
      char new_path[1024];
      strcpy(new_path, buf);
      strcat(new_path, "/");
      strcat(new_path, temp->data);
      if (lstat(new_path, &stat_buf) < 0) {
         perror("");
         exit(-1);
      }
      if (getFileTypeChar(stat_buf) == 'd') {
         sfind(new_path, will_print, will_exec,
               will_name, sub, substitute, argv);
      } else {
         if (doNotFilter(will_name, sub, temp->data)) {
            processFile(will_print, will_exec, substitute, new_path, argv);
         }
      }
      temp = temp->next;
   }
   freeList(l);
}

int main(int argc, char **argv) {
   int will_print = 0, will_exec = 0, will_name = 0, substitute = 0;
   char sub[1024];
   struct stat stat_buf;
   sub[0] = '\0';
   if (parseArgs(&will_print, &will_exec,
                  &will_name, &substitute, sub, argc, argv) == 0) {
      fprintf(stderr,
              "Print or exec have not been specified, use one or the other\n");
      exit(-1);
   }
   if (lstat(argv[1], &stat_buf) < 0) {
      fprintf(stderr, "File/directory %s does not exist\n", argv[1]);
      exit(-1);
   }
   if (getFileTypeChar(stat_buf) == 'd') {
      char buf[1024];
      buf[0] = '\0';
      strcat(buf, argv[1]);
      if (buf[strlen(argv[1]) - 1] == '/') {
         buf[strlen(argv[1]) - 1] = '\0';
      }
      sfind(buf, will_print, will_exec, will_name, sub, substitute, argv);
   } else {
      processFile(will_print, will_exec, substitute, argv[1], argv);
   }

   return 0;
}
