#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "find_utils.c"

void sfind(char *buf, int will_print, int will_exec, int will_name,
            char *sub, int substitute, char ** argv) {
   DIR *dir;
   struct dirent *entry;
   struct stat stat_buf;
   List *l = initializeList();
   if (doNotFilter(will_name, sub, buf)) {
      processFile(will_print, will_exec, substitute, buf, argv);
   }
   dir = opendir(buf);
   if ((entry = readdir(dir)) == NULL) {
      return;
   }
   do {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
         continue;
      } else {
         Node *n = initializeNode(entry->d_name);
         insert(l, n);
      }
   } while ((entry = readdir(dir)) != NULL);
   closedir(dir);
   Node *temp = l->head;
   while (temp != NULL) {
      char new_path[1024];
      if (stat(temp->data, &stat_buf) < 0) {
         perror("");
         exit(-1);
      }
      strcpy(new_path, buf);
      strcat(new_path, "/");
      strcat(new_path, temp->data);
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
   char *sub = NULL;
   struct stat buf;
   if (parseArgs(&will_print, &will_exec,
                  &will_name, &substitute, sub, argc, argv) == 0) {
      fprintf(stderr,
              "Print or exec have not been specified, use one or the other\n");
      exit(-1);
   }
   if (stat(argv[1], &buf) < 0) {
      fprintf(stderr, "File/directory %s does not exist\n", argv[1]);
      exit(-1);
   }
   if (getFileTypeChar(buf) == 'd') {
      char buf[1024];
      buf[0] = '\0';
      strcat(buf, argv[1]);
      sfind(buf, will_print, will_exec, will_name, sub, substitute, argv);
   } else {
      processFile(will_print, will_exec, substitute, argv[1], argv);
   }

   return 0;
}
